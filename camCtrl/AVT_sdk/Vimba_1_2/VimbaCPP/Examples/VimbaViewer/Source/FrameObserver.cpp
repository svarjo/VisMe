/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        FrameObserver.cpp

  Description: Frame callback.
               

-------------------------------------------------------------------------------

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
  NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/


#include "FrameObserver.h"
#include "VmbImageTransformHelper.hpp"
#include <QFile>
#include <QMetaType>
#include <QTextStream>
#include <QDebug>
#include <math.h>

template <class T>
void DeleteArray(T *pArray)
{
    delete [] pArray;
}

FrameObserver::FrameObserver ( CameraPtr pCam ) : IFrameObserver( pCam ), m_nFramesCounter            ( 0 ), m_nNumberOfFrames    ( 0 ),
                                                                          m_nImageProcAverageDuration ( 0 ), m_dFPS_NoImgProc     ( 0 ),
                                                                          m_nWidth                    ( 0 ), m_nHeight            ( 0 ), 
                                                                          m_nSize                     ( 0 ), m_nImageProcDuration ( 0 ),
                                                                          m_DisplayInterval           ( 0 ), m_dFPS               ( 0 ),
                                                                          m_nCountPosition            ( 0 ), m_nRawImagesToSave   ( 0 ),
                                                                          m_nRawImagesCounter         ( 0 ), m_bIsReset           ( false ), 
                                                                          m_nFrames ( MAX_FRAMES_TO_COUNT ), m_nMaximumFramesToCount ( MAX_FRAMES_TO_COUNT ),
                                                                          m_bIsTriggeredByStart ( true ), m_bIsHistogramEnabled ( false ),
                                                                          m_bColorInterpolation ( true ), m_nNumberOfFPSToAvgCalc ( 0 ),
                                                                          m_dAvgFPS ( 0 )
                                                                          
{ 
    m_pCam = pCam;
    m_pHelperThread = QSharedPointer<HelperThread>(new HelperThread());
    m_pHistogramThread	= QSharedPointer<HistogramThread>(new HistogramThread());
    
    connect ( m_pHelperThread.data(), SIGNAL ( frameReadyFromThread (QImage, const QString &, const QString &, const QString &) ), 
              this, SLOT ( getFrameFromThread (QImage, const QString &, const QString &, const QString &) ) );

    // We need to register QVector <quint32> because it is not known to Qt's meta-object system
    qRegisterMetaType< QVector<QVector <quint32> > >("QVector<QVector <quint32> >");
    qRegisterMetaType< QVector <QStringList> >("QVector <QStringList>");

    connect ( m_pHistogramThread.data(), SIGNAL ( histogramDataFromThread ( const QVector<QVector <quint32> > &, const QString &, const double &, const double &, const QVector<QStringList> & )), 
              this, SLOT ( getHistogramDataFromThread ( const QVector<QVector <quint32> > &, const QString &, const double &, const double &, const QVector<QStringList>& )) );
}

FrameObserver::~FrameObserver ( void )
{
    if( NULL != m_pHelperThread || m_pHelperThread->isRunning() )
    {
        m_pHelperThread->quit();
    }
}

