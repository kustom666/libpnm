#ifndef DEF_BASEIMAGE
#define DEF_BASEIMAGE

#include <cstdint>

class BaseImage
{
public:
	BaseImage(){}
	~BaseImage(){}

	virtual void loadFromFile(char* iFileName) = 0;
	virtual void loadFromFile(std::string iFileName) = 0;

	virtual void saveToFile(char* iFileName) = 0;
	virtual void saveToFile(std::string iFileName) = 0;
	virtual void appliqueSobel(int aSeuil) = 0;

	int getMWidth(){ return mWidth; }
	int getMHeight(){ return mHeight; }
	uint8_t* getMPixmap(){return mPixmap;}
	uint8_t* getMPixmapMod(){return mPixmapMod;}

	
protected:
	int mWidth;
	int mHeight;
	int mTaille;
	int mOffsetPixmap;

	char* mFileName;
	uint8_t* mData;
	uint8_t* mPixmap;
	uint8_t* mPixmapMod;
	uint8_t* mHeader;
};

#endif
