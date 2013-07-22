/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        ApiController.cpp

  Description: Implementation file for the ApiController helper class that
               demonstrates how to implement a synchronous single image
               acquisition with VimbaCPP.

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

#include <ApiController.h>

namespace AVT {
namespace VmbAPI {
namespace Examples {

#define NUM_FRAMES 3

ApiController::ApiController()
    // Get a reference to the Vimba singleton
    : m_system ( VimbaSystem::GetInstance() )
{
    m_ErrorCodeToMessage[ VmbErrorSuccess ] =           "Success.";
    m_ErrorCodeToMessage[ VmbErrorInternalFault ] =     "Unexpected fault in VmbApi or driver.";    
    m_ErrorCodeToMessage[ VmbErrorApiNotStarted ] =     "API not started.";     
    m_ErrorCodeToMessage[ VmbErrorNotFound ] =          "Not found.";
    m_ErrorCodeToMessage[ VmbErrorBadHandle ] =         "Invalid handle ";
    m_ErrorCodeToMessage[ VmbErrorDeviceNotOpen ] =     "Device not open.";
    m_ErrorCodeToMessage[ VmbErrorInvalidAccess ] =     "Invalid access.";
    m_ErrorCodeToMessage[ VmbErrorBadParameter ] =      "Bad parameter.";
    m_ErrorCodeToMessage[ VmbErrorStructSize ] =        "Wrong DLL version.";
    m_ErrorCodeToMessage[ VmbErrorMoreData ] =          "More data returned than memory provided.";
    m_ErrorCodeToMessage[ VmbErrorWrongType ] =         "Wrong type.";
    m_ErrorCodeToMessage[ VmbErrorInvalidValue ] =      "Invalid value.";
    m_ErrorCodeToMessage[ VmbErrorTimeout ] =           "Timeout.";
    m_ErrorCodeToMessage[ VmbErrorOther ] =             "TL error.";
    m_ErrorCodeToMessage[ VmbErrorResources ] =         "Resource not available.";
    m_ErrorCodeToMessage[ VmbErrorInvalidCall ] =       "Invalid call.";
    m_ErrorCodeToMessage[ VmbErrorNoTL ] =              "TL not loaded.";
    m_ErrorCodeToMessage[ VmbErrorNotImplemented ] =    "Not implemented.";
    m_ErrorCodeToMessage[ VmbErrorNotSupported ] =      "Not supported.";
}

ApiController::~ApiController()
{
}

// Translates Vimba error codes to readable error messages
std::string ApiController::ErrorCodeToMessage( VmbErrorType eErr )
{
    std::map<VmbErrorType, std::string>::const_iterator iter = m_ErrorCodeToMessage.find( eErr );
    if ( m_ErrorCodeToMessage.end() != iter )
    {
        return iter->second;
    }

    return "Unsupported error code passed.";
}

VmbErrorType ApiController::StartUp()
{
    VmbErrorType res;

    // Start Vimba
    res = m_system.Startup();
    if ( VmbErrorSuccess == res )
    {
        // This will be wrapped in a shared_ptr so we don't delete it
        m_pCameraObserver = new CameraObserver();
        // Register an observer whose callback routine gets triggered whenever a camera is plugged in or out
        res = m_system.RegisterCameraListObserver( ICameraListObserverPtr( m_pCameraObserver ));
    }
    
    return res;
}

void ApiController::ShutDown()
{
    // Release Vimba
    m_system.Shutdown();
}

VmbErrorType ApiController::AcquireSingleImage( const std::string &rStrCameraID, FramePtr &rpFrame )
{
    // Open the desired camera by its ID
    VmbErrorType res = m_system.OpenCameraByID( rStrCameraID.c_str(), VmbAccessModeFull, m_pCamera );
    if ( VmbErrorSuccess == res )
    {
        // Set the GeV packet size to the highest possible value
        // (In this example we do not test whether this cam actually is a GigE cam)
        FeaturePtr pCommandFeature;
        if ( VmbErrorSuccess == m_pCamera->GetFeatureByName( "GVSPAdjustPacketSize", pCommandFeature ))
        {
            if ( VmbErrorSuccess == pCommandFeature->RunCommand() )
            {
                bool bIsCommandDone = false;
                do
                {
                    if ( VmbErrorSuccess != pCommandFeature->IsCommandDone( bIsCommandDone ))
                    {
                        break;
                    }
                } while ( false == bIsCommandDone );
            }
        }
        FeaturePtr pFormatFeature;
        // Save the current width
        res = m_pCamera->GetFeatureByName( "Width", pFormatFeature );
        if ( VmbErrorSuccess == res )
        {
            res = pFormatFeature->GetValue( m_nWidth );
            if ( VmbErrorSuccess == res )
            {
                // Save the current height
                res = m_pCamera->GetFeatureByName( "Height", pFormatFeature );
                if ( VmbErrorSuccess == res )
                {
                    pFormatFeature->GetValue( this->m_nHeight );
                    if ( VmbErrorSuccess == res )
                    {
                        // Set pixel format. For the sake of simplicity we only support Mono and RGB in this example.
                        res = m_pCamera->GetFeatureByName( "PixelFormat", pFormatFeature );
                        if ( VmbErrorSuccess == res )
                        {
                            // Try to set RGB
                            res = pFormatFeature->SetValue( VmbPixelFormatRgb8 );
                            if ( VmbErrorSuccess != res )
                            {
                                // Fall back to Mono
                                res = pFormatFeature->SetValue( VmbPixelFormatMono8 );
                            }

                            // Read back the currently selected pixel format
                            pFormatFeature->GetValue( m_nPixelFormat );

                            if ( VmbErrorSuccess == res )
                            {
                                // Acquire
                                res = m_pCamera->AcquireSingleImage( rpFrame, 2000 );
                            }
                        }
                    }
                }
            }
        }
        m_pCamera->Close();
    }

    return res;
}

CameraPtrVector ApiController::GetCameraList()
{
    CameraPtrVector cameras;
    // Get all known cameras
    if ( VmbErrorSuccess == m_system.GetCameras( cameras ))
    {
        // And return them
        return cameras;
    }
    return CameraPtrVector();
}

int ApiController::GetWidth()
{
    return (int)m_nWidth;
}

int ApiController::GetHeight()
{
    return (int)m_nHeight;
}

VmbPixelFormatType ApiController::GetPixelFormat()
{
    return (VmbPixelFormatType)m_nPixelFormat;
}

// Returns the camera observer as QObjects to connect their signals to the view's sots
QObject* ApiController::GetCameraObserver()
{
    return m_pCameraObserver;
}

}}} // namespace AVT::VmbAPI::Examples
