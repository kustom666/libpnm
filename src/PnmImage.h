#ifndef DEF_PGM_IMAGE
#define DEF_PGM_IMAGE
#define IMG_PPM_BIN 0x01
#define IMG_PGM_BIN 0x02
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

	void appliqueSobel(int aSeuil);

	int getMType(){return mType;}
	uint8_t* getRgb();
	
private:
	void loadBin();

private:
	int mType;
	int mMinVal;
	int mMaxVal;
	bool utilise;
};

#endif