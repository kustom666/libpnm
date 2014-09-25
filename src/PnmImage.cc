#include "PnmImage.h"

PnmImage::PnmImage() : BaseImage(),
	mMinVal(0),
	utilise(false)
{
	mWidth = 0;
	mHeight = 0;
	mTaille = 0;
	mOffsetPixmap = 0;

}

PnmImage::~PnmImage()
{
	if(utilise)
	{
		delete[] mPixmap;
		delete[] mHeader;
	}
}

void PnmImage::loadFromFile(char* iFileName)
{
	try
	{
		std::ifstream file(iFileName, std::ios::in | std::ios::binary | std::ios::ate);
		std::streampos size;
		size = file.tellg();
		file.seekg(0, std::ios::beg);

		mData = new uint8_t [size];
		mTaille = size;
		file.read((char *)mData, size);

		if(mData[0] == (uint8_t)'P' && mData[1] == (uint8_t)'5')
		{
			mType = IMG_PGM_BIN;
			loadBin();
		}
		else if(mData[0] == (uint8_t)'P' && mData[1] == (uint8_t)'6')
		{
			mType = IMG_PPM_BIN;
			loadBin();
		}
		utilise = true;
	}catch(std::exception e)
	{
		std::cout << e.what() <<std::endl;
	}
}

void PnmImage::saveToFile(char* iFileName)
{
	std::ofstream file(iFileName, std::ios::out | std::ios::binary);
	file.write((char*)mHeader, mOffsetPixmap);
	if(mType == IMG_PGM_BIN)
	{
		file.write((char*)mPixmap, mWidth * mHeight);
	}
	else
	{
		file.write((char*)mPixmap, mWidth * mHeight * 3);
	}
}

void PnmImage::appliqueSobel(int aSeuil)
{
	if(mType ==  IMG_PGM_BIN)
	{
		uint8_t* mPixCopy = new uint8_t[mWidth*mHeight];
		memcpy(mPixCopy, mPixmap, mWidth*mHeight);
		for(int i=1; i < mHeight - 1; i++)
		{
			for(int j=1; j< mWidth -1 ; j++)
			{
				int offPixel = i*mWidth + j;
				int valPixel = (-mPixCopy[offPixel-mWidth-1] -2*mPixCopy[offPixel-1] - mPixCopy[offPixel+mWidth-1] + mPixCopy[offPixel - mWidth+1] +2*mPixCopy[offPixel +1] + mPixCopy[offPixel+mWidth+1])/4;
				valPixel = valPixel + (mPixCopy[offPixel-mWidth-1] +2*mPixCopy[offPixel-mWidth] + mPixCopy[offPixel-mWidth+1] - mPixCopy[offPixel + mWidth-1] - 2*mPixCopy[offPixel+mWidth] - mPixCopy[offPixel+mWidth+1])/4;
				valPixel /=2;
				if(valPixel >= aSeuil)
				{
					mPixmapMod[offPixel] = (char)255;
				}
				else
				{
					mPixmapMod[offPixel] = (char)0;
				}
			}
		}
		delete[] mPixCopy;
	}
	else
	{
		uint8_t* mPixCopy = new uint8_t[mWidth*mHeight*3];
		memcpy(mPixCopy, mPixmap, mWidth*mHeight*3);
		for(int i=1; i < mHeight - 1; i++)
		{
			for(int j=1; j< mWidth -1 ; j++)
			{
				int offPixel = i*mWidth + j;
				// Sobel sur R
				int valPixelR = (-mPixCopy[offPixel-mWidth-1] -2*mPixCopy[offPixel-1] - mPixCopy[offPixel+mWidth-1] + mPixCopy[offPixel - mWidth+1] +2*mPixCopy[offPixel +1] + mPixCopy[offPixel+mWidth+1])/4;
				valPixelR = valPixelR + (mPixCopy[offPixel-mWidth-1] +2*mPixCopy[offPixel-mWidth] + mPixCopy[offPixel-mWidth+1] - mPixCopy[offPixel + mWidth-1] - 2*mPixCopy[offPixel+mWidth] - mPixCopy[offPixel+mWidth+1])/4;
				valPixelR /=2;
				mPixmap[offPixel] = (char)valPixelR;

				// Sobel sur G
				int valPixelG = (-mPixCopy[offPixel+1-mWidth-1] -2*mPixCopy[offPixel+1-1] - mPixCopy[offPixel+1+mWidth-1] + mPixCopy[offPixel+1 - mWidth+1] +2*mPixCopy[offPixel+1 +1] + mPixCopy[offPixel+1+mWidth+1])/4;
				valPixelG = valPixelG + (mPixCopy[offPixel+1-mWidth-1] +2*mPixCopy[offPixel+1-mWidth] + mPixCopy[offPixel+1-mWidth+1] - mPixCopy[offPixel+1 + mWidth-1] - 2*mPixCopy[offPixel+1+mWidth] - mPixCopy[offPixel+1+mWidth+1])/4;
				valPixelG /=2;
				mPixmap[offPixel+1] = (char)valPixelG;

				// Sobel sur B
				int valPixelB = (-mPixCopy[offPixel+2-mWidth-1] -2*mPixCopy[offPixel+2-1] - mPixCopy[offPixel+2+mWidth-1] + mPixCopy[offPixel+2 - mWidth+1] +2*mPixCopy[offPixel+2 +1] + mPixCopy[offPixel+2+mWidth+1])/4;
				valPixelB = valPixelB + (mPixCopy[offPixel+2-mWidth-1] +2*mPixCopy[offPixel+2-mWidth] + mPixCopy[offPixel+2-mWidth+1] - mPixCopy[offPixel+2 + mWidth-1] - 2*mPixCopy[offPixel+2+mWidth] - mPixCopy[offPixel+2+mWidth+1])/4;
				valPixelB /=2;
				mPixmap[offPixel+2] = (char)valPixelB;
			}
		}
		delete[] mPixCopy;
	}
}


