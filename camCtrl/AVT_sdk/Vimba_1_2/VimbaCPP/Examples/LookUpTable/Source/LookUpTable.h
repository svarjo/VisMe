/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        LookUpTable.h

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

#ifndef AVT_VMBAPI_EXAMPLES_LOOKUPTABLE
#define AVT_VMBAPI_EXAMPLES_LOOKUPTABLE

#include <VimbaCPP/Include/VimbaCPP.h>

using namespace std;

namespace AVT {
namespace VmbAPI {
namespace Examples {

class LookUpTableControl
{
private:
    CameraPtr       m_pCamera;
    VmbInt64_t      m_nIndex;
    UcharVector     m_data;

public:
    //ctor
    LookUpTableControl ( CameraPtr pCamera, VmbInt64_t nIndex ); 

    //Enable
    VmbErrorType Enable ( bool bEnable );

    //Is enabled
    VmbErrorType IsEnabled ( bool& pEnable );

    //Get index
    VmbErrorType GetIndex ( VmbInt64_t& nIndex );

    //Get value count
    VmbErrorType GetValueCount ( VmbInt64_t& nValueCount );

    //Get data
    VmbErrorType GetRawData ( UcharVector& data );

    //Set data
    VmbErrorType SetRawData ( UcharVector data );

    //Get Value
    VmbErrorType GetValue ( VmbInt64_t& nValue );

    //Set Value
    VmbErrorType SetValue ( VmbInt64_t nValue );

    //Get bit depth in
    VmbErrorType GetBitDepthIn ( VmbInt64_t& nBitDepthIn );

    //Get bit depth out
    VmbErrorType GetBitDepthOut ( VmbInt64_t& nBitDepthOut );
    
    //Upload the selected look up table into internal memory of the camera
    VmbErrorType Upload ();

    //Download look up table from internal memory into the camera
    VmbErrorType Download ();

    //Load from Csv
    VmbErrorType LoadFromCsv ( const char *pFileName, int nIndex );

    //Save to Csv
    VmbErrorType SaveToCsv ( const char *pFileName );

    //Load from flash
    VmbErrorType LoadFromFlash ();

    //Save to flash
    VmbErrorType SaveToFlash ();
};

class LookUpTableCollection
{
private:
    static CameraPtr                    m_pCamera;
    static vector<LookUpTableControl>   m_tables;

public:
    //ctor
    LookUpTableCollection ( CameraPtr pCamera );

    //Get count
    static VmbErrorType GetCount ( VmbInt64_t& nCount );

    // Get control
    static VmbErrorType GetControl ( VmbInt64_t nIndex, LookUpTableControl& rTable );

    //Get active index
    static VmbErrorType GetActiveIndex ( VmbInt64_t& nIndex );
};

}}} // namespace AVT::Vimba::Examples

#endif
