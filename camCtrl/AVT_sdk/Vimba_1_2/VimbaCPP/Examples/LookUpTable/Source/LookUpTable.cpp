/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        LookUpTable.cpp

  Description: The LookUpTable example will demonstrate how to use
               the look up table feature of the camera using VimbaCPP.
               

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

#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

#include <LookUpTable.h>
#include <Csv.h>
#include <Exception.h>

#include <VimbaCPP/Include/VimbaCPP.h>


namespace AVT {
namespace VmbAPI {
namespace Examples {

CameraPtr                   LookUpTableCollection::m_pCamera;
vector<LookUpTableControl>  LookUpTableCollection::m_tables;

LookUpTableCollection::LookUpTableCollection( CameraPtr pCamera )
{
    //Check parameters
    if( NULL == pCamera )
    {
        throw Exception("LookUpTableCollection failed.", VmbErrorBadParameter);
    }

    m_pCamera = pCamera;

    VmbErrorType err = VmbErrorSuccess;

    VmbInt64_t nCount;
    err = GetCount ( nCount );
    if ( VmbErrorSuccess == err )
    {
        m_tables.reserve( (size_t)nCount );

        for ( int i = 0; i < nCount; i++ )
        {
            AVT::VmbAPI::Examples::LookUpTableControl control( m_pCamera, i ) ;
            m_tables.push_back( control );
        }
    }

    if ( VmbErrorSuccess != err )
    {
        throw Exception("LookUpTableCollection failed.", err);
    }
}

//Get look up table count
VmbErrorType LookUpTableCollection::GetCount ( VmbInt64_t& nCount )
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr pFeature;
    AVT::VmbAPI::StringVector selector;

    if ( VmbErrorSuccess == err )
    {
        err = m_pCamera->GetFeatureByName ( "LUTSelector", pFeature );

        if ( VmbErrorSuccess == err )
        {
            err = pFeature->GetValues( selector );
            nCount = selector.size();
        }
    }

    return err;
}

//Get look up table control
VmbErrorType LookUpTableCollection::GetControl ( VmbInt64_t nIndex, LookUpTableControl& rTable )
{
    VmbErrorType err = VmbErrorSuccess;

    VmbInt64_t nCount;
    err = GetCount ( nCount );
    if ( VmbErrorSuccess == err )
    {
        if ( nIndex >= nCount )
        {
            err = VmbErrorBadParameter;
        }
        
        FeaturePtr pFeature;

        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "LUTSelector", pFeature );
        }

        StringVector selectorList;
        if ( VmbErrorSuccess == err )
        {
            err = pFeature->GetValues( selectorList );
        }

        if ( VmbErrorSuccess == err )
        {
            err = pFeature->SetValue ( selectorList[ (unsigned int)nIndex ].c_str() );
        }

        if ( VmbErrorSuccess == err )
        {
            rTable = m_tables[ (unsigned int)nIndex ];
        }
    }
    return err;
}

//Get look up table active index
VmbErrorType LookUpTableCollection::GetActiveIndex ( VmbInt64_t& nIndex )
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr pFeature;

    if ( VmbErrorSuccess == err )
    {
        err = m_pCamera->GetFeatureByName ( "LUTSelector", pFeature );
    }

    StringVector selectorList;
    if ( VmbErrorSuccess == err )
    {
        err = pFeature->GetValues( selectorList );
    }

    string selector;
    if ( VmbErrorSuccess == err )
    {
        err = pFeature->GetValue ( selector );
    }

    for ( VmbInt64_t i = 0; i < selectorList.size(); i++ )
    {
        if ( selector == selectorList[ (unsigned int)i ] )
        {
            nIndex = i;
        }
    }

    return err;
}

LookUpTableControl::LookUpTableControl( CameraPtr pCamera, VmbInt64_t nIndex )
{
    //Check parameters
    if( NULL == pCamera )
    {
        throw Exception("LookUpTableControl failed.", VmbErrorBadParameter);
    }

    m_pCamera = pCamera;
    m_nIndex = nIndex;
}

