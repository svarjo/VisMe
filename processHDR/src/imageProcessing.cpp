/*****************************************************************
 * imageProcessing.cpp
 *
 * implement imageProcessing.h
 *
 * Sami Varjo 
 *****************************************************************/

#include <cstdlib>
#include <stdint.h>
#include <string.h>
#include <limits>
#include <cmath>
#include "imageProcessing.h"
 
 #include <iostream> //DEBUG only
 
/***************************************************
 * Create simple sum image from stack of gray images
 */
  int sumGrayStack( std::vector<commonImage_t> &stack, commonImage_t *output)
  {    
	std::vector<commonImage_t>::iterator image = stack.begin();  
	int width  = (*image).width;
	int height = (*image).height;
	int pixels = width*height;
	
	if ((*output).data == NULL || (*output).mode != Gray32bpp || (*output).width*(*output).height < pixels ){
		(*output).mode = Gray32bpp;
		(*output).width = width;
		(*output).height = height;
		if ((*output).data != NULL){ free((*output).data); }
		(*output).data = calloc(  pixels, sizeof(int) );
		if ((*output).data == NULL){
			return -1;
		}		
	}
	
	unsigned int *pOut;
	
	switch((*image).mode){
	
		case Gray8bpp:{
			unsigned char *pIn;			
			while ( image != stack.end() ){  
				pIn = (unsigned char*)(*image++).data;
				pOut = (unsigned int*)(*output).data;
				int count = pixels;
				while(count-- > 0){
					*pOut++ += *pIn++;
				}
			}
			break;
		}
		case Gray10bpp:
		case Gray12bpp:
		case Gray14bpp:
		case Gray16bpp:{
			unsigned short *pIn;			
			while ( image != stack.end() ){  
				pIn = (unsigned short*)(*image++).data;
				pOut = (unsigned int*)(*output).data;
				int count = pixels;
				while(count-- > 0){
					*pOut++ += *pIn++;
				}
			}
			break;
		}
		
			break;
		case Gray24bpp:
		case Gray32bpp:
		{
			unsigned int *pIn;			
			while ( image != stack.end() ){  
				pIn = (unsigned int*)(*image++).data;
				pOut = (unsigned int*)(*output).data;
				int count = pixels;
				while(count-- > 0){
					*pOut++ += *pIn++;
				}
			}
			break;
		}			
		case Double1D:
		{
			double *pIn;			
			while ( image != stack.end() ){  
				pIn = (double*)(*image++).data;
				pOut = (unsigned int*)(*output).data;
				int count = pixels;
				while(count-- > 0){
					*pOut++ += *pIn++;
				}
			}
			break;
		}
		case RGB8bpp:
		case RGBA8bpp:		
			return -2;
    } 
	
	return 0;
  }
  

  /*****************************************************************************
   * Create a sum image from stack of images with linear exposure time weighting
   */
  int sumGrayStackWExpTimes( std::vector<commonImage_t> &stack, double *expTimes, commonImage_t *output, int maxId)
  {    
	std::vector<commonImage_t>::iterator image = stack.begin();  
	int width  = (*image).width;
	int height = (*image).height;
	int pixels = width*height;
		
	if (maxId<0) { maxId = stack.size(); }
	
	if ((*output).data == NULL || (*output).mode != Double1D || (*output).width*(*output).height < pixels ){
		(*output).mode = Double1D;
		(*output).width = width;
		(*output).height = height;		
		if ((*output).data != NULL){ free((*output).data); }
		(*output).data = calloc( pixels, sizeof(double));
		if ((*output).data == NULL){
			return -1;
		}		
	}	
	
	double *pOut;
	int id = 0;
	double weight;
	switch((*image).mode){
	
		case Gray8bpp:{
			unsigned char *pIn;			
			while ( image != stack.end() && id <= maxId ){  
				pIn = (unsigned char*)(*image++).data;
				pOut = (double*)(*output).data;
				weight = expTimes[id++];
				int count = pixels;
				while(count-- > 0){
					*pOut++ += (*pIn++) * weight;
				}
			}
			break;
		}
		case Gray10bpp:
		case Gray12bpp:
		case Gray14bpp:
		case Gray16bpp:{
			unsigned short *pIn;			
			while ( image != stack.end() && id <= maxId ){  
				pIn = (unsigned short*)(*image++).data;
				pOut = (double*)(*output).data;
				weight = expTimes[id++];
				int count = pixels;
				while(count-- > 0){
					*pOut++ += (*pIn++) * weight;
				}
			}
			break;
		}
		
			break;
		case Gray24bpp:
		case Gray32bpp:
		{
			unsigned int *pIn;			
			while ( image != stack.end() && id <= maxId ){  
				pIn = (unsigned int*)(*image++).data;
				pOut = (double*)(*output).data;
				weight = expTimes[id++];
				int count = pixels;
				while(count-- > 0){
					*pOut++ += (*pIn++) * weight;
				}
			}
			break;
		}
		
			break;
		case RGB8bpp:
		case RGBA8bpp:		
			return -2;
			break;
			
		case Double1D:
		{
			double *pIn;			
			while ( image != stack.end() && id <= maxId ){  
				pIn = (double*)(*image++).data;
				pOut = (double*)(*output).data;
				weight = expTimes[id++];
				int count = pixels;
				while(count-- > 0){
					*pOut++ += (*pIn++) * weight;
				}
			}
			break;
		}
    } 
	
	return 0;
  }
  
  
