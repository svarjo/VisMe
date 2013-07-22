/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        AncillaryData.cpp

  Description: Implementation of class AVT::VmbAPI::AncillaryData.

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

#include <VimbaCPP/Include/AncillaryData.h>

namespace AVT {
namespace VmbAPI {

struct AncillaryData::Impl
{
    VmbFrame_t  *m_pFrame;
};

AncillaryData::AncillaryData()
{
    // No default ctor
}

AncillaryData::AncillaryData( const AncillaryData& )
{
    // No copy ctor
}

AncillaryData& AncillaryData::operator=( const AncillaryData& )
{
    // No assignment operator
    return *this;
}

AncillaryData::AncillaryData( VmbFrame_t *pFrame )
    :   m_pImpl( new Impl() )
{
    m_pImpl->m_pFrame = pFrame;
}

AncillaryData::~AncillaryData()
{
    delete m_pImpl;
}

VmbErrorType AncillaryData::GetBuffer( VmbUchar_t* &rpValue )
{
    rpValue = (VmbUchar_t*)m_pImpl->m_pFrame->buffer;

    return VmbErrorSuccess;
}

VmbErrorType AncillaryData::GetBuffer( const VmbUchar_t* &rpValue ) const
{
    rpValue = (VmbUchar_t*)m_pImpl->m_pFrame->buffer;

    return VmbErrorSuccess;
}

VmbErrorType AncillaryData::GetSize( VmbUint32_t &nSize ) const
{
    nSize = m_pImpl->m_pFrame->ancillarySize;

    return VmbErrorSuccess;
}

VmbErrorType AncillaryData::Open()
{
    VmbError_t res;
    VmbHandle_t hHandle;

    res = VmbAncillaryDataOpen( m_pImpl->m_pFrame, &hHandle );

    if ( VmbErrorSuccess == res )
    {
        SetHandle( hHandle );
    }

    return (VmbErrorType)res;
}

VmbError_t AncillaryData::Close()
{
    VmbError_t res = VmbErrorSuccess;

    Reset();

    if ( NULL != GetHandle() )
    {
        res = VmbAncillaryDataClose( GetHandle() );
    }    

    RevokeHandle();
    
    return (VmbErrorType)res;
}

}} // namespace AVT::VmbAPI
