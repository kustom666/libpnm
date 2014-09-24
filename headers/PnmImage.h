#ifndef DEF_PGM_IMAGE
#define DEF_PGM_IMAGE
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "BaseImage.h"

class PnmImage : public BaseImage
{
public:
	PnmImage();
	~PnmImage();
	void loadFromFile(char* iFileName);
	void saveToFile(char* iFileName);

	void loadFromFile(std::string iFileName);
	void saveToFile(std::string iFileName);

	void appliqueSobel();

	std::string getMType(){return mType;}

private:
	void loadBin();

private:
	std::string mType;
	int mMinVal;
	int mMaxVal;
};

#endif