/**
 * @file commonImage.h
 *
 * @DESCRIPTION
 *
 * A simple image container/interface to describe data between 3rd party
 * software (eg data from cameraAPI -> own stuff). Several functions are
 * provided to work with commonImage types like saving using libtiff.
 * 
 * struct commonImage has four data members: 
 *   - enum mode
 *   - int width
 *   - int height
 *   - void* data
 *
 * API: readTIFF, saveTIFF
 *
 * namespace:  commonImage::
 *
 * @AUTHOR Sami Varjo 2013
 *
 **************************************************************************/

#ifndef COMMON_IMAGE_H
#define COMMON_IMAGE_H

namespace commonImage{

  typedef enum mode{ 
    Gray8bpp,   // Gray == single channel
    Gray10bpp,
    Gray12bpp,
    Gray14bpp,
    Gray16bpp,
    RGB8bpp,    // 3 channels 8 bpp (ie total 24 bpp)
    RGBA8bpp,   // 4 channels 8 bpp 
  } mode_e;

  /**
   * The image container structure 
   */
  typedef struct commonImage{
    mode_e mode;    ///Mode describing the color structure of the data bpp = bits per pixel
    int width;      ///the image width 
    int height;     ///the image height
    void *data;     ///the image data
  }commonImage_t;

  /**
   * The supported image compression modes (as in tiff specification)
   */
  typedef enum compressionType{ 
    COMPRESSION_NONE=1,  ///no compression 
    COMPRESSION_LZW=5,   ///Lempel-Ziv-Welch (lossless)
    COMPRESSION_JPG=7,   ///Joint Photographic Experts Group lossy file format. Note that do not work for 16 bit data.
    COMPRESSION_PACKBITS=32773, ///Lossless run-length encoding of data by Apple
    COMPRESSION_ZIP=32946,      ///Lossless compression using ZIP
    
  }compressionType_e;

  //////////////////////////////////////////////////////////////////////////////
  //API

  /**
   * Save an image using libtiff
   *
   * The data in image.data is saved using 8bit alignment without packing bits over 
   * several bytes. 8bit alignment is assumed for both the incoming data and 
   * the saved data (ie if data per pixel is 14bit long, 16 bits of is used).
   *
   * @param path the output filename
   * @param image pointer to the data to be saved
   * @param cType compression scheme to be used (defualt COMPRESSION_NONE)
   * @param verbose to give or not some extra output to std::cout or std::cerr
   */
  int saveTIFF(const char *path, commonImage_t *image, compressionType_e cType=COMPRESSION_NONE, bool verbose=false );

  /**
   * Load an image using libtiff
   *
   * Read an image in TIFF file. If image is 10-16 bit gray scale then
   * the buffer in commonImage_t image is filled with 16 bit (short) 
   * data (so the data won't be packed over several bytes)
   *
   * @param path the input image filename
   * @param image the image to be populated. The data in image.data is allocated here.
   * @param verbose to give or not extra output at std::cout or std::cerr
   */
  int readTIFF (const char *path, commonImage_t *image, bool verbose=false );

}

#endif
