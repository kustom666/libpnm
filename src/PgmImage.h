#ifndef DEF_PGM_IMAGE
#define DEF_PGM_IMAGE
#include <iostream>
#include <fstream>

#include "BaseImage.h"

class PgmImage : public BaseImage
{
public:
	PgmImage();
	~PgmImage();
	void loadFromFile(char* iFileName);

private:
	string mType;
	
};

#endif