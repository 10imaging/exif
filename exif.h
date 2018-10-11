/*
 * Copyright 2017 10 Imaging Inc.
 */
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
#include <cstring>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>

//#define DEBUG
#define LOG_TAG "Exif"
#ifdef DEBUG
    #ifdef __ANDROID__
        #include <android/log.h>
        #define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
    #else
        #define LOGD(...) (fprintf(stderr,__VA_ARGS__), fprintf(stderr,"\n"))
    #endif
#else
    #define LOGD(...) // Turn off logging
#endif

#ifdef __ANDROID__
#include <android/log.h>
#define ERROR(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#else
#define ERROR(...) (fprintf(stderr,__VA_ARGS__), fprintf(stderr,"\n"))
#endif


#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) || \
    defined(__MIBSEB__)
#error "Big endian architecture unsupported"
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || \
    defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || \
    defined(__MIPSEL) || defined(__MIPSEL__)
#else
#error "Unknown endian architecture!"
#endif

#define ENTRY_SIZE  12
#define EXIF_START  6
#define JPEG_SOI    0xFFD8
#define EXIF_MARKER 0xFFE1
#define MAX_TO_PRINT 10
#define CURR_10_VERSION  1

// Exif defined format types
#define ENTRY_FORMAT_BYTE       1
#define ENTRY_FORMAT_ASCII      2
#define ENTRY_FORMAT_SHORT      3
#define ENTRY_FORMAT_LONG       4
#define ENTRY_FORMAT_RATIONAL   5
#define ENTRY_FORMAT_SBYTE      6
#define ENTRY_FORMAT_UNDEFINED  7
//#define ENTRY_FORMAT_SSHORT     8
//#define ENTRY_FORMAT_SLONG      9
#define ENTRY_FORMAT_SRATIONAL 10

// Internally defined directory types
#define IFD0_DIRECTORY          1
#define EXIF_IFD_DIRECTORY      2
#define GPS_IFD_DIRECTORY       3
#define INTEROP_IFD_DIRECTORY   4
#define IFD1_DIRECTORY          5
#define EXIF_10_DIRECTORY      10

// See http://www.cipa.jp/std/documents/e/DC-008-Translation-2016-E.pdf for definitions
// Tags used in IFD0 (Primary Image) and IFD1 (Thumbnail) directory
#define EXIF_TAG_IFD_IMAGE_WIDTH    0x0100
#define EXIF_TAG_IFD_IMAGE_HEIGHT   0x0101
#define EXIF_TAG_BITS_PER_SAMPLE    0x0102
#define EXIF_TAG_COMPRESSION_SCHEME 0x0103
#define EXIF_TAG_PIXEL_COMPOSITION  0x0106
#define EXIF_TAG_IMAGE_DESCRIPTION  0x010E
#define EXIF_TAG_DIGICAM_MAKE       0x010F
#define EXIF_TAG_DIGICAM_MODEL      0x0110
#define EXIF_TAG_STRIP_OFFSETS      0x0111
#define EXIF_TAG_ORIENTATION        0x0112
#define EXIF_TAG_NUM_COMPONENTS     0x0115
#define EXIF_TAG_ROWS_PER_STRIP     0x0116
#define EXIF_TAG_STRIP_BYTE_COUNT   0x0117
#define EXIF_TAG_X_RESOLUTION       0x011A
#define EXIF_TAG_Y_RESOLUTION       0x011B
#define EXIF_TAG_Y_PLANAR_CONFIG    0x011C
#define EXIF_TAG_RESOLUTION_UNIT    0x0128
#define EXIF_TAG_TRANSFER_FUNCTION  0x012D
#define EXIF_TAG_SOFTWARE           0x0131
#define EXIF_TAG_MODIFY_DATE_TIME   0x0132
#define EXIF_TAG_ARTIST             0x013B
#define EXIF_TAG_WHITE_POINT        0x013E
#define EXIF_TAG_PRIMARY_CHROMA     0x013F
#define EXIF_TAG_JPEG_SOI_OFFSET    0x0201
#define EXIF_TAG_JPEG_DATA_BYTES    0x0202
#define EXIF_TAG_YCBCR_COEFF        0x0211
#define EXIF_TAG_YCBCR_SUBSAMPLING  0x0212
#define EXIF_TAG_YCBCR_POSITION     0x0213
#define EXIF_TAG_REF_BW             0x0214
#define EXIF_TAG_RATING             0x4746
#define EXIF_TAG_RATING_PERCENT     0x4749
#define EXIF_TAG_COPYRIGHT          0x8298
#define EXIF_TAG_EXIF_IFD_OFFSET    0x8769
#define EXIF_TAG_GPS_IFD_OFFSET     0x8825
#define EXIF_TAG_10_IFD_OFFSET      0xAAAA

