#include <iostream>
#include <PnmImage.h>
#include <dirent.h>
#include <sqlite3.h>

static int callbackPrint(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
	  printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char const *argv[])
{
	int dbHandle;
	sqlite3 *db;
	char* errMsg = 0;
	dbHandle = sqlite3_open("dbImages.sqlite", &db);
	if(dbHandle)
	{
		fprintf(stderr, "Impossible d'ouvrir la base de données dbImages.sqlite : %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

	dbHandle = sqlite3_exec(db, "CREATE TABLE Metadata(id int, type varchar(255), moyenne_rouge int, moyenne_vert int, moyenne_bleu int, nombre_contour int, moyenne_gradient int, PRIMARY KEY (id));",
					  callbackPrint, 0, &errMsg);

	if(dbHandle != SQLITE_OK)
	{
		fprintf(stderr, "Erreur sql : %s\n", errMsg);
		sqlite3_free(errMsg);
		return 1;	
	}

	dbHandle = sqlite3_exec(db, "CREATE TABLE Images(id int, nom varchar(255), hauteur int, largeur int, id_metadata int, PRIMARY KEY (id), FOREIGN KEY (id_metadata) REFERENCES Metadata(id));",
					  callbackPrint, 0, &errMsg);

	if(dbHandle != SQLITE_OK)
	{
		fprintf(stderr, "Erreur sql : %s\n", errMsg);
		sqlite3_free(errMsg);
		return 1;	
	}

	int idImage = 0;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (".")) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {

	  	PnmImage im;
	  	if(ent->d_name[0] != '.' && ent->d_name[1] != '.')
	  	{
	  		int last = 0, cont = 0;
	  		while(cont == 0 && last < 256)
	  		{
	  			if(ent->d_name[last] == '\0')
	  			{
	  				cont = 1;
	  				last--;
	  			}
	  			else
	  			{
	  				last++;
	  			}
	  		}
	  		if(ent->d_name[last-2] == 'p' && (ent->d_name[last-1] == 'n' || ent->d_name[last-1] == 'g') && ent->d_name[last] == 'm')
	  		{
	  			PnmImage im;
	  			im.loadFromFile(ent->d_name);
	    		printf ("%s\n", ent->d_name);
	    		char requete[2048];
	    		sprintf(requete, "INSERT INTO Images(id, nom, hauteur, largeur) VALUES(%d,\'%s\', %d, %d)", idImage, ent->d_name, im.getMHeight(), im.getMWidth());
	    		dbHandle = sqlite3_exec(db, requete, callbackPrint, 0, &errMsg);
				if(dbHandle != SQLITE_OK)
				{
					fprintf(stderr, "Erreur sql : %s\n", errMsg);
					sqlite3_free(errMsg);
					return 1;	
				}
				idImage ++;
	    	}
	    }
	  }
	  closedir (dir);
	} else {
	  /* could not open directory */
	  perror ("Impossible d'ouvrir le repertoire courant\n");
	  return EXIT_FAILURE;
	}
	sqlite3_close(db);
	return 0;
}