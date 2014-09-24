#include <iostream>
#include <PnmImage.h>

int main(int argc, char const *argv[])
{
	PnmImage im;
	char fileALoad[255];
	char fileASave[255];

	std::cout << "Entrer le path de l'image à sobeliser" << std::endl;
	std::cin >> fileALoad;
	im.loadFromFile(fileALoad);
	im.appliqueSobel();
	std::cout << "Entrer le path de l'image à sauver" << std::endl;
	std::cin >> fileASave;
	im.saveToFile(fileASave);
	return 0;
}