/************************************************************
 * Scale image data to range 0-255 
 */
 int normaliseGrayTo8bit( commonImage_t *input, commonImage_t *output )
 {
	int width  = (*input).width;
	int height = (*input).height;
	int pixels = width*height;
	
	double maxVal = std::numeric_limits<double>::min();
	double minVal = std::numeric_limits<double>::max();
	double range;
		
	if ((*output).data == NULL || (*output).mode != Gray8bpp || (*output).width*(*output).height < pixels ){
		(*output).mode = Gray8bpp;
		(*output).width = width;
		(*output).height = height;
		
		(*output).data = realloc((*output).data, pixels*sizeof(char) );
		if ((*output).data == NULL){
			return -1;
		}
		//memset( (void *) (*output).data, '\0', pixels*sizeof(char) ); //But the out is not read!?		
	}
	
	unsigned char *pOut = (unsigned char*)(*output).data;	
	
	switch((*input).mode){
	
		case Gray8bpp:{
			unsigned char *pIn;			
			pIn = (unsigned char*)(*input).data;
			
			int count = pixels;
			while(count-- > 0){
				unsigned char val = *pIn++;
				if (val < minVal)		{ minVal = val; }
				else if(val > maxVal) 	{ maxVal = val; }
			}						
			range = 255/(maxVal-minVal);			
			pIn = (unsigned char*)(*input).data;			
			count = pixels;
			while(count-- > 0){
				*pOut++ = (unsigned char)(((double)(*pIn++)-minVal)*range);
			}
			break;
		}
		
		case Gray10bpp:
		case Gray12bpp:
		case Gray14bpp:
		case Gray16bpp:{
			unsigned short *pIn;			
			pIn = (unsigned short*)(*input).data;
			
			int count = pixels;
			while(count-- > 0){
				unsigned short val = *pIn++;
				if (val < minVal)		{ minVal = val; }
				else if(val > maxVal) 	{ maxVal = val; }
			}						
			range = 255/(maxVal-minVal);
			
			pIn = (unsigned short*)(*input).data;
						
			count = pixels;
			while(count-- > 0){
				*pOut++ = (unsigned char)(((double)(*pIn++)-minVal)*range);
			}
						
			break;
		}
		case Gray24bpp:
		case Gray32bpp:{
			unsigned int *pIn;			
			pIn = (unsigned int*)(*input).data;
			
			int count = pixels;
			while(count-- > 0){
				int val = *pIn++;
				if (val < minVal)		{ minVal = val; }
				else if(val > maxVal) 	{ maxVal = val; }
			}						
			range = 255/(maxVal-minVal);
			
			pIn = (unsigned int*)(*input).data;
			pOut = (unsigned char*)(*output).data;				
			count = pixels;
			while(count-- > 0){
				*pOut++ = (unsigned char)(((double)(*pIn++)-minVal)*range);
			}						
			break;
		}

		case RGB8bpp:
		case RGBA8bpp:		
			return -2;
			break;
			
		case Double1D:{
			double *pIn = (double*)(*input).data;
			
			int count = pixels;
			while(count-- > 0){
				int val = *pIn++;
				if (val < minVal)		{ minVal = val; }
				else if(val > maxVal) 	{ maxVal = val; }
			}						
			range = 255/(maxVal-minVal);
			
			pIn = (double*)(*input).data;
			pOut = (unsigned char*)(*output).data;				
			count = pixels;
			while(count-- > 0){
				*pOut++ = (unsigned char) (((*pIn++)-minVal)*range);
			}
						
			break;
		}
    } 
	
	
	return 0;
 }
 
