CC=g++
CXXFLAGS=-Wall -Wno-write-strings -std=c++11
LDFLAGS=
BUILDFOLD=build
all: pnmview-static pnmparse-static

pnmview-static: pnmview.cc libpxm-static
	$(CC) -c pnmview.cc -I./headers $(CXXFLAGS)
	$(CC) pnmview.o -o pnmview -L. -lpxm -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLEW
	cp pnmview bin/pnmview

pnmparse-static: pnmparse.cc libpxm-static
	$(CC) -c pnmparse.cc -I./headers $(CXXFLAGS)
	$(CC) pnmparse.o -o pnmparse -L. -lpxm -lsqlite3
	cp pnmparse bin/pnmparse

pnmview: pnmview.cc libpxm
	$(CC) -c pnmview.cc -I./headers $(CXXFLAGS)
	$(CC) pnmview.o -o pnmview -L. -lpxm

libpxm: PnmImage.o
	$(CC) -shared -Wl,-soname,libpxm.so.1 -o libpxm.so PnmImage.o  $(LDFLAGS)
	cp src/*.h headers/
	cp libpxm.so libpxm.so.1

PnmImage.o: src/PnmImage.cc src/PnmImage.h
	$(CC) -fPIC -c src/PnmImage.cc $(CXXFLAGS)

libpxm-static: PnmImage.o src/NotEnoughPixelsException.h
	ar rvs libpxm.a PnmImage.o
	cp src/*.h headers/

clean:
	rm -f *.o