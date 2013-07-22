/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        program.cpp

  Description: Main entry point of LookUpTable example of VimbaCPP.

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
#include <string.h>
#include <stdlib.h>

#include <LookUpTable.h>
#include <Exception.h>

using namespace std;

enum Mode
{
    ModeUnknown         = 0,
    ModeSave            = 1,
    ModeLoad            = 2,
    ModeSaveCSV         = 3,
    ModeLoadCSV         = 4,
    ModeEnable          = 7,
    ModeIsEnabled       = 8,
    ModeSetValue        = 9,
    ModeGetValue        = 10,
    ModeBitIn           = 11,
    ModeBitOut          = 12,
    ModeCount           = 13
};

bool StartsWith(const char *pString, const char *pStart)
{
    if(NULL == pString)
    {
        return false;
    }
    if(NULL == pStart)
    {
        return false;
    }

    if(strlen(pString) < strlen(pStart))
    {
        return false;
    }

    if(memcmp(pString, pStart, strlen(pStart)) != 0)
    {
        return false;
    }

    return true;
}

int main( int argc, char* argv[] )
{
    cout << "///////////////////////////////////////////" << endl;
    cout << "/// AVT Vimba API Look Up Table Example ///" << endl;
    cout << "///////////////////////////////////////////" << endl << endl;

    VmbErrorType err = VmbErrorSuccess;

    string      cameraID = "";
    string      fileName;
    string      controlIndex;
    bool        bValue = false;
    VmbInt64_t  nValue = 0;
    string      parameter;
    Mode        eMode = ModeUnknown;     
    bool        printHelp = false;

    //////////////////////
    //Parse command line//
    //////////////////////

    for(int i = 1; i < argc; i++)
    {
        char *pParameter = argv[i];
        if(strlen(pParameter) < 0)
        {
            err = VmbErrorBadParameter;
            break;
        }

        if(pParameter[0] == '/')
        {
            if(strcmp(pParameter, "/s") == 0)
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                eMode = ModeSave;
            }
            else if(strcmp(pParameter, "/l") == 0)
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                eMode = ModeLoad;
            }
            else if(strcmp(pParameter, "/sc") == 0)
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                eMode = ModeSaveCSV;
            }
            else if(StartsWith(pParameter, "/lc:"))
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                if(parameter.empty() == false)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                parameter = pParameter + 4;
                if(parameter.size() <= 0)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                nValue = atoi( parameter.c_str() );

                eMode = ModeLoadCSV;
            }
            else if(strcmp(pParameter, "/bi") == 0)
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                eMode = ModeBitIn;
            }
            else if(strcmp(pParameter, "/bo") == 0)
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                eMode = ModeBitOut;
            }
            else if(strcmp(pParameter, "/n") == 0)
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                eMode = ModeCount;
            }
            else if(StartsWith(pParameter, "/e:"))
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                if(parameter.empty() == false)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                parameter = pParameter + 3;
                if(parameter.size() <= 0)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                if(strcmp(parameter.c_str(), "on") == 0)
                {
                    bValue = true;
                }
                else if(strcmp(parameter.c_str(), "off") == 0)
                {
                    bValue = false;
                }
                else
                {
                    cout << "Could not set look up table enable. Wrong parameter!" << endl;
                    err = VmbErrorBadParameter;
                    break;
                }
                eMode = ModeEnable;
            }
            else if(strcmp(pParameter, "/e") == 0)
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                eMode = ModeIsEnabled;
            }
            else if(StartsWith(pParameter, "/v:"))
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                if(parameter.empty() == false)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                parameter = pParameter + 3;
                if(parameter.size() <= 0)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                nValue = atoi( parameter.c_str() );

                eMode = ModeSetValue;
            }
            else if(strcmp(pParameter, "/v") == 0)
            {
                if(ModeUnknown != eMode)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                eMode = ModeGetValue;
            }
            else if(StartsWith(pParameter, "/f:"))
            {
                if(    (ModeUnknown != eMode) &&
                    (ModeSaveCSV != eMode) &&
                    (ModeLoadCSV != eMode) )
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                if(fileName.empty() == false)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                fileName = pParameter + 3;
                if(fileName.size() <= 0)
                {
                    err = VmbErrorBadParameter;
                    break;
                }
            }
            else if(StartsWith(pParameter, "/i:"))
            {
                if(controlIndex.empty() == false)
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                controlIndex = pParameter + 3;
                if(controlIndex.size() <= 0)
                {
                    err = VmbErrorBadParameter;
                    break;
                }
            }
            else if(strcmp(pParameter, "/h") == 0)
            {
                if(     (cameraID.empty() == false)
                    ||  (ModeUnknown != eMode)
                    ||  (true == printHelp))
                {
                    err = VmbErrorBadParameter;
                    break;
                }

                printHelp = true;
            }
            else
            {
                err = VmbErrorBadParameter;
                break;
            }
        }
        else
        {
            if(cameraID.empty() == false)
            {
                err = VmbErrorBadParameter;
                break;
            }

            cameraID = pParameter;
        }
    }

    //Write out an error if we could not parse the command line
    if(VmbErrorBadParameter == err)
    {
        cout << "Invalid parameters!" << endl << endl;
        printHelp = true;
    }

    //Print out help and end program
    if(true == printHelp)
    {
        cout << "Usage: LookUpTable.exe [CameraID] [/i:Index] [/h] [/{s|l|sc|lc:Column|u|d|v(:Value)|e(:Enable)|bi|bo|n}] [/f:FileName]" << endl;
        cout << "Parameters:   CameraID       ID of the camera to use" << endl;
        cout << "                             (using first camera if not specified)" << endl;
        cout << "              /i:Index       Set look up table index" << endl;
        cout << "              /h             Print out help" << endl;
        cout << "              /s             Save look up table to flash" << endl;
        cout << "              /l             Load look up table from flash" << endl;
        cout << "              /sc            Save look up table to Csv" << endl;
        cout << "                             (Look up table previously downloaded)" << endl;
        cout << "              /lc:Column     Load look up table from Csv using specified column" << endl;
        cout << "                             (default if not specified)" << endl;
        cout << "              /e:Enable      Set look up table enable [on/off]" << endl;
        cout << "              /e             Get look up table enable" << endl;
        cout << "                             (default if not specified)" << endl;
        cout << "              /v:Value       Set look up table value" << endl;
        cout << "              /v             Get look up table value" << endl;
        cout << "              /bi            Get look up table bit depth in" << endl;
        cout << "              /bo            Get look up table bit depth out" << endl;
        cout << "              /n             Get look up table count" << endl;
        cout << "              /f:FileName    File name for operation" << endl;
        cout << endl;
        cout << "For example to load a look up table from the csv file C:\\lut.csv and " << endl << "write it to the camera's flash as LUT1 call" << endl <<endl;
        cout << "LookUpTable.exe /i:0 /lc:0 /f:\"C:\\lut.csv\"" << endl << endl;
        cout << "To load the look up table LUT2 from the camera and write it " << endl << "to the csv file C:\\lut.csv call" << endl << endl;
        cout << "LookUpTable.exe /i:1 /sc /f:\"C:\\lut.csv\"" << endl;
        cout << endl;

        return err;
    }

    bool bVimbaStarted = false;
    AVT::VmbAPI::CameraPtr pCamera;
    AVT::VmbAPI::VimbaSystem * pVimbaSystem = NULL;

    try
    {
        // Get a pointer to the VimbaSystem singleton
        pVimbaSystem = &AVT::VmbAPI::VimbaSystem::GetInstance();

        //Startup API
        if(VmbErrorSuccess == err)
        {
            err = pVimbaSystem->Startup();
            if(VmbErrorSuccess != err)
            {
                throw AVT::VmbAPI::Examples::Exception("Could not start system.", err);
            }

            bVimbaStarted = true;
        }

        //Open camera
        if(VmbErrorSuccess == err)
        {
            if(cameraID.empty())
            {
                //Open first available camera

                //Fetch all cameras known to Vimba
                AVT::VmbAPI::CameraPtrVector cameras;
                err = pVimbaSystem->GetCameras(cameras);
                if(VmbErrorSuccess == err)
                {
                    if(cameras.size() > 0)
                    {
                        for (   AVT::VmbAPI::CameraPtrVector::const_iterator iter = cameras.begin();
                                cameras.end() != iter;
                                ++iter )
                        {
                            //Check if we can open the camera in full mode
                            VmbAccessModeType eAccessMode = VmbAccessModeNone;
                            err = (*iter)->GetPermittedAccess(eAccessMode);
                            if(VmbErrorSuccess == err)
                            {
                                if( (VmbAccessModeFull == (VmbAccessModeFull & eAccessMode)) ||
                                    ((cameras.end() - 1) == iter) )
                                {
                                    //Now get the camera ID
                                    err = (*iter)->GetID(cameraID);
                                    if(VmbErrorSuccess == err)
                                    {
                                        //Try to open the camera
                                        err = (*iter)->Open(VmbAccessModeFull);
                                        if(VmbErrorSuccess == err)
                                        {
                                            pCamera = *iter;
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        if(VmbErrorSuccess != err)
                        {
                            err = VmbErrorNotFound;
                            throw AVT::VmbAPI::Examples::Exception("Could not open any camera.", err);
                        }
                    }
                    else
                    {
                        err = VmbErrorNotFound;
                        throw AVT::VmbAPI::Examples::Exception("Could not open any camera.", err);
                    }
                }
                else
                {
                    throw AVT::VmbAPI::Examples::Exception("Could not list cameras.", err);
                }
            }
            else
            {
                //Open specific camera
                err = pVimbaSystem->OpenCameraByID(cameraID.c_str(), VmbAccessModeFull, pCamera);
                if(VmbErrorSuccess != err)
                {
                    throw AVT::VmbAPI::Examples::Exception("Could not open camera.", err);
                }
            }
        }

        if(VmbErrorSuccess == err)
        {
            cout << "Camera ID: " << cameraID << endl << endl;

            AVT::VmbAPI::Examples::LookUpTableCollection collection( pCamera );

        
            VmbInt64_t nIndex;

            if ( controlIndex.empty() == false )
            {
                nIndex = atoi( controlIndex.c_str() );
            }
            else
            {
                err = collection.GetActiveIndex( nIndex );
                if(VmbErrorSuccess != err)
                {
                    throw AVT::VmbAPI::Examples::Exception("Could not get active index of look up table collection.", err);
                }
            }

            AVT::VmbAPI::Examples::LookUpTableControl control( pCamera, nIndex );
            err = collection.GetControl( nIndex, control );
            if(VmbErrorSuccess != err)
            {
                throw AVT::VmbAPI::Examples::Exception("Could not get look up table control.", err);
            }

            if(VmbErrorSuccess == err)
            {
                switch( eMode )
                {
                default:
                case ModeIsEnabled:
                    {
                        bool bEnable;

                        //Is look up table enabled
                        err = control.IsEnabled( bEnable );
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not get Look up table enable.", err);
                        }

                        cout << "Get look up table enable was successful. Enable = " << bEnable << endl;
                    }
                    break;

                case ModeLoad:
                    {                
                        //Load look up table from flash
                        err = control.LoadFromFlash();
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not load look up table from flash.", err);
                        }

                        cout << "Look up table successfully loaded from flash." << endl;
                    }
                    break;

                case ModeSave:
                    {
                        //Save look up table to flash
                        err = control.SaveToFlash();
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not save look up table fo flash.", err);
                        }

                        cout << "Look up table successfully saved to flash." << endl;
                    }
                    break;

                case ModeSaveCSV:
                    {
                        //Download LUT
                        err = control.Download();

                        //Save look up table to file
                        if ( VmbErrorSuccess == err )
                        {
                            err = control.SaveToCsv( fileName.c_str() );
                        }

                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not save look up table.", err);
                        }

                        cout << "Look up table successfully saved to CSV." << endl;
                    }
                    break;

                case ModeLoadCSV:
                    {                
                        //Load look up table from file
                        err = control.LoadFromCsv( fileName.c_str(), (int)nValue );
                    
                        //Upload LUT
                        if ( VmbErrorSuccess == err )
                        {
                            err = control.Upload();
                        }
                    
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not load look up table.", err);
                        }

                        cout << "Look up table successfully loaded from CSV." << endl;
                    }
                    break;

                case ModeEnable:
                    {
                        //Set look up table enable
                        err = control.Enable( bValue );
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not set look up table enable.", err);
                        }

                        cout << "Look up table was enabled successfully." << endl;
                    }
                    break;

                case ModeBitIn:
                    {
                        //Get bit depth in of look up table
                        VmbInt64_t nBitDepthIn;
                        err = control.GetBitDepthIn( nBitDepthIn );
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not get bit depth in of look up table.", err);
                        }

                        cout << "Get look up table 'bit depth in' was successful. BitDepthIn = " << nBitDepthIn << endl;
                    }
                    break;

                case ModeBitOut:
                    {
                        //Get bit depth out of look up table
                        VmbInt64_t nBitDepthOut;
                        err = control.GetBitDepthOut( nBitDepthOut );
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not get bit depth out of look up table.", err);
                        }

                        cout << "Get look up table 'bit depth out' was successful. BitDepthOut = " << nBitDepthOut << endl;
                    }
                    break;

                case ModeSetValue:
                    {
                        //Set look up table value
                        err = control.SetValue( nValue );
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not set look up table value.", err);
                        }

                        cout << "Look up table value was set successfully." << endl;
                    }
                    break;

                case ModeGetValue:
                    {
                        //Get look up table value
                        err = control.GetValue( nValue );
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not get look up table value.", err);
                        }

                        cout << "Get look up table value was successful. Value = " << nValue << endl;
                    }
                    break;

                case ModeCount:
                    {
                        //Get look up table count
                        VmbInt64_t nCount;
                        err = collection.GetCount( nCount );
                        if(VmbErrorSuccess != err)
                        {
                            throw AVT::VmbAPI::Examples::Exception("Could not get look up table count.", err);
                        }

                        cout << "Get look up table count was successful. Count = " << nCount << endl;
                    }
                    break;

                }
            }
        }
    }

    catch( AVT::VmbAPI::Examples::Exception& ex )
    {
        cout << ex.GetMessageStr() << " VimbaException: " << ex.GetError() << " = " << ex.ErrorCodeToMessage(ex.GetError()) << endl;
    }

    //Close camera
    if( NULL != pCamera )
    {
        pCamera->Close();
    }
    
    //Shutdown API
    if(true == bVimbaStarted)
    {
        pVimbaSystem->Shutdown();
    }

    return err;
}