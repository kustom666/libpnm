#include <iostream>
#include <PnmImage.h>
#include <dirent.h>
#include <sqlite3.h>

int main(int argc, char const *argv[])
{
	int dbHandle;
	sqlite3 *db;
	dbHandle = sqlite3_open("dbImages.sqlite", &db);
	if(dbHandle)
	{
		fprintf(stderr, "Impossible d'ouvrir la base de données dbImages.sqlite : %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

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
	  		if(ent->d_name[last-2] == 'p' && ent->d_name[last-1] == 'n' && ent->d_name[last] == 'm')
	  		{
	    		printf ("%s\n", ent->d_name);
	    	}
	    }
	  }
	  closedir (dir);
	} else {
	  /* could not open directory */
	  perror ("Impossible d'ouvrir le repertoire courant\n");
	  return EXIT_FAILURE;
	}
	return 0;
}