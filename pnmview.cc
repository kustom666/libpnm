#include <iostream>
#include <PnmImage.h>


int main(int argc, char const *argv[])
{
	PnmImage im1, im2;
	char fileALoad[255];
	char fileASave[255];
	/*float diff = 0;*/

	std::cout << "Entrer le path de l'image 1" << std::endl;
	std::cin >> fileALoad;
	im1.loadFromFile(fileALoad);
	/*std::cout << "Entrer le path de l'image 2" << std::endl;
	std::cin >> fileALoad;
	im2.loadFromFile(fileALoad);

	im1.historiser ();
	im2.historiser ();

	diff = im1.Bhattacharyya(im2.getHistogramme ());

	std :: cout << "diff batavia = " << diff << std :: endl;

	diff = im1.kld (im2.getHistogramme ());

	std :: cout << "diff kld = " << diff << std :: endl;
	
	std :: cout << "diff kldNormalisé = " << diff << std :: endl;

	im1.histogrammeCumule ();*/



	im1.appliqueSobel();
	im1.moyenneGradient ();
	im1.nombreContour ();
	std::cout << "MoyGradient : " << im1.getMoyGradient () << std::endl;
	std::cout << "NbContour : " << im1.getNbContour () << std::endl;

	std::cout << "Entrer le path de l'image à sauver" << std::endl;
	std::cin >> fileASave;
	im1.saveToFile(fileASave);
	return 0;
}
