/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        program.cpp

  Description: Main entry point of BandwidthHelper example of VimbaCPP.

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

#include <iostream>
#include <map>
#include <stdlib.h>
#include <string.h>

#include <BandwidthHelper.h>

enum SettingsMode
{
    SettingsModeUnknown = 0,
    SettingsModeGet     = 1,
    SettingsModeSet     = 2,
    SettingsModeGetMin  = 3,
    SettingsModeGetMax  = 4
};

bool StartsWith( const char *pString, const char *pStart )
{
    if( NULL == pString )
    {
        return false;
    }
    if( NULL == pStart )
    {
        return false;
    }

    if( strlen(pString) < strlen(pStart) )
    {
        return false;
    }

    if( memcmp(pString, pStart, strlen(pStart)) != 0 )
    {
        return false;
    }

    return true;
}

int main( int argc, char* argv[] )
{
    std::string cameraID, cameraName;
    VmbInterfaceType cameraInterfaceType;
    double fValue;
    SettingsMode settingsMode = SettingsModeUnknown; 
    bool printHelp = false;

    std::cout << std::endl;
    std::cout << "//////////////////////////////////////////////" << std::endl;
    std::cout << "/// AVT Vimba API Bandwidth Helper Example ///" << std::endl;
    std::cout << "//////////////////////////////////////////////" << std::endl;
    std::cout << std::endl;

    VmbErrorType err = VmbErrorSuccess;

    //////////////////////
    //Parse command line//
    //////////////////////

    if ( 4 < argc )
    {
        err = VmbErrorBadParameter;
        printHelp = true;
    }
    else
    {
        for ( int i=1; i<argc; ++i )
        {
            char *pParameter = argv[i];
            if ( 0 >= strlen( pParameter ))
            {
                err = VmbErrorBadParameter;
                break;
            }

            if ( pParameter[0] == '/' )
            {
                // Get bandwidth usage
                if( 0 == strcmp( pParameter, "/g" ))
                {
                    if( SettingsModeUnknown != settingsMode )
                    {
                        err = VmbErrorBadParameter;
                        break;
                    }

                    settingsMode = SettingsModeGet;
                }
                // Set bandwidth usage
                else if ( true == StartsWith( pParameter, "/s:" ))
                {
                    if( SettingsModeUnknown != settingsMode )
                    {
                        err = VmbErrorBadParameter;
                        break;
                    }

                    settingsMode = SettingsModeSet;
                    
                    std::string strVal = pParameter + 3;
                    if ( 0 >= strVal.size() )
                    {
                        err = VmbErrorBadParameter;
                        break;
                    }
                    fValue = atof( strVal.c_str() ) / 100;
                }
                else if ( 0 == strcmp( pParameter, "/h" ))
                {
                    if( true == printHelp )
                    {
                        err = VmbErrorBadParameter;
                        break;
                    }

                    printHelp = true;
                }
                // Get min bandwidth usage
                else if( 0 == strcmp( pParameter, "/min" ))
                {
                    if( SettingsModeUnknown != settingsMode )
                    {
                        err = VmbErrorBadParameter;
                        break;
                    }

                    settingsMode = SettingsModeGetMin;
                }
                // Get max bandwidth usage
                else if( 0 == strcmp( pParameter, "/max" ))
                {
                    if( SettingsModeUnknown != settingsMode )
                    {
                        err = VmbErrorBadParameter;
                        break;
                    }

                    settingsMode = SettingsModeGetMax;
                }
                else
                {
                    err = VmbErrorBadParameter;
                    break;
                }
            }
            else
            {
                if ( false == cameraID.empty() )
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                cameraID = pParameter;
            }
        }
    }
    
    // Write out an error if we could not parse the command line
    if( VmbErrorBadParameter == err )
    {
        std::cout << "Invalid parameter found." << std::endl << std::endl;
        printHelp = true;
    }

    // Print out help
    if( true == printHelp )
    {
        std::cout << "Gets or sets the current bandwidth as percentage of the theoretically possible bandwidth." << std::endl << std::endl;
        std::cout << "Usage: BandwidthHelper.exe [CameraID] [/h] [/{g|s:val|min|max}]" << std::endl;
        std::cout << "Parameters:   CameraID    ID of the camera to use (using first camera if not specified)" << std::endl;
        std::cout << "              /h          Print out help" << std::endl;
        std::cout << "              /g          Get bandwidth usage (default if not specified)" << std::endl;
        std::cout << "              /s:val      Set bandwidth usage to <val> % of the maximum bandwidth" << std::endl;
        std::cout << "              /min        Get minimal possible bandwidth usage" << std::endl;
        std::cout << "              /max        Get maximal possible bandwidth usage" << std::endl << std::endl;

        return err;
    }

    if ( VmbErrorSuccess == err )
    {
        // Get a reference to the VimbaSystem singleton
        AVT::VmbAPI::VimbaSystem &rVimbaSystem = AVT::VmbAPI::VimbaSystem::GetInstance();

        // Startup API
        err = rVimbaSystem.Startup();
        if ( VmbErrorSuccess != err )
        {
            std::cout << "Could not start system. Error code: " << err << std::endl;
        }
        else
        {
            // Open camera
            AVT::VmbAPI::CameraPtr pCamera;
            // Open first available camera
            if ( true == cameraID.empty() )
            {
                // Fetch all cameras known to Vimba
                AVT::VmbAPI::CameraPtrVector cameras;
                err = rVimbaSystem.GetCameras( cameras );
                if(VmbErrorSuccess == err)
                {
                    if(cameras.size() > 0)
                    {
                        for (   AVT::VmbAPI::CameraPtrVector::const_iterator iter = cameras.begin();
                                cameras.end() != iter;
                                ++iter )
                        {
                            // Check if we can open the camera in full mode
                            VmbAccessModeType accessMode = VmbAccessModeNone;
                            err = (*iter)->GetPermittedAccess( accessMode );
                            if( VmbErrorSuccess == err )
                            {
                                if( VmbAccessModeFull & accessMode )
                                {
                                    // Now get the camera ID
                                    err = ( *iter )->GetID( cameraID );
                                    if( VmbErrorSuccess == err )
                                    {
                                        // Try to open the camera
                                        err = ( *iter )->Open( VmbAccessModeFull );
                                        if( VmbErrorSuccess == err )
                                        {
                                            pCamera = *iter;
                                            // Get camera name and interface type
                                            if (    VmbErrorSuccess == pCamera->GetName( cameraName )
                                                 && VmbErrorSuccess == pCamera->GetInterfaceType( cameraInterfaceType ))
                                            {
                                                std::cout << "Successfully opened " << AVT::VmbAPI::Examples::BandwidthHelper::InterfaceToString( cameraInterfaceType ) << " camera " << cameraName << " (" << cameraID << ") " << std::endl;
                                            }
                                            else
                                            {
                                                std::cout << "Successfully opened camera " << "(" << cameraID << ") " << std::endl;
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        if( NULL == pCamera )
                        {
                            std::cout << "Could not open any camera." << std::endl;
                            err = VmbErrorNotFound;
                        }
                    }
                    else
                    {
                        std::cout << "No camera available." << std::endl;
                        err = VmbErrorNotFound;
                    }
                }
                else
                {
                    std::cout << "Could not list cameras. Error code: " << err << std::endl;
                }
            }
            else
            {
                //Open specific camera
                err = rVimbaSystem.OpenCameraByID( cameraID.c_str(), VmbAccessModeFull, pCamera );
                if(VmbErrorSuccess != err)
                {
                    std::cout << "Could not open camera. Error code: " << err << std::endl;
                }
            }
        
            if( VmbErrorSuccess == err )
            {
                switch( settingsMode )
                {
                    default:
                    case SettingsModeGet:
                    {
                        // Get bandwidth
                        err = AVT::VmbAPI::Examples::BandwidthHelper::GetBandwidthUsage( pCamera, fValue );
                        if(VmbErrorSuccess != err)
                        {
                            std::cout << "Could not get bandwidth usage. Error code: " << err << std::endl;
                        }
                        else
                        {
                            std::cout << "Bandwidth usage: " << fValue * 100 << "%" << std::endl;
                        }
                    }
                    break;

                    case SettingsModeSet:
                    {
                        // Set bandwidth
                        err = AVT::VmbAPI::Examples::BandwidthHelper::SetBandwidthUsage( pCamera, fValue );
                        if ( VmbErrorSuccess == err )
                        {
                            // Read back written value
                            err = AVT::VmbAPI::Examples::BandwidthHelper::GetBandwidthUsage( pCamera, fValue );
                            if ( VmbErrorSuccess == err )
                            {
                                std::cout << "Bandwidth usage successfully set to: " << fValue * 100 << "%" << std::endl;
                            }
                        }
                        if( VmbErrorSuccess != err )
                        {
                            std::cout << "Could not set bandwidth usage. Error code: " << err << std::endl;
                        }
                    }
                    break;

                    case SettingsModeGetMin:
                    {
                        // Get bandwidth
                        err = AVT::VmbAPI::Examples::BandwidthHelper::GetMinPossibleBandwidthUsage( pCamera, fValue );
                        if(VmbErrorSuccess != err)
                        {
                            std::cout << "Could not get minimal possible bandwidth usage. Error code: " << err << std::endl;
                        }
                        else
                        {
                            std::cout << "Minimal possible bandwidth usage: " << fValue * 100 << "%" << std::endl;
                        }
                    }
                    break;

                    case SettingsModeGetMax:
                    {
                        // Get bandwidth
                        err = AVT::VmbAPI::Examples::BandwidthHelper::GetMaxPossibleBandwidthUsage( pCamera, fValue );
                        if(VmbErrorSuccess != err)
                        {
                            std::cout << "Could not get maximal possible bandwidth usage. Error code: " << err << std::endl;
                        }
                        else
                        {
                            std::cout << "Maximal possible bandwidth usage: " << fValue * 100 << "%" << std::endl;
                        }
                    }
                    break;
                }
                // Close camera
                err = pCamera->Close();
            }
            // Shutdown API
            rVimbaSystem.Shutdown();
        }

        return err;
    }
}