void FrameObserver::FrameReceived ( const AVT::VmbAPI::FramePtr frame  )
{	
    VmbFrameStatusType statusType = VmbFrameStatusInvalid;
    VmbPixelFormatType pixelFormat;
    
    if( VmbErrorSuccess == frame->GetPixelFormat(pixelFormat) && 
        VmbErrorSuccess == frame->GetReceiveStatus(statusType) )
    {
        /* ignore any incompletely frame */
        if( VmbFrameStatusComplete != statusType )
        {
            m_pCam->QueueFrame(frame);
            return; 
        }

        m_nFramesCounter++;
        emit setFrameCounter ( m_nFramesCounter );
        countFPS();		
        
        double dIntervalBetween2Frames_NoImgProc = 0;

        /* get to know how long does a frame take to the next (in ms) */
        if( 0 != m_dFPS)
         dIntervalBetween2Frames_NoImgProc = (1/m_dFPS_NoImgProc) * 1000; 
        
        /* sampling frames for continuous mode only */
        if( 0 != m_DisplayInterval )
        {
            /* make sure that thread is not running on transition after 30 frames */
            if( m_pHelperThread->isRunning() && (0 != m_dFPS))
            {
                m_pCam->QueueFrame(frame);
                return;
            }

            /* display all frames for frames received equal to or greater than  20 frames per sec */
            if( 20 <= m_dFPS)
            {
                /* image processing takes longer than the FPS with no converting */
                if( m_nImageProcAverageDuration > dIntervalBetween2Frames_NoImgProc) 
                {
                    double dRoundUpValue = ceil (m_nImageProcAverageDuration/dIntervalBetween2Frames_NoImgProc);
                    /* modulo */
                    int nRoundUpValue = 0; 

                    if( (20 <= m_dFPS) && (35 > m_dFPS) )
                    {
                        nRoundUpValue = (int) dRoundUpValue ;
                    }

                    else if( (35 <= m_dFPS) && (60 > m_dFPS) )
                    {
                        nRoundUpValue = (int) dRoundUpValue + 1;
                    }

                    else if( (60 <= m_dFPS) && (100 > m_dFPS) )
                    {
                        nRoundUpValue = (int) dRoundUpValue + 2;
                    }

                    else if( 100 <= m_dFPS && (250 > m_dFPS) )
                    {
                        nRoundUpValue = (int) dRoundUpValue + 5;
                    }
                    
                    else if( 250 <= m_dFPS && (700 > m_dFPS) )
                    {
                        nRoundUpValue = (int) dRoundUpValue + 10;
                    }

                    else if( 700 <= m_dFPS  )
                    {
                        nRoundUpValue = (int) dRoundUpValue + 20;
                    }

                    if(0 == (m_nFramesCounter%nRoundUpValue) )
                    {
                        imageProcessing( frame );
                        m_nMaximumFramesToCount = nRoundUpValue * 30;
                    }

                    if(m_bIsHistogramEnabled)
                        startHistogramThread();
                }
                /* image processing takes shorter than the FPS with no converting */
                else
                {
                    int nModulo = 0;
                
                    if( (20 <= m_dFPS) && (35 > m_dFPS) )
                    {
                        nModulo = 2;
                        m_nMaximumFramesToCount = 30;
                    }

                    else if( (35 <= m_dFPS) && (100 > m_dFPS) )
                    {
                        nModulo = 3;
                        m_nMaximumFramesToCount = 60;
                    }
               
                    else if( (100 <= m_dFPS) && (250 > m_dFPS) )
                    {
                        nModulo = 6;
                        m_nMaximumFramesToCount = 180;
                    }

                    else if( (250 <= m_dFPS) && (700 > m_dFPS) )
                    {
                        nModulo = 10;
                        m_nMaximumFramesToCount = 360;
                    }

                    else if( 700 <= m_dFPS )
                    {
                        nModulo = 30;
                        m_nMaximumFramesToCount = 1000;
                    }
                
                    if(0 == (m_nFramesCounter%nModulo) )
                    {
                        imageProcessing( frame );
                    }

                    if(m_bIsHistogramEnabled)
                        startHistogramThread();
                }
            }
            /* as long as no FPS available display all frames using helper thread or
            *  if FPS less than 20, then display all frames
            */
            else
            {
                /* start processing when the first FPS available (after 30 Frames since start) and FPS < 20 */
                if( 0 != m_dFPS) 
                {
                    imageProcessing( frame );
                    m_nMaximumFramesToCount = 30;
                }
                /* if not, start a thread for processing as long as FPS not available. Need this to display frame immediately after start.
                *  Need the thread to count the first FPS as fast as possible (image processing is going on the thread)
                */
                else 
                {
                    if( !m_pHelperThread->isRunning() )
                    {
                        imageProcessing ( frame );
                        m_pHelperThread->setThreadFrame ( m_pFrame, m_nWidth, m_nHeight, m_Format, m_bColorInterpolation );
                        m_pHelperThread->start();
                    }
                }

                if(m_bIsHistogramEnabled)
                    startHistogramThread();
            }
            
        }
        /* display all frames without sampling in Single and Multiple mode */
        else 
        {
            imageProcessing( frame );
            if(m_bIsHistogramEnabled)
                startHistogramThread();
        }
    }
    else
    {
        qDebug() << "DROPED";
    }
    m_pCam->QueueFrame(frame);
}

void FrameObserver:: enableHistogram ( const bool &bIsHistogramEnabled)
{
    m_bIsHistogramEnabled = bIsHistogramEnabled;
}

void FrameObserver:: setColorInterpolation ( const bool &bState)
{
    m_bColorInterpolation = bState;
}

bool FrameObserver:: getColorInterpolation ( void )
{
    return m_bColorInterpolation;
}

void FrameObserver::startHistogramThread ( void )
{
    if( !m_pHistogramThread->isRunning() )
    {
        m_pHistogramThread->setThreadFrame ( m_pFrame, m_nWidth, m_nHeight, m_nSize, m_Format );
        m_pHistogramThread->start();
    }
}

void FrameObserver::setDisplayInterval ( const unsigned int &nInterval )
{
    m_DisplayInterval = nInterval;
}

void FrameObserver::resetFrameCounter ( const bool &bIsRestart )
{
    if( bIsRestart )
    {
        m_nFramesCounter        = 0;
        m_bIsTriggeredByStart   = true;
    }
    else
    {
        m_pHelperThread->keepCurrentFormatName( m_sFormat );
        m_bIsTriggeredByStart   = false;
    }

    m_pHelperThread->setStartTriggerStatus(bIsRestart);

    m_nFrames                   = MAX_FRAMES_TO_COUNT;
    m_nMaximumFramesToCount     = MAX_FRAMES_TO_COUNT;
    m_dFPS                      = 0;
    m_dFPS_NoImgProc            = 0;
    m_nImageProcAverageDuration = 0;
    m_nImageProcDuration	    = 0;
    m_nNumberOfFrames           = 0;
    m_nCountPosition			= 0;
    m_dAvgFPS                   = 0;
    m_nNumberOfFPSToAvgCalc     = 0;

    m_bIsReset = true;
    emit setCurrentFPS ( "-" );
}

void FrameObserver::imageProcessing ( const AVT::VmbAPI::FramePtr &frame )
{
    /* if fps not available yet, get the frame params(m_pFrame, m_nWidth, m_nHeight, m_Format) for helper thread 
    *  While investigating FPS, show all frames received using helper thread
    */
    if((0 == m_dFPS) && (0 != m_DisplayInterval) ) 
    {
        /* dont care if return value ok or not */
        setFrame (frame); 
        return;
    }

    m_FrameIntervalTimer.start();
    if( setFrame(frame) )
    {
        processFrame();
    }
    else
    {
        /* just ignore it */
        m_FrameIntervalTimer.elapsed();
        return;
    }

    m_nImageProcDuration = m_nImageProcDuration + m_FrameIntervalTimer.elapsed();
    m_nNumberOfFrames++;
    
    /* get the average of image processing duration every 60 frames */
    if ( (MAX_FRAMES_TO_COUNT * 2) == m_nNumberOfFrames )
    {
        m_nNumberOfFrames = 0;
        m_nImageProcAverageDuration = m_nImageProcDuration/(MAX_FRAMES_TO_COUNT *2);
        m_nImageProcDuration = 0;
    }
}

