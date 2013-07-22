/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        Exception.h

  Description: Helper for exceptions.
               

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

#ifndef AVT_VMBAPI_EXAMPLES_EXCEPTION
#define AVT_VMBAPI_EXAMPLES_EXCEPTION

#include <VimbaCPP/Include/VimbaCPP.h>

using namespace std;

namespace AVT {
namespace VmbAPI {
namespace Examples {

class Exception: public exception
{
private:
    const std::string m_sMsg;
    // Vimba error code
    VmbErrorType      m_eError;
    // A readable value for every Vimba error code
    std::map<VmbErrorType, std::string> m_ErrorCodeToMessage;
public:
    Exception( const std::string &rsMessage, const VmbErrorType eError )
    : m_sMsg(rsMessage)
    , m_eError(eError)
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
    
    virtual ~Exception() throw()
    {
    }

    VmbErrorType GetError() const
    {
        return m_eError;
    }

    std::string GetMessageStr() const
    {
        return m_sMsg;
    }

    // Translates Vimba error codes to readable error messages
    std::string ErrorCodeToMessage( VmbErrorType eErr )
    {
        std::map<VmbErrorType, std::string>::const_iterator iter = m_ErrorCodeToMessage.find( eErr );
        if ( m_ErrorCodeToMessage.end() != iter )
        {
            return iter->second;
        }

        return "Unsupported error code passed.";
    }
};

}}} // namespace AVT::VmbAPI::Examples

#endif