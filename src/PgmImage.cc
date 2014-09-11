#include "PgmImage.h"

PgmImage::PgmImage()
{

}

PgmImage::~PgmImage()
{

}

void PgmImage::loadFromFile(char* iFileName)
{
	std::ifstream file(iFileName, ios::in | ios::binary | ios::ate);
	std::streampos size;
	size = file.tellg();
	file.seekg(0, ios::beg);
}