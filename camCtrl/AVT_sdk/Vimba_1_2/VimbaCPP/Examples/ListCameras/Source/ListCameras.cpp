/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        ListCameras.cpp

  Description: The ListCameras example will list all available cameras that
               are found by VimbaCPP.

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

#include <sstream>
#include <iostream>
#include <vector>

#include <ListCameras.h>

#include <VimbaCPP/Include/VimbaCPP.h>

namespace AVT {
namespace VmbAPI {
namespace Examples {

void ListCameras::Print()
{
    VimbaSystem& sys = VimbaSystem::GetInstance();  // Get a reference to the VimbaSystem singleton
    VmbErrorType err = sys.Startup();               // Initialize the Vimba API
    CameraPtrVector cameras;                        // A vector of std::shared_ptr<AVT::VmbAPI::Camera> objects

    std::string strID;                              // The ID of the cam
    std::string strName;                            // The name of the cam
    std::string strModelname;                       // The model name of the cam
    std::string strSerialNumber;                    // The serial number of the cam
    std::string strInterfacaeID;                    // The ID of the interface the cam is connected to

    std::stringstream strError;
    
    if ( VmbErrorSuccess == err )
    {
        err = sys.GetCameras( cameras );            // Fetch all cameras known to Vimba
        if ( VmbErrorSuccess == err )
        {
            std::cout << "Cameras found: " << cameras.size() << std::endl << std::endl;

            // Query all static details of all known cameras and print them out.
            // We don't have to open the cameras for that.
            for (   CameraPtrVector::const_iterator iter = cameras.begin();
                    cameras.end() != iter;
                    ++iter )
            {
                err = (*iter)->GetID( strID );
                if ( VmbErrorSuccess != err )
                {
                    strError << "[Could not get camera ID. Error code: " << err << "]";
                    strID.assign( strError.str() );
                }
                
                err = (*iter)->GetName( strName );
                if ( VmbErrorSuccess != err )
                {
                    strError << "[Could not get camera name. Error code: " << err << "]";
                    strName.assign( strError.str() );
                }

                err = (*iter)->GetModel( strModelname );
                if ( VmbErrorSuccess != err )
                {
                    strError << "[Could not get camera mode name. Error code: " << err << "]";
                    strModelname.assign( strError.str() );
                }

                err = (*iter)->GetSerialNumber( strSerialNumber );
                if ( VmbErrorSuccess != err )
                {
                    strError << "[Could not get camera serial number. Error code: " << err << "]";
                    strSerialNumber.assign( strError.str() );
                }

                err = (*iter)->GetInterfaceID( strInterfacaeID );
                if ( VmbErrorSuccess != err )
                {
                    strError << "[Could not get interface ID. Error code: " << err << "]";
                    strInterfacaeID.assign( strError.str() );
                }

                std::cout << "/// Camera Name: " << strName << \
                std::endl << "/// Model Name: " << strModelname << \
                std::endl << "/// Camera ID: " << strID << \
                std::endl << "/// Serial Number: " << strSerialNumber << \
                std::endl << "/// @ Interface ID: " << strInterfacaeID << \
                std::endl << std::endl;
            }
        }
        else
        {
            std::cout << "Could not list cameras. Error code: " << err << std::endl;
        }

        sys.Shutdown();                             // Close Vimba
    }
    else
    {
        std::cout << "Could not start system. Error code: " << err << std::endl;
    }
}



}}} // mamespace AVT::Vimba::Examples

