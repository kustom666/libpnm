#ifndef DEF_BASEIMAGE
#define DEF_BASEIMAGE

class BaseImage
{
public:
	BaseImage(){}
	~BaseImage(){}

	virtual void loadFromFile(char* iFileName) = 0;
	int getMWidth(){ return mWidth }
	int getMHeight(){ return mHeight }
	
protected:
	int mWidth;
	int mHeight;
	int mTaille;
	uint8_t* mData;

};

#endif