// Tags used in EXIF directory
#define EXIF_TAG_EXPOSURE_TIME      0x829A
#define EXIF_TAG_FNUMBER            0x829D
#define EXIF_TAG_EXPOSURE_PROGRAM   0x8822
#define EXIF_TAG_ISO_SPEED_RATING   0x8827
#define EXIF_TAG_EXIF_VERSION       0x9000
#define EXIF_TAG_ORIGINAL_DATE      0x9003
#define EXIF_TAG_DIGITIZATION_DATE  0x9004
#define EXIF_TAG_COMPONENTS_CONFIG  0x9101
#define EXIF_TAG_COMPRESSED_BPP     0x9102
#define EXIF_TAG_SHUTTER_SPEED      0x9201
#define EXIF_TAG_APERTURE_VALUE     0x9202
#define EXIF_TAG_BRIGHTNESS_VALUE   0x9203
#define EXIF_TAG_EXPOSURE_BIAS      0x9204
#define EXIF_TAG_MAX_APERTURE       0x9205
#define EXIF_TAG_SUBJECT_DIST       0x9206
#define EXIF_TAG_METERING_MODE      0x9207
#define EXIF_TAG_LIGHT_SOURCE       0x9208
#define EXIF_TAG_FLASH_USED         0x9209
#define EXIF_TAG_FOCAL_LENGTH       0x920A
#define EXIF_TAG_SUBJECT_LOCATION   0x9214
#define EXIF_TAG_MAKER_NOTE         0x927C
#define EXIF_TAG_USER_COMMENT       0x9286
#define EXIF_TAG_SUB_SEC_TIME       0x9290
#define EXIF_TAG_SUB_SEC_ORIG_TIME  0x9291
#define EXIF_TAG_DIGITIZED_TIME     0x9292
#define EXIF_TAG_FLASH_PIX_VERSION  0xA000
#define EXIF_TAG_COLOR_SPACE        0xA001
#define EXIF_TAG_IMAGE_WIDTH        0xA002
#define EXIF_TAG_IMAGE_HEIGHT       0xA003
#define EXIF_TAG_SOUND_FILE         0xA004
#define EXIF_TAG_INTEROP_OFFSET     0xA005
#define EXIF_TAG_FOCAL_X_RESOLUTION 0xA20E
#define EXIF_TAG_FOCAL_Y_RESOLUTION 0xA20F
#define EXIF_TAG_FOCAL_RES_UNIT     0xA210
#define EXIF_TAG_SENSING_METHOD     0xA217
#define EXIF_TAG_FILE_SOURCE        0xA300
#define EXIF_TAG_SCENE_TYPE         0xA301
#define EXIF_TAG_CUSTOM_RENDERED    0xA401
#define EXIF_TAG_EXPOSURE_MODE      0xA402
#define EXIF_TAG_WHITE_BALANCE      0xA403
#define EXIF_TAG_DIGITAL_ZOOM_RATIO 0xA404
#define EXIF_TAG_FOCAL_LENGTH35MM   0xA405
#define EXIF_TAG_SCENE_CAPTURE_TYPE 0xA406
#define EXIF_TAG_GAIN_CONTROL       0xA407
#define EXIF_TAG_CONTRAST           0xA408
#define EXIF_TAG_SATURATION         0xA409
#define EXIF_TAG_SHARPNESS          0xA40A
#define EXIF_TAG_DEVICE_SETTINGS    0xA40B
#define EXIF_TAG_SUBJ_DIST_RANGE    0xA40C
#define EXIF_UNIQUE_IMAGE_ID        0xA420
#define EXIF_CAMERA_OWNER_NAME      0xA430
#define EXIF_BODY_SERIAL_NUMBER     0xA431
#define EXIF_TAG_FOCAL_LENGTH_FSTOP 0xA432
#define EXIF_TAG_LENS_MAKE          0xA433
#define EXIF_TAG_LENS_MODEL         0xA434
#define EXIF_TAG_LENS_SERIAL_NUMBER 0xA435