//Enable look up table
VmbErrorType LookUpTableControl::Enable ( bool bEnable )
{

    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr pFeature;

    if ( VmbErrorSuccess == err )
    {
        err = m_pCamera->GetFeatureByName ( "LUTEnable", pFeature );

        if ( VmbErrorSuccess == err )
        {
            err = pFeature->SetValue ( bEnable );
        }
    }

    return err;
}

//Is look up table enabled
VmbErrorType LookUpTableControl::IsEnabled ( bool& bEnable )
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr pFeature;

    if ( VmbErrorSuccess == err )
    {
        err = m_pCamera->GetFeatureByName ( "LUTEnable", pFeature );

        if ( VmbErrorSuccess == err )
        {
            err = pFeature->GetValue ( bEnable );
        }
    }

    return err;
}

//Get look up table index
VmbErrorType LookUpTableControl::GetIndex ( VmbInt64_t& nIndex )
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr    pFeature;
    err = m_pCamera->GetFeatureByName ( "LUTIndex", pFeature );
            
    if ( VmbErrorSuccess == err )
    {
        err = pFeature->GetValue ( nIndex );
    }

    return err;
}

//Get look up table value
VmbErrorType LookUpTableControl::GetValue ( VmbInt64_t& nValue )
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr    pFeature;
    err = m_pCamera->GetFeatureByName ( "LUTValue", pFeature );
            
    if ( VmbErrorSuccess == err )
    {
        err = pFeature->GetValue ( nValue );
    }

    return err;
}

//Set look up table value
VmbErrorType LookUpTableControl::SetValue ( VmbInt64_t nValue )
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr    pFeature;
    err = m_pCamera->GetFeatureByName ( "LUTValue", pFeature );
            
    if ( VmbErrorSuccess == err )
    {
        err = pFeature->SetValue ( nValue );
    }

    return err;
}

//Get value count
VmbErrorType LookUpTableControl::GetValueCount ( VmbInt64_t& nValueCount )
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr    pLUTSize;

    err = m_pCamera->GetFeatureByName ( "LUTSizeBytes", pLUTSize );

    VmbInt64_t    nLUTSize;
    if ( VmbErrorSuccess == err )
    {
        err = pLUTSize->GetValue ( nLUTSize );
    }    

    VmbInt64_t    nLUTBitDepthOut;
    if ( VmbErrorSuccess == err )
    {
        err = GetBitDepthOut ( nLUTBitDepthOut );
    }

    if ( VmbErrorSuccess == err )
    {
        VmbInt64_t nLUTBytePerValue = nLUTBitDepthOut > 8 ? 2 : 1;
        nValueCount = nLUTSize / nLUTBytePerValue;
    }

    return err;
}

//Get raw data
VmbErrorType LookUpTableControl::GetRawData ( UcharVector& data )
{
    VmbErrorType err = VmbErrorSuccess;

    data = m_data;

    return err;
}

//Set raw data
VmbErrorType LookUpTableControl::SetRawData ( UcharVector data )
{
    VmbErrorType err = VmbErrorSuccess;

    m_data = data;

    return err;
}

//Get bit depth in
VmbErrorType LookUpTableControl::GetBitDepthIn ( VmbInt64_t& nBitDepthIn )
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr pFeature;

    err = m_pCamera->GetFeatureByName ( "LUTBitDepthIn", pFeature );

    if ( VmbErrorSuccess == err )
    {
        err = pFeature->GetValue ( nBitDepthIn );
    }

    return err;
}

//Get bit depth out
VmbErrorType LookUpTableControl::GetBitDepthOut ( VmbInt64_t& nBitDepthOut )
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr pFeature;

    err = m_pCamera->GetFeatureByName ( "LUTBitDepthOut", pFeature );

    if ( VmbErrorSuccess == err )
    {
        err = pFeature->GetValue ( nBitDepthOut );
    }

    return err;
}

