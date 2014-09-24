CC=g++
CXXFLAGS=-Wall -Wno-write-strings -std=c++11
LDFLAGS=
BUILDFOLD=build
all: pnmview

pnmview-static: pnmview.cc libpxm-static
	$(CC) -c pnmview.cc -I./headers $(CXXFLAGS)
	$(CC) pnmview.o -o pnmview -L. -lpxm

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