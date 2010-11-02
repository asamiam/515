/*
 *  segment.cpp
 *  
 *
 *  Created by Ace Sorensen on 9/16/10.
 *		Code used to segment images and then count the
 *		number of segments.
 *
 */
#include <Magick++.h>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;
using namespace Magick;

int main(int argc, char * argv[])
{
	cout << "CSE 515 Project 1 - Part 1" << endl;

	if(argc != 2)
	{
		cout << "Error parsing command line arguments." << endl;
		cout << "Usage: \nmoment <image-name> <p> <q>";
		return 1;
	}

	// Get imageName
	char *imageName;
	int p, q;
	imageName = argv[1];
	
	cout << "ImageName: " << imageName << endl;
	
	InitializeMagick(*argv);
	try
	{
		Image image(imageName);
		cout <<	"Accepted the image, MOMENTING" << endl;
		p=0; q=2;
		
		//Expecting black and white RGB, which would allow the ColorMono class
		ColorMono color;
		
		//Get the image dimensions
		Geometry dim = image.size();
		size_t width = dim.width();
		size_t height = dim.height();
		cout << "Image width: " << width << endl;
		cout << "Image height: " << height << endl;
		
		int s[width][height];
		long xavg,yavg,moment,total;
		xavg=0; yavg=0; total=0; moment=0;
		
		//Initialize all pixels, those in the image are black and get
		// a vote, otherwise 0.
		for (size_t j=0; j<height; j++) {
			for (size_t i=0; i<width; i++) {
				//ColorMono is false for black, true for white
				color = image.pixelColor(i,j);
				s[i][j]=color.mono() ? 0 : 1;
				
				//cout << "x: " << i << " y: " << j << " black? " << s[i][j] << endl;
				//Now get the center of gravity;
				if(s[i][j]==1){
					//It's in the image
					xavg+=i;
					yavg+=j;
					total+=1;
				}
			}
		};
		
		xavg=xavg/total;
		yavg=yavg/total;
		
		cout << "xavg: " << xavg << " yavg: " << yavg << endl;
		
		//Now calculate the Moment
		for (size_t j=0; j<height; j++) {
			for (size_t i=0; i<width; i++) {
				if (s[i][j]==1) {
					int x = i-xavg;
					int y = j-yavg;
					int px = pow(x,p);
					int qy = pow(y,q);
					//cout << "x, px: " << x << ":" << px << "y, py: " << y << ":" << qy << endl;
					moment+= px*qy;
				}
			}
		}
		
		
		cout << "Moment val: " << moment << endl;
	}
	catch(Magick::Exception &error_)
	{
		cout << "Error, image not loaded";
		cout << error_.what();
	}
	return 0;
}