// Tags used in GPS directory
#define EXIF_TAG_GPS_VERSION_ID     0x0000
#define EXIF_TAG_GPS_LATITUDE_REF   0x0001
#define EXIF_TAG_GPS_LATITUDE       0x0002
#define EXIF_TAG_GPS_LONGITUDE_REF  0x0003
#define EXIF_TAG_GPS_LONGITUDE      0x0004
#define EXIF_TAG_GPS_ALTITUDE_REF   0x0005
#define EXIF_TAG_GPS_ALTITUDE       0x0006
#define EXIF_TAG_GPS_TIME_STAMP     0x0007
#define EXIF_TAG_GPS_SATELLITES     0x0008
#define EXIF_TAG_GPS_STATUS         0x0009
#define EXIF_TAG_GPS_MEASURE_MODE   0x000A
#define EXIF_TAG_GPS_DOP            0x000B
#define EXIF_TAG_GPS_SPEED_REF      0x000C
#define EXIF_TAG_GPS_SPEED          0x000D
#define EXIF_TAG_GPS_TRACK_REF      0x000E
#define EXIF_TAG_GPS_TRACK          0x000F
#define EXIF_TAG_GPS_IMG_DIR_REF    0x0010
#define EXIF_TAG_GPS_IMG_DIR        0x0011
#define EXIF_TAG_GPS_MAP_DATUM      0x0012
#define EXIF_TAG_GPS_DEST_LAT_REF   0x0013
#define EXIF_TAG_GPS_DEST_LATITUDE  0x0014
#define EXIF_TAG_GPS_DEST_LONG_REF  0x0015
#define EXIF_TAG_GPS_DEST_LONGITUDE 0x0016
#define EXIF_TAG_GPS_DEST_BEARING_REF 0x0017
#define EXIF_TAG_GPS_DEST_BEARING   0x0018
#define EXIF_TAG_GPS_DEST_DIST_REF  0x0019
#define EXIF_TAG_GPS_DEST_DIST      0x001A
#define EXIF_TAG_GPS_PROCESSING_METHOD 0x001B
#define EXIF_TAG_GPS_AREA_INFO      0x001C
#define EXIF_TAG_GPS_DATE_STAMP     0x001D
#define EXIF_TAG_GPS_DIFFERENTIAL   0x001E
#define EXIF_TAG_GPS_HORIZ_POS_ERR  0x001F

// Tags used in 10 Imaging directory
#define EXIF_10_SCENE_TYPE          0xAA00
#define EXIF_10_SCENE_PROBABILITY   0xAA01
#define EXIF_10_SCENE_SUB_TYPES     0xAA02
#define EXIF_10_SCENE_SUB_PROBS     0xAA03
#define EXIF_10_NPE_MODEL           0xAA04
#define EXIF_10_SCENE_RESULT        0xAA05
#define EXIF_10_CORR_SCENE_TYPE     0xAA06
#define EXIF_10_ENHANCEMENT_STRENGTH 0xAA07
#define EXIF_10_TRAINING_MODE       0xAA08
#define EXIF_10_PANO_NUM_IMAGES     0xAA0B
#define EXIF_10_PANO_NUM_STITCHES   0xAA0C
#define EXIF_10_ROI_CASCADE         0xAA0D
#define EXIF_10_ROI_RECTS           0xAA0E
#define EXIF_10_ROI_TYPES           0xAA0F
#define EXIF_10_ROI_RESULTS         0xAA10
#define EXIF_10_ROI_ENHANCEMENTS    0xAA11
#define EXIF_10_ROI_NAMES           0xAA12
#define EXIF_10_VERSION             0xAAAA

// Tags used in Interop directory
#define EXIF_TAG_INTEROP_INDEX      0x0001
#define EXIF_TAG_INTEROP_VERSION    0x0002

namespace exif {

    /**
     * TagInfo structure describes the information associated with a Tag, ie, format type and directory but doesn't store the Tag data
     */
    struct TagInfo {
        uint16_t tag;           ///< Tag ID
        uint16_t format;        ///< Tag format type
        u_int8_t directory;     ///< Directory type
        uint32_t length;        ///< Typical length of tag or 0 if variable
        std::string name;       ///< Name of tag used for output
        std::string desc;       ///< Additional description of Tag, used for output

        /**
         * Constructor
         * @param tag_          ///< Tag ID
         * @param format_       ///< Tag format
         * @param directory_    ///< Directory tag belongs in
         * @param length_       ///< Typical length of tag or 0 if variable
         * @param name_         ///< Name of tag used for output
         * @param desc_         ///< Additional description of Tag, used for output
         * @return TagInfo data
         */
        TagInfo (uint16_t tag_, uint16_t format_, u_int8_t directory_, uint32_t length_, std::string name_, std::string desc_) {
            tag = tag_;
            format = format_;
            directory = directory_;
            length = length_;
            name = name_;
            desc = desc_;
        }
    };

    /**
     * Get the TagInfo given a tag and directory
     * @param tag   Tag to get TagInfo for
     * @param dir   Directory Tag is in
     * @return TagInfo struct
     */
    TagInfo* getTagInfo(uint16_t tag, uint8_t dir);

    /**
     * structure for an unsigned Rational number
     */
    struct Rational {
        uint32_t numerator, denominator;

        operator double() const {
            if (denominator < 1e-20) {
                return 0;
            }
            return static_cast<double>(numerator) / static_cast<double>(denominator);
        }

        /**
         * Output Rational number as a string in simplified format
         * @return
         */
        std::string toString() {
            char tmp[50];

            if (denominator == 1)
                sprintf(tmp,"%d",numerator);
            else if (numerator == 0)
                return ("0");
            else if ((denominator % 10) == 0) {
                sprintf(tmp, "%.4f", (float)numerator/denominator);
            }
            else
                sprintf(tmp,"%d/%d (%.4f)",numerator,denominator, (float)numerator/denominator);
            return std::string(tmp);
        }
    };

    /**
     * Signed rational
     */
    struct SRational {
        int32_t numerator, denominator;

        operator double() const {
            if (denominator < 1e-20) {
                return 0;
            }
            return static_cast<double>(numerator) / static_cast<double>(denominator);
        }

        /**
         * Output Signed Rational number as a string in simplified format
         * @return
         */
        std::string toString() {
            char tmp[50];

            if (denominator == 1)
                sprintf(tmp, "%d", numerator);
            else if (numerator == 0)
                return ("0");
            else if ((denominator % 10) == 0) {
                sprintf(tmp, "%.4f", (float)numerator/denominator);
            }
            else
                sprintf(tmp,"%d/%d (%.4f)",numerator,denominator, (float)numerator/denominator);
            return std::string(tmp);
        }
    };

    /**
     * IFEntry describes the entry data stored in the EXIFInfo
     */
    class IFEntry {
    public:
        using byte_vector = std::vector<uint8_t>;
        using ascii_vector = std::string;
        using short_vector = std::vector<uint16_t>;
        using long_vector = std::vector<uint32_t>;
        using rational_vector = std::vector<Rational>;
        using srational_vector = std::vector<SRational>;

        /**
         * Default constructor
         * @return new IFEntry
         */
        IFEntry() : tag_(0xFF), directory_(IFD0_DIRECTORY), format_(0xFF), data_(0), length_(0), val_byte_(nullptr) {}

        /**
         * Constructor for IFEntry for a string type
         * @param tagIn     Tag ID
         * @param dir       Directory to store tag in
         * @param valStr    String data to store
         * @return
         */
        IFEntry(unsigned short tagIn, uint8_t dir, std::string valStr) {
            tag_ = tagIn;
            directory_ = dir;
            format_ = ENTRY_FORMAT_ASCII;
            length_ = (unsigned int) valStr.size();
            new_union();
            val_string() = valStr;
        }

        /**
         * Constructor for IFEntry for all single non string types.  Format is determined based on TagInfo
         * @param tagIn         Tag data
         * @param dir           Directory to store tag in
         * @param valIn         Value to store
         * @return  new IFEntry value
         */
        IFEntry(unsigned short tagIn, uint8_t dir, int valIn) {
            tag_ = tagIn;
            directory_ = dir;
            TagInfo *tagInfo = getTagInfo(tagIn,dir);
            format_ = tagInfo->format;
            length_ = 1;
            new_union();
            switch (format_) {
                case ENTRY_FORMAT_BYTE:
                case ENTRY_FORMAT_UNDEFINED:
                    val_byte().push_back((unsigned char)valIn);
                    break;
                case ENTRY_FORMAT_SHORT:
                    val_short().push_back((unsigned short)valIn);
                    break;
                case ENTRY_FORMAT_LONG:
                    val_long_->push_back((unsigned int)valIn);
                    break;
                case ENTRY_FORMAT_RATIONAL:
                    Rational r;
                    r.numerator = (uint32_t)valIn;
                    r.denominator = 1;
                    val_rational().push_back(r);
                    break;
                case ENTRY_FORMAT_SRATIONAL:
                    SRational sr;
                    sr.numerator = valIn;
                    sr.denominator = 1;
                    val_srational().push_back(sr);
                    break;
                default:
                    ERROR("Unexpected format %d",format_);
                    break;
            }
        }

