CC=g++
CXXFLAGS=-Wall
LDFLAGS=
BUILDFOLD=build
all: libpxm

libpxm: PgmImage.o
	$(CC) -shared -Wl,-soname,libpxm.so.1 -o libpxm.so $(LDFLAGS)

PgmImage.o: src/PgmImage.cc src/PgmImage.h
  $(CC) -c -fPIC src/PgmImage.cc $(CXXFLAGS)

