/***************************************************************************
 * commonImage.h
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
 * Sami Varjo 2013
 *
 **************************************************************************/

#ifndef COMMON_IMAGE_H
#define COMMON_IMAGE_H

namespace commonImage{

  //Mode describing the color structure of the data bpp = bits per pixel
  typedef enum mode{ 
    Gray8bpp,   // Gray == single channel
    Gray10bpp,
    Gray12bpp,
    Gray14bpp,
    Gray16bpp,
    RGB8bpp,    // 3 channels 8 bpp (ie total 24 bpp)
    RGBA8bpp,   // 4 channels 8 bpp 
  } mode_e;

  typedef struct commonImage{
    mode_e mode;
    int width;
    int height;
    void *data;
  }commonImage_t;

  typedef enum compressionType{ 
    COMPRESSION_NONE=1, 
    COMPRESSION_LZW=5,
    COMPRESSION_JPG=7, //OBS not for 16 bit data
    COMPRESSION_BACKBITS=32773,
    COMPRESSION_ZIP=32946,
    
  }compressionType_e;

  
  //API

  int saveTIFF(const char *path, commonImage_t *image, compressionType_e cType=COMPRESSION_NONE, bool verbose=false );
  int readTIFF (const char *path, commonImage_t *image, bool verbose=false );

}

#endif
