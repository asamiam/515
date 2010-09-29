/*
 *  magic.cpp
 *  
 *
 *  Created by Ace Sorensen on 9/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <Magick++.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace Magick;

int main(int argc, char * argv[])
{
	cout << "CSE 515 Project 1 - Part 1" << endl;

	if(argc != 2)
	{
		cout << "Error parsing command line arguments." << endl;
		cout << "Usage: \nsegement <image-name>";
		return 1;
	}

	// Get imageName
	char *imageName;
	imageName = argv[1];
	cout << "ImageName: " << imageName << endl;

	InitializeMagick(*argv);
	
	try
	{
		Image image(imageName);
		cout <<	"Accepted the image, segmenting" << endl;
		image.quantizeColorSpace(YUVColorspace);
		image.segment(1.0, 1.5);
		
		Geometry dim = image.size();
		size_t width = dim.width();
		size_t height = dim.height();
		cout << "Image width: " << width << endl;
		cout << "Image height: " << height << endl;
		
		ColorYUV color;
		double y,u,v;
		double y1,u1,v1;
		size_t w_max, h_max, w_min;
		int segments=0;
		int s[width][height];
		int max_sets = width*height;
		int sets[max_sets];
		
		//Initialize all pixels to -1
		for (size_t i=0; i<width; i++) {
			for (size_t j=0; j<height; j++) {
				s[i][j]=-1;
			}
		}
		for (size_t i=0; i<width*height; i++){
			sets[i]=-1;
		}
		for (size_t j = 0; j<height; j++) {
			for (size_t i = 0; i<width; i++) {
				
				//Initialize the pixel we're checking
				color = image.pixelColor(i,j);
				y = color.y();
				u = color.u();
				v = color.v();
				
				//If I'm not assigned, increment segments and assign
				//cout << "sij: " << s[i][j] << " segments: " << segments << endl;
				if (s[i][j]==-1) {
					s[i][j]=++segments;
				}
				//cout << "nsij: " << s[i][j] << " segments: " << segments <<endl;
				
				//cout << "My segment: " << s[i][j] << " number of segments: " << segments << " ";
				w_max = i!=(width-1) ? i+1 : i;
				w_min = i!=0 ? i-1 : i;
				h_max = j!=(height-1) ? j+1 : j;
				
				for (size_t w = w_min; w<=w_max; w++) {
					for (size_t h = j; h<=h_max; h++) {
						if ((i==w&&j==h) || (w_min==w && h==j)) {
							//It's ME
						} else {
							//I've found a neighbor
							color = image.pixelColor(w,h);
							y1 = color.y();
							u1 = color.u();
							v1 = color.v();
							
							//cout << "Comparing: " << y << ", " << u << ", " << v << " x-coord: " << i << " y-coord: " << j << endl;
							//cout << "With:      " << y1 << ", " << u1 << ", " << v1 << " x-coord: " << w << " y-coord: " << h << endl;
							
							if ((y1==y) && (u1==u) && (v1==v)) {
								//SAME!!!  Add to my segment
								if (s[w][h]<0) {
									//Hasn't been assigned, assign mine
									//cout << "===MATCH: I assign wh:" << s[w][h] << " = ij:" << s[i][j] << endl;
									s[w][h] = s[i][j];
								}else if (s[w][h] != s[i][j]) {
									//Has been assigned, but we're the same
									//so remember these two are the same
									//Status remembers each segment and whether
									//it's been joined or not.
									int o_min, seg_min, seg_max;
									bool z, z1;
									if (s[i][j]>s[w][h]) {
										seg_max = s[i][j];
										seg_min = s[w][h];
									} else {
										seg_max = s[w][h];
										seg_min = s[i][j];
									}
									z = z1 = true;
									
									//cout << "min omin max w w1:" << seg_min << ":" << o_min << ":" << seg_max << ":" << w << ":" << w1 << ":" << s[i][j] << ":" << s[w][h] << endl;
									while (z) {
										
										if (seg_min == seg_max || sets[seg_min] == seg_max) {
											//cout << "Break because min = max" << endl;
											z=false;
										};
										if (sets[seg_min]>0 && z) {
											// If the value is already set test it vs new max
											if (sets[seg_min]<seg_max) {
												//If the new max is bigger, follow the chain
												//cout << "New max bigger, following chain: "<< seg_min << ":"<< sets[seg_min] << ":" << seg_max << endl;
												seg_min = sets[seg_min];
											} else {
												// If the new max is smaller, insert it
												//cout << "New max is smaller, inserting: " << seg_min << ":" << sets[seg_min] << ":" << seg_max << endl;
												int tmp = sets[seg_min];
												sets[seg_min] = seg_max;
												seg_min = seg_max;
												seg_max = tmp;
											}
											if (z1) {
												// We've been through more than once
												//cout << "We've been more than once" << endl;
												z1 = false;
											}
										} else if(z) {
											//cout << "=================>>>>>>>>>>>>>>>>Setting empty value: "<< seg_min << ":" << sets[seg_min] << ":" << seg_max << endl;
											sets[seg_min] = seg_max;
											z=false;
										}
									}
									//cout << "===MATCH: seg!!!!!!!!!!: " << s[i][j] << ", and " << s[w][h] << " original min is: " << o_min << " new min: " << seg_min << endl;
								}
								//cout << "END:::::::::" << s[i][j] << ":" << s[w][h] << endl;
							} else {
								//cout << "<<<<<======<<<<< " << s[i][j] << " different than: " << s[w][h] << endl;
							}

						}

					}
				}
				//Done checking for this width
			}
			//Done checking for this height
		}
		//Done checking all pixels
		cout << "Done, now count joins!" << endl;
		
		/*
		for (int i=1; i<=segments; i++) {
			cout << i << " has: " << sets[i] << endl;
		}
		*/
		int joins = 0;
		sets[0] = 0;
		for (int i=1; i<=segments; i++) {
			//cout << "counting! " << i << endl;
			if (sets[i]!=0) {
				joins+=1;
				//cout << "Set: " << i << " with val: " << sets[i] << " added to join." << endl;
			}
			int cur = i;
			bool tracing = false;
			bool deleting = true;
			while (deleting) {
				//cout << "=======cur: " << cur << endl;
				if (sets[cur]>0) {
					int tmp = cur;
					cur = sets[cur];
					sets[tmp] = 0;
					//cout << tmp << " was set to 0, next check is " << cur << endl;
					tracing = true;
				} else if (sets[cur] < 1 && tracing) {
					// We're tracing a chain and its end has already been erased which means
					// we have an overlapping chain
					//cout << cur << " is 0, deleting its join " << joins << endl;
					joins-= 1;
					//cout << " to " << joins << endl;
					deleting = false;
				} else {
					deleting = false;
				}

			}
		}
		string output, segs;
		output = "OUTPUT: ";
		stringstream tmp;
		tmp << joins;
		segs = tmp.str();
		output = output + segs;
		image.draw(DrawableText(20,20,output));
		image.write("1out.png");
		
		cout << "Total segments: " << segments << endl;
		cout << "Segments after joins: " << joins <<endl;
		cout << "Width, height, total: " << width << " " << height << " " << width*height << endl;
		
	}
	catch(Magick::Exception &error_)
	{
		cout << "Error, image not loaded";
		cout << error_.what();
	}
	return 0;
}