/************************************************************
 * Scale data to range 32 bits uint
 */
 int normaliseGrayTo32bit( commonImage_t *input, commonImage_t *output )
 {
	int width  = (*input).width;
	int height = (*input).height;
	int pixels = width*height;
	
std::cout << "W:" << width << " H:" << height << " pix:" << pixels << std::endl;	
	
	double maxVal = std::numeric_limits<double>::min();
	double minVal = std::numeric_limits<double>::max();
	double range;
	
	
	if ((*output).data == NULL || (*output).mode != Gray32bpp || (*output).width*(*output).height < pixels ){
		(*output).mode = Gray32bpp;
		(*output).width = width;
		(*output).height = height;
		(*output).data = realloc((*output).data, pixels*sizeof(unsigned int) );
		if ((*output).data == NULL){
			return -1;
		}
		memset( (void *) (*output).data, '\0', sizeof( (*output).data ) ); //But the out is not read!?
	}
		
	unsigned int *pOut = (unsigned int*)(*output).data;	
	
	switch((*input).mode){
	
		case Gray8bpp:{
			unsigned char *pIn = (unsigned char*)(*input).data;
			
			int count = pixels;
			while(count-- > 0){
				unsigned char val = *pIn++;
				if (val < minVal)		{ minVal = val; }
				else if(val > maxVal) 	{ maxVal = val; }
			}						
			range = std::numeric_limits<unsigned int>::max()/(maxVal-minVal);
			
			pIn = (unsigned char*)(*input).data;
			
			count = pixels;
			while(count-- > 0){
				*pOut++ = (unsigned int)(floor((double)(*pIn++)-minVal)*range); //floor to escape possible overflow?
			}
						
			break;
		}
		
		case Gray10bpp:
		case Gray12bpp:
		case Gray14bpp:
		case Gray16bpp:{
			unsigned short *pIn = (unsigned short*)(*input).data;
			
			int count = pixels;
			while(count-- > 0){
				unsigned short val = *pIn++;
				if (val < minVal)		{ minVal = val; }
				else if(val > maxVal) 	{ maxVal = val; }
			}						
			range = std::numeric_limits<unsigned int>::max()/(maxVal-minVal);
			
			pIn = (unsigned short*)(*input).data;
						
			count = pixels;
			while(count-- > 0){
				*pOut++ = (unsigned int)(floor((double)(*pIn++)-minVal)*range);
			}
						
			break;
		}
		case Gray24bpp:
		case Gray32bpp:{
			unsigned int *pIn = (unsigned int*)(*input).data;
			
			int count = pixels;
			while(count-- > 0){
				int val = *pIn++;
				if (val < minVal)		{ minVal = val; }
				else if(val > maxVal) 	{ maxVal = val; }
			}						
			range = std::numeric_limits<unsigned int>::max()/(maxVal-minVal);
			
			pIn = (unsigned int*)(*input).data;
			pOut = (unsigned int*)(*output).data;				
			count = pixels;
			while(count-- > 0){
				*pOut++ = (unsigned int)(floor((double)(*pIn++)-minVal)*range);
			}						
			break;
		}

		case RGB8bpp:
		case RGBA8bpp:		
			return -2;
			break;
			
		case Double1D:{
			double *pIn = (double*)(*input).data;
			
			int count = pixels;
			while(count-- > 0){
				int val = *pIn++;
				if (val < minVal)		{ minVal = val; }
				else if(val > maxVal) 	{ maxVal = val; }
			}
			
			range = std::numeric_limits<unsigned int>::max()/(maxVal-minVal);
			
			pIn = (double*)(*input).data;
			pOut = (unsigned int*)(*output).data;				
			count = pixels;
			while(count-- > 0){
				*pOut++ = (unsigned int) (floor((*pIn++)-minVal)*range);
			}
						
			break;
		}
    } 
	return 0;
 }
  
 /**************************************************************************
  * Memory clean up
  */
void releaseStackData( std::vector<commonImage_t> &stack )
{
	 std::vector<commonImage_t>::iterator image = stack.begin();  
	while ( image != stack.end() ){
		if ( (*image).data != NULL ){
			free( (*image).data );
		}
		image++;
	}
	stack.clear();
	
}
  
/********************************************************************************
 * find index of first over exposed image (assume increasing exp times in stack)
 */
int findFirstOverExposedImage( std::vector<commonImage_t> &stack , double th)
{
	commonImage_t imBuf;	
		
	int   hist[256];
	double cdf[256];
	int idx = -1; //No over exposed found neg!
	
	std::vector<commonImage_t>::iterator image = stack.end();  
	
	
	while ( image >= stack.begin() ){
	
		for(int i=0;i<256;i++){ 
			cdf[i]=0;	
			hist[i]=0;	
		}		
		normaliseGrayTo8bit( &(*image), &imBuf );
		
		int count = imBuf.width*imBuf.height;
		unsigned char *ptD = (unsigned char *)imBuf.data;		
		while( count-- > 0){
			hist[ (*ptD++) ]++;
		}
				
		cdf[0] = hist[0];		
		for(int i=1;i<256;i++){ 
			cdf[i] = cdf[i-1] + hist[i];				
		}
		
		for(int i=0;i<256;i++){ 
			cdf[i]/=cdf[255]; //Would be enough id 0 254 255
			if ( isnan(cdf[i]) )
				cdf[i] = 0;
			if ( isinf(cdf[i]) )
				cdf[i] = 1;
		}		
	
		//std::cout << "DBG 0:" << cdf[0] << " 254:" << cdf[254] <<" 255:" <<  cdf[255] << std::endl;
		
		if ( cdf[0] > 0.99 || cdf[254] < 0.01 ){ //About black/white image
			image--;
			//std::cout << "DBG ID:" << image - stack.begin()  << " skipped" << std::endl;
			continue;
		}
		
		//A bit shorter than Matlab version (maybe good - maybe not TODO test)
		if ( cdf[254] < th && cdf[255] > th ){
			idx = image - stack.begin() +1; 
			//std::cout << "DBG MAX idx:"  << idx << std::endl;	
			if (cdf[254] > 0.5)
				break;
		}		
		image--;
	}

	if( imBuf.data != NULL ) { free(imBuf.data); }	
	
	
	return idx;
	
}

