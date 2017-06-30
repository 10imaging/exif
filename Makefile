CXX := g++
#CXXFLAGS=-O2 -pedantic -Wall -Wextra -ansi -std=c++11
CXXFLAGS := -g -pedantic -Wall -Wextra -Wno-unused-parameter -ansi -std=c++11

ifeq ($(DEBUG), 1)
	CXXFLAGS += -DDEBUG
endif

all: exifprint

exif.o: exif.cpp
	$(CXX) $(CXXFLAGS) -c exif.cpp

exifprint: exif.o exifprint.cpp
	$(CXX) $(CXXFLAGS) -o exifprint exif.o exifprint.cpp

clean:
	rm -f *.o exifprint exifprint.exe
	
format:
	clang-format -style=Google -i exifprint.cpp exif.cpp exif.h
	
test: exifprint valgrind
	./test.sh

valgrind: 
	valgrind -v --leak-check=full --track-origins=yes --show-leak-kinds=definite,indirect,possible,reachable ./exifprint test-images/test1.jpg
	
contrib: format test valgrind