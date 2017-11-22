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
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "exif.h"

using std::string;

exif::TagInfo tagInfoData[] = {
        {EXIF_TAG_BITS_PER_SAMPLE,   ENTRY_FORMAT_SHORT, EXIF_MAIN_DIRECTORY, 1, "Bits Per Sample",""},
        {EXIF_TAG_IMAGE_DESCRIPTION, ENTRY_FORMAT_ASCII, EXIF_MAIN_DIRECTORY, 0, "Image Description",""},
        {EXIF_TAG_DIGICAM_MAKE, ENTRY_FORMAT_ASCII, EXIF_MAIN_DIRECTORY, 0, "Camera make",""},
        {EXIF_TAG_DIGICAM_MODEL, ENTRY_FORMAT_ASCII, EXIF_MAIN_DIRECTORY, 0, "Camera model",""},
        {EXIF_TAG_ORIENTATION, ENTRY_FORMAT_SHORT, EXIF_MAIN_DIRECTORY, 1, "Image Orientation",""},
        {EXIF_TAG_X_RESOLUTION, ENTRY_FORMAT_RATIONAL, EXIF_MAIN_DIRECTORY, 1, "X Resolution",""},
        {EXIF_TAG_Y_RESOLUTION, ENTRY_FORMAT_RATIONAL, EXIF_MAIN_DIRECTORY, 1, "Y Resolution",""},
        {EXIF_TAG_RESOLUTION_UNIT, ENTRY_FORMAT_SHORT, EXIF_MAIN_DIRECTORY, 1, "Resolution Unit"," (1-noUnit, 2-inches, 3-cm)"},
        {EXIF_TAG_SOFTWARE, ENTRY_FORMAT_ASCII, EXIF_MAIN_DIRECTORY, 0, "Software",""},
        {EXIF_TAG_MODIFY_DATE_TIME, ENTRY_FORMAT_ASCII, EXIF_MAIN_DIRECTORY, 0, "Image date/time",""},
        {EXIF_TAG_WHITE_POINT, ENTRY_FORMAT_RATIONAL, EXIF_MAIN_DIRECTORY, 2, "White Point",""},
        {EXIF_TAG_PRIMARY_CHROMA, ENTRY_FORMAT_RATIONAL, EXIF_MAIN_DIRECTORY, 6, "Primary Chromaticities",""},
        {EXIF_TAG_YCBCR_COEFF, ENTRY_FORMAT_RATIONAL, EXIF_MAIN_DIRECTORY, 3, "YCbCr Coefficients",""},
        {EXIF_TAG_YCBCR_POSITION, ENTRY_FORMAT_SHORT, EXIF_MAIN_DIRECTORY, 1, "YCbCr Positioning"," (1-Centered, 2-Co-sited)"},
        {EXIF_TAG_REF_BW, ENTRY_FORMAT_RATIONAL, EXIF_MAIN_DIRECTORY, 6, "Reference Black White",""},
        {EXIF_TAG_COPYRIGHT, ENTRY_FORMAT_ASCII, EXIF_MAIN_DIRECTORY, 0, "Copyright",""},
        {EXIF_TAG_SUB_IFD_OFFSET, ENTRY_FORMAT_LONG, EXIF_MAIN_DIRECTORY, 1, "SUB IFD Offset",""},
        {EXIF_TAG_GPS_IFD_OFFSET, ENTRY_FORMAT_LONG, EXIF_MAIN_DIRECTORY, 1, "GPS IFD Offset",""},
        {EXIF_TAG_10_IFD_OFFSET, ENTRY_FORMAT_LONG, EXIF_MAIN_DIRECTORY, 1, "10 IFD Offset",""},

        {EXIF_TAG_EXPOSURE_TIME, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "Exposure Time"," s"},
        {EXIF_TAG_FNUMBER, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "F-stop",""},
        {EXIF_TAG_EXPOSURE_PROGRAM, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "Exposure Program",""},
        {EXIF_TAG_ISO_SPEED_RATING, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "ISO Speed",""},
        {EXIF_TAG_EXIF_VERSION, ENTRY_FORMAT_UNDEFINED, EXIF_SUB_DIRECTORY, 4, "Exif Version",""},
        {EXIF_TAG_ORIGINAL_DATE, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "Original date/time",""},
        {EXIF_TAG_DIGITIZATION_DATE, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "Digitize date/time",""},
        {EXIF_TAG_COMPONENTS_CONFIG, ENTRY_FORMAT_UNDEFINED, EXIF_SUB_DIRECTORY, 4, "Components Configuration",""},
        {EXIF_TAG_COMPRESSED_BPP, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "Compressed BitsPerPixel",""},
        {EXIF_TAG_SHUTTER_SPEED, ENTRY_FORMAT_SRATIONAL, EXIF_SUB_DIRECTORY, 1, "Shutter Speed Value"," s"},
        {EXIF_TAG_APERTURE_VALUE, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "Aperture Value",""},
        {EXIF_TAG_BRIGHTNESS_VALUE, ENTRY_FORMAT_SRATIONAL, EXIF_SUB_DIRECTORY, 1, "Brightness Value",""},
        {EXIF_TAG_EXPOSURE_BIAS, ENTRY_FORMAT_SRATIONAL, EXIF_SUB_DIRECTORY, 1, "Exposure Bias"," EV"},
        {EXIF_TAG_MAX_APERTURE, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "Max Aperture Value"," m"},
        {EXIF_TAG_SUBJECT_DIST, ENTRY_FORMAT_SRATIONAL, EXIF_SUB_DIRECTORY, 1, "Subject Distance"," m"},
        {EXIF_TAG_METERING_MODE, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "Metering Mode",""},
        {EXIF_TAG_LIGHT_SOURCE, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "Light Source"," (1-average, 2-center weighted, 3-spot, 4-multiSpot, 5-multiSegment"},
        {EXIF_TAG_FLASH_USED, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "Flash Used",""},
        {EXIF_TAG_FOCAL_LENGTH, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "Focal Length"," mm"},
        {EXIF_TAG_SUBJECT_LOCATION, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 4, "Subject Location",""},
        {EXIF_TAG_MAKER_NOTE, ENTRY_FORMAT_UNDEFINED, EXIF_SUB_DIRECTORY, 1, "Maker Note",""},
        {EXIF_TAG_USER_COMMENT, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "User Comment",""},
        {EXIF_TAG_SUB_SEC_TIME, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "Subsec time",""},
        {EXIF_TAG_SUB_SEC_ORIG_TIME, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "Subsec orig time",""},
        {EXIF_TAG_DIGITIZED_TIME, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "Digitize date/time",""},
        {EXIF_TAG_FLASH_PIX_VERSION, ENTRY_FORMAT_UNDEFINED, EXIF_SUB_DIRECTORY, 4, "Flashpix Version",""},
        {EXIF_TAG_COLOR_SPACE, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "ColorSpace",""},
        {EXIF_TAG_IMAGE_WIDTH, ENTRY_FORMAT_LONG, EXIF_SUB_DIRECTORY, 1, "Image Width",""},
        {EXIF_TAG_IMAGE_HEIGHT, ENTRY_FORMAT_LONG, EXIF_SUB_DIRECTORY, 1, "Image Height",""},
        {EXIF_TAG_SOUND_FILE, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "Related Sound File",""},
        {EXIF_TAG_INTEROP_OFFSET, ENTRY_FORMAT_LONG, EXIF_SUB_DIRECTORY, 1, "Interop Offset",""},
        {EXIF_TAG_FOCAL_X_RESOLUTION, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "Focal plane XRes",""},
        {EXIF_TAG_FOCAL_Y_RESOLUTION, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "Focal plane YRes",""},
        {EXIF_TAG_FOCAL_RES_UNIT, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "Focal plane Resolution Unit"," (1-noUnit, 2-inch, 3-cm"},
        {EXIF_TAG_SENSING_METHOD, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "Sensing Method",""},
        {EXIF_TAG_FILE_SOURCE, ENTRY_FORMAT_UNDEFINED, EXIF_SUB_DIRECTORY, 1, "File Source",""},
        {EXIF_TAG_SCENE_TYPE, ENTRY_FORMAT_UNDEFINED, EXIF_SUB_DIRECTORY, 1, "Scene Type",""},
        {EXIF_TAG_CUSTOM_RENDERED, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "Custom Rendered"," (0-Normal, 1-Custom)"},
        {EXIF_TAG_EXPOSURE_MODE, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "Exposure Mode"," (0-Auto, 1-Manual, 2-Auto-Bracket)"},
        {EXIF_TAG_WHITE_BALANCE, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "White Balance"," (0-Auto, 1-Manual)"},
        {EXIF_TAG_DIGITAL_ZOOM_RATIO, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "Digital Zoom Ratio",""},
        {EXIF_TAG_FOCAL_LENGTH35MM, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "35mm Focal Length","mm"},
        {EXIF_TAG_SCENE_CAPTURE_TYPE, ENTRY_FORMAT_SHORT, EXIF_SUB_DIRECTORY, 1, "Scene Capture Type",""},
        {EXIF_TAG_FOCAL_LENGTH_FSTOP, ENTRY_FORMAT_RATIONAL, EXIF_SUB_DIRECTORY, 1, "Focal Length/FStop Min/Max",""},
        {EXIF_TAG_LENS_MAKE, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "Lens Make",""},
        {EXIF_TAG_LENS_MODEL, ENTRY_FORMAT_ASCII, EXIF_SUB_DIRECTORY, 0, "Lens Model",""},

        {EXIF_TAG_GPS_VERSION_ID, ENTRY_FORMAT_BYTE, EXIF_GPS_DIRECTORY, 4, "GPS Version ID",""},
        {EXIF_TAG_GPS_LATITUDE_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 2, "GPS Latitude Ref",""},
        {EXIF_TAG_GPS_LATITUDE, ENTRY_FORMAT_RATIONAL, EXIF_GPS_DIRECTORY, 0, "GSP Latitude",""},
        {EXIF_TAG_GPS_LONGITUDE_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Longitude Ref",""},
        {EXIF_TAG_GPS_LONGITUDE, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Longitude",""},
        {EXIF_TAG_GPS_ALTITUDE_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Altitude Ref",""},
        {EXIF_TAG_GPS_ALTITUDE, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Altitude",""},
        {EXIF_TAG_GPS_TIME_STAMP, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Time Stamp",""},
        {EXIF_TAG_GPS_SATELLITES, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Satellites",""},
        {EXIF_TAG_GPS_STATUS, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Status",""},
        {EXIF_TAG_GPS_MEASURE_MODE, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Measure Mode",""},
        {EXIF_TAG_GPS_DOP, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS DOP",""},
        {EXIF_TAG_GPS_SPEED_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Speed Ref",""},
        {EXIF_TAG_GPS_SPEED, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Speed",""},
        {EXIF_TAG_GPS_TRACK_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Track Ref",""},
        {EXIF_TAG_GPS_TRACK, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Track",""},
        {EXIF_TAG_GPS_IMG_DIR_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Image Direction Ref",""},
        {EXIF_TAG_GPS_IMG_DIR, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Image Direction",""},
        {EXIF_TAG_GPS_MAP_DATUM, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Map Datum",""},
        {EXIF_TAG_GPS_DEST_LAT_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Destination Latitude Ref",""},
        {EXIF_TAG_GPS_DEST_LATITUDE, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Destination Latitude",""},
        {EXIF_TAG_GPS_DEST_LONG_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Destination Longitude Ref",""},
        {EXIF_TAG_GPS_DEST_LONGITUDE, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Destination Longitude",""},
        {EXIF_TAG_GPS_DEST_BEARING_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Destination Bearing Ref",""},
        {EXIF_TAG_GPS_DEST_BEARING, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Destination Bearing",""},
        {EXIF_TAG_GPS_DEST_DIST_REF, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Destination Distance Ref",""},
        {EXIF_TAG_GPS_DEST_DIST, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Destination Distance",""},
        {EXIF_TAG_GPS_PROCESSING_METHOD, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Processing Method",""},
        {EXIF_TAG_GPS_AREA_INFO, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Area Info",""},
        {EXIF_TAG_GPS_DATE_STAMP, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Date Stamp",""},
        {EXIF_TAG_GPS_DIFFERENTIAL, ENTRY_FORMAT_ASCII, EXIF_GPS_DIRECTORY, 0, "GPS Differential",""},
        {EXIF_TAG_GPS_HORIZ_POS_ERR, ENTRY_FORMAT_RATIONAL, EXIF_GPS_DIRECTORY, 1, "GPS Horizontal Positioning Error",""},

        {EXIF_10_SCENE_TYPE, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "Scene Type",""},
        {EXIF_10_SCENE_PROBABILITY, ENTRY_FORMAT_RATIONAL, EXIF_10_DIRECTORY, 1, "Scene Probability",""},
        {EXIF_10_SCENE_SUB_TYPES, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "Scene Sub Types"," (comma separated)"},
        {EXIF_10_SCENE_SUB_PROBS, ENTRY_FORMAT_RATIONAL, EXIF_10_DIRECTORY, 0, "Scene Probabilities for Sub Types",""},
        {EXIF_10_SCENE_MODEL, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "Scene Model",""},
        {EXIF_10_SCENE_RESULT, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "Scene Training Result",""},
        {EXIF_10_CORR_SCENE_TYPE, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "Corrected Scene Type",""},
        {EXIF_10_ENHANCEMENT_STRENGTH, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "Enhancement Strength",""},
        {EXIF_10_WHITE_BALANCE, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "White Balance",""},
        {EXIF_10_AF_MODE, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "AF Mode",""},
        {EXIF_10_FOCUS_DISTANCE, ENTRY_FORMAT_RATIONAL, EXIF_10_DIRECTORY, 1, "Focus Distance",""},
        {EXIF_10_PANO_NUM_IMAGES, ENTRY_FORMAT_SHORT, EXIF_10_DIRECTORY, 1, "Panostitch Number of Input Images",""},
        {EXIF_10_PANO_NUM_STITCHES, ENTRY_FORMAT_SHORT, EXIF_10_DIRECTORY, 1, "Panotstitch Number of Stitches",""},
        {EXIF_10_ROI_CASCADE, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "ROI Cascade",""},
        {EXIF_10_ROI_RECTS, ENTRY_FORMAT_SHORT, EXIF_10_DIRECTORY, 0, "ROI Rectangles"," (groups of 4: x y w h"},
        {EXIF_10_ROI_TYPES, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "ROI Types"," (Either 1 type for all or type for each ROI Rect}"},
        {EXIF_10_ROI_RESULTS, ENTRY_FORMAT_SHORT, EXIF_10_DIRECTORY, 0, "ROI Training Results"," (0-Detected, 1-Correct, 2-Incorrect, 3-Added)"},
        {EXIF_10_ROI_ENHANCEMENTS, ENTRY_FORMAT_ASCII, EXIF_10_DIRECTORY, 0, "ROI Enhancements"," (comma separated list of enhancements for each ROI)"},
        {EXIF_10_VERSION, ENTRY_FORMAT_SHORT, EXIF_10_DIRECTORY, 1, "10 Version",""},

        {EXIF_TAG_INTEROP_INDEX, ENTRY_FORMAT_ASCII, EXIF_INTEROP_DIRECTORY, 0, "Interop Index",""},
        {EXIF_TAG_INTEROP_VERSION, ENTRY_FORMAT_UNDEFINED, EXIF_INTEROP_DIRECTORY, 0, "Interop Version",""},

};

/**
 * Get the TagInfo given a tag and directory
 * @param tag   Input tag id to find
 * @param dir   Input directory id
 * @return Pointer to TagInfo or a default value if no TagInfo found
 */
exif::TagInfo* exif::getTagInfo(uint16_t tag, uint8_t dir) {

    int len = sizeof(tagInfoData)/sizeof(exif::TagInfo);

    for (int i=0; i< len; i++) {
        if (tag == tagInfoData[i].tag && dir == tagInfoData[i].directory)
            return &tagInfoData[i];
    }
    LOGD("Couldn't find tag %x %d",tag, dir);
    char *tagName = (char*)malloc(6);
    sprintf(tagName,"%x",tag);
    return new exif::TagInfo(tag,ENTRY_FORMAT_LONG,dir,1,tagName,"");
}

/**
 * Locates the EXIF segment and returns the length including the JPEG_SOI & EXIF header
 * @param buf   Buffer containing the JPEG EXIF data, staring with JPEG_SOI (FFD8)
 * @param len   Input length of the buffer
 * @return The size of the EXIF segment within the given buffer
 */
unsigned long exif::getDataStart(const unsigned char *buf, unsigned long len) {
    // Sanity check: all JPEG files start with JPEG_SOI
    if (!buf || len < 4) return 0;
    if (parse_value<uint16_t>(buf,false) != JPEG_SOI) return 0;

    unsigned long offs = 2; // Skip JPEG_SOI
    uint16_t type;
    uint16_t length;
    while (isAppMarker(&buf[offs],&type,&length)) {
        offs += length+2; //Marker type and length
    }

    return offs;
}

/**
 * Get an Application Marker starting at buf location.  An App Marker is a JPEG marker of type 0xFFEx
 * @param buf   Buffer containing App marker
 * @return  Pointer to a new AppMarker
 */
exif::AppMarker* exif::EXIFInfo::getAppMarker(const unsigned char *buf) {
    AppMarker *newMarker = new AppMarker();
    newMarker->type = parse_value<uint16_t>(buf, false);
    newMarker->length = parse_value<uint16_t>(&buf[2], false);
    int bufLen = newMarker->length-2; //Don't include length field
    newMarker->buffer = (unsigned char*)malloc((size_t)bufLen);
    memcpy(newMarker->buffer,&buf[4],(size_t)bufLen);
    return newMarker;
}

/**
 * Check if next item in the buffer is an app marker
 * @param buf       Start of buffer to check for App marker
 * @param type      Type ID of potential App Marker (0xFFEx)
 * @param length    Length of potential App Marker
 * @return True if type is an App Marker, otherwise false if not an App Marker
 */
bool exif::isAppMarker(const unsigned char *buf, uint16_t *type, uint16_t *length) {
    *type = parse_value<uint16_t>(buf, false);
    *length = parse_value<uint16_t>(&buf[2], false);
    return (*type & 0xFFE0) == 0xFFE0;
}

/**
 * Check if given App Marker is an Exif Marker.  Type is 0xFFE1 and starts with "Exif\0\0"
 * @param marker    AppMarker to check
 * @return True if Marker is an Exif Marker, otherwise false
 */
bool isExifMarker(exif::AppMarker *marker) {
    if (marker->type != EXIF_MARKER) return false;

    return (std::equal(marker->buffer, marker->buffer + 6, "Exif\0\0"));
}

/**
 * Decode JPEG file given a unsigned char buf.
 * Starts with JPEG_SOI (0xFFD8) followed by different markers.  We store any App markers
 * (ones of type 0xFFEx) and decode the Exif App Marker (0xFFE1) further.  Other Markers
 * such as FFC4 (Huffman Table) and FFDB (Quantization Table) are not saved since they are
 * part of the Image data
 * @param buf       buffer to decode
 * @param bufLen    Length of buffer to decode
 * @return True if decoding was successful, otherwise false
 */
bool exif::EXIFInfo::decodeJPEGFile(const unsigned char *buf, unsigned long bufLen) {
    bool retVal = true;
    // Sanity check: all JPEG files start with JPEG_SOI.
    if (!buf || bufLen < 4) return 0;
    if (parse_value<uint16_t>(buf,false) != JPEG_SOI) return false;

    unsigned long offs = 2; // Skip JPEG_SOI
    uint16_t type, length;
    while (offs < bufLen && isAppMarker(&buf[offs],&type,&length)) {
        AppMarker *marker = getAppMarker(&buf[offs]);
        offs += marker->length+2;
        if (isExifMarker(marker)) {
            retVal &= decodeEXIFsegment(marker);
            free(marker);
        } else {
            AppMarkers.push_back(marker);
            LOGD("Found marker %x len %x", marker->type, marker->length);
        }
    }
    return retVal;
}


/**
 * Read in given file and parse into EXIFInfo structure
 * @param inputFile     Full path of file to read
 * @return  True if reading/parsing were successful
 */
bool exif::EXIFInfo::readEXIF(std::string inputFile) {
    // Read the JPEG file into a buffer
    FILE *fp = fopen(inputFile.data(), "rb");
    if (!fp) {
        ERROR("File not found %s",inputFile.c_str());
        return false;
    }
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = (unsigned long)ftell(fp);
    rewind(fp);
    unsigned char *buf = new unsigned char[fsize];
    if (fread(buf, 1, fsize, fp) != fsize) {
        ERROR("Cannot read file %s ",inputFile.c_str());
        delete[] buf;
        fclose(fp);
        return false;
    }

    fclose(fp);
    bool rval = readEXIF(buf,fsize);
    LOGD("Free buf");
    free(buf);
    return rval;
}


/**
 * Parse the given buffer into EXIFInfo
 * @param buf       Input buffer to parse
 * @param bufLen    Length of input buffer
 * @return True if parsing was succesful
 */
bool exif::EXIFInfo::readEXIF(unsigned char *buf, unsigned long bufLen) {
    return decodeJPEGFile(buf, bufLen);
}

/**
 * Get the value of the entry as a string
 * @param entry     Input entry to get value for
 * @return  Value of entry as a string
 */
std::string getValStr(exif::IFEntry entry) {
    char tmp[20];
    string tmpStr;
    //LOGD("Tag %x Format %d length %d",entry.tag(),entry.format(),entry.length());

    if (entry.length() > MAX_TO_PRINT && entry.format()!=ENTRY_FORMAT_ASCII) {
        sprintf(tmp,"%d values...",entry.length());
        tmpStr.append(tmp);
    } else {
        switch (entry.format()) {
            case ENTRY_FORMAT_BYTE:
            case ENTRY_FORMAT_UNDEFINED: {
                for (unsigned long i = 0; i < entry.val_byte().size(); i++) {
                    sprintf(tmp, "%x ", entry.val_byte().at(i));
                    tmpStr.append(tmp);
                }
                break;
            }
            case ENTRY_FORMAT_ASCII: {
                tmpStr = entry.val_string();
                std::replace(tmpStr.begin(), tmpStr.end(), '\0', ' ');
                break;
            }
            case ENTRY_FORMAT_SHORT:
                for (unsigned long i = 0; i < entry.val_short().size(); i++) {
                    sprintf(tmp, "%d ", entry.val_short().at(i));
                    tmpStr.append(tmp);
                }
                break;
            case ENTRY_FORMAT_LONG:
                for (unsigned long i = 0; i < entry.val_long().size(); i++) {
                    sprintf(tmp, "%d ", entry.val_long().at(i));
                    tmpStr.append(tmp);
                }
                break;
            case ENTRY_FORMAT_RATIONAL:
                for (unsigned long i = 0; i < entry.val_rational().size(); i++) {
                    tmpStr.append(entry.val_rational().at(i).toString());
                    tmpStr.append(" ");
                }
                break;
            case ENTRY_FORMAT_SRATIONAL:
                for (unsigned long i = 0; i < entry.val_srational().size(); i++) {
                    tmpStr.append(entry.val_srational().at(i).toString());
                    tmpStr.append(" ");
                }
                break;
            default:
                ERROR("Unknown format %d", entry.format());
                break;
        }
    }
    return tmpStr;
}

/**
 * Comparison function used to sort Tags in ascending order
 * @param i     First IFEntry to compare
 * @param j     Second IFEntry to compare
 * @return True if first entry is smaller than 2nd entry
 */
bool tagComparator(exif::IFEntry i, exif::IFEntry j) {
    return (i.tag() < j.tag());
}

/**
 * Output list of IFEntries as a string
 * @param entries   Input entries to output
 * @return String outputing all IFEntries
 */
std::string entries_toString(std::vector<exif::IFEntry> *entries) {
    std::string str;

    std::sort(entries->begin(),entries->end(),tagComparator); // Sort in case of new entries
    for (unsigned long i = 0; i < entries->size(); i++) {
        exif::IFEntry entry = entries->at(i);
        exif::TagInfo *tagInfo = exif::getTagInfo(entry.tag(),entry.directory());
        str.append(tagInfo->name);
        str.append(": ");
        str.append(getValStr(entry));
        str.append(tagInfo->desc);
        str.append("\n");
    }
    return str;
}

/**
 * Output the EXIF data as a string.  Includes all directories.
 * @return EXIF data as a string
 */
std::string exif::EXIFInfo::toString() {
    string str;

    for (unsigned long i=0; i<IFDirectories.size(); i++) {
        str.append(entries_toString(IFDirectories.at(i)->entries));
    }

    return str;
}

/**
 * Output given directory as a string
 * @param directory     Directory to output
 * @return Directory data as a String
 */
std::string exif::EXIFInfo::toString(int directory) {

    return entries_toString(getDirectory(directory)->entries);
}

/**
 * Get pointer to given Directory type in EXIFInfo or create a new empty directory if it doesn't exist
 * @param type  Directory type to get
 * @return Pointer to existing IFDirectory or new/empty IFDirectory
 */
exif::IFDirectory* exif::EXIFInfo::getDirectory(int type) {
    for (unsigned long i=0; i<IFDirectories.size(); i++) {
        if (IFDirectories.at(i)->type == type) {
            return IFDirectories.at(i);
        }
    }
    return addDirectory(type,new std::vector<exif::IFEntry>);
}

/**
 * Add a new IFDirectory of given type and with list of entries
 * @param type      Directory type to add
 * @param entries   Entries in IFDirectory to add
 * @return pointer new new IFDirectory
 */
exif::IFDirectory* exif::EXIFInfo::addDirectory(int type, std::vector<exif::IFEntry> *entries) {
    IFDirectory *directory = new IFDirectory((uint8_t)type,entries);
    IFDirectories.push_back(directory);
    return directory;
}

/**
 * Given a buffer containing EXIF data, parse/decode the EXIF data into list of IFDirectories
 * @param marker   Exif Marker data.  Buffer starts at the EXIF TIFF data ("Exif\0\0").
 * @return True if decoding was successful, otherwise return false
 */
bool exif::EXIFInfo::decodeEXIFsegment(AppMarker *marker) {
    LOGD("In decodeExif");
    bool isLittleEndian;        // byte alignment (defined in EXIF header)
    unsigned long offs = 0;     // current offset into buffer

    if (!isExifMarker(marker)) {
        ERROR("Not an Exif Marker");
        return false;
    }
    unsigned char *buf = marker->buffer;
    offs += 6; // Skip "Exif\0\0"

    // Now parsing the TIFF header. The first two bytes are either "II" or
    // "MM" for Intel or Motorola byte alignment. Sanity check by parsing
    // the unsigned short that follows, making sure it equals 0x2a. The
    // last 4 bytes are an offset into the first IFD, which are added to
    // the global offset counter. For this block, we expect the following
    // minimum size:
    //  2 bytes: 'II' or 'MM'
    //  2 bytes: 0x002a
    //  4 bytes: offset to first IDF
    // -----------------------------
    //  8 bytes
    if (offs + 8 > marker->length) {
        ERROR("Marker not long enough");
        return false;
    }
    unsigned long tiff_header_start = offs;
    if (buf[offs] == 'I' && buf[offs + 1] == 'I')
        isLittleEndian = true;
    else {
        if (buf[offs] == 'M' && buf[offs + 1] == 'M')
            isLittleEndian = false;
        else {
            ERROR("Unknown byte align");
            return false;
        }
    }

    offs += 2;
    if (0x2a != parse_value<uint16_t>(buf + offs, isLittleEndian)) {
        ERROR("0x2a value is missing");
        return false;
    }

    offs += 2;
    unsigned long first_ifd_offset =
            parse_value<uint32_t>(buf + offs, isLittleEndian);
    offs += first_ifd_offset - 4;
    if (offs >= marker->length) {
        ERROR("Marker not long enough");
        return false;
    }
    LOGD("First IFD offset 0x%x", (int)first_ifd_offset);

    // Now parsing the first Image File Directory (IFD0, for the main image).
    // An IFD consists of a variable number of 12-byte directory entries. The
    // first two bytes of the IFD section contain the number of directory
    // entries in the section. The last 4 bytes of the IFD contain an offset
    // to the next IFD, which means this IFD must contain exactly 6 + 12 * num
    // bytes of data.
    if (offs + 2 > marker->length) {
        ERROR("Marker not long enough");
        return false;
    }
    int num_entries = parse_value<uint16_t>(buf + offs, isLittleEndian);
    LOGD("IFD entries %d", num_entries);

    if (offs + 6 + 12 * num_entries > marker->length) {
        ERROR("Marker not long enough");
        return false;
    }
    offs += 2;
    unsigned long exif_sub_ifd_offset = 0;
    unsigned long ifd_offset_interop = 0;
    unsigned long ifd_offset_gps = 0;
    unsigned long ifd_offset_10 = 0;

    std::vector<exif::IFEntry> *Main_IFentries = new std::vector<exif::IFEntry>;

    while (--num_entries >= 0) {
        IFEntry result = parseIFEntry(buf, offs, isLittleEndian, tiff_header_start, marker->length, EXIF_MAIN_DIRECTORY);
        LOGD("Entry %x %d %d", result.tag(), result.format(), result.length());
        offs += 12;
        switch (result.tag()) {
            case EXIF_TAG_SUB_IFD_OFFSET:
                exif_sub_ifd_offset = tiff_header_start + result.data();
                break;
            case EXIF_TAG_GPS_IFD_OFFSET:
                ifd_offset_gps = tiff_header_start + result.data();
                break;
            case EXIF_TAG_10_IFD_OFFSET:
                ifd_offset_10 = tiff_header_start + result.data();
                break;
            default:
                Main_IFentries->push_back(result);
                break;
        }
    }
    addDirectory(EXIF_MAIN_DIRECTORY,Main_IFentries);

    LOGD("Main IFentries after read %d", (int) Main_IFentries->size());

    // Jump to the EXIF SubIFD if it exists and parse all the information
    // there. Note that it's possible that the EXIF SubIFD doesn't exist.
    // The EXIF SubIFD contains most of the interesting information that a
    // typical user might want.
    if (exif_sub_ifd_offset != 0 && exif_sub_ifd_offset + 4 <= marker->length) {
        std::vector<exif::IFEntry> *Sub_IFentries = new std::vector<exif::IFEntry>;

        offs = exif_sub_ifd_offset;
        num_entries = parse_value<uint16_t>(buf + offs, isLittleEndian);
        LOGD("SubIFD entries %d", num_entries);
        if (offs + 6 + 12 * num_entries > marker->length) {
            ERROR("Marker not long enough");
            return false;
        }
        offs += 2;
        while (--num_entries >= 0) {
            IFEntry result = parseIFEntry(buf, offs, isLittleEndian, tiff_header_start, marker->length, EXIF_SUB_DIRECTORY);
            offs += 12;
            switch (result.tag()) {
                case EXIF_TAG_INTEROP_OFFSET:
                    ifd_offset_interop = tiff_header_start + result.data();
                    break;
                default:
                    Sub_IFentries->push_back(result);
                    break;
            }
        }
        addDirectory(EXIF_SUB_DIRECTORY,Sub_IFentries);
        LOGD("Sub IFentries after read %d", (int) Sub_IFentries->size());
    }


    // Jump to the GPS SubIFD if it exists and parse all the information
    // there. Note that it's possible that the GPS SubIFD doesn't exist.
    if (ifd_offset_gps != 0 && ifd_offset_gps + 4 <= marker->length) {
        std::vector<exif::IFEntry> *GPS_IFentries = new std::vector<exif::IFEntry>;

        offs = ifd_offset_gps;
        num_entries = parse_value<uint16_t>(buf + offs, isLittleEndian);
        LOGD("Num GPS entries %d", num_entries);
        if (offs + 6 + 12 * num_entries > marker->length) {
            ERROR("Marker not long enough");
            return false;
        }
        offs += 2;
        while (--num_entries >= 0) {
            IFEntry result = parseIFEntry(buf, offs, isLittleEndian, tiff_header_start, marker->length, EXIF_GPS_DIRECTORY);
            GPS_IFentries->push_back(result);
            offs += 12;
        }
        addDirectory(EXIF_GPS_DIRECTORY,GPS_IFentries);
        LOGD("GPS Entries after read %d",(int)GPS_IFentries->size());
    }

    // Jump to the 10 SubIFD if it exists and parse all the information
    // there. Note that it's possible that the IFD doesn't exist.
    if (ifd_offset_10 != 0 && ifd_offset_10 + 4 <= marker->length) {
        std::vector<exif::IFEntry> *IFentries = new std::vector<exif::IFEntry>;

        offs = ifd_offset_10;
        num_entries = parse_value<uint16_t>(buf + offs, isLittleEndian);
        LOGD("Num 10 entries %d", num_entries);
        if (offs + 6 + 12 * num_entries > marker->length) {
            ERROR("Marker not long enough");
            return false;
        }
        offs += 2;
        while (--num_entries >= 0) {
            IFEntry result = parseIFEntry(buf, offs, isLittleEndian, tiff_header_start, marker->length, EXIF_10_DIRECTORY);
            IFentries->push_back(result);
            offs += 12;
        }
        addDirectory(EXIF_10_DIRECTORY,IFentries);
    }

    // Jump to the Interop IFD if it exists and parse all the information
    // there. Note that it's possible that the IFD doesn't exist.
    if (ifd_offset_interop != 0 && ifd_offset_interop + 4 <= marker->length) {
        std::vector<exif::IFEntry> *IFentries = new std::vector<exif::IFEntry>;

        offs = ifd_offset_interop;
        num_entries = parse_value<uint16_t>(buf + offs, isLittleEndian);
        LOGD("Num Interop entries %d", num_entries);
        if (offs + 6 + 12 * num_entries > marker->length) {
            ERROR("Marker not long enough");
            return false;
        }
        offs += 2;
        while (--num_entries >= 0) {
            IFEntry result = parseIFEntry(buf, offs, isLittleEndian, tiff_header_start, marker->length, EXIF_INTEROP_DIRECTORY);
            IFentries->push_back(result);
            offs += 12;
        }
        addDirectory(EXIF_INTEROP_DIRECTORY,IFentries);
    }


    return true;
}

/**
 * Write 2 bytes in big endian format
 * @param buf   Buffer location to write data
 * @param val   16 bit value to write
 * @return  length of data written (2)
 */
int write_buffer_2(unsigned char *buf, uint16_t val) {
    buf[0] = (unsigned char) (val >> 8);
    buf[1] = (unsigned char) (val & 0xFF);
    return 2;
}

/**
 * Write 4 bytes in big endian format
 * @param buf   Buffer location to write data
 * @param val   32 bit value to write
 * @return  length of data written (4)
 */
int write_buffer_4(unsigned char *buf, uint32_t val) {
    write_buffer_2(buf, (uint16_t) (val >> 16));
    write_buffer_2(&buf[2], (uint16_t) (val & 0xFFFF));
    return 4;
}

/**
 * Get the value from the entry itself or write the value in the data buffer and return the address of the data
 * @param entry         Input entry to get value from
 * @param buf           Starting address of the EXIF buffer for getting the data buffer address
 * @param dataOffset    Offset of data buffer from EXIF buffer
 * @return  Either the actual value of the entry or the address where the value is stored
 */
u_int32_t get_val(exif::IFEntry entry, unsigned char *buf, u_int32_t *dataOffset) {
    u_int32_t dataAddr = *dataOffset - EXIF_START;
    //LOGD("Entry %x format %d length %d",entry.tag(),entry.format(),entry.length());
    unsigned char *dataBuf = &buf[*dataOffset];
    int offset = 0;
    switch (entry.format()) {
        case ENTRY_FORMAT_BYTE:
        case ENTRY_FORMAT_UNDEFINED:
            if (entry.val_byte().size() > 4) {
                memcpy(dataBuf, entry.val_byte().data(), entry.val_byte().size());
                *dataOffset += entry.val_byte().size();
            } else {
                return entry.val_byte()[0] << 24 |
                       entry.val_byte()[1] << 16 |
                       entry.val_byte()[2] << 8 |
                       entry.val_byte()[3];
            }
            break;
        case ENTRY_FORMAT_ASCII:
            if (entry.val_string().size() > 4) {
                memcpy(dataBuf, entry.val_string().c_str(), entry.val_string().length());
                if (entry.length() - entry.val_string().length() == 1)
                    dataBuf[entry.length() - 1] = 0;
                *dataOffset += entry.length();
            } else {
                return (u_int32_t ) entry.val_string()[0] << 24 |
                                    entry.val_string()[1] << 16 |
                                    entry.val_string()[2] << 8 |
                                    entry.val_string()[3];
            }

            break;
        case ENTRY_FORMAT_SHORT:
            if (entry.val_short().size() > 2) {
                for (unsigned long i=0; i<entry.val_short().size(); i++) {
                    offset += write_buffer_2(&dataBuf[offset],entry.val_short().at(i));
                }
                *dataOffset += offset;
            } else {
                return entry.val_short()[0] << 16 |
                       entry.val_short()[1];
            }
            break;
        case ENTRY_FORMAT_LONG:
            if (entry.val_long().size() > 1) {
                for (unsigned long i=0; i<entry.val_long().size(); i++) {
                    offset += write_buffer_4(&dataBuf[offset],entry.val_long().at(i));
                }
                *dataOffset += offset;
            } else
                return entry.val_long().front();
            break;
        case ENTRY_FORMAT_RATIONAL:
            for (unsigned long i=0; i<entry.val_rational().size(); i++) {
                offset += write_buffer_4(&dataBuf[offset], entry.val_rational().at(i).numerator);
                offset += write_buffer_4(&dataBuf[offset], entry.val_rational().at(i).denominator);
            }
            *dataOffset += offset;
            break;
        case ENTRY_FORMAT_SRATIONAL:
            for (unsigned long i=0; i<entry.val_srational().size(); i++) {
                offset += write_buffer_4(&dataBuf[offset], (u_int32_t) entry.val_srational().at(i).numerator);
                offset += write_buffer_4(&dataBuf[offset], (u_int32_t) entry.val_srational().at(i).denominator);
            }
            *dataOffset += offset;
            break;
        default:
            ERROR("Unsupported format %d",entry.format());
            return 0;
    }
    return dataAddr;
}

/**
 * Write the given entry to the buffer in big endian format
 *      Each directory entry is composed of:
 *          2 bytes: tag number (data field)
 *          2 bytes: data format
 *          4 bytes: number of components
 *          4 bytes: data value or offset to data value
 * @param buf           Starting address of the EXIF buffer
 * @param entry_offset  Offset to write entry
 * @param entry         Entry to write
 * @param dataOffset    Offset for data buffer to write values
 */
void write_entry(unsigned char *buf, u_int32_t entry_offset, exif::IFEntry entry, u_int32_t *dataOffset) {
    unsigned char *entryBuf = &buf[entry_offset];
    write_buffer_2(entryBuf, entry.tag());
    write_buffer_2(&entryBuf[2], entry.format());
    write_buffer_4(&entryBuf[4], entry.length());
    u_int32_t val = get_val(entry, buf, dataOffset);
    write_buffer_4(&entryBuf[8], val);
    //LOGD("Writing entry %x format %d length %d val %x",entry.tag(),entry.format(),entry.length(),val);
}

/**
 * Write given list of IFD entries to the buffer with any data values following the entry data.  Data is written in big endian format.
 * @param entries   List of entries to write
 * @param buf       Starting location of the EXIF buffer
 * @param offset    Starting offset relative to buffer
 * @param link_offset Offset to write link data.  Default is to not link another directory.
 * @return  Ending offset of group of entries including the entry data
 */
unsigned long write_ifd_entries(std::vector<exif::IFEntry> *entries, unsigned char *buf, unsigned long offset, unsigned long *link_offset) {
    LOGD("IFentries to write %d", (int) entries->size());

    offset += write_buffer_2(&buf[offset], (uint16_t) entries->size());
    u_int32_t dataOffset = (u_int32_t) (offset + entries->size() * ENTRY_SIZE + 4); // End of fixed section.  4 bytes for next IFD

    for (unsigned long i = 0; i < entries->size(); i++) {
        exif::IFEntry entry = entries->at(i);
        write_entry(buf, (u_int32_t) offset, entry, &dataOffset);
        offset += ENTRY_SIZE;
    }
    *link_offset = offset;
    write_buffer_4(&buf[*link_offset],0); // Don't link the next IFD directory yet

    return dataOffset;
}

/**
 * Write the given App Marker to the buffer.  Data is written in big endian format.
 * @param buff      Buffer location to write the AppMarker data
 * @param marker    The AppMarker data
 * @return The offset after writing the AppMarker
 */
unsigned long write_app_marker(unsigned char* buff,exif::AppMarker *marker) {
    unsigned long offset = 0;
    offset += write_buffer_2(buff,marker->type);
    offset += write_buffer_2(&buff[offset],marker->length);
    unsigned long bufLen = (unsigned long)marker->length-2; // Remove bytes to store length
    memcpy(&buff[offset],marker->buffer,bufLen);
    LOGD("Wrote %x %x %x %x %x %x",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5]);
    offset += bufLen;
    return offset;
}

/**
 * Generate the EXIF header buffer starting from the "Exif" string. The data is written in big endian format.
 * @param buf           Buffer to place header (starts at EXIF_START (6))
 * @return The offset after the segment was written
 */
uint16_t exif::EXIFInfo::encodeEXIFsegment(unsigned char *buf) {

    //   2 bytes: 0xFFD8 (big-endian)
    // EXIF header
    //   2 bytes: 0xFFE1 (big-endian)
    //   2 bytes: section size
    //   6 bytes: "Exif\0\0" string
    //   2 bytes: TIFF header (either "II" or "MM" string)
    //   2 bytes: TIFF magic (short 0x2a00 in Motorola byte order)
    //   4 bytes: Offset to first IFD
    //  16 bytes: TOTAL

    unsigned long offset = 0;
    buf[offset++] = (unsigned char) 'E';
    buf[offset++] = (unsigned char) 'x';
    buf[offset++] = (unsigned char) 'i';
    buf[offset++] = (unsigned char) 'f';
    offset += write_buffer_2(&buf[offset], 0);
    buf[offset++] = (unsigned char) 'M';
    buf[offset++] = (unsigned char) 'M';
    offset += write_buffer_2(&buf[offset], 0x002A);
    unsigned long first_ifd_offset = offset;
    offset += write_buffer_4(&buf[offset], 0); // Leave space for first IFD offset

    // Sort entries in case new entries were added
    for (unsigned long i=0; i<IFDirectories.size(); i++) {
        std::sort(IFDirectories.at(i)->entries->begin(), IFDirectories.at(i)->entries->end(), tagComparator);
    }

    IFDirectory *MainIFD = getDirectory(EXIF_MAIN_DIRECTORY);
    if (MainIFD == NULL) return (uint16_t)offset;
    IFDirectory *SubIFD = getDirectory(EXIF_SUB_DIRECTORY);

    unsigned long end_ifd = offset;
    unsigned long link_offset;

    // Keep track of any offset entries we add for encoding purposes and then remove them.
    std::vector<exif::IFEntry> *tmpEntries = new std::vector<exif::IFEntry>;

    // Put Interop IFD First
    IFDirectory *InteropIFD = getDirectory(EXIF_INTEROP_DIRECTORY);
    if (InteropIFD ->entries->size() > 0){
        unsigned long ifd_offset = end_ifd;
        end_ifd = write_ifd_entries(InteropIFD->entries, buf, ifd_offset, &link_offset);

        // Add pointer to Interop IFD in Sub IFD directory
        IFEntry *ifd_entry = new exif::IFEntry(EXIF_TAG_INTEROP_OFFSET, EXIF_SUB_DIRECTORY,  (int)ifd_offset - EXIF_START);
        updateEntry(ifd_entry);
        tmpEntries->push_back(*ifd_entry);
    }

    // Put SubIFD next
    if (SubIFD->entries->size() > 0) {
        unsigned long sub_ifd_offset = end_ifd;
        end_ifd = write_ifd_entries(SubIFD->entries, buf, sub_ifd_offset, &link_offset);

        // Add pointer to SubIFD in main IFD entry
        IFEntry *sub_ifd_entry = new exif::IFEntry(EXIF_TAG_SUB_IFD_OFFSET, EXIF_MAIN_DIRECTORY, (int)sub_ifd_offset - EXIF_START);
        updateEntry(sub_ifd_entry);
        tmpEntries->push_back(*sub_ifd_entry);
    }

    // Put GPS IFD next
    IFDirectory *GPSIFD = getDirectory(EXIF_GPS_DIRECTORY);
    if (GPSIFD ->entries->size() > 0){
        LOGD("Encoding %d GPS entries",(int)GPSIFD->entries->size());
        unsigned long gps_ifd_offset = end_ifd;
        end_ifd = write_ifd_entries(GPSIFD->entries, buf, gps_ifd_offset, &link_offset);

        // Add pointer to GPSIFD in main IFD entry
        IFEntry *gps_ifd_entry = new exif::IFEntry(EXIF_TAG_GPS_IFD_OFFSET, EXIF_MAIN_DIRECTORY,  (int)gps_ifd_offset - EXIF_START);
        updateEntry(gps_ifd_entry);
        tmpEntries->push_back(*gps_ifd_entry);
    }

    // Put 10 IFD next
    IFDirectory *IFD = getDirectory(EXIF_10_DIRECTORY);
    if (IFD->entries->size() > 0) {
        unsigned long ten_ifd_offset = end_ifd;

        // Add 10 version to the 10 directory
        IFEntry *ifd_entry = new exif::IFEntry(EXIF_10_VERSION, EXIF_10_DIRECTORY, CURR_10_VERSION);
        updateEntry(ifd_entry);

        end_ifd = write_ifd_entries(IFD->entries, buf, end_ifd, &link_offset);

        // Add pointer to 10 IFD in main IFD entry
        ifd_entry = new exif::IFEntry(EXIF_TAG_10_IFD_OFFSET, EXIF_MAIN_DIRECTORY,  (int)ten_ifd_offset - EXIF_START);
        updateEntry(ifd_entry);
        tmpEntries->push_back(*ifd_entry);
    }

    // Place first ifd after the final ifd data
    write_buffer_4(&buf[first_ifd_offset], (u_int32_t) end_ifd - EXIF_START);
    unsigned long end_first_ifd = write_ifd_entries(MainIFD->entries, buf, end_ifd, &link_offset);

    // Now that we are done writing the encoded buffer, remove all of the temporary offset entries
    for (unsigned long i=0; i<tmpEntries->size(); i++) {
        removeEntry(tmpEntries->at(i).tag(),tmpEntries->at(i).directory());
    }
    tmpEntries->clear();

    return (uint16_t) (end_first_ifd + 4);
}


/**
 * Write the JPEG EXIF data in a buffer starting with the JPEG_SOI
 * @param buf   Newly created buffer containing the data
 * @param len   Output the length of the generated buffer
 */
void exif::EXIFInfo::encodeJPEGHeader(unsigned char **buf,
                                      unsigned long *len) {

    int curr_size = 0xFFFF; // Maximum header size
    unsigned char *tmp = (unsigned char *) malloc((size_t) curr_size);

    //   2 bytes: 0xFFD8 (big-endian)
    // EXIF header
    //   2 bytes: 0xFFE1 (big-endian)
    //   2 bytes: section size

    unsigned long offset = 0;
    offset += write_buffer_2(&tmp[offset], JPEG_SOI);

    // EXIF header (APP1 Marker)
    offset += write_buffer_2(&tmp[offset], EXIF_MARKER);
    unsigned long size_offset = offset;
    offset +=2; // skip size
    LOGD("Exif segment start %x",(int)offset);
    uint16_t exif_size = encodeEXIFsegment(&tmp[offset]);
    LOGD("Exif segment length to write %x",exif_size);
    write_buffer_2(&tmp[size_offset], exif_size);

    offset += exif_size-2; // Includes 2 bytes for size
    //Encode other segments
    for (unsigned long i=0; i<AppMarkers.size(); i++) {
        LOGD("Start marker at 0x%x",(int)offset);
        offset += write_app_marker(&tmp[offset],AppMarkers.at(i));
    }
    LOGD("Total header len %x",(int)offset);
    *len = offset;
    *buf = (unsigned char *)realloc(tmp, *len);
}

/**
 * Return a pointer to the IFEntry with given tag, otherwise NULL if entry not found
 * @param tag   Input tag to get
 * @param dir   Directory tag is in
 * @return pointer to the IFEntry found or NULL
 */
exif::IFEntry* exif::EXIFInfo::getTagData(uint16_t tag, uint8_t dir) {
    IFDirectory *directory = getDirectory(dir);
    if (directory == NULL) return NULL;

    std::vector<exif::IFEntry> *entries = directory->entries;
    for (unsigned long i = 0; i < entries->size(); i++) {
        IFEntry *entry = &entries->at(i);
        if (entry->tag() == tag) return entry;
    }

    return NULL;
}

/**
 * Remove the IFEntry with given tag and return the index which was removed.  If entry didn't exist return -1.
 * @param tag   Input entry tag to remove
 * @param dir   Directory tag is in
 * @return index removed or -1 if nothing to remove
 */
int exif::EXIFInfo::removeEntry(uint16_t tag, uint8_t dir) {
    IFDirectory *directory = getDirectory(dir);
    if (directory == NULL) return -1;
    std::vector<exif::IFEntry> *entries = directory->entries;
    for (unsigned long i = 0; i < entries->size(); i++) {
        IFEntry *entry = &entries->at(i);
        if (entry->tag() == tag) {
            entries->erase(entries->begin() + i);
            return (int)i;
        }
    }
    return -1;
}

/**
 * Update the directory for the given entry.  Removes the tag if it exists and then adds it back in.
 * @param entry     New entry data to update
 */
void exif::EXIFInfo::updateEntry(exif::IFEntry *entry) {
    removeEntry(entry->tag(),entry->directory());

    std::vector<exif::IFEntry> *entries = getDirectory(entry->directory())->entries;
    entries->push_back(*entry);
}

/**
 * Clear the EXIFInfo data
 */
void exif::EXIFInfo::clear() {
    for (unsigned long i=0; i<IFDirectories.size(); i++) {
        IFDirectories.at(i)->entries->clear();
    }
    IFDirectories.clear();
}
