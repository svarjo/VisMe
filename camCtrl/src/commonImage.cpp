#include "commonImage.h"

#include <cstring>
#include <iostream>

#include <tiffio.h> //specs TIFF 6.0 : http://partners.adobe.com/public/developer/en/tiff/TIFF6.pdf
                    //man libtiff    : http://www.bigbiz.com/cgi-bin/manpage?3+libtiff

namespace commonImage{

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

     if (image->mode == Gray8bpp || image->mode == Gray10bpp || image->mode == Gray12bpp || 
	 image->mode == Gray14bpp || image->mode == Gray16bpp ){

       TIFFSetField( out, TIFFTAG_PLANARCONFIG, 1);
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

       if (lineBuffer)
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


int readTIFF (const char *path, commonImage_t *image, bool verbose )
{
  int rval = 0;
  TIFF *tif = TIFFOpen( path, "r" );

  uint32 w,h;

  if (tif != NULL ){
    
    uint16 spp, bps, photo;
    uint32 width, height, linesize,i;
    char *buf;
    
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photo);
    linesize = TIFFScanlineSize(tif);
    buf = (char*)(_TIFFmalloc(linesize * height)) ;

    for (i = 0; i < height; i++)
      TIFFReadScanline(tif, &buf[i * linesize], i, 0);

    TIFFClose(tif);
    
    image->data = buf;
    image->height=height;
    image->width=width;
    
    if (bps == 8 && spp == 1)
      image->mode = Gray8bpp;
    else if (bps > 7  && bps < 17 && spp == 1)
      image->mode = Gray16bpp;

    else if (bps == 8 && spp == 3)
      image->mode = RGB8bpp;
    else if (bps == 8 && spp == 4)
      image->mode = RGBA8bpp;
    else{
      if (verbose)
	std::cerr << "commonImage::readTIFF unsupported image type encountered" << std::endl;
    }
    
    /*
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, 1, &w); //image->width ); 
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, 1, &h); //image->height);

    int bps, spp;    
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps );
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp );

    if (bps == 8 && spp == 1)
      image->mode = Gray8bpp;
    else if (bps > 7  && bps < 17 && spp == 1)
      image->mode = Gray16bpp;

    else if (bps == 8 && spp == 3)
      image->mode = RGB8bpp;
    else if (bps == 8 && spp == 4)
      image->mode = RGBA8bpp;
    else{
      if (verbose)
	std::cerr << "commonImage::readTIFF unsupported image type encountered" << std::endl;
    }
    
    uint32 npixels = image->width * image->height;
    int bitsPerPixel = spp*bps;
    uint32 dataByteSize = (bitsPerPixel/8) *npixels;

    tsize_t linebytes = bitsPerPixel * image->width;
    int fract = linebytes % 8;
    linebytes /= 8;
    if (fract >0)
      linebytes++;
    
    image->data = (void*) _TIFFmalloc( dataByteSize*sizeof(char)  );
    if (!image->data){
      if (verbose) 
	std::cerr << "commonImage::readTIFF error allocating memory";
      rval = -4;
    }
    else{
      
      char* pTarg = (char*)(image->data);
      for (int row = 0; row < 10; row ++){
	//	TIFFReadScanline(tif, pTarg,row,0);
	pTarg += linebytes;
      }
      
    }
    
    TIFFClose( tif );
    */
   
  }

  else {
    if (verbose)
      std::cerr << "Error while opening file: " << path << std::endl;
    rval = -1;
  }
 
  return rval;

}


}//end namespace commonImage