//Download look up table
VmbErrorType LookUpTableControl::Download ()
{
    VmbErrorType err = VmbErrorSuccess;

    //Evaluate size of LUT
    FeaturePtr    pLUTSize;
    if ( VmbErrorSuccess == err )
    {                        
        err = m_pCamera->GetFeatureByName ( "LUTSizeBytes", pLUTSize );
    }

    VmbInt64_t    nLUTSize;
    if ( VmbErrorSuccess == err )
    {
        err = pLUTSize->GetValue ( nLUTSize );
    }    

    //Allocate buffer
    if ( VmbErrorSuccess == err )
    {
        m_data.resize( (size_t)nLUTSize );
    }

    //Read from camera
    if ( VmbErrorSuccess == err )
    {
        //file access control
        FeaturePtr    pFileOperationSelector;
        err = m_pCamera->GetFeatureByName ( "FileOperationSelector", pFileOperationSelector );

        FeaturePtr    pFileOperationExecute;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileOperationExecute", pFileOperationExecute );
        }

        FeaturePtr    pFileOpenMode;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileOpenMode", pFileOpenMode );
        }

        FeaturePtr    pFileAccessBuffer;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileAccessBuffer", pFileAccessBuffer );
        }

        FeaturePtr    pFileAccessOffset;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileAccessOffset", pFileAccessOffset );
        }

        FeaturePtr    pFileAccessLength;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileAccessLength", pFileAccessLength );
        }
            
        FeaturePtr    pFileOperationStatus;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileOperationStatus", pFileOperationStatus );
        }

        FeaturePtr    pFileOperationResult;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileOperationResult", pFileOperationResult );
        }

        FeaturePtr    pFileStatus;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileStatus", pFileStatus );
        }

        FeaturePtr    pFileSize;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileSize", pFileSize );
        }

        FeaturePtr    pFileSelector;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileSelector", pFileSelector );
        }

        FeaturePtr    pLUTSelector;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "LUTSelector", pLUTSelector );
        }

        //Get current LUTSelector
        string LUTSelectorValue;
        if ( VmbErrorSuccess == err )
        {
            err = pLUTSelector->GetValue ( LUTSelectorValue );
        }

        //Select current LUTSelector as selected file
        string FileSelectorValue;
        if ( VmbErrorSuccess == err )
        {
            FileSelectorValue = "LUT" + LUTSelectorValue;
            err = pFileSelector->SetValue ( FileSelectorValue.c_str() );
        }

        //Select read as open mode
        if ( VmbErrorSuccess == err )
        {
            err = pFileOpenMode->SetValue( "Read" );
        }

        //Select Open as file operation and Execute
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationSelector->SetValue( "Open" );
        }
            
        //Execute file operation
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationExecute->RunCommand ();
        }

        //File size
        VmbInt64_t nFileSize = 0;
        if ( VmbErrorSuccess == err )
        {
            err = pFileSize->GetValue ( nFileSize );
        }

        //File access length
        VmbInt64_t nMinFileAccessLength = 0;
        VmbInt64_t nMaxFileAccessLength = 0;
        if ( VmbErrorSuccess == err )
        {
            err = pFileAccessLength->GetRange( nMinFileAccessLength, nMaxFileAccessLength );
        }

        //Select read as file operation
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationSelector->SetValue ( "Read" );
        }

        VmbInt64_t nFileAccessOffset = 0;
        VmbInt64_t nFileAccessLength = min( nLUTSize, nMaxFileAccessLength );
        UcharVector data( (size_t)nFileAccessLength );

        string LUTOperationStatus;

        do
        {
            //Set FileAccessLength
            if ( VmbErrorSuccess == err )
            {
                err = pFileAccessLength->SetValue ( nFileAccessLength );
            }

            //Execute file operation
            if ( VmbErrorSuccess == err )
            {
                err = pFileOperationExecute->RunCommand ();
            }

            //Get file operation status
            if ( VmbErrorSuccess == err )
            {
                err = pFileOperationStatus->GetValue ( LUTOperationStatus );
            }

            if ( VmbErrorSuccess == err )
            {
                if ( LUTOperationStatus != "Success")
                {
                    err = VmbErrorOther;
                }
            }

            //Get file access buffer
            if ( VmbErrorSuccess == err )
            {
                err = pFileAccessBuffer->GetValue( data );
            }

            //Copy to buffer
            if ( VmbErrorSuccess == err )
            {
                copy( data.begin(), data.end(), m_data.begin() + (size_t)nFileAccessOffset );
            }

            //Get file access offset
            if ( VmbErrorSuccess == err )
            {
                err = pFileAccessOffset->GetValue( nFileAccessOffset );
            }

                
            if ( VmbErrorSuccess != err )
            {
                break;
            }

            nFileAccessLength = min( nFileSize - nFileAccessOffset, nMaxFileAccessLength );
        }
        while ( nFileSize != nFileAccessOffset );

        //Select close as file operation
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationSelector->SetValue ( "Close" );
        }

        //Execute file operation
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationExecute->RunCommand ();
        }

        //Get file operation status
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationStatus->GetValue ( LUTOperationStatus );
        }

        if ( VmbErrorSuccess == err )
        {
            if ( LUTOperationStatus != "Success")
            {
                err = VmbErrorOther;
            }
        }

        if ( VmbErrorSuccess != err )
        {
            //Used LUT index and value as indicator for GigE
            FeaturePtr    pLUTIndex;
            err = m_pCamera->GetFeatureByName ( "LUTIndex", pLUTIndex );
            
            FeaturePtr    pLUTValue;
            if ( VmbErrorSuccess == err )
            {
                err = m_pCamera->GetFeatureByName ( "LUTValue", pLUTValue );
            }

            if ( VmbErrorSuccess == err )
            {
                //Get LUT address for GigE (indicator for direct memory access)
                VmbInt64_t nLUTAddress = 0;
                if ( VmbErrorSuccess == err )
                {
                    FeaturePtr    pLUTAddress;
                    err = m_pCamera->GetFeatureByName ( "LUTAddress", pLUTAddress );
                        
                    if ( VmbErrorSuccess == err )
                    {
                        err = pLUTAddress->GetValue ( nLUTAddress );
                    }
                }

                //Camera supports direct memory access for LUT
                if ( VmbErrorSuccess == err )
                {
                    VmbUint32_t nCompletedReads = 0;
                    err = m_pCamera->ReadMemory ( nLUTAddress, m_data, nCompletedReads);
                }
                //Camera doesn't support direct memory access for LUT
                else
                {
                    VmbInt64_t    nLUTBitDepthOut;
                    err = GetBitDepthOut( nLUTBitDepthOut );

                    //Evaluate number of LUT entries
                    VmbInt64_t nLUTEntries;
                    VmbInt64_t nLUTBytePerValue; 
                    if ( VmbErrorSuccess == err )
                    {
                        nLUTBytePerValue = (nLUTBitDepthOut > 8) ? 2 : 1;
                        nLUTEntries = nLUTSize / nLUTBytePerValue;
                    }

                    //Get LUT values by iteration over indexes
                    if ( VmbErrorSuccess == err )
                    {
                        int iter = 0;
                        for ( VmbInt64_t i = 0; i < nLUTEntries ; i++ )
                        {
                            err = pLUTIndex->SetValue ( i );
                            if ( VmbErrorSuccess != err )
                            {
                                break;
                            }
                            VmbInt64_t nValue;
                            err = pLUTValue->GetValue ( nValue );
                            if ( VmbErrorSuccess != err )
                            {
                                break;
                            }
                            m_data[iter++] = (int)(nValue & 0xFF00) >> 8;
                            if ( 2 == nLUTBytePerValue )
                            {
                                m_data[iter++] = nValue & 0xFF;
                            }
                        }
                    }
                }
            }
        }
    }

    return err;
}

