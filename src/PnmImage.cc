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
				int valPixel = pow((-mPixCopy[offPixel-mWidth-1] -2*mPixCopy[offPixel-1] - mPixCopy[offPixel+mWidth-1] + mPixCopy[offPixel - mWidth+1] +2*mPixCopy[offPixel +1] + mPixCopy[offPixel+mWidth+1])/4, 2);
				valPixel = valPixel + pow((mPixCopy[offPixel-mWidth-1] +2*mPixCopy[offPixel-mWidth] + mPixCopy[offPixel-mWidth+1] - mPixCopy[offPixel + mWidth-1] - 2*mPixCopy[offPixel+mWidth] - mPixCopy[offPixel+mWidth+1])/4, 2);
				valPixel = sqrt(valPixel) / 2;
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
				int valPixelR = pow((-mPixCopy[offPixel-mWidth-1] -2*mPixCopy[offPixel-1] - mPixCopy[offPixel+mWidth-1] + mPixCopy[offPixel - mWidth+1] +2*mPixCopy[offPixel +1] + mPixCopy[offPixel+mWidth+1])/4, 2);
				valPixelR = valPixelR + pow((mPixCopy[offPixel-mWidth-1] +2*mPixCopy[offPixel-mWidth] + mPixCopy[offPixel-mWidth+1] - mPixCopy[offPixel + mWidth-1] - 2*mPixCopy[offPixel+mWidth] - mPixCopy[offPixel+mWidth+1])/4, 2);
				valPixelR = sqrt(valPixelR) / 2;
				mPixmap[offPixel] = (char)valPixelR;

				// Sobel sur G
				int valPixelG = pow((-mPixCopy[offPixel+1-mWidth-1] -2*mPixCopy[offPixel+1-1] - mPixCopy[offPixel+1+mWidth-1] + mPixCopy[offPixel+1 - mWidth+1] +2*mPixCopy[offPixel+1 +1] + mPixCopy[offPixel+1+mWidth+1])/4, 2);
				valPixelG = valPixelG + pow((mPixCopy[offPixel+1-mWidth-1] +2*mPixCopy[offPixel+1-mWidth] + mPixCopy[offPixel+1-mWidth+1] - mPixCopy[offPixel+1 + mWidth-1] - 2*mPixCopy[offPixel+1+mWidth] - mPixCopy[offPixel+1+mWidth+1])/4, 2);
				valPixelG = sqrt(valPixelG) / 2;
				mPixmap[offPixel+1] = (char)valPixelG;

				// Sobel sur B
				int valPixelB = pow((-mPixCopy[offPixel+2-mWidth-1] -2*mPixCopy[offPixel+2-1] - mPixCopy[offPixel+2+mWidth-1] + mPixCopy[offPixel+2 - mWidth+1] +2*mPixCopy[offPixel+2 +1] + mPixCopy[offPixel+2+mWidth+1])/4, 2);
				valPixelB = valPixelB + pow((mPixCopy[offPixel+2-mWidth-1] +2*mPixCopy[offPixel+2-mWidth] + mPixCopy[offPixel+2-mWidth+1] - mPixCopy[offPixel+2 + mWidth-1] - 2*mPixCopy[offPixel+2+mWidth] - mPixCopy[offPixel+2+mWidth+1])/4, 2);
				valPixelB = sqrt(valPixelB) / 2;
				mPixmap[offPixel+2] = (char)valPixelB;
			}
		}
		delete[] mPixCopy;
	}
}

int PnmImage::getTauxRouge()
{
	uint8_t* rgb = getRgb(); 
	int somme;
	for(int i=0; i< mWidth*mHeight*3; i+=3)
	{
		somme += rgb[i];
	}
	somme /= mWidth * mHeight;
	return somme;
}

int PnmImage::getTauxVert()
{
	uint8_t* rgb = getRgb(); 
	int somme;
	for(int i=1; i< mWidth*mHeight*3; i+=3)
	{
		somme += rgb[i];
	}
	somme /= mWidth * mHeight;
	return somme;
}	


int PnmImage::getTauxBleu()
{
	uint8_t* rgb = getRgb(); 
	int somme;
	for(int i=2; i< mWidth*mHeight*3; i+=3)
	{
		somme += rgb[i];
	}
	somme /= mWidth * mHeight;
	return somme;
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


void PnmImage::historiser ()
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

int PnmImage::kld (int * comparaison)
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

float PnmImage::Bhattacharyya (int * comparaison)
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

float * PnmImage::normaliser (int * histogramme)
{
	float * histoFloat = new float [mMaxVal + 1];
	for(int i = 0; i <= mMaxVal; i++){
		histoFloat[i] = (float) ((float)histogramme[i] / (mWidth * mHeight));
	}

	return histoFloat;
}

void PnmImage::moyenneGradient (int aSeuil)
{
	moyGradient = 0;

	for (int i = 0; i < mWidth * mHeight; i ++)
	{
		if (mPixmap [i] > aSeuil)
		{
			moyGradient += mPixmap [i];
		}
	}

	moyGradient /= (mWidth * mHeight);
}

void PnmImage::nombreContour (int aSeuil)
{
	nbContour = 0;

	for (int i = 0; i < mWidth * mHeight; i ++)
	{
		if (mPixmap [i] > aSeuil)
		{
			nbContour += mPixmap [i];
		}
	}
}

void PnmImage::histogrammeCumule ()
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

int PnmImage::comparaison (PnmImage *compare, int mode)
{
	int compare1, compare2, res;
	switch(mode)
	{
		case 1 :
			std::cout << "comparaison kld : " << std::endl;
			if(abs(this->kld ((*compare).getHistogramme ())) < 1000)
			{
				res = 1;
			}		
			else
			{
				res = 0;
			}
			break;
		case 2 :
			std::cout << "comparaison battacharyya : " << std::endl;
			if(this->Bhattacharyya (compare->getHistogramme ()) < 1)
			{
				res = 1;
			}		
			else
			{
				res = 0;
			}
			break;
		case 3 :
			std::cout << "comparaison moyenne du gradient : " << std::endl;
			(*compare).moyenneGradient (0);
			this->moyenneGradient (0);
			compare1 = compare->getMoyGradient ();
			compare2 = this->getMoyGradient ();
			if(abs(compare2 - compare1) < 2)
			{
				res = 1;
			}
			else
			{
				res = 0;
			}
			break;
		case 4 :
			std::cout << "comparaison nb de contour : " << std::endl;
			(*compare).nombreContour (0);
			this->nombreContour (0);
			compare1 = compare->getNbContour ();
			compare2 = this->getNbContour ();
			if(abs(compare2 - compare1) < 200)
			{
				res = 1;
			}
			else
			{
				res = 0;
			}
			break;
		default :
			res =  0;
			break;
	}
	return res;
}



