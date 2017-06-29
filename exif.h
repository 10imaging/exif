/*
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  -- Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  -- Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
   OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
   NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __EXIF_H
#define __EXIF_H

#include <stdio.h>
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#ifdef DEBUG
#include <iostream>
#define VERBOSE(a) (a)
#else
#define VERBOSE(a)
#endif

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN ||                 \
    defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) ||       \
    defined(__MIBSEB__)
#error "Big endian architecture unsupported"
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN ||         \
    defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) ||                   \
    defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || \
    defined(__MIPSEL) || defined(__MIPSEL__)
#else
#error "Unknown endian architecture!"
#endif

namespace exif {

struct Rational {
  uint32_t numerator, denominator;
  operator double() const {
    if (denominator < 1e-20) {
      return 0;
    }
    return static_cast<double>(numerator) / static_cast<double>(denominator);
  }
};

struct SRational {
  int32_t numerator, denominator;
  operator double() const {
    if (denominator < 1e-20) {
      return 0;
    }
    return static_cast<double>(numerator) / static_cast<double>(denominator);
  }
};

// IF Entry
class IFEntry {
 public:
  using byte_vector = std::vector<uint8_t>;
  using ascii_vector = std::string;
  using short_vector = std::vector<uint16_t>;
  using long_vector = std::vector<uint32_t>;
  using rational_vector = std::vector<Rational>;
  using srational_vector = std::vector<SRational>;

  IFEntry()
      : tag_(0xFF), format_(0xFF), data_(0), length_(0), val_byte_(nullptr) {}
  /*
  IFEntry(const IFEntry &) = delete;
  IFEntry& operator= (const IFEntry &) = delete;
  IFEntry(IFEntry &&other)
      : tag_(other.tag_),
        format_(other.format_),
        data_(other.data_),
        length_(other.length_),
        val_byte_(other.val_byte_) {
    other.tag_ = 0xFF;
    other.format_ = 0xFF;
    other.data_ = 0;
    other.length_ = 0;
    other.val_byte_ = nullptr;
  }
  */
  ~IFEntry() { delete_union(); }
  unsigned short tag() const { return tag_; }
  void tag(unsigned short tag) { tag_ = tag; }
  unsigned short format() const { return format_; }
  bool format(unsigned short format) {
    switch (format) {
      case 0x01:
      case 0x02:
      case 0x03:
      case 0x04:
      case 0x05:
      case 0x07:
      case 0x09:
      case 0x0a:
      case 0xff:
        break;
      default:
        return false;
    }
    delete_union();
    format_ = format;
    new_union();
    return true;
  }
  unsigned data() const { return data_; }
  void data(unsigned data) { data_ = data; }
  unsigned length() const { return length_; }
  void length(unsigned length) { length_ = length; }

  // functions to access the data
  //
  // !! it's CALLER responsibility to check that format !!
  // !! is correct before accessing it's field          !!
  //
  // - getters are use here to allow future addition
  //   of checks if format is correct
  byte_vector &val_byte() { return *val_byte_; }
  ascii_vector &val_string() { return *val_string_; }
  short_vector &val_short() { return *val_short_; }
  long_vector &val_long() { return *val_long_; }
  rational_vector &val_rational() { return *val_rational_; }
  srational_vector &val_srational() { return *val_srational_; }

 private:
  // Raw fields
  unsigned short tag_;
  unsigned short format_;
  unsigned data_;
  unsigned length_;

  // Parsed fields
  union {
    byte_vector *val_byte_;
    ascii_vector *val_string_;
    short_vector *val_short_;
    long_vector *val_long_;
    rational_vector *val_rational_;
    srational_vector *val_srational_;
  };

  void delete_union() {
    switch (format_) {
      case 0x1:
        delete val_byte_;
        val_byte_ = nullptr;
        break;
      case 0x2:
        delete val_string_;
        val_string_ = nullptr;
        break;
      case 0x3:
        delete val_short_;
        val_short_ = nullptr;
        break;
      case 0x4:
        delete val_long_;
        val_long_ = nullptr;
        break;
      case 0x5:
        delete val_rational_;
        val_rational_ = nullptr;
        break;
      case 0x7:
        delete val_byte_;
        val_byte_ = nullptr;
        break;
      case 0xA:
        delete val_srational_;
        val_srational_ = nullptr;
        break;
      case 0xff:
        break;
      default:
        // TODO should not get here
        break;
    }
  }
  void new_union() {
    switch (format_) {
      case 0x1:
        val_byte_ = new byte_vector();
        break;
      case 0x2:
        val_string_ = new ascii_vector();
        break;
      case 0x3:
        val_short_ = new short_vector();
        break;
      case 0x4:
        val_long_ = new long_vector();
        break;
      case 0x5:
        val_rational_ = new rational_vector();
        break;
      case 0x7:
        val_byte_ = new byte_vector();
        break;
      case 0xA:
        val_srational_ = new srational_vector();
        break;
      case 0xff:
        break;
      default:
        // TODO should not get here
        VERBOSE(std::cerr << "ERROR");
        break;
    }
  }
};