//Upload look up table
VmbErrorType LookUpTableControl::Upload ()
{
    VmbErrorType err = VmbErrorSuccess;

    //Look up table raw data empty
    if ( 0 == m_data.size() )
    {
        err = VmbErrorOther;
    }

    //Evaluate size of LUT
    FeaturePtr    pLUTSize;
    if ( VmbErrorSuccess == err )
    {                        
        err = m_pCamera->GetFeatureByName ( "LUTSizeBytes", pLUTSize );
    }

    VmbInt64_t    nLUTSize;
    if ( VmbErrorSuccess == err )
    {
        err = pLUTSize->GetValue ( nLUTSize );
    }    

    //Write to camera
    if ( VmbErrorSuccess == err )
    {
        //file access control
        FeaturePtr    pFileOperationSelector;
        err = m_pCamera->GetFeatureByName ( "FileOperationSelector", pFileOperationSelector );

        FeaturePtr    pFileOperationExecute;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileOperationExecute", pFileOperationExecute );
        }

        FeaturePtr    pFileOpenMode;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileOpenMode", pFileOpenMode );
        }

        FeaturePtr    pFileAccessBuffer;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileAccessBuffer", pFileAccessBuffer );
        }

        FeaturePtr    pFileAccessOffset;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileAccessOffset", pFileAccessOffset );
        }

        FeaturePtr    pFileAccessLength;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileAccessLength", pFileAccessLength );
        }
            
        FeaturePtr    pFileOperationStatus;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileOperationStatus", pFileOperationStatus );
        }

        FeaturePtr    pFileOperationResult;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileOperationResult", pFileOperationResult );
        }

        FeaturePtr    pFileStatus;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileStatus", pFileStatus );
        }

        FeaturePtr    pFileSize;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileSize", pFileSize );
        }

        FeaturePtr    pFileSelector;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "FileSelector", pFileSelector );
        }

        FeaturePtr    pLUTSelector;
        if ( VmbErrorSuccess == err )
        {
            err = m_pCamera->GetFeatureByName ( "LUTSelector", pLUTSelector );
        }

        //Get current LUTSelector
        string LUTSelectorValue;
        if ( VmbErrorSuccess == err )
        {
            err = pLUTSelector->GetValue ( LUTSelectorValue );
        }

        //Select current LUTSelector as selected file
        string FileSelectorValue;
        if ( VmbErrorSuccess == err )
        {
            FileSelectorValue = "LUT" + LUTSelectorValue;
            err = pFileSelector->SetValue ( FileSelectorValue.c_str() );
        }

        //Select Write as open mode
        if ( VmbErrorSuccess == err )
        {
            err = pFileOpenMode->SetValue( "Write" );
        }

        //Select Open as file operation and Execute
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationSelector->SetValue( "Open" );
        }
            
        //Execute file operation
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationExecute->RunCommand ();
        }

        //File size
        VmbInt64_t nFileSize = 0;
        if ( VmbErrorSuccess == err )
        {
            err = pFileSize->GetValue ( nFileSize );
        }

        //File access length
        VmbInt64_t nMinFileAccessLength = 0;
        VmbInt64_t nMaxFileAccessLength = 0;
        if ( VmbErrorSuccess == err )
        {
            err = pFileAccessLength->GetRange( nMinFileAccessLength, nMaxFileAccessLength );
        }

        //Select write as file operation
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationSelector->SetValue ( "Write" );
        }

        VmbInt64_t nFileAccessOffset = 0;
        VmbInt64_t nFileAccessLength = min( nFileSize, nMaxFileAccessLength );
        UcharVector data( (size_t)nFileAccessLength );
        string LUTOperationStatus;

        do
        {
            //Set FileAccessLength
            if ( VmbErrorSuccess == err )
            {
                err = pFileAccessLength->SetValue ( nFileAccessLength );
            }

            //Fill buffer
            if ( VmbErrorSuccess == err )
            {
                copy( &m_data[ (size_t)nFileAccessOffset ], &m_data[ (size_t)(nFileAccessLength+nFileAccessOffset-1) ], data.begin() ); 
                err = pFileAccessBuffer->SetValue( data );
            }

            //Execute file operation
            if ( VmbErrorSuccess == err )
            {
                err = pFileOperationExecute->RunCommand ();
            }

            //Get file operation status
            if ( VmbErrorSuccess == err )
            {
                err = pFileOperationStatus->GetValue ( LUTOperationStatus );
            }

            if ( VmbErrorSuccess == err )
            {
                if ( LUTOperationStatus != "Success")
                {
                    err = VmbErrorOther;
                }
            }

            //File access offset
            if ( VmbErrorSuccess == err )
            {
                err = pFileAccessOffset->GetValue( nFileAccessOffset );
            }

                
            if ( VmbErrorSuccess != err )
            {
                break;
            }

            nFileAccessLength = min( nFileSize - nFileAccessOffset, nMaxFileAccessLength );
        }
        while ( nFileSize != nFileAccessOffset );

        //Select close as file operation
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationSelector->SetValue ( "Close" );
        }

        //Execute file operation
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationExecute->RunCommand ();
        }

        //Get file operation status
        if ( VmbErrorSuccess == err )
        {
            err = pFileOperationStatus->GetValue ( LUTOperationStatus );
        }

        if ( VmbErrorSuccess == err )
        {
            if ( LUTOperationStatus != "Success")
            {
                err = VmbErrorOther;
            }
        }

        if ( VmbErrorSuccess != err )
        {
            //Used LUT index and value as indicator for GigE
            FeaturePtr    pLUTIndex;
            err = m_pCamera->GetFeatureByName ( "LUTIndex", pLUTIndex );
            
            FeaturePtr    pLUTValue;
            if ( VmbErrorSuccess == err )
            {
                err = m_pCamera->GetFeatureByName ( "LUTValue", pLUTValue );
            }

            if ( VmbErrorSuccess == err )
            {
                //Get LUT address for GigE (indicator for direct memory access)
                VmbInt64_t nLUTAddress = 0;
                if ( VmbErrorSuccess == err )
                {
                    FeaturePtr    pLUTAddress;
                    err = m_pCamera->GetFeatureByName ( "LUTAddress", pLUTAddress );
                        
                    if ( VmbErrorSuccess == err )
                    {
                        err = pLUTAddress->GetValue ( nLUTAddress );
                    }
                }


                //Camera supports direct memory access for LUT
                if ( VmbErrorSuccess == err )
                {
                    VmbUint32_t nCompletedWrites = 0;
                    err = m_pCamera->WriteMemory ( nLUTAddress, m_data, nCompletedWrites );
                }

                //Camera doesn't support direct memory access for LUT
                else
                {
                    FeaturePtr    pLUTBitDepthOut;
                    err = m_pCamera->GetFeatureByName ( "LUTBitDepthOut", pLUTBitDepthOut );

                    VmbInt64_t    nLUTBitDepthOut;
                    if ( VmbErrorSuccess == err )
                    {
                        err = pLUTSize->GetValue ( nLUTBitDepthOut );
                    }    

                    //Evaluate number of LUT entries
                    VmbInt64_t    nLUTEntries;
                    VmbInt64_t    nLUTBytePerValue;
                    if ( VmbErrorSuccess == err )
                    {
                        nLUTBytePerValue = (nLUTBitDepthOut > 8) ? 2 : 1;
                        nLUTEntries = nLUTSize / nLUTBytePerValue;
                    }

                    //Set LUT values by iteration over indexes
                    if ( VmbErrorSuccess == err )
                    {
                        int iter = 0;
                        VmbInt64_t nValue;
                        for ( VmbInt64_t i = 0; i < nLUTEntries ; i++ )
                        {
                            err = pLUTIndex->SetValue ( i );
                            if ( VmbErrorSuccess != err )
                            {
                                break;
                            }
                            nValue = m_data[iter++];
                            if ( 2 == nLUTBytePerValue )
                            {
                                nValue = nValue << 8;
                                nValue += m_data[iter++];
                            }
                            err = pLUTValue->SetValue ( nValue );
                            if ( VmbErrorSuccess != err )
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    return err;
}

//Load look up table from Csv
VmbErrorType LookUpTableControl::LoadFromCsv ( const char *pFileName, int nIndex )
{
    VmbErrorType err = VmbErrorSuccess;

    //Evaluate size of LUT
    FeaturePtr    pLUTSize;
    if ( VmbErrorSuccess == err )
    {                        
        err = m_pCamera->GetFeatureByName ( "LUTSizeBytes", pLUTSize );
    }

    VmbInt64_t    nLUTSize;
    if ( VmbErrorSuccess == err )
    {
        err = pLUTSize->GetValue ( nLUTSize );
    }    

    VmbInt64_t    nLUTBitDepthOut;
    if ( VmbErrorSuccess == err )
    {
        err = GetBitDepthOut ( nLUTBitDepthOut );
    }

    VmbInt64_t nLUTBytePerValue;
    if ( VmbErrorSuccess == err )
    {
        nLUTBytePerValue = nLUTBitDepthOut > 8 ? 2 : 1;
    }

    // Load LUT from CSV
    if ( VmbErrorSuccess == err )
    {
        //Clear buffer
        m_data.clear();

        typedef std::istream_iterator<char> istream_iterator;
        std::ifstream file(pFileName, ios::binary);
 
        Csv lut;
        string fileName = string(pFileName);
        Csv::CsvLoad load(fileName, lut);
        {
            Csv::CsvRow row;
            while (load.Row(&row))
            {
                if ( row.size() <= (size_t)nIndex )
                {
                    break;
                }

                char data[2];
                if (2 == nLUTBytePerValue)
                {
                    int nData;
                    stringstream ss(row[nIndex]);
                    ss >> nData;
                    data[0] = (char)(nData >> 8);
                    data[1] = (char)nData;
                }
                else
                {
                    char nData = *(row[nIndex].c_str());
                    data[0] = nData;
                }

                copy ( data, data + nLUTBytePerValue, std::back_inserter(m_data));
            }
        }

        file.close();
    }

    return err;
}

//Save look up table to CSV
VmbErrorType LookUpTableControl::SaveToCsv ( const char *pFileName )
{
    VmbErrorType err = VmbErrorSuccess;

    //Raw data empty
    if ( m_data.empty() )
    {
        err = VmbErrorOther;
    }

    //Evaluate size of LUT
    FeaturePtr    pLUTSize;
    if ( VmbErrorSuccess == err )
    {                        
        err = m_pCamera->GetFeatureByName ( "LUTSizeBytes", pLUTSize );
    }

    VmbInt64_t    nLUTSize;
    if ( VmbErrorSuccess == err )
    {
        err = pLUTSize->GetValue ( nLUTSize );
    }    

    VmbInt64_t    nLUTBitDepthOut;
    if ( VmbErrorSuccess == err )
    {
        err = GetBitDepthOut ( nLUTBitDepthOut );
    }

    VmbInt64_t nLUTBytePerValue;
    if ( VmbErrorSuccess == err )
    {
        nLUTBytePerValue = nLUTBitDepthOut > 8 ? 2 : 1;
    }

    VmbInt64_t nLUTEntries;
    if ( VmbErrorSuccess == err )
    {
        nLUTEntries = nLUTSize/nLUTBytePerValue;
    }

    // Save LUT data to CSV
    if ( VmbErrorSuccess == err )
    {
        typedef std::ostream_iterator<char> ostream_iterator;
        std::ofstream file(pFileName, ios::binary);
 
        Csv lut;
        string fileName = string(pFileName);
        Csv::CsvSave save(fileName, lut);
        {
            for (int i = 0; i < nLUTEntries; i++)
            {
                Csv::CsvRow row;
                size_t nItem = (size_t)(i * nLUTBytePerValue);
                int data = m_data[nItem];
                if (2 == nLUTBytePerValue)
                {
                    data = data << 8;
                    data += m_data[nItem + 1]; 
                }
                stringstream ss;
                ss << data;
                row.SetLineText(ss.str());
                save.Row(row);
            }
        }

        file.close();
    }

    return err;
}

//Load look up table from flash
VmbErrorType LookUpTableControl::LoadFromFlash ()
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr pFeature;

    err = m_pCamera->GetFeatureByName ( "LUTLoad", pFeature );
    if ( VmbErrorNotFound == err )
    {
        err = m_pCamera->GetFeatureByName ( "LUTLoadAll", pFeature );
    }

    if ( VmbErrorSuccess == err )
    {
        err = pFeature->RunCommand();
    }

    return err;
}

//Save look up table to flash
VmbErrorType LookUpTableControl::SaveToFlash ()
{
    VmbErrorType err = VmbErrorSuccess;

    FeaturePtr pFeature;

    err = m_pCamera->GetFeatureByName ( "LUTSave", pFeature );
    if ( VmbErrorNotFound == err )
    {
        err = m_pCamera->GetFeatureByName ( "LUTSaveAll", pFeature );
    }

    if ( VmbErrorSuccess == err )
    {
        err = pFeature->RunCommand();
    }

    return err;
}

}}} // namespace AVT::VmbAPI::Examples

