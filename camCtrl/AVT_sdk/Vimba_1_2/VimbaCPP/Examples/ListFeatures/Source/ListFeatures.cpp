/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        ListFeatures.cpp

  Description: The ListFeatures example will list all available features of a
               camera that are found by VimbaCPP.

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

#include <ListFeatures.h>

#include <VimbaCPP/Include/VimbaCPP.h>

namespace AVT {
namespace VmbAPI {
namespace Examples {

void ListFeatures::Print( const char *pStrID )
{
    VimbaSystem& sys = VimbaSystem::GetInstance();                          // Get a reference to the VimbaSystem singleton
    VmbErrorType err = sys.Startup();                                       // Initialize the Vimba API
    FeaturePtrVector features;                                              // A vector of std::shared_ptr<AVT::VmbAPI::Feature> objects
    CameraPtr pCamera = CameraPtr();                                        // Our camera
    std::string strCamID;                                                   // The ID of our camera

    // The static details of a feature
    std::string strName;                                                    // The name of the feature
    std::string strDisplayName;                                             // The display name of the feature
    std::string strTooltip;                                                 // A short description of the feature
    std::string strDescription;                                             // A long description of the feature
    std::string strCategory;                                                // A category to group features
    std::string strSFNCNamespace;                                           // The Standard Feature Naming Convention namespace
    std::string strUnit;                                                    // The measurement unit of the value
    VmbFeatureDataType eType;                                               // The data type of the feature

    // The changeable value of a feature
    VmbInt64_t  nValue;                                                     // An int value
    double      fValue;                                                     // A float value
    std::string strValue;                                                   // A string value
    bool        bValue;                                                     // A bool value

    std::stringstream strError;

    if ( VmbErrorSuccess == err )
    {
        if ( NULL == pStrID )                                               // If no ID was provided use the first camera
        {
            CameraPtrVector cameras;
            err = sys.GetCameras( cameras );
            if (    VmbErrorSuccess == err
                 && 0 < cameras.size() )
            {
                pCamera = cameras[0];                                       // Get the camera
                err = pCamera->Open( VmbAccessModeFull );                   // Open the camera
                if ( VmbErrorSuccess == err)
                {
                    err = pCamera->GetID( strCamID );
                    pStrID = strCamID.c_str();
                }
            }
        }
        else
        {
            err = sys.OpenCameraByID( pStrID, VmbAccessModeFull, pCamera ); // Get and open the camera
        }

        if ( NULL != pCamera )
        {
            std::cout << "Printing all features of camera with ID: " << pStrID << std::endl;
            err = pCamera->GetFeatures( features );                         // Fetch all features of our cam
            if ( VmbErrorSuccess == err )
            {
                // Query all static details as well as the value of all fetched features and print them out.
                for (   FeaturePtrVector::const_iterator iter = features.begin();
                        features.end() != iter;
                        ++iter )
                {
                    err = (*iter)->GetName( strName );
                    if ( VmbErrorSuccess != err )
                    {
                        strError << "[Could not get feature Name. Error code: " << err << "]";
                        strName.assign( strError.str() );
                    }

                    err = (*iter)->GetDisplayName( strDisplayName );
                    if ( VmbErrorSuccess != err )
                    {
                        strError << "[Could not get feature Display Name. Error code: " << err << "]";
                        strDisplayName.assign( strError.str() );
                    }

                    err = (*iter)->GetToolTip( strTooltip );
                    if ( VmbErrorSuccess != err )
                    {
                        strError << "[Could not get feature Tooltip. Error code: " << err << "]";
                        strTooltip.assign( strError.str() );
                    }

                    err = (*iter)->GetDescription( strDescription );
                    if ( VmbErrorSuccess != err )
                    {
                        strError << "[Could not get feature Description. Error code: " << err << "]";
                        strDescription.assign( strError.str() );
                    }

                    err = (*iter)->GetCategory( strCategory );
                    if ( VmbErrorSuccess != err )
                    {
                        strError << "[Could not get feature Category. Error code: " << err << "]";
                        strCategory.assign( strError.str() );
                    }

                    err = (*iter)->GetSFNCNamespace( strSFNCNamespace );
                    if ( VmbErrorSuccess != err )
                    {
                        strError << "[Could not get feature SNFC Namespace. Error code: " << err << "]";
                        strSFNCNamespace.assign( strError.str() );
                    }

                    err = (*iter)->GetUnit( strUnit );
                    if ( VmbErrorSuccess != err )
                    {
                        strError << "[Could not get feature Unit. Error code: " << err << "]";
                        strUnit.assign( strError.str() );
                    }

                    std::cout << "/// Feature Name: " << strName << std::endl;
                    std::cout << "/// Display Name: " << strDisplayName << std::endl;
                    std::cout << "/// Tooltip: " << strTooltip << std::endl;
                    std::cout << "/// Description: " << strDescription << std::endl;
                    std::cout << "/// SNFC Namespace: " << strSFNCNamespace << std::endl;
                    std::cout << "/// Value: ";

                    err = (*iter)->GetDataType( eType );
                    if ( VmbErrorSuccess != err )
                    {
                        std::cout << "[Could not get feature Data Type. Error code: " << err << "]" << std::endl;
                    }
                    else
                    {
                        switch ( eType )
                        {
                            case VmbFeatureDataBool:
                                err = (*iter)->GetValue( bValue );
                                if ( VmbErrorSuccess == err )
                                {
                                    std::cout << bValue << std::endl;
                                }
                                break;
                            case VmbFeatureDataEnum:
                                err = (*iter)->GetValue( strValue );
                                if ( VmbErrorSuccess == err )
                                {
                                    std::cout << strValue << std::endl;
                                }
                                break;
                            case VmbFeatureDataFloat:
                                err = (*iter)->GetValue( fValue );
                                {
                                    std::cout << fValue << std::endl;
                                }
                                break;
                            case VmbFeatureDataInt:
                                err = (*iter)->GetValue( nValue );
                                {
                                    std::cout << nValue << std::endl;
                                }
                                break;
                            case VmbFeatureDataString:
                                err = (*iter)->GetValue( strValue );
                                {
                                    std::cout << strValue << std::endl;
                                }
                                break;
                            case VmbFeatureDataCommand:
                            default:
                                std::cout << "[None]" << std::endl;
                                break;

                            if ( VmbErrorSuccess != err )
                            {
                                std::cout << "Could not get feature value. Error code: " << err << std::endl;
                            }
                        }
                    }
                    
                    std::cout << std::endl;
                }
            }
            else
            {
                std::cout << "Could not get features. Error code: " << err << std::endl;
            }

            pCamera->Close();
        }
        else
        {
            std::cout << "Could not open camera or no camera available. Error code: " << err << std::endl;
        }

        sys.Shutdown();
    }
    else
    {
        std::cout << "Could not start system. Error code: " << err << std::endl;
    }
}

}}} // mamespace AVT::Vimba::Examples