        /**
          * Constructor for IFEntry for an array of non string types.  Format is determined based on TagInfo
          * @param tagIn         Tag data
          * @param dir           Directory to store tag in
          * @param valIn         Array of values to store
          * @param num           Number of values
          * @return  new IFEntry value
          */
        IFEntry(unsigned short tagIn, uint8_t dir, int valIn[], int num) {
            tag_ = tagIn;
            directory_ = dir;
            TagInfo *tagInfo = getTagInfo(tagIn, dir);
            format_ = tagInfo->format;
            length_ = (unsigned int)num;
            new_union();
            for (int i=0; i<num; i++) {
                switch (format_) {
                    case ENTRY_FORMAT_BYTE:
                    case ENTRY_FORMAT_UNDEFINED:
                        val_byte().push_back((unsigned char) valIn[i]);
                        break;
                    case ENTRY_FORMAT_SHORT:
                        val_short().push_back((unsigned short) valIn[i]);
                        break;
                    case ENTRY_FORMAT_LONG:
                        val_long_->push_back((unsigned int) valIn[i]);
                        break;
                    case ENTRY_FORMAT_RATIONAL:
                        Rational r;
                        r.numerator = (uint32_t) valIn[i];
                        r.denominator = 1;
                        val_rational().push_back(r);
                        break;
                    case ENTRY_FORMAT_SRATIONAL:
                        SRational sr;
                        sr.numerator = valIn[i];
                        sr.denominator = 1;
                        val_srational().push_back(sr);
                        break;
                    default:
                        ERROR("Unexpected format %d",format_);
                        break;

                }
            }
        }

        /**
          * Constructor for IFEntry for a single Rational (signed or unsigned) number expressed by numerator and denominator
          * @param tagIn         Tag data
          * @param dir           Directory to store tag in
          * @param numerator     Numerator for the S/Rational
          * @param denominator   Denominator for the S/Rational
          * @return  new IFEntry value
          */
        IFEntry(unsigned short tagIn, uint8_t dir, int numerator, int denominator) {
            tag_ = tagIn;
            directory_ = dir;
            TagInfo *tagInfo = getTagInfo(tagIn,dir);
            format_ = tagInfo->format;
            length_ = 1;
            new_union();
            switch (format_) {
                case ENTRY_FORMAT_RATIONAL:
                    Rational r;
                    r.numerator = (uint32_t )numerator;
                    r.denominator = (uint32_t )denominator;
                    val_rational().push_back(r);
                    break;
                case ENTRY_FORMAT_SRATIONAL:
                    SRational sr;
                    sr.numerator = numerator;
                    sr.denominator = denominator;
                    val_srational().push_back(sr);
                    break;
                default:
                    ERROR("Unexpected format %d",format_);
                    break;

            }
        }

        /**
          * Constructor for IFEntry for a single Rational (signed or unsigned) number expressed by a float
          * @param tagIn         Tag data
          * @param dir           Directory to store tag in
          * @param valIn         Floating point value
          * @return  new IFEntry value
          */
        IFEntry(unsigned short tagIn, uint8_t dir, float valIn) {
            tag_ = tagIn;
            directory_ = dir;
            TagInfo *tagInfo = getTagInfo(tagIn,dir);
            format_ = tagInfo->format;
            length_ = 1;
            new_union();
            int numerator;
            int denominator;
            getNumDen(valIn,&numerator,&denominator);
            switch (format_) {
                case ENTRY_FORMAT_RATIONAL:
                    Rational r;
                    r.numerator = (uint32_t )numerator;
                    r.denominator = (uint32_t )denominator;
                    val_rational().push_back(r);
                    break;
                case ENTRY_FORMAT_SRATIONAL:
                    SRational sr;
                    sr.numerator = numerator;
                    sr.denominator = denominator;
                    val_srational().push_back(sr);
                    break;
                default:
                    ERROR("Unexpected format %d",format_);
                    break;

            }
        }

        /**
         * Constructor for IFEntry for an array of Rationals (signed or unsigned) expressed by floats
         * @param tagIn         Tag data
         * @param dir           Directory to store tag in
         * @param valIn         Array of floating point values
         * @param num           Number of values
         * @return  new IFEntry value
         */
        IFEntry(unsigned short tagIn, uint8_t dir, float valIn[], int num) {
            tag_ = tagIn;
            directory_ = dir;
            TagInfo *tagInfo = getTagInfo(tagIn,dir);
            format_ = tagInfo->format;
            length_ = (unsigned int)num;
            new_union();
            int numerator;
            int denominator;
            for (int i=0; i<num; i++) {
                getNumDen(valIn[i], &numerator, &denominator);
                switch (format_) {
                    case ENTRY_FORMAT_RATIONAL:
                        Rational r;
                        r.numerator = (uint32_t) numerator;
                        r.denominator = (uint32_t) denominator;
                        val_rational().push_back(r);
                        break;
                    case ENTRY_FORMAT_SRATIONAL:
                        SRational sr;
                        sr.numerator = numerator;
                        sr.denominator = denominator;
                        val_srational().push_back(sr);
                        break;
                    default:
                        ERROR("Unexpected format %d",format_);
                        break;

                }
            }
        }

        /**
         * Get tag of IFEntry
         * @return Tag value
         */
        unsigned short tag() const { return tag_; }

        /**
         * Set tag value
         * @param tag  Tag value
         */
        void tag(unsigned short tag) { tag_ = tag; }

        /**
         * Get directory of IFEntry
         * @return Directory value
         */
        uint8_t directory() const { return directory_; }

        /**
         * Set directory value
         * @param dir   Directory value
         */
        void directory(uint8_t dir) { directory_ = dir; }

        /**
         * Get format of IFEntry
         * @return  Format value
         */
        unsigned short format() const { return format_; }

        /**
         * Set format value for IFEntry
         * @param format Format value
         * @return true if format is valid, otherwise return false;
         */
        bool format(unsigned short format) {
            switch (format) {
                case ENTRY_FORMAT_BYTE:
                case ENTRY_FORMAT_ASCII:
                case ENTRY_FORMAT_SHORT:
                case ENTRY_FORMAT_LONG:
                case ENTRY_FORMAT_RATIONAL:
                case ENTRY_FORMAT_UNDEFINED:
                case ENTRY_FORMAT_SBYTE:
                case ENTRY_FORMAT_SRATIONAL:
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

        /**
         * get Data from IFEntry
         * @return data
         */
        unsigned data() const { return data_; }

        /**
         * Set Data for IFEntry
         * @param data Data value
         */
        void data(unsigned data) { data_ = data; }

        /**
         * Get Length of IFEntry value
         * @return length
         */
        unsigned length() const { return length_; }

        /**
         * Set length value of IFEntry
         * @param length
         */
        void length(unsigned length) { length_ = length; }

        // functions to access the data
        //
        // !! it's CALLER responsibility to check that format !!
        // !! is correct before accessing it's field          !!
        byte_vector &val_byte() { return *val_byte_; }

        ascii_vector &val_string() { return *val_string_; }

        short_vector &val_short() { return *val_short_; }

        long_vector &val_long() { return *val_long_; }

        rational_vector &val_rational() { return *val_rational_; }

        srational_vector &val_srational() { return *val_srational_; }

    private:
        // Raw fields
        unsigned short tag_;
        uint8_t directory_;
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
                case ENTRY_FORMAT_BYTE:
                    if (val_byte_) delete val_byte_;
                    val_byte_ = nullptr;
                    break;
                case ENTRY_FORMAT_ASCII:
                    if (val_string_) delete val_string_;
                    val_string_ = nullptr;
                    break;
                case ENTRY_FORMAT_SHORT:
                    if (val_short_) delete val_short_;
                    val_short_ = nullptr;
                    break;
                case ENTRY_FORMAT_LONG:
                    if (val_long_) delete val_long_;
                    val_long_ = nullptr;
                    break;
                case ENTRY_FORMAT_RATIONAL:
                    if (val_rational_) delete val_rational_;
                    val_rational_ = nullptr;
                    break;
                case ENTRY_FORMAT_UNDEFINED:
                    if (val_byte_) delete val_byte_;
                    val_byte_ = nullptr;
                    break;
                case ENTRY_FORMAT_SRATIONAL:
                    if (val_srational_) delete val_srational_;
                    val_srational_ = nullptr;
                    break;
                case 0xff:
                    break;
                default:
                    // TODO should not get here
                    ERROR("delete_union ERROR");
                    break;
            }
        }

        void new_union() {
            switch (format_) {
                case ENTRY_FORMAT_BYTE:
                case ENTRY_FORMAT_UNDEFINED:
                    val_byte_ = new byte_vector();
                    break;
                case ENTRY_FORMAT_ASCII:
                    val_string_ = new ascii_vector();
                    break;
                case ENTRY_FORMAT_SHORT:
                    val_short_ = new short_vector();
                    break;
                case ENTRY_FORMAT_LONG:
                    val_long_ = new long_vector();
                    break;
                case ENTRY_FORMAT_RATIONAL:
                    val_rational_ = new rational_vector();
                    break;
                case ENTRY_FORMAT_SRATIONAL:
                    val_srational_ = new srational_vector();
                    break;
                case 0xff:
                    break;
                default:
                    // TODO should not get here
                    ERROR("new_union ERROR");
                    break;
            }
        }

        /**
         * Get the numerator and denominator for a floating point number
         * @param valIn         Input value to get numerator and denominator
         * @param numerator     Output numerator
         * @param denominator   Output denominator
         */
        void getNumDen(float valIn, int *numerator, int *denominator) {
            if (valIn < 1e-20) {
                *numerator = 0;
                *denominator = 0;
                return;
            }

            float num = valIn;
            int den = 1;
            while ((int)num/den < valIn && den < 1000000 ) {
                num *= 10;
                den *= 10;
            }
            *numerator = (int)num;
            *denominator = den;
        }
    };

    /**
     * IFDirectory structure which contains a list of IFEntries
     */
    struct IFDirectory {
        uint8_t type;
        std::vector<exif::IFEntry> *entries;
        IFDirectory (uint8_t type_, std::vector<exif::IFEntry> *entries_ ) {
            type = type_;
            entries = entries_;
        }
        ~IFDirectory() {
            LOGD("~IFDirectory");
            free(entries);
        }
    };

// Helper functions
    template<typename T>
    T inline parse(const unsigned char *buf, const bool);

    template<>
    uint8_t inline parse<uint8_t>(const unsigned char *buf,
                                  const bool isLittleEndian) {
        return *buf;
    }

