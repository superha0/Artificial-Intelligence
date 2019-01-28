.SUFFIXES : .cc .o
TARGETS = ids.exe gbs.exe ass.exe
OBJECTS1 = assignment1_2015058222_IDS.cc
OBJECTS2 = assignment1_2015058222_GBS.cc
OBJECTS3 = assignment1_2015058222_ASS.cc
CC = x86_64-w64-mingw32-g++
CPPFLAGS = -static -std=c++11
all : makedir $(TARGETS)
makedir:
		if [ ! -d "./bin" ]; then \
			mkdir bin; \
		fi
ids.exe : $(OBJECTS1)
		  $(CC) $(CPPFLAGS) -o bin/$@ $^
gbs.exe : $(OBJECTS2)
	      $(CC) $(CPPFLAGS) -o bin/$@ $^
ass.exe : $(OBJECTS3)
	      $(CC) $(CPPFLAGS) -o bin/$@ $^
clean : 
		rm -rf ./bin
