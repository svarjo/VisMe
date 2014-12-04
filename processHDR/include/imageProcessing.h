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
	* @param expTimes array of exposure times (float)
	* @param output the result image structure. If output.data is NULL a new buffer of Gray32bpp is allocated
	* @param maxIdx use only N first images (if -1) use all;
	* @return error code, zero if success negative on error (eg memory allocation)
	*/ 
   int sumGrayStackWExpTimes( std::vector<commonImage_t> &stack, float *expTimes, commonImage_t *output, int maxIdx=-1);

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
   * Normalise a gray scale image to double format (eg for displaying)
   *
   * @param input the image to be normalised to range 0-1
   * @param output a pointer to commonImage_t .data (re)allocated if no suitable buffer is detected
   * @return error code, zero if success negative on error 
   */
   int normaliseGrayToDouble( commonImage_t *input, commonImage_t *output );
  
   /**
   * Normalise a gray scale image to float format (eg for displaying)
   *
   * @param input the image to be normalised to range 0-1
   * @param output a pointer to commonImage_t .data (re)allocated if no suitable buffer is detected
   * @return error code, zero if success negative on error 
   */
   int normaliseGrayToFloat( commonImage_t *input, commonImage_t *output );
   
   /**
    * Release the memory allocatd by the images in vector stack
	* @param stack std::vector<commonImage_t> containing images
	*/
   void releaseStackData( std::vector<commonImage_t> &stack );
   
   /**
    * Find the image index that is is not yet highly overexposed in image stack (may be severly but...)
	* @param stack std::vector<commonImage_t> containing images
	* @param th the required threshold for cumulative distribution of histogram 
	*	        saturated at the last bin (cdf[254] < th && cdf[255] > th)
	* @return id of the image over exposed 
	*/
   int findLastOkExposureImage( std::vector<commonImage_t> &stack , double th=0.5);

   /**
    * Multiscale Retinex filtering inspired by :
	*
	*  ref:"A Multiscale Retinex for Bridging the Gap Between Color Images and 
    *      the Human Observation of Scenes" Daniel J. Jobson, Zia-ur Rahman, and Glenn A. Woodell
    *      IEEE TRANSACTIONS ON IMAGE PROCESSING, VOL. 6, NO. 7, JULY 1997
	*	
	*	filtering parameters are hardcoded as in the above ref. 
	*
	*  @param input  commonImage_t *image (double)
	*  @param output commonImage_t *image (double), (re)allocated if no suitable buffer is found at .data
	*  @return error code, zero if success negative on error 
	*/   
	int multiscaleRetinexFilter( commonImage_t *input, commonImage_t *output );
	
	/**
	 * Convolution using variable kernel size
	 * 
	 * @param input 		image of Float 
	 * @param imgF1 		output image (Float)
	 * @param H1    		pointer to kernel
	 * @param Hsize 		kernel size (must be odd) 
	 * @param zeroBorders   if true set borders outside valid convolution area to 0 
	 * @return error code, zero if success negative on error 
	 */
    int convolution2D_Float(  commonImage_t *input, commonImage_t *imgF1, float *H1, unsigned int Hsize, bool zeroBorders=false);
	int convolution2D_Double( commonImage_t *input, commonImage_t *imgD1, double *H1, unsigned int Hsize );
         
	/**
	 * Set border values
	 * 
	 * @param input 	input image
	 * @param border    size of border (assume same for all top,bottom,left,right)
	 * @param value 	value to be set
	 * @return error code, zero if success negative on error 
	 */
	int setBordersTo( commonImage_t *input, unsigned int border, float value);
   
   

	double totalSum( commonImage_t *input );
   
#endif // IMAGEPROCESSING_H