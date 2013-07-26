#ifndef COMMON_IMAGE_H
#define COMMON_IMAGE_H

namespace commonImage{

  typedef enum mode{ 
    Gray8bpp,  
    Gray10bpp,  
    Gray12bpp,  
    Gray14bpp,  
    Gray16bpp, 
    RGB8bpp,
    RGBA8bpp,
  } mode_e;

  typedef struct commonImage{
    mode_e mode;
    int width;
    int height;
    void *data;
  }commonImage_t;

  typedef enum compressionType{ 
    COMPRESSION_NONE=1, 
    COMPRESSION_BACKBITS=32773 
  }compressionType_e;

  int saveTIFF(const char *path, commonImage_t *image, compressionType_e cType=COMPRESSION_NONE, bool verbose=false );
  int readTIFF (const char *path, commonImage_t *image, bool verbose=false );

}

#endif
