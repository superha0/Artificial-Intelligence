TARGETS = assignment2_2015058222.exe
OBJECTS = assignment2_2015058222.cc
CC = x86_64-w64-mingw32-g++
CPPFLAGS = -static -std=c++11 -O3

all : $(TARGETS)

assignment2_2015058222.exe : $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $@ $^

clean :
	rm $(TARGETS) 