void FrameObserver::countFPS ( void )
{
    double dFramesPerSecond = 0;

    if( m_nMaximumFramesToCount == m_nFrames )
    {
        m_FrameCounterTimer.start();
    }

    if( 0 == m_nFrames-- )
    {
        dFramesPerSecond = (1000.0 * m_nMaximumFramesToCount)/m_FrameCounterTimer.elapsed(); 
        
        if( 0 == m_dFPS )
        {	/*  save this once only @ the beginning of streaming */
            m_dFPS_NoImgProc  = m_dFPS = dFramesPerSecond; 
        }
        else
        {   /* refresh FPS */
            m_dFPS = dFramesPerSecond; 
        }
        
        m_nFrames = m_nMaximumFramesToCount;
        
        /* after reset, don't start immediately
        *  start emit fps after 4 rounds around 120 Frames to make sure transmitting correct fps
        */
        if( m_bIsReset )
        {
            m_nCountPosition++;

            if( 0 == ( m_nCountPosition % 2 ) )
                emit setCurrentFPS ( "/" );
            else
                emit setCurrentFPS ( "\\" );

            unsigned int nFactor = 4;
            
            if( 1000 == m_nMaximumFramesToCount)
                nFactor = 15;
            else if( 360 == m_nMaximumFramesToCount)
                nFactor = 8;

            if( m_nCountPosition > nFactor )
            {
                nFactor = 0;
                m_nCountPosition = 0;
            }

            if( nFactor == m_nCountPosition )
            {
                m_bIsReset = false;
                emit setCurrentFPS ( QString::number(dFramesPerSecond) );
                return;
            }
        }

        if(!m_bIsReset)
        {
            if(10 == m_nNumberOfFPSToAvgCalc)
            {
                dFramesPerSecond = m_dAvgFPS /10;
                emit setCurrentFPS ( QString::number(dFramesPerSecond) );
                m_nNumberOfFPSToAvgCalc = 0;
                m_dAvgFPS = 0;
            }

            m_dAvgFPS = m_dAvgFPS + dFramesPerSecond;
            m_nNumberOfFPSToAvgCalc++;
        }
    }
}

bool FrameObserver::setFrame (const AVT::VmbAPI::FramePtr &frame )
{
    VmbUchar_t          *imgData = NULL;
    VmbPixelFormatType   pixelFormat;
    VmbUint32_t			 nWidth = 0, nHeight = 0;

    if( VmbErrorSuccess == frame->GetWidth(nWidth)	&&
        VmbErrorSuccess == frame->GetHeight(nHeight)	&&
        VmbErrorSuccess == frame->GetBuffer(imgData)	&&
        VmbErrorSuccess == frame->GetPixelFormat(pixelFormat) )
    {
        m_Format    = pixelFormat;
        m_sHeight   = QString::number(nHeight);
        m_sWidth    = QString::number(nWidth);

        VmbUint32_t nSize;
        if( VmbErrorSuccess != frame->GetImageSize(nSize) )
            return false;
        
        m_nSize		= nSize;
        m_nWidth    = nWidth;
        m_nHeight   = nHeight;

        try
        {
            m_pFrame = QSharedPointer<unsigned char>(new unsigned char[nSize], DeleteArray<unsigned char>);
            memcpy(m_pFrame.data(), imgData, nSize);

            /* saving Raw Data */
            if( m_nRawImagesToSave > 0)
            {
                m_nRawImagesCounter++;
                QString s = m_sPathDestination;
                s.append("\\");
                s.append(m_sRawImagesName).append(QString::number(m_nRawImagesCounter)).append(".bin");
                m_nRawImagesToSave--;

                QFile rawFile(s);
                rawFile.open(QIODevice::WriteOnly);
                QDataStream out(&rawFile);
                out.writeRawData((const char*)m_pFrame.data(), m_nSize);
                rawFile.close();
            }
        }
        catch (...)
        {
            return false;
        }
        
        return true;
    }

    return false;
}

void FrameObserver::saveRawData ( const unsigned int &nNumberOfRawImagesToSave, const QString& sPath, const QString &sFileName )
{
    m_nRawImagesCounter = 0;
    m_nRawImagesToSave = nNumberOfRawImagesToSave;
    m_sPathDestination = sPath;
    m_sRawImagesName   = sFileName;
}

void FrameObserver::processFrame ( void )
{
    QImage convertedImage( m_nWidth, m_nHeight, QImage::Format_RGB32 );
   
    VmbError_t error;

    if( m_bColorInterpolation )
    {
        error = AVT::VmbImageTransform( convertedImage, m_pFrame.data(), m_nWidth, m_nHeight, m_Format);
    }
    else
    {
        if ((VmbPixelFormatBayerGR8 == m_Format) || (VmbPixelFormatBayerRG8 == m_Format) ||
            (VmbPixelFormatBayerGB8 == m_Format) || (VmbPixelFormatBayerBG8 == m_Format) ||(VmbPixelFormatMono8 == m_Format) )
        {
            error = AVT::VmbImageTransform( convertedImage, m_pFrame.data(),m_nWidth,m_nHeight,VmbPixelFormatMono8);
        }
    
        else if ((VmbPixelFormatBayerGR12 == m_Format) || (VmbPixelFormatBayerRG12 == m_Format)||
                 (VmbPixelFormatBayerGB12 == m_Format) || (VmbPixelFormatBayerBG12 == m_Format)) 
        {
            error = AVT::VmbImageTransform( convertedImage, m_pFrame.data(),m_nWidth,m_nHeight,VmbPixelFormatMono12);
        }

        else if ((VmbPixelFormatBayerGR16 == m_Format) || (VmbPixelFormatBayerRG16 == m_Format)||
                 (VmbPixelFormatBayerGB16 == m_Format) || (VmbPixelFormatBayerBG16 == m_Format))
        {
            error = AVT::VmbImageTransform( convertedImage, m_pFrame.data(),m_nWidth,m_nHeight,VmbPixelFormatMono16);
        }
        else
        {
            error = AVT::VmbImageTransform( convertedImage, m_pFrame.data(),m_nWidth,m_nHeight,m_Format);
            m_bColorInterpolation = true;
        }
    }
    
    if( VmbErrorSuccess != error )
    {
        /* Show this information to viewer statusbar when Height and or Width not supported */
        if(convertedImage.isNull())
            m_sFormat = "Convert Error: Destination Buffer NULL";
        else
            m_sFormat = "Convert Error: " + QString::number(error,16);
            
        if( m_sFormat.contains("88000005") )
            m_sFormat.append(" (Height NOT supported!)");

        else if( m_sFormat.contains("88000004") )
            m_sFormat.append("(Width NOT supported!)");
        
        emit frameReadyFromObserver (convertedImage, m_sFormat, m_sHeight, m_sWidth);
        return;
    }
    
    switch(m_Format)
    {
            /* Monochrome, 8 bits */
        case VmbPixelFormatMono8: 
            m_sFormat = "Mono8";
            break;

            /* Monochrome, 10 bits in 16 bits */
        case VmbPixelFormatMono10:
            m_sFormat = "Mono10";
            break;

            /* Monochrome, 12 bits in 16 bits */
        case VmbPixelFormatMono12:
            m_sFormat = "Mono12";
            break;

            /* Monochrome, 2x12 bits in 24 bits */
        case VmbPixelFormatMono12Packed:
            m_sFormat = "Mono12Packed";
            break;

            /* Monochrome, 14 bits in 16 bits */
        case VmbPixelFormatMono14:
            m_sFormat = "Mono14";
            break;
        
            /* Monochrome, 16 bits */
        case VmbPixelFormatMono16:
            m_sFormat = "Mono16";
            break;

            /* Bayer-color, 8 bits */
        case VmbPixelFormatBayerGR8:
            m_sFormat = "BayerGR8";
            break;

            /* Bayer-color, 8 bits */
        case VmbPixelFormatBayerRG8:
            m_sFormat = "BayerRG8";
            break;

            /* Bayer-color, 8 bits */
        case VmbPixelFormatBayerGB8:
            m_sFormat = "BayerGB8";
            break;

            /* Bayer-color, 8 bits */
        case VmbPixelFormatBayerBG8:
            m_sFormat = "BayerBG8";
            break;

            /* Bayer-color, 10 bits in 16 bits */
        case VmbPixelFormatBayerGR10:
            m_sFormat = "BayerGR10";
            break;

            /* Bayer-color, 10 bits in 16 bits */
        case VmbPixelFormatBayerRG10:
            m_sFormat = "BayerRG10";
            break;

            /* Bayer-color, 10 bits in 16 bits */
        case VmbPixelFormatBayerGB10:
            m_sFormat = "BayerGB10";
            break;

            /* Bayer-color, 10 bits in 16 bits */
        case VmbPixelFormatBayerBG10:
            m_sFormat = "BayerBG10";
            break;

            /* Bayer-color, 12 bits in 16 bits */
        case VmbPixelFormatBayerGR12:
            m_sFormat = "BayerGR12";
            break;

            /* Bayer-color, 12 bits in 16 bits */
        case VmbPixelFormatBayerRG12:
            m_sFormat = "BayerRG12";
            break;

            /* Bayer-color, 12 bits in 16 bits */
        case VmbPixelFormatBayerGB12:
            m_sFormat = "BayerGB12";
            break;

            /* Bayer-color, 12 bits in 16 bits */
        case VmbPixelFormatBayerBG12:
            m_sFormat = "BayerBG12";
            break;

            /* RGB, 8 bits x 3 */
        case VmbPixelFormatRgb8:
            m_sFormat = "RGB8";
            break;

            /* BGR, 8 bits x 3 */
        case VmbPixelFormatBgr8:
            m_sFormat = "BGR8";
            break;

            /* ARGB, 8 bits x 4 */
        case VmbPixelFormatArgb8:
            m_sFormat = "ARGB8";
            break;

            /* BGRA, 8 bits x 4 */
        case VmbPixelFormatBgra8:
            m_sFormat = "BGRA8";
            break;

            /* RGB, 16 bits x 3 */
        case VmbPixelFormatRgb16:
            m_sFormat = "RGB16";
            break;

            /* YUV 411 with 8 bits */
        case VmbPixelFormatYuv411:
            m_sFormat = "YUV411";
            break;

            /* YUV  422 */
        case VmbPixelFormatYuv422:
            m_sFormat = "YUV422";
            break;

            /* YUV 444 */
        case VmbPixelFormatYuv444:
            m_sFormat = "YUV444";
            break;

            /* BAYERGR12PACKED Bayer-color, 2x12 bits in 24 bits, starting with GR line*/
        case VmbPixelFormatBayerGR12Packed:
            m_sFormat = "BayerGR12Packed";
            break;

            /* BAYERRG12PACKED Bayer-color, 2x12 bits in 24 bits, starting with RG line */
        case VmbPixelFormatBayerRG12Packed:
            m_sFormat = "BayerRG12Packed";
            break;

            /* BAYERGB12PACKED Bayer-color, 2x12 bits in 24 bits, starting with GB line*/
        case VmbPixelFormatBayerGB12Packed:
            m_sFormat = "BayerGB12Packed";
            break;

            /* BAYERBG12PACKED Bayer-color, 2x12 bits in 24 bits, starting with BG line*/
        case VmbPixelFormatBayerBG12Packed:
            m_sFormat = "BayerBG12Packed";
            break;

        /* Bayer-color, 16 bits, starting with GR line*/
        case VmbPixelFormatBayerGR16:
            m_sFormat = "BayerGR16";
            break;

        /* Bayer-color, 16 bits, starting with RG line */
        case VmbPixelFormatBayerRG16:
            m_sFormat = "BayerRG16";
            break;

        /* Bayer-color, 16 bits, starting with GB line */
        case VmbPixelFormatBayerGB16:
            m_sFormat = "BayerGB16";
            break;

        /* Bayer-color, 16 bits, starting with BG line */
        case VmbPixelFormatBayerBG16:
            m_sFormat = "BayerBG16";
            break;

        default:
            m_sFormat = " ";
            break;
    }
    
    emit frameReadyFromObserver (convertedImage, m_sFormat, m_sHeight, m_sWidth);
    
}

void FrameObserver::getFrameFromThread ( QImage image, const QString &sFormat, const QString &sHeight, const QString &sWidth )
{
    emit frameReadyFromObserver (image, sFormat, sHeight, sWidth);
}

void FrameObserver::getHistogramDataFromThread ( const QVector<QVector <quint32> > &histData, const QString &sHistogramTitle, 
                                                 const double &nMaxHeight_YAxis, const double &nMaxWidth_XAxis ,const QVector <QStringList> &statistics)
{
    emit histogramDataFromObserver ( histData, sHistogramTitle, nMaxHeight_YAxis, nMaxWidth_XAxis, statistics );
}

void HelperThread::run()
{
    QImage convertedImage( m_tFrameInfo.m_nThreadFrameWidth, m_tFrameInfo.m_nThreadFrameHeight, QImage::Format_RGB32 ); 
    
    if(convertedImage.isNull())
        return;

    if (NULL != m_tFrameInfo.m_pThreadFrame)
    {
        VmbError_t error;

        if(m_tFrameInfo.m_ColorInterpolation)
        {
            error = AVT::VmbImageTransform( convertedImage, m_tFrameInfo.m_pThreadFrame.data(), m_tFrameInfo.m_nThreadFrameWidth, 
                                            m_tFrameInfo.m_nThreadFrameHeight, m_tFrameInfo.m_ThreadFormat);
        }
        else
        {
            if ((VmbPixelFormatBayerGR8 == m_tFrameInfo.m_ThreadFormat) || (VmbPixelFormatBayerRG8 == m_tFrameInfo.m_ThreadFormat)||
                (VmbPixelFormatBayerGB8 == m_tFrameInfo.m_ThreadFormat) || (VmbPixelFormatBayerBG8 == m_tFrameInfo.m_ThreadFormat)||
                (VmbPixelFormatMono8 == m_tFrameInfo.m_ThreadFormat) )
            {
                error = AVT::VmbImageTransform( convertedImage, m_tFrameInfo.m_pThreadFrame.data(), m_tFrameInfo.m_nThreadFrameWidth, 
                                                m_tFrameInfo.m_nThreadFrameHeight, VmbPixelFormatMono8);
            }

            else if ((VmbPixelFormatBayerGR12 == m_tFrameInfo.m_ThreadFormat) || (VmbPixelFormatBayerRG12 == m_tFrameInfo.m_ThreadFormat)||
                     (VmbPixelFormatBayerGB12 == m_tFrameInfo.m_ThreadFormat) || (VmbPixelFormatBayerBG12 == m_tFrameInfo.m_ThreadFormat))
            {
                error = AVT::VmbImageTransform( convertedImage, m_tFrameInfo.m_pThreadFrame.data(), m_tFrameInfo.m_nThreadFrameWidth, 
                                                m_tFrameInfo.m_nThreadFrameHeight, VmbPixelFormatMono12);
            }

            else if ((VmbPixelFormatBayerGR16 == m_tFrameInfo.m_ThreadFormat) || (VmbPixelFormatBayerRG16 == m_tFrameInfo.m_ThreadFormat)||
                     (VmbPixelFormatBayerGB16 == m_tFrameInfo.m_ThreadFormat) || (VmbPixelFormatBayerBG16 == m_tFrameInfo.m_ThreadFormat))
            {
                error = AVT::VmbImageTransform( convertedImage, m_tFrameInfo.m_pThreadFrame.data(), m_tFrameInfo.m_nThreadFrameWidth, 
                                                m_tFrameInfo.m_nThreadFrameHeight, VmbPixelFormatMono16);
            }
            else
            {
                error = AVT::VmbImageTransform( convertedImage, m_tFrameInfo.m_pThreadFrame.data(), m_tFrameInfo.m_nThreadFrameWidth, 
                                                m_tFrameInfo.m_nThreadFrameHeight, m_tFrameInfo.m_ThreadFormat);
                m_tFrameInfo.m_ColorInterpolation = true;
            }
        }
        
        QString sFormat = "-";

        if(! getStartTriggerStatus() )
        {
            sFormat = m_sCurrentFormat;
        }

        if( VmbErrorSuccess != error )
            sFormat = "Convert Error: " + QString::number(error);

        /* while calculating FPS keep streaming for the first 30 frames */
        emit frameReadyFromThread (convertedImage, sFormat, QString::number (m_tFrameInfo.m_nThreadFrameHeight), 
            QString::number (m_tFrameInfo.m_nThreadFrameWidth));
        
    }
}