    template<>
    uint16_t inline parse<uint16_t>(const unsigned char *buf,
                                    const bool isLittleEndian) {
        if (isLittleEndian) {
            return (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
        } else {
            return (static_cast<uint16_t>(buf[0]) << 8) | buf[1];
        }
    }

    template<>
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

    template<>
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

    template<>
    Rational inline parse<Rational>(const unsigned char *buf,
                                    const bool isLittleEndian) {
        Rational r;
        r.numerator = parse<uint32_t>(buf, isLittleEndian);
        r.denominator = parse<uint32_t>(buf + 4, isLittleEndian);
        return r;
    }

    template<>
    SRational inline parse<SRational>(const unsigned char *buf,
                                      const bool isLittleEndian) {
        SRational r;
        r.numerator = parse<int32_t>(buf, isLittleEndian);
        r.denominator = parse<int32_t>(buf + 4, isLittleEndian);
        return r;
    }

    /**
     * Try to read entry.length() values for this entry.
     *
     * Returns:
     *  true  - entry.length() values were read
     *  false - something went wrong, vec's content was not touched
     */
    template<typename T, typename C>
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
                unsigned char *tmpData = reinterpret_cast<unsigned char *>(&reversed_data);
                unsigned char tmp;
                tmp = tmpData[0];
                tmpData[0] = tmpData[3];
                tmpData[3] = tmp;
                tmp = tmpData[1];
                tmpData[1] = tmpData[2];
                tmpData[2] = tmp;
            }
            data = reinterpret_cast<const unsigned char *>(&(reversed_data));
        } else {
            data = buf + base + entry.data();
            if (data < buf || data + sizeof(T) * entry.length() > buf + len) {
                LOGD("Data start %lu end %lu doesn't fit in buf start %lu end %lu",(unsigned long)data,(unsigned long)(data + sizeof(T) * entry.length()),(unsigned long)buf,(unsigned long)(buf+len));
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
                                   uint8_t directory, IFEntry &result) {
        unsigned short tag;
        unsigned short format;
        unsigned length;
        unsigned data;

        parseIFEntryHeader(buf, isLittleEndian, tag, format, length, data);

        result.tag(tag);
        result.directory(directory);
        result.format(format);
        result.length(length);
        result.data(data);

        LOGD("IFD tag=0x%x (%d) format %d length %d",tag,tag,format,length);
    }

    IFEntry inline parseIFEntry(const unsigned char *buf, const unsigned long offs,
                                bool isLittleEndian, const unsigned long base,
                                const unsigned long len, uint8_t directory) {
        IFEntry result;

        // check if there even is enough data for IFEntry in the buffer
        if (buf + offs + 12 > buf + len) {
            result.tag(0xFF);
            return result;
        }

        parseIFEntryHeader(buf + offs, isLittleEndian, directory, result);

        // Parse value in specified format
        switch (result.format()) {
            case ENTRY_FORMAT_BYTE:
                if (!extract_values<uint8_t>(result.val_byte(), buf, base, isLittleEndian,
                                             len, result)) {
                    ERROR("Error extracting ENTRY_FORMAT_BYTE for %x",result.tag());
                }
                break;
            case ENTRY_FORMAT_ASCII:
                // string is basically sequence of uint8_t so just read it as bytes
                if (!extract_values<uint8_t>(result.val_string(), buf, base,
                                             isLittleEndian, len, result)) {
                    ERROR("Error extracting ENTRY_FORMAT_ASCII for %x",result.tag());
                    result.val_string().resize(0);
                } else {
                    // and cut zero byte at the end, since we don't want that in the
                    // std::string
                    if (result.val_string().length() > 0 &&
                        result.val_string()[result.val_string().length() - 1] == '\0') {
                        result.val_string().resize(result.val_string().length() - 1);
                    }
                }
                break;
            case ENTRY_FORMAT_SHORT:
                if (!extract_values<uint16_t>(result.val_short(), buf, base,
                                              isLittleEndian, len, result)) {
                    ERROR("Error extracting ENTRY_FORMAT_SHORT for %x",result.tag());
                }
                break;
            case ENTRY_FORMAT_LONG:
                if (!extract_values<uint32_t>(result.val_long(), buf, base,
                                              isLittleEndian, len, result)) {
                    ERROR("Error extracting ENTRY_FORMAT_LONG for %x",result.tag());
                }
                break;
            case ENTRY_FORMAT_RATIONAL:
                if (!extract_values<Rational>(result.val_rational(), buf, base,
                                              isLittleEndian, len, result)) {
                    ERROR("Error extracting ENTRY_FORMAT_RATIONAL for %x",result.tag());
                }
                 break;
            case ENTRY_FORMAT_UNDEFINED:
                if (!extract_values<uint8_t>(result.val_byte(), buf, base, isLittleEndian,
                                             len, result)) {
                    ERROR("Error extracting ENTRY_FORMAT_UNDEFINED for %x",result.tag());
                }
                break;
            case ENTRY_FORMAT_SRATIONAL:
                if (!extract_values<SRational>(result.val_srational(), buf, base,
                                               isLittleEndian, len, result)) {
                    ERROR("Error extracting ENTRY_FORMAT_SRATIONAL for %x",result.tag());
                }
                break;
            default:
                ERROR("Unsupported format %d",result.format());
        }
        return result;
    }

    // helper functions for convenience
    template<typename T>
    T inline parse_value(const unsigned char *buf, bool isLittleEndian) {
        return parse<T>(buf, isLittleEndian);
    }

    /**
     * Structure to store non EXIF (0xFFE1) Application Markers
     */
    struct AppMarker {
        uint16_t type;
        uint16_t length;
        unsigned char* buffer;
    };
    bool isAppMarker(const unsigned char *buf, uint16_t *type, uint16_t *length);

    unsigned long getDataStart(const unsigned char *buf, unsigned long len);

    /**
     * Class responsible for storing and parsing EXIF information from a JPEG blob
     */
    class EXIFInfo {
    public:
        bool readEXIF(std::string inputFile);

        bool readEXIF(unsigned char *buf, unsigned long bufLen);

        void encodeJPEGHeader(unsigned char **buf, unsigned long *len);

        std::string toString();
        std::string toString(int directory);

        IFEntry* getTagData(uint16_t tag, uint8_t dir);

        int removeEntry(uint16_t tag, uint8_t dir);
        void updateEntry(IFEntry *entry);

        void clear();

        std::vector<IFDirectory*> IFDirectories;
        std::vector<AppMarker*> AppMarkers;

        EXIFInfo() {
        }

        ~EXIFInfo() {
            LOGD("~EXIFInfo");
            for (unsigned long i=0; i<IFDirectories.size(); i++) {
                free(IFDirectories.at(i));
            }
            IFDirectories.clear();
        }
    private:
        IFDirectory* getDirectory(int type);
        IFDirectory* addDirectory(int type, std::vector<exif::IFEntry> *entries);
        bool isInDirectory(uint16_t tag, uint8_t dir);
        uint16_t encodeEXIFsegment(unsigned char *buf);
        AppMarker* getAppMarker(const unsigned char *buf);
        bool decodeJPEGFile(const unsigned char *buf, unsigned long len);
        bool decodeEXIFsegment(AppMarker *marker);
       };

}  // namespace exif

#endif