// Helper functions
template <typename T>
T inline parse(const unsigned char *buf, const bool);

template <>
uint8_t inline parse<uint8_t>(const unsigned char *buf,
                              const bool isLittleEndian) {
  return *buf;
}

template <>
uint16_t inline parse<uint16_t>(const unsigned char *buf,
                                const bool isLittleEndian) {
  if (isLittleEndian) {
    return (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
  } else {
    return (static_cast<uint16_t>(buf[0]) << 8) | buf[1];
  }
}

template <>
uint32_t inline parse<uint32_t>(const unsigned char *buf,
                                const bool isLittleEndian) {
  if (isLittleEndian) {
    return (static_cast<uint32_t>(buf[3]) << 24) |
           (static_cast<uint32_t>(buf[2]) << 16) |
           (static_cast<uint32_t>(buf[1]) << 8) | buf[0];
  } else {
    return (static_cast<uint32_t>(buf[0]) << 24) |
           (static_cast<uint32_t>(buf[1]) << 16) |
           (static_cast<uint32_t>(buf[2]) << 8) | buf[3];
  }
}

template <>
int32_t inline parse<int32_t>(const unsigned char *buf,
                              const bool isLittleEndian) {
  if (isLittleEndian) {
    return (static_cast<int32_t>(buf[3]) << 24) |
           (static_cast<int32_t>(buf[2]) << 16) |
           (static_cast<int32_t>(buf[1]) << 8) | buf[0];
  } else {
    return (static_cast<int32_t>(buf[0]) << 24) |
           (static_cast<int32_t>(buf[1]) << 16) |
           (static_cast<int32_t>(buf[2]) << 8) | buf[3];
  }
}

template <>
Rational inline parse<Rational>(const unsigned char *buf,
                                const bool isLittleEndian) {
  Rational r;
  if (isLittleEndian) {
    r.numerator = parse<uint32_t>(buf, isLittleEndian);
    r.denominator = parse<uint32_t>(buf + 4, isLittleEndian);
  } else {
    r.numerator = parse<uint32_t>(buf, isLittleEndian);
    r.denominator = parse<uint32_t>(buf + 4, isLittleEndian);
  }
  return r;
}

template <>
SRational inline parse<SRational>(const unsigned char *buf,
                                  const bool isLittleEndian) {
  SRational r;
  if (isLittleEndian) {
    r.numerator = parse<int32_t>(buf, isLittleEndian);
    r.denominator = parse<int32_t>(buf + 4, isLittleEndian);
  } else {
    r.numerator = parse<int32_t>(buf, isLittleEndian);
    r.denominator = parse<int32_t>(buf + 4, isLittleEndian);
  }
  return r;
}

/**
 * Try to read entry.length() values for this entry.
 *
 * Returns:
 *  true  - entry.length() values were read
 *  false - something went wrong, vec's content was not touched
 */
template <typename T, typename C>
bool inline extract_values(C &container, const unsigned char *buf,
                           const unsigned long base, const bool isLittleEndian,
                           const unsigned long len, const IFEntry &entry) {
  const unsigned char *data;
  uint32_t reversed_data;
  // if data fits into 4 bytes, they are stored directly in
  // the data field in IFEntry
  if (sizeof(T) * entry.length() <= 4) {
    if (isLittleEndian) {
      reversed_data = entry.data();
    } else {
      reversed_data = entry.data();
      // this reversing works, but is ugly
      unsigned char *data = reinterpret_cast<unsigned char *>(&reversed_data);
      unsigned char tmp;
      tmp = data[0];
      data[0] = data[3];
      data[3] = tmp;
      tmp = data[1];
      data[1] = data[2];
      data[2] = tmp;
    }
    data = reinterpret_cast<const unsigned char *>(&(reversed_data));
  } else {
    data = buf + base + entry.data();
    if (data + sizeof(T) * entry.length() > buf + len) {
      return false;
    }
  }
  container.resize(entry.length());
  for (size_t i = 0; i < entry.length(); ++i) {
    container[i] = parse<T>(data + sizeof(T) * i, isLittleEndian);
  }
  return true;
}

void inline parseIFEntryHeader(const unsigned char *buf,
                               const bool isLittleEndian, unsigned short &tag,
                               unsigned short &format, unsigned &length,
                               unsigned &data) {
  // Each directory entry is composed of:
  // 2 bytes: tag number (data field)
  // 2 bytes: data format
  // 4 bytes: number of components
  // 4 bytes: data value or offset to data value
  tag = parse<uint16_t>(buf, isLittleEndian);
  format = parse<uint16_t>(buf + 2, isLittleEndian);
  length = parse<uint32_t>(buf + 4, isLittleEndian);
  data = parse<uint32_t>(buf + 8, isLittleEndian);
}

void inline parseIFEntryHeader(const unsigned char *buf, bool isLittleEndian,
                               IFEntry &result) {
  unsigned short tag;
  unsigned short format;
  unsigned length;
  unsigned data;

  parseIFEntryHeader(buf, isLittleEndian, tag, format, length, data);

  result.tag(tag);
  result.format(format);
  result.length(length);
  result.data(data);

  VERBOSE(std::cerr << "\nIFD tag=0x" << std::hex << tag << "(" << std::dec
                    << tag << ")"
                    << " format=" << format << " length=" << length
                    << std::dec);
}

IFEntry inline parseIFEntry(const unsigned char *buf, const unsigned long offs,
                            bool isLittleEndian, const unsigned long base,
                            const unsigned long len) {
  IFEntry result;

  // check if there even is enough data for IFEntry in the buffer
  if (buf + offs + 12 > buf + len) {
    result.tag(0xFF);
    return result;
  }

  parseIFEntryHeader(buf + offs, isLittleEndian, result);

  // Parse value in specified format
  switch (result.format()) {
    case 1:
      if (!extract_values<uint8_t>(result.val_byte(), buf, base, isLittleEndian,
                                   len, result)) {
        result.tag(0xFF);
      }
      break;
    case 2:
      // string is basically sequence of uint8_t (well, according to EXIF even
      // uint7_t, but
      // we don't have that), so just read it as bytes
      if (!extract_values<uint8_t>(result.val_string(), buf, base,
                                   isLittleEndian, len, result)) {
        result.tag(0xFF);
      }
      // and cut zero byte at the end, since we don't want that in the
      // std::string
      if (result.val_string()[result.val_string().length() - 1] == '\0') {
        result.val_string().resize(result.val_string().length() - 1);
      }
      break;
    case 3:
      if (!extract_values<uint16_t>(result.val_short(), buf, base,
                                    isLittleEndian, len, result)) {
        result.tag(0xFF);
      }
      break;
    case 4:
      if (!extract_values<uint32_t>(result.val_long(), buf, base,
                                    isLittleEndian, len, result)) {
        result.tag(0xFF);
      }
      break;
    case 5:
      if (!extract_values<Rational>(result.val_rational(), buf, base,
                                    isLittleEndian, len, result)) {
        result.tag(0xFF);
      }
      break;
    case 7:
      if (!extract_values<uint8_t>(result.val_byte(), buf, base, isLittleEndian,
                                   len, result)) {
        result.tag(0xFF);
      }
      break;
    case 9:
      VERBOSE(std::cerr << " - unknown format");
      break;
    case 10:
      if (!extract_values<SRational>(result.val_srational(), buf, base,
                                     isLittleEndian, len, result)) {
        result.tag(0xFF);
      }
      break;
    default:
      result.tag(0xFF);
  }
  return result;
}

// helper functions for convinience
template <typename T>
T inline parse_value(const unsigned char *buf, bool isLittleEndian) {
  return parse<T>(buf, isLittleEndian);
}

//
// Class responsible for storing and parsing EXIF information from a JPEG blob
//
class EXIFInfo {
 public:
  // Parsing function for an entire JPEG image buffer.
  //
  // PARAM 'data': A pointer to a JPEG image.
  // PARAM 'length': The length of the JPEG image.
  // RETURN:  PARSE_EXIF_SUCCESS (0) on succes with 'result' filled out
  //          error code otherwise, as defined by the PARSE_EXIF_ERROR_* macros
  int read(const unsigned char *data, unsigned long length);
  //  int read(const std::string &data);
  int read(std::string inputFile);
  int write(std::string outputFile);
  // Parsing function for an EXIF segment. This is used internally by
  // parseFrom() but can be called for special cases where only the EXIF section
  // is available (i.e., a blob starting with the bytes "Exif\0\0").
  int decodeEXIFsegment(const unsigned char *buf, unsigned long len,
                        std::vector<IFEntry> &IFentries);

  // Set all data members to default values.
  void clear();

  // Data fields filled out by parseFrom()
  char ByteAlign;                // 0 = Motorola byte alignment, 1 = Intel
  std::string ImageDescription;  // Image description
  std::string Make;              // Camera manufacturer's name
  std::string Model;             // Camera model
  std::string Software;          // Software used
  std::string ExifVersion;
  std::string DateTime;           // File change date and time
  std::string DateTimeOriginal;   // Original file date and time (may not exist)
  std::string DateTimeDigitized;  // Digitization date and time (may not exist)
  std::string SubSecTime;         // Sub-second time
  std::string
      SubSecTimeOriginal;  // Sub-second time that original picture was taken
  std::string SubSecTimeDigitized;  //
  std::string Copyright;            // File copyright information
  std::string UserComment;          // UserComment field
  std::string FlashpixVersion;

  unsigned short
      Orientation;  // Image orientation, start of data corresponds to
                    // 0: unspecified in EXIF data
                    // 1: upper left of image
                    // 3: lower right of image
                    // 6: upper right of image
                    // 8: lower left of image
                    // 9: undefined
  unsigned short BitsPerSample;     // Number of bits per component
  unsigned short ResolutionUnit;    // 1 = None (is not standard EXIF)
                                    // 2 = inches
                                    // 3 = cm
  unsigned short YCbCrPositioning;  // 1 = Centered, 2 = Co-sited
  unsigned char ComponentsConfiguration[4];
  // std::vector<unsigned char> MakerNote;
  double ExposureTime;             // Exposure time in seconds
  double FNumber;                  // F/stop
  unsigned short ExposureProgram;  // Exposure program
                                   // 0: Not defined
                                   // 1: Manual
                                   // 2: Normal program
                                   // 3: Aperture priority
                                   // 4: Shutter priority
                                   // 5: Creative program
                                   // 6: Action program
                                   // 7: Portrait mode
                                   // 8: Landscape mode
  unsigned short ISOSpeedRatings;  // ISO speed
  double ShutterSpeedValue;  // Shutter speed (reciprocal of exposure time)
  double ApertureValue;      // Aperture value as F#
  double BrightnessValue;    //
  double ExposureBiasValue;  // Exposure bias value in EV
  double MaxApertureValue;   //
  double SubjectDistance;    // Distance to focus point in meters
  double FocalLength;        // Focal length of lens in millimeters
  unsigned short FocalLengthIn35mm;   // Focal length in 35mm film
  char Flash;                         // 0 = no flash, 1 = flash used
  unsigned short FlashReturnedLight;  // Flash returned light status
                                      // 0: No strobe return detection function
                                      // 1: Reserved
                                      // 2: Strobe return light not detected
                                      // 3: Strobe return light detected
  unsigned short FlashMode;           // Flash mode
                                      // 0: Unknown
                                      // 1: Compulsory flash firing
                                      // 2: Compulsory flash suppression
                                      // 3: Automatic mode
  unsigned short MeteringMode;        // Metering mode
                                      // 1: average
                                      // 2: center weighted average
                                      // 3: spot
                                      // 4: multi-spot
                                      // 5: multi-segment
  unsigned ImageWidth;                // Image width reported in EXIF data
  unsigned ImageHeight;               // Image height reported in EXIF data
  unsigned ColorSpace;
  unsigned InteropOffset;
  unsigned CustomRendered;
  unsigned ExposureMode;
  unsigned WhiteBalance;
  unsigned SceneCaptureType;
  unsigned SceneType;
  double Xresolution;     // X resolution
  double Yresolution;     // Y resolution
  struct Geolocation_t {  // GPS information embedded in file
    double Latitude;      // Image latitude expressed as decimal
    double Longitude;     // Image longitude expressed as decimal
    double Altitude;      // Altitude in meters, relative to sea level
    char AltitudeRef;     // 0 = above sea level, -1 = below sea level
    double DOP;           // GPS degree of precision (DOP)
    struct Coord_t {
      double degrees;
      double minutes;
      double seconds;
      char direction;
    } LatComponents,
        LonComponents;  // Latitude, Longitude expressed in deg/min/sec
  } GeoLocation;
  struct LensInfo_t {              // Lens information
    double FStopMin;               // Min aperture (f-stop)
    double FStopMax;               // Max aperture (f-stop)
    double FocalLengthMin;         // Min focal length (mm)
    double FocalLengthMax;         // Max focal length (mm)
    double FocalPlaneXResolution;  // Focal plane X-resolution
    double FocalPlaneYResolution;  // Focal plane Y-resolution
    std::string Make;              // Lens manufacturer
    std::string Model;             // Lens model
  } LensInfo;

  EXIFInfo() { clear(); }
};

}  // namespace exif

// Parse was successful
#define PARSE_EXIF_SUCCESS 0
// No JPEG markers found in buffer, possibly invalid JPEG file
#define PARSE_EXIF_ERROR_NO_JPEG 1982
// No EXIF header found in JPEG file.
#define PARSE_EXIF_ERROR_NO_EXIF 1983
// Byte alignment specified in EXIF file was unknown (not Motorola or Intel).
#define PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN 1984
// EXIF header was found, but data was corrupted.
#define PARSE_EXIF_ERROR_CORRUPT 1985

#endif