void PnmImage::loadBin()
{
	int i = 0;
	while(i!=4)
	{
		if(mData[mOffsetPixmap] == (uint8_t)0x23)
		{
			while(mData[mOffsetPixmap] != 0x0A)
			{
				mOffsetPixmap++;
			}
			mOffsetPixmap++;
		}
		if(mData[mOffsetPixmap] == (uint8_t)0x0A || (uint8_t)mData[mOffsetPixmap] == 0x20)
		{
			i++;
		}
		mOffsetPixmap++;
	}
	std::cout << "Offset pixmap : " << mOffsetPixmap << std::endl;
	mHeader = new uint8_t[mOffsetPixmap];
	memcpy(mHeader, mData, mOffsetPixmap);
	int elementAGet = 0;
	for(int j=2; j < mOffsetPixmap; ++j)
	{
		if(mData[j] == (uint8_t)0x23)
		{
			while(mData[j] != 0x0A)
			{
				j++;
			}
			j++;
		}
		if(mData[j] != (uint8_t)0x0A && mData[j] != (uint8_t)0x20)
		{
			int offElement = j;
			while(mData[j] != (uint8_t)0x0A && mData[j] != (uint8_t)0x20)
			{
				j++;
			}
			char* repElement = new char[j-offElement];
			memcpy(repElement, mData+offElement, j-offElement);
			if(elementAGet == 0)
			{
				mWidth = atoi(repElement);
			}
			else if(elementAGet == 1)
			{
				mHeight = atoi(repElement);
			}
			else if(elementAGet == 2)
			{
				mMaxVal = atoi(repElement);
			}
			elementAGet++;
			delete[] repElement;
		}
	}
	if(mWidth*mHeight > mTaille - mOffsetPixmap)
	{
		std::cout << "Erreur taille" << std::endl;
	}
	std::cout << "Width : " << mWidth << " Height : " << mHeight << " Max Value : " << mMaxVal << " Offset debut données : " << mOffsetPixmap << std::endl;
	if(mType == IMG_PGM_BIN)
	{
		mPixmap = new uint8_t[mWidth * mHeight];
		mPixmapMod = new uint8_t[mWidth * mHeight];

		memcpy(mPixmap, &mData[mOffsetPixmap], mWidth * mHeight);
		memcpy(mPixmapMod, &mData[mOffsetPixmap], mWidth * mHeight);
	}
	else
	{
		mPixmap = new uint8_t[mWidth*mHeight*3];
		mPixmapMod = new uint8_t[mWidth*mHeight*3];

		memcpy(mPixmap, &mData[mOffsetPixmap], mWidth*mHeight*3);
		memcpy(mPixmapMod, &mData[mOffsetPixmap], mWidth*mHeight*3);

	}
}

uint8_t* PnmImage::getRgb()
{
	uint8_t* output;
	if(mType == IMG_PGM_BIN)
	{
		output = new uint8_t[mWidth*mHeight*3];
		int j=0;
		for(int i=0; i < mWidth*mHeight; i++)
		{
			output[j] = mPixmapMod[i];
			output[j+1] = mPixmapMod[i];
			output[j+2] = mPixmapMod[i];

			j+=3;
		}
	}
	else
	{
		output = mPixmapMod;
	}
	return output;
}
void PnmImage::loadFromFile(std::string iFileName)
{
	loadFromFile(iFileName.c_str());
}
void PnmImage::saveToFile(std::string iFileName)
{
	saveToFile(iFileName.c_str());
}