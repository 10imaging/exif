#include <stdio.h>
#include "exif.h"


int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: demo <JPEG file>\n");
    return -1;
  }
  exif::EXIFInfo *exifInfo = new exif::EXIFInfo; 
  if (!exifInfo->readEXIF(argv[1]))
	  printf("Error reading file %s",argv[1]);

  std::string output = exifInfo->toString();
  printf("%s",output.c_str());
  
  std::string newOutput = std::string("test.jpg");

  return 0;
}
