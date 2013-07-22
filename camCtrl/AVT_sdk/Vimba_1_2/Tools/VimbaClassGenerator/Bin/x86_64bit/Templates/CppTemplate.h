/*=============================================================================
  Copyright (C) 2013 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  Please do not modify this file, because it was created automatically by a 
  code generator tool (AVT VimbaClassGenerator). So any manual modifications 
  will be lost if you run the tool again.

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

#ifndef ### HEADER_FILE_MACRO_NAME ###
#define ### HEADER_FILE_MACRO_NAME ###

#include "VimbaCPP/Include/VimbaCPP.h"

class ### CLASS_NAME ### : public AVT::VmbAPI::Camera
{
    public:
        typedef SP_DECL( ### CLASS_NAME ### ) Ptr;

        ### ENUM_DECLARATIONS ###
        
        ### CLASS_NAME ### (
            const char         *pCameraID,
            const char         *pCameraName,
            const char         *pCameraModel,
            const char         *pCameraSerialNumber,
            const char         *pInterfaceID,
            VmbInterfaceType    interfaceType,
            const char         *pInterfaceName,
            const char         *pInterfaceSerialNumber,
            VmbAccessModeType   interfacePermittedAccess );

        ### METHOD_DECLARATIONS ###

    private:
        ### VARIABLE_DECLARATIONS ###
};

#endif