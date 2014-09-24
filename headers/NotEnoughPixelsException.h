#ifndef DEF_NOTENOUGHPIXELS
#define DEF_NOTENOUGHPIXELS

#include <exception>

class NotEnoughPixelsException : public std::exception
{

public:
	NotEnoughPixelsException();
	~NotEnoughPixelsException();
	virtual const char* what() const throw(){return "L'image a déclaré un nombre de pixel supérieur aux nombre de pixels dans le fichiers";}
}NotEnoughPixelsException;

#endif