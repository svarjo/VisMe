/***************************************************************************
 * commonImage.cpp
 *
 * Implementation of commonImage.h
 *
 * Sami Varjo 2013
 *
 **************************************************************************/

#include "commonImage.h"

#include <cstring>
#include <iostream>

#include <tiffio.h> //specs TIFF 6.0 : http://partners.adobe.com/public/developer/en/tiff/TIFF6.pdf
                    //man libtiff    : http://www.bigbiz.com/cgi-bin/manpage?3+libtiff

namespace commonImage{

  /*
   * Save an image using libtiff 
   */
int saveTIFF( const char *path, commonImage_t *image, compressionType_e cType, bool verbose )
 {
   int rval = 0;
   TIFF *out = TIFFOpen( path, "w" );

   int bitspersample;
   int samplesperpixel=1;

   if (out != NULL){

     //////////////////////////////////////////////////
     // Fill out the "header" info
     TIFFSetField( out, TIFFTAG_IMAGEWIDTH, image->width);
     TIFFSetField( out, TIFFTAG_IMAGELENGTH, image->height);

     TIFFSetField( out, TIFFTAG_COMPRESSION, cType);

     TIFFSetField( out, TIFFTAG_PLANARCONFIG, 1); //RGBRGBRGB... (or GGGGGG...)

     if (image->mode == Gray8bpp || image->mode == Gray10bpp || image->mode == Gray12bpp || 
	 image->mode == Gray14bpp || image->mode == Gray16bpp || image->mode == Gray32bpp ){

       TIFFSetField( out, TIFFTAG_PHOTOMETRIC, 1);
     }

     //Assume that bits are not packed. ie if over 8 bits but less than 16 bits per channel => 2bytes
     switch (image->mode){
     case Gray8bpp:       
     case RGB8bpp:
     case RGBA8bpp:
       bitspersample = 8;       
       break;
     case Gray10bpp:
     case Gray12bpp:
     case Gray14bpp:
     case Gray16bpp:
       bitspersample = 16;
       break;
     case Gray24bpp:
       bitspersample = 24;
       break;
     case Gray32bpp:
       bitspersample = 32;
       break;
     default:
       if (verbose)
	 std::cerr << "Unsupported image format encountered" << std::endl;
       rval = -2;
       break;
     }
     TIFFSetField( out, TIFFTAG_BITSPERSAMPLE, bitspersample);   

     if (image->mode == RGB8bpp)
       samplesperpixel=3;
     else if (image->mode == RGBA8bpp)
       samplesperpixel=4;

     TIFFSetField( out, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);

     //////////////////////////////////////////////////////
     //Do the actual data write (buffered to row at once)
     tsize_t linebytes = bitspersample*samplesperpixel*image->width ;
     int fract = linebytes%8;
     linebytes /= 8;
     if (fract > 0)
       linebytes++;

     TIFFSetField( out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize( out, linebytes ));
     
   
     unsigned char *lineBuffer;
     lineBuffer = (unsigned char*) _TIFFmalloc( linebytes );
     if(!lineBuffer){
       if (verbose){
	 std::cerr << "Error while allocating file write buffer" << std::endl;
       }
       rval = -3;
     }
     else{     
       char *ptIn = (char*)(image->data);
       for(uint32 row = 0; row < image->height; row++){
	 
	 memcpy(lineBuffer, ptIn, linebytes);
	 if (TIFFWriteScanline(out, lineBuffer, row, 0) < 0)
	   break;
	 
	 ptIn += linebytes;

       }
       _TIFFfree(lineBuffer);          
     }

   }else{
     if (verbose){
       std::cerr << "commonImage::saveTIFF Error opening file for writing: " << path << std::endl;       
       rval = -1;
     }
   }
   
   TIFFClose(out);   
   return rval;
 }

  /******************************************************************************
   * Read an image in TIFF file. If image is 10-16 bit gray scale then
   * the buffer in commonImage_t image is filled with 16 bit (short) 
   * data (so the data won't be packed over several bytes)
   */
int readTIFF (const char *path, commonImage_t *image, bool verbose )
{
  if (path == NULL || image == NULL)
    return -1;

  int rval = 0;
  TIFF *tif = TIFFOpen( path, "r" );

  if (tif) {
    tsize_t scanline;
    char *buf;
    uint32 row, col;
    uint16 spp, bps, photo;

    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &image->height );
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &image->width );

    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photo);

    if (verbose){
      std::cout << "Image height:" << image->height << " width:" 
		<< image->width << " bps:" << bps << " spp: " << spp<< std::endl;
    }

    //Note that no bit packing is supported. ie only full bytes are read
    if (bps == 8 && spp == 1)
      image->mode = Gray8bpp;
    else if (bps > 8 && bps < 17 && spp == 1)
      image->mode = Gray16bpp;
    else if (bps > 17 && bps < 33 && spp == 1) //OBS no support for 24bps separately
      image->mode = Gray32bpp;
    else if (bps == 8 && spp == 3)
      image->mode = RGB8bpp;
    else if (bps == 8 && spp == 4)
      image->mode = RGBA8bpp;
    else{
      if (verbose)
	std::cerr << "commonImage::readTIFF unsupported image type encountered" << std::endl;
    }    

    scanline = TIFFScanlineSize(tif);
    buf =(char*) _TIFFmalloc(scanline*sizeof(char) );
    image->data = (char*) _TIFFmalloc(scanline*sizeof(char)*image->height );

    if (!buf || !image->data){
      if (verbose)
	std::cerr << "commonImage::readTIFF could not allocate memory" << std::endl;
      rval = -2;
    }
    else{
      char *ptOut = (char*)(image->data);
      for (row = 0; row < image->height ; row++)
	{
	  TIFFReadScanline(tif, buf, row);
	  memcpy( ptOut, buf, scanline );
	  ptOut += scanline;
	}
      _TIFFfree(buf);
      TIFFClose(tif);
    }
  }
  else {
    if (verbose)
      std::cerr << "Error while opening file: " << path << std::endl;
    rval = -1;
  }
 
  return rval;

}


}//end namespace commonImage
