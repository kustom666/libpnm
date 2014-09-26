#ifndef DEF_BASEIMAGE
#define DEF_BASEIMAGE

#include <cstdint>

#define SEUIL 10

class BaseImage
{
public:
	BaseImage(){}
	~BaseImage(){}

	virtual void loadFromFile(char* iFileName) = 0;
	virtual void loadFromFile(std::string iFileName) = 0;

	virtual void saveToFile(char* iFileName) = 0;
	virtual void saveToFile(std::string iFileName) = 0;
	virtual void appliqueSobel() = 0;

	virtual void historiser () = 0;
	virtual int kld (int * comparaison) = 0;
	virtual float Bhattacharyya (int * comparaison) = 0;
	virtual float * normaliser (int * histogramme) = 0;
	virtual void moyenneGradient () = 0;
	virtual void nombreContour () = 0;
	virtual void histogrammeCumule () = 0;

	int getMWidth(){ return mWidth; }
	int getMHeight(){ return mHeight; }
	uint8_t* getMPixmap(){return mPixmap;}
	int * getHistogramme () {return histogramme;}
	int getMoyGradient () {return moyGradient;}
	int getNbContour () {return nbContour;}
protected:
	int mWidth;
	int mHeight;
	int mTaille;
	int mOffsetPixmap;

	char* mFileName;
	uint8_t* mData;
	uint8_t* mPixmap;
	uint8_t* mHeader;

	int * histogramme;
	int moyGradient;
	int nbContour;
};

#endif
