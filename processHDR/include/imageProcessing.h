/**
 * @file imageProcessing.h
 *
 * @section DESCRIPTION
 *
 * Image processing tasks using common image type.
 *
 * Depend on: commonImage.h
 * API: sumGrayStack, sumGrayStackWExpTimes
 *
 * namespace:  commonImage::
 *
 *
 * @author Sami Varjo 2014
 *
 **************************************************************************/

#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "commonImage.h"

using namespace commonImage;


   /**
    *Create a sum image from stack of images 
	* @param stack  vector of standard images
	* @param output the result image structure. If output.data is NULL a new buffer of Gray32bpp is allocated
	* @return zero if success negative on error (eg memory allocation)
	*/ 
   int sumGrayStack( std::vector<commonImage_t> &stack, commonImage_t *output);
   
   /**
    *Create a sum image from stack of images with linear exposure time weighting
	* @param stack  vector of standard images
	* @param expTimes array of exposure times (double)
	* @param output the result image structure. If output.data is NULL a new buffer of Gray32bpp is allocated
	* @param maxIdx use only N first images (if -1) use all;
	* @return error code, zero if success negative on error (eg memory allocation)
	*/ 
   int sumGrayStackWExpTimes( std::vector<commonImage_t> &stack, double *expTimes, commonImage_t *output, int maxIdx=-1);

  /**
   * Convert and normalise a gray scale image to 8 bit format (eg for displaying)
   *
   * @param input the image to be normalised to range 0-255
   * @param output a pointer to fresh commonImage_t. output.data is expected to be NULL and a new buffer will be allocated (if not null it is freed).
   * @return error code, zero if success negative on error
   */
   int normaliseGrayTo8bit( commonImage_t *input, commonImage_t *output );
     
  /**
   * Normalise a gray scale image to 32 bit format (eg for displaying)
   *
   * @param input the image to be normalised to range 0-4294967295
   * @param output a pointer to fresh commonImage_t. output.data is expected to be NULL and a new buffer will be allocated
 (if not null it is freed).
   * @return error code, zero if success negative on error 
   */
   int normaliseGrayTo32bit( commonImage_t *input, commonImage_t *output );
   
   /**
    * Gradient Magnitude in image
	* @param input image to take gradient from
	* @param output a pointer to commonImage_t, memory is allocated/reallocated if not fitting buffer
	* @return error code, zero if success negative on error 
	*/
	int gradientMag( commonImage_t *input, commonImage_t *output );
   
   /**
    * Release the memory allocatd by the images in vector stack
	* @param stack std::vector<commonImage_t> containing images
	*/
   void releaseStackData( std::vector<commonImage_t> &stack );
   
   /**
    * Find the image index that is first clearly overexposed image in image stack
	* @param stack std::vector<commonImage_t> containing images
	* @return id of the image over exposed 
	*/
   int findFirstOverExposedImage( std::vector<commonImage_t> &stack , double th=0.5);

   
#endif // IMAGEPROCESSING_H