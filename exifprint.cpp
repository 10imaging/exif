#include <stdio.h>
#include "exif.h"

const char *tr_ResolutionUnit(unsigned int ru) {
  switch (ru) {
    case 1:
      return "none";
    case 2:
      return "inches";
    case 3:
      return "cm";
    default:
      return "";
  }
}

std::string tr_ComponentsConfiguration(unsigned char cc[4]) {
  std::string rval;
  for (int i = 0; i < 4; i++) {
    switch (cc[i]) {
      case 1:
        rval.append("Y ");
        break;
      case 2:
        rval.append("Cb ");
        break;
      case 3:
        rval.append("Cr ");
        break;
      case 4:
        rval.append("R ");
        break;
      case 5:
        rval.append("G ");
        break;
      case 6:
        rval.append("B ");
        break;
      case 0:
      default:
        rval.append("");
        break;
    }
  }
  return rval;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: demo <JPEG file>\n");
    return -1;
  }

  // Read the JPEG file into a buffer
  FILE *fp = fopen(argv[1], "rb");
  if (!fp) {
    printf("Can't open file.\n");
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  unsigned long fsize = ftell(fp);
  rewind(fp);
  unsigned char *buf = new unsigned char[fsize];
  if (fread(buf, 1, fsize, fp) != fsize) {
    printf("Can't read file.\n");
    delete[] buf;
    return -2;
  }
  fclose(fp);

  // Parse EXIF
  exif::EXIFInfo result;
  int code = result.read(buf, fsize);
  if (code) {
    printf("Error parsing EXIF: code %d\n", code);
    return -3;
  }

  // Dump EXIF information
  printf("Camera make          : %s\n", result.Make.c_str());
  printf("Camera model         : %s\n", result.Model.c_str());
  printf("Software             : %s\n", result.Software.c_str());
  printf("Bits per sample      : %d\n", result.BitsPerSample);
  printf("Image width          : %d\n", result.ImageWidth);
  printf("Image height         : %d\n", result.ImageHeight);
  printf("ColorSpace           : %d\n", result.ColorSpace);
  printf("FlashpixVersion      : %s\n", result.FlashpixVersion.c_str());
  printf("InteropOffset        : %d\n", result.InteropOffset);
  printf("Image description    : %s\n", result.ImageDescription.c_str());
  printf("Image orientation    : %d\n", result.Orientation);
  printf("Xresolution          : %.0f\n", result.Xresolution);
  printf("Yresolution          : %.0f\n", result.Yresolution);
  printf("ResolutionUnit       : %d (%s)\n", result.ResolutionUnit,
         tr_ResolutionUnit(result.ResolutionUnit));
  printf("Image copyright      : %s\n", result.Copyright.c_str());
  printf("YCbCrPositioning     : %d\n", result.YCbCrPositioning);
  printf("ComponentsConfiguration : %s\n",
         tr_ComponentsConfiguration(result.ComponentsConfiguration).c_str());
  printf("ExifVersion          : %s\n", result.ExifVersion.c_str());
  printf("Image date/time      : %s\n", result.DateTime.c_str());
  printf("Original date/time   : %s\n", result.DateTimeOriginal.c_str());
  printf("Digitize date/time   : %s\n", result.DateTimeDigitized.c_str());
  printf("Subsec time          : %s\n", result.SubSecTime.c_str());
  printf("Subsec Original time : %s\n", result.SubSecTimeOriginal.c_str());
  printf("SubSecTimeDigitized  : %s\n", result.SubSecTimeDigitized.c_str());
  printf("UserComment          : \n%s", result.UserComment.c_str());
  printf("Exposure time        : 1/%d s\n",
         (unsigned)(1.0 / result.ExposureTime));
  printf("F-stop               : f/%.1f\n", result.FNumber);
  printf("ShutterSpeedValue    : %f s\n", result.ShutterSpeedValue);
  printf("ApertureValue        : f/%.2f\n", result.ApertureValue);
  printf("Exposure program     : %d\n", result.ExposureProgram);
  printf("ISO speed            : %d\n", result.ISOSpeedRatings);
  printf("MaxApertureValue     : %f m\n", result.MaxApertureValue);
  printf("Subject distance     : %f m\n", result.SubjectDistance);
  printf("Exposure bias        : %f EV\n", result.ExposureBiasValue);
  printf("Flash used?          : %d\n", result.Flash);
  printf("Flash returned light : %d\n", result.FlashReturnedLight);
  printf("Flash mode           : %d\n", result.FlashMode);
  printf("Metering mode        : %d\n", result.MeteringMode);
  printf("Lens focal length    : %f mm\n", result.FocalLength);
  printf("SceneType            : %u\n", result.SceneType);
  printf("CustomRendered       : %u - Normal(0) Custom(1)\n",
         result.CustomRendered);
  printf("ExposureMode         : %u - Auto(0) Manual(1) Auto-Bracket(2)\n",
         result.ExposureMode);
  printf("WhiteBalance         : %u - Auto(0) Manual(1)\n",
         result.WhiteBalance);
  printf("35mm focal length    : %u mm\n", result.FocalLengthIn35mm);
  printf("SceneCaptureType     : %u\n", result.SceneCaptureType);
  printf("GPS Latitude         : %f deg (%f deg, %f min, %f sec %c)\n",
         result.GeoLocation.Latitude, result.GeoLocation.LatComponents.degrees,
         result.GeoLocation.LatComponents.minutes,
         result.GeoLocation.LatComponents.seconds,
         result.GeoLocation.LatComponents.direction);
  printf("GPS Longitude        : %f deg (%f deg, %f min, %f sec %c)\n",
         result.GeoLocation.Longitude, result.GeoLocation.LonComponents.degrees,
         result.GeoLocation.LonComponents.minutes,
         result.GeoLocation.LonComponents.seconds,
         result.GeoLocation.LonComponents.direction);
  printf("GPS Altitude         : %f m\n", result.GeoLocation.Altitude);
  printf("GPS Precision (DOP)  : %f\n", result.GeoLocation.DOP);
  printf("Lens min focal length: %f mm\n", result.LensInfo.FocalLengthMin);
  printf("Lens max focal length: %f mm\n", result.LensInfo.FocalLengthMax);
  printf("Lens f-stop min      : f/%.1f\n", result.LensInfo.FStopMin);
  printf("Lens f-stop max      : f/%.1f\n", result.LensInfo.FStopMax);
  printf("Lens make            : %s\n", result.LensInfo.Make.c_str());
  printf("Lens model           : %s\n", result.LensInfo.Model.c_str());
  printf("Focal plane XRes     : %f\n", result.LensInfo.FocalPlaneXResolution);
  printf("Focal plane YRes     : %f\n", result.LensInfo.FocalPlaneYResolution);

  result.UserComment =
      result.UserComment + std::string("   testing new EXIF UserComment TAG\n");
  std::string newOutput = std::string("test.jpg");
  code = result.write(newOutput);
  if (code) {
    printf("Error writing ''%s' : code %d\n", newOutput.c_str(), code);
    return -4;
  }

  delete[] buf;

  return 0;
}
