#include "PnmImage.h"
#include <cmath>

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
			mType = "Binary PGM";
			loadBin();
		}
		else if(mData[0] == (uint8_t)'P' && mData[1] == (uint8_t)'2')
		{
			mType = "ASCII PGM";
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
	file.write((char*)mPixmap, mWidth * mHeight);
}

void PnmImage::appliqueSobel()
{
	uint8_t* mPixCopy = new uint8_t[mWidth*mHeight];
	memcpy(mPixCopy, mPixmap, mWidth*mHeight);

	for(int i=1; i < mHeight - 1; i++)
	{
		for(int j=1; j< mWidth -1 ; j++)
		{
			int offPixel = i*mWidth + j;
			int valPixel = pow((-mPixCopy[offPixel-mWidth-1] -2*mPixCopy[offPixel-1] - mPixCopy[offPixel+mWidth-1] + mPixCopy[offPixel - mWidth+1] + 2*mPixCopy[offPixel +1] + mPixCopy[offPixel+mWidth+1])/4, 2);

			valPixel = valPixel + pow((mPixCopy[offPixel-mWidth-1] + 2*mPixCopy[offPixel-mWidth] + mPixCopy[offPixel-mWidth+1] - mPixCopy[offPixel + mWidth-1] - 2*mPixCopy[offPixel+mWidth] - mPixCopy[offPixel+mWidth+1])/4, 2);

			valPixel = sqrt(valPixel) / 2;
			mPixmap[offPixel] = (char)valPixel;
		}
	}

	delete[] mPixCopy;
}


void PnmImage::loadBin()
{
	int i = 0;
	while(i!=4)
	{
		if(mData[mOffsetPixmap] == (uint8_t)0x0A || (uint8_t)mData[mOffsetPixmap] == 0x20)
		{
			i++;
		}
		mOffsetPixmap++;
	}
	mHeader = new uint8_t[mOffsetPixmap];
	memcpy(mHeader, mData, mOffsetPixmap);
	int elementAGet = 0;
	for(int j=2; j < mOffsetPixmap; ++j)
	{
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
	}
	std::cout << "Width : " << mWidth << " Height : " << mHeight << " Max Value : " << mMaxVal << std::endl;
	mPixmap = new uint8_t[mWidth * mHeight];
	memcpy(mPixmap, mData+mOffsetPixmap, mWidth * mHeight);
}


void PnmImage::loadFromFile(std::string iFileName)
{
	loadFromFile(iFileName.c_str());
}
void PnmImage::saveToFile(std::string iFileName)
{
	saveToFile(iFileName.c_str());
}


void PnmImage :: historiser ()
{
	histogramme = new int [mMaxVal + 1];
	for (int t = 0; t <= mMaxVal; t ++)
	{
		histogramme [t] = 0;
	}

	for (int i = 0; i < mWidth * mHeight; i ++)
	{
			histogramme [mPixmap [i]] ++;			
	}
}

int PnmImage :: kld (int * comparaison)
{
	int difference;
	difference = 0;

	for (int i = 0; i <= mMaxVal; i ++)
	{
		if (comparaison[i] != 0)
		{
			difference += (histogramme [i] * log10 (histogramme [i] / comparaison [i]));
		}
		else
		{
			difference += histogramme [i];
		}
	}

	return difference;
}

float PnmImage :: Bhattacharyya (int * comparaison)
{
	float difference = 0;
	float * histoFloat;
	float * histoFloat2;
	histoFloat = normaliser (histogramme);
	histoFloat2 = normaliser (comparaison);

	for (int i = 0; i <= mMaxVal; i ++)
	{
		difference += sqrt(histoFloat[i] * histoFloat2[i]);
	}
	return -log(difference);
}

float * PnmImage :: normaliser (int * histogramme)
{
	float * histoFloat = new float [mMaxVal + 1];
	for(int i = 0; i <= mMaxVal; i++){
		histoFloat[i] = (float) ((float)histogramme[i] / (mWidth * mHeight));
	}

	return histoFloat;
}

void PnmImage :: moyenneGradient ()
{
	moyGradient = 0;

	for (int i = 0; i < mWidth * mHeight; i ++)
	{
		if (mPixmap [i] > SEUIL)
		{
			moyGradient += mPixmap [i];
		}
	}

	moyGradient /= (mWidth * mHeight);
}

void PnmImage :: nombreContour ()
{
	nbContour = 0;

	for (int i = 0; i < mWidth * mHeight; i ++)
	{
		if (mPixmap [i] > SEUIL)
		{
			nbContour += mPixmap [i];
		}
	}
}

void PnmImage :: histogrammeCumule ()
{
	float somme = 0;
	float * histo;
	histo = normaliser (histogramme);
	for (int i = 0; i <= mMaxVal; i++)
	{
		somme += histo[i];
	}
	std :: cout << "La somme donne : " << somme << std :: endl;
}



