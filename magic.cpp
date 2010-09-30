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

using namespace std;
using namespace Magick;

int main(int argc, char * argv[])
{
	cout << "CSE 515 Project 1 - SIFT Prep" << endl;

	if(argc != 2)
	{
		cout << "Error parsing command line arguments." << endl;
		cout << "Usage: \nmagic <image-name>";
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
		image.quantizeColorSpace(GRAYColorspace);
		image.write("test.pgm");
		
	}
	catch(Magick::Exception &error_)
	{
		cout << "Error, image not loaded";
		cout << error_.what();
	}
	return 0;
}


