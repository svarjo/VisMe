#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "commonImage.h"


int main(int argc, char** argv)
{
  if (argc < 1){
    std::cout << "usage: > " << argv[0] << " <<image.tif>>" << std::endl;
    return 0;
  }

  commonImage::commonImage_t imgIn;
  int ok;
  bool verbose = true;
  ok = commonImage::readTIFF( argv[1], &imgIn, verbose );
  
  if (ok != 0){
    std::cerr << "Error while opening file:" << argv[1] << std::endl;

  }
  
  else {

    cv::namedWindow(argv[1], CV_WINDOW_AUTOSIZE);

  }

  commonImage::commonImage_t image8Bit;
  commonImage::normaliseTo8bit( &imgIn, &image8Bit );
  
  commonImage::saveTIFF( "debug.tif", &image8Bit, commonImage::COMPRESSION_ZIP, verbose );

  //  cv::Mat cvImage;
  if (image8Bit.mode ==  commonImage::RGB8bpp){
  }
  else if (image8Bit.mode ==  commonImage::RGBA8bpp){

  }else{
    //cvImage = cv::Mat(imgIn.height, imgIn.width, CV_8UC1, image8Bit.data);
    cv::Mat cvImage(imgIn.height, imgIn.width, CV_8UC1, image8Bit.data);
    cv::imshow(argv[1], cvImage);
    cv::waitKey();	

  }

  /*
    cv::Mat image8bpp(img.height, img.width, CV_8UC1);
    //Scale data 	
    
    unsigned int *in = (unsigned int*) (img.data);
    unsigned char *out = image8bpp.data;
    int nPix = img.height * img.width;
    int count = nPix;
    
    double minV = 4294967295;
    double maxV = 0;
    while (count-- > 0) {
 	if (*in > maxV)
	  maxV = *in;
	if (*in < minV)
	  minV = *in;
	in++;
    }

  
    
    in = (unsigned int*) (img.data);
    double range = 255/(maxV-minV);
    while (count-- > 0) {
	*out++ = (*in++-minV)*range;
    }

    cv::imshow(argv[1], image8bpp);
    cv::waitKey();	
  }
    */
  return ok;
}
