#pragma once

//https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
namespace BMPFileWriter
{
	void GenerateBitmapImage(unsigned char* image, int height, int width, const char* imageFileName);
};
