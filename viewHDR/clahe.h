/**
 * file: clahe.h
 *
 * ANSI C code from the article
 * "Contrast Limited Adaptive Histogram Equalization"
 * by Karel Zuiderveld, karel@cv.ruu.nl
 * in "Graphics Gems IV", Academic Press, 1994
 */ 
#ifndef _CLAHE_H_
#define _CLAHE_H_

#define SHORT_12B_IMAGE

#ifdef BYTE_IMAGE
	typedef unsigned char kz_pixel_t;	 /* for 8 bit-per-pixel images */
	#define uiNR_OF_GREY (256)
#elseif SHORT_12B_IMAGE
	typedef unsigned short kz_pixel_t;	 /* for 12 bit-per-pixel images */
	# define uiNR_OF_GREY (4096)
#elseif SHORT_14B_IMAGE
	typedef unsigned short kz_pixel_t;	 /* for 14 bit-per-pixel images */
	# define uiNR_OF_GREY (16384)
#elseif INT_16B_IMAGE
	typedef unsigned int kz_pixel_t;	 /* for 16 bit-per-pixel images */
	# define uiNR_OF_GREY (65536)
#elseif INT_24B_IMAGE
	typedef unsigned int kz_pixel_t;	 /* for 24 bit-per-pixel images */
	# define uiNR_OF_GREY (16777216)
#elseif INT_32B_IMAGE
	typedef unsigned int kz_pixel_t;	 /* for 32 bit-per-pixel images */
	# define uiNR_OF_GREY (4294967296)
#endif

#ifdef __cplusplus
extern "C" {
#endif

int CLAHE(	kz_pixel_t* pImage, 							//image data
			unsigned int uiXRes, unsigned int uiYRes, 		//image size X,Y
			kz_pixel_t Min, kz_pixel_t Max, 				//value range (both in and out)
			unsigned int uiNrX, unsigned int uiNrY,			//number of regions in x,y (min 2, max uiMAX_REG_X)
			unsigned int uiNrBins, 							//Number of greybins for histogram ("dynamic range")
			float fCliplimit);							    //Normalized cliplimit, A clip limit smaller than 1 
															//results in standard (non-contrast limited) AHE


#ifdef __cplusplus
}
#endif
	  
#endif	//_CLAHE_H_