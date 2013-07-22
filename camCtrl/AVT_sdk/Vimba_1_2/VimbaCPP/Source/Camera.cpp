/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------
 
  File:        Camera.cpp

  Description: Implementation of class AVT::VmbAPI::Camera.

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
#pragma warning(disable:4996)
#include <sstream>
#pragma warning(default:4996)

#include <VimbaCPP/Include/Camera.h>

#include <VimbaCPP/Include/LoggerDefines.h>
#include <VimbaCPP/Source/ConditionHelper.h>
#include <VimbaCPP/Source/FrameImpl.h>
#include <VimbaCPP/Source/FrameHandler.h>
#include <VimbaCPP/Source/Helper.h>

namespace AVT {
namespace VmbAPI {

struct Camera::Impl
{
    // Copy of camera infos
    struct CameraInfo
    {
        std::string     cameraIdString;         // Unique identifier for each camera
        std::string     cameraName;             // Name of the camera
        std::string     modelName;              // Model name
        std::string     serialString;           // Serial number
        std::string     interfaceIdString;      // Unique value for each interface or bus
    } m_cameraInfo;

    VmbInterfaceType m_eInterfaceType;          // The type of the interface the camera is connected to

    LockableVector<FrameHandlerPtr> m_frameHandlers;
    ConditionHelper                 m_conditionHelper;

    void AppendFrameToVector( const FramePtr &frame );
};

Camera::Camera()
{
    // No default ctor
}

Camera::Camera( const Camera& )
{
    // No copy ctor
}

Camera& Camera::operator=( const Camera& )
{
    // No assignment operator
    return *this;
}

Camera::Camera( const char *pID,
                const char *pName,
                const char *pModel,
                const char *pSerialNumber,
                const char *pInterfaceID,
                VmbInterfaceType eInterfaceType )
    :   m_pImpl( new Impl() )
{
    m_pImpl->m_cameraInfo.cameraIdString.assign( pID ? pID : "" );
    m_pImpl->m_cameraInfo.cameraName.assign( pName ? pName : "" );
    m_pImpl->m_cameraInfo.interfaceIdString.assign( pInterfaceID ? pInterfaceID : "" );
    m_pImpl->m_cameraInfo.modelName.assign( pModel ? pModel : "" );
    m_pImpl->m_cameraInfo.serialString.assign( pSerialNumber ? pSerialNumber : "" );
    m_pImpl->m_eInterfaceType = eInterfaceType;
}

Camera::~Camera()
{
    Close();

    delete m_pImpl;
}

VmbErrorType Camera::Open( VmbAccessModeType eAccessMode )
{
    VmbError_t res;
    VmbHandle_t hHandle;

    res = VmbCameraOpen( m_pImpl->m_cameraInfo.cameraIdString.c_str(), (VmbAccessMode_t)eAccessMode, &hHandle );

    if ( VmbErrorSuccess == res )
    {
        SetHandle( hHandle );
    }
    
    return (VmbErrorType)res;
}

VmbErrorType Camera::Close()
{
    VmbError_t res = VmbErrorSuccess;

    if ( NULL != GetHandle() )
    {
        if (    0 < m_pImpl->m_frameHandlers.Vector.size()
             && (   VmbErrorSuccess != EndCapture()
                 || VmbErrorSuccess != RevokeAllFrames()) )
        {
            // Do some logging
            LOG_FREE_TEXT( "Could not successfully revoke all frames")
        }

        Reset();

        res = VmbCameraClose( GetHandle() );

        RevokeHandle();
    }

    return (VmbErrorType)res;
}

VmbErrorType Camera::GetID( char * const pStrID, VmbUint32_t &rnLength ) const
{
    VmbErrorType res;

    if ( NULL == pStrID )
    {
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.cameraIdString.length();
        res = VmbErrorSuccess;
    }
    else if ( m_pImpl->m_cameraInfo.cameraIdString.length() <= rnLength )
    {
        std::copy( m_pImpl->m_cameraInfo.cameraIdString.begin(), m_pImpl->m_cameraInfo.cameraIdString.end(), pStrID );
        pStrID[m_pImpl->m_cameraInfo.cameraIdString.length()] = '\0';
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.cameraIdString.length();
        res = VmbErrorSuccess;
    }
    else
    {
        res = VmbErrorMoreData;
    }

    return res;
}

VmbErrorType Camera::GetName( char * const pStrName, VmbUint32_t &rnLength ) const
{
    VmbErrorType res;

    if ( NULL == pStrName )
    {
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.cameraName.length();
        res = VmbErrorSuccess;
    }
    else if ( m_pImpl->m_cameraInfo.cameraName.length() <= rnLength )
    {
        std::copy( m_pImpl->m_cameraInfo.cameraName.begin(), m_pImpl->m_cameraInfo.cameraName.end(), pStrName );
        pStrName[m_pImpl->m_cameraInfo.cameraName.length()] = '\0';
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.cameraName.length();
        res = VmbErrorSuccess;
    }
    else
    {
        res = VmbErrorMoreData;
    }

    return res;
}

VmbErrorType Camera::GetModel( char * const pStrModel, VmbUint32_t &rnLength ) const
{
    VmbErrorType res;

    if ( NULL == pStrModel )
    {
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.modelName.length();
        res = VmbErrorSuccess;
    }
    else if ( m_pImpl->m_cameraInfo.modelName.length() <= rnLength )
    {
        std::copy( m_pImpl->m_cameraInfo.modelName.begin(), m_pImpl->m_cameraInfo.modelName.end(), pStrModel );
        pStrModel[m_pImpl->m_cameraInfo.modelName.length()] = '\0';
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.modelName.length();
        res = VmbErrorSuccess;
    }
    else
    {
        res = VmbErrorMoreData;
    }

    return res;
}

VmbErrorType Camera::GetSerialNumber( char * const pStrSerial, VmbUint32_t &rnLength ) const
{
    VmbErrorType res;

    if ( NULL == pStrSerial )
    {
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.serialString.length();
        res = VmbErrorSuccess;
    }
    else if ( m_pImpl->m_cameraInfo.serialString.length() <= rnLength )
    {
        std::copy( m_pImpl->m_cameraInfo.serialString.begin(), m_pImpl->m_cameraInfo.serialString.end(), pStrSerial );
        pStrSerial[m_pImpl->m_cameraInfo.serialString.length()] = '\0';
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.serialString.length();
        res = VmbErrorSuccess;
    }
    else
    {
        res = VmbErrorMoreData;
    }

    return res;
}

VmbErrorType Camera::GetInterfaceID( char * const pStrInterfaceID, VmbUint32_t &rnLength ) const
{    
    VmbErrorType res;

    if ( NULL == pStrInterfaceID )
    {
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.interfaceIdString.length();
        res = VmbErrorSuccess;
    }
    else if ( m_pImpl->m_cameraInfo.interfaceIdString.length() <= rnLength )
    {
        std::copy( m_pImpl->m_cameraInfo.interfaceIdString.begin(), m_pImpl->m_cameraInfo.interfaceIdString.end(), pStrInterfaceID );
        pStrInterfaceID[m_pImpl->m_cameraInfo.interfaceIdString.length()] = '\0';
        rnLength = (VmbUint32_t)m_pImpl->m_cameraInfo.interfaceIdString.length();
        res = VmbErrorSuccess;
    }
    else
    {
        res = VmbErrorMoreData;
    }

    return res;
}

VmbErrorType Camera::GetInterfaceType( VmbInterfaceType &reInterfaceType ) const
{
    reInterfaceType = m_pImpl->m_eInterfaceType;

    return VmbErrorSuccess;
}

VmbErrorType Camera::GetPermittedAccess( VmbAccessModeType &rePermittedAccess ) const
{
    VmbError_t res;
    VmbCameraInfo_t info;

    res = VmbCameraInfoQuery( m_pImpl->m_cameraInfo.cameraIdString.c_str(), &info, sizeof( VmbCameraInfo_t ));

    if ( VmbErrorSuccess == res )
    {
        rePermittedAccess = (VmbAccessModeType)info.permittedAccess;
    }

    return (VmbErrorType)res;
}

VmbErrorType Camera::ReadRegisters( const VmbUint64_t *pAddressArray, VmbUint32_t nAddressSize, VmbUint64_t *pDataArray, VmbUint32_t *pCompletedReads ) const
{
    return (VmbErrorType)VmbRegistersRead( GetHandle(), nAddressSize, pAddressArray, pDataArray, pCompletedReads );
}

VmbErrorType Camera::WriteRegisters( const VmbUint64_t *pAddressArray, VmbUint32_t nAddressSize, const VmbUint64_t *pDataArray, VmbUint32_t *pCompletedWrites )
{
    return (VmbErrorType)VmbRegistersWrite( GetHandle(), nAddressSize, pAddressArray, pDataArray, pCompletedWrites );
}

VmbErrorType Camera::ReadMemory( const VmbUint64_t address, VmbUchar_t *pBuffer, VmbUint32_t nBufferSize, VmbUint32_t *pSizeComplete ) const
{
    return (VmbErrorType)VmbMemoryRead( GetHandle(), address, nBufferSize, (char*)pBuffer, pSizeComplete );
}

VmbErrorType Camera::WriteMemory( const VmbUint64_t address, const VmbUchar_t *pBuffer, VmbUint32_t nBufferSize, VmbUint32_t *pSizeComplete )
{
    return (VmbErrorType)VmbMemoryWrite( GetHandle(), address, nBufferSize, (char *)pBuffer, pSizeComplete );
}

//Get one image synchronously.
VmbErrorType Camera::AcquireSingleImage( FramePtr &rFrame, VmbUint32_t nTimeout )
{
    VmbErrorType res;
    VmbInt64_t nPLS;
    FeaturePtr pFeature;

    res = GetFeatureByName( "PayloadSize", pFeature );
    if ( VmbErrorSuccess == res )
    {
        res = SP_ACCESS(pFeature)->GetValue( nPLS );
        if ( VmbErrorSuccess == res )
        {
            SP_SET( rFrame, new Frame( nPLS ));
            
            res = AnnounceFrame( rFrame );
            if ( VmbErrorSuccess == res )
            {
                res = StartCapture();
                if ( VmbErrorSuccess == res )
                {
                    res = QueueFrame( rFrame );
                    if ( VmbErrorSuccess == res )
                    {
                        res = GetFeatureByName( "AcquisitionStart", pFeature );
                        if ( VmbErrorSuccess == res )
                        {
                            res = SP_ACCESS( pFeature )->RunCommand();
                            if ( VmbErrorSuccess == res )
                            {
                                res = (VmbErrorType)VmbCaptureFrameWait( GetHandle(), &(SP_ACCESS( rFrame )->m_pImpl->m_frame), nTimeout );
                                if ( VmbErrorSuccess != res )
                                {
                                    // Do some logging
                                    LOG_FREE_TEXT( "Could not acquire single image." )
                                }

                                res = GetFeatureByName( "AcquisitionStop", pFeature );
                                if ( VmbErrorSuccess == res )
                                {
                                    res = SP_ACCESS( pFeature )->RunCommand();

                                    if ( VmbErrorSuccess != res )
                                    {
                                        // Do some logging
                                        LOG_FREE_TEXT( "Could not stop acquisition" )
                                    }
                                }
                                else // Couldn't get feature "AcquisitionStop"
                                {
                                    // Do some logging
                                    LOG_FREE_TEXT( "Could not get command feature AcquisitionStop" )
                                }
                            }
                            else // Couldn't run feature "AcquisitionStart"
                            {
                                // Do some logging
                                LOG_FREE_TEXT( "Could not start acquisition" )
                            }                            
                        }
                        else // Couldn't get feature "AcquisitionStart"
                        {
                            // Do some logging
                            LOG_FREE_TEXT( "Could not get command feature AcquisitionStart" )
                        }

                        res = FlushQueue();
                        if ( VmbErrorSuccess != res )
                        {
                            // Do some logging
                            LOG_FREE_TEXT( "Could not flush frame queue")
                        }
                    } // Couldn't queue frame
                    else
                    {
                        // Do some logging
                        LOG_FREE_TEXT( "Could not queue frame" )
                    }

                    res = EndCapture();
                    if ( VmbErrorSuccess != res )
                    {
                        // Do some logging
                        LOG_FREE_TEXT( "Could not end capture" )
                    }
                }
                else // Couldn't start capture
                {
                    // Do some logging
                    LOG_FREE_TEXT( "Could not start capture" )
                }

                res = RevokeAllFrames();
                if ( VmbErrorSuccess != res )
                {
                    // Do some logging
                    LOG_FREE_TEXT( "Could not revoke frame" )
                }
            }
            else // Couldn't announce frame
            {
                // Do some logging
                LOG_FREE_TEXT( "Could not successfully announce frame");
            }
        }
        else // Couldn't get payload size value
        {
            // Do some logging
            LOG_FREE_TEXT( "Could not get value of feature PayloadSize");
        }
    }
    else // Couldn't get feature "PayloadSize"
    {
        // Do some logging
        LOG_FREE_TEXT( "Could not get feature PayloadSize");
    }

    return res;
}

VmbErrorType Camera::AcquireMultipleImages( FramePtr *pFrames, VmbUint32_t nSize, VmbUint32_t nTimeout, VmbUint32_t *pNumFramesCompleted )
{
    VmbErrorType res = VmbErrorBadParameter;

    if (    NULL == pFrames
         || 0 == nSize )
    {
        return res;
    }

    if ( NULL != pNumFramesCompleted )
    {
        *pNumFramesCompleted = 0;
    }

    VmbInt64_t nPLS;
    FeaturePtr pFeature;

    res = GetFeatureByName( "PayloadSize", pFeature );
    if ( VmbErrorSuccess == res )
    {
        res = SP_ACCESS( pFeature )->GetValue( nPLS );

        if ( VmbErrorSuccess == res )
        {
            VmbUint32_t nFramesAnnounced = 0;
            for ( VmbUint32_t i=0; i<nSize; ++i )
            {
                SP_SET( pFrames[i], new Frame( nPLS ));
                res = AnnounceFrame( pFrames[i] );
                if ( VmbErrorSuccess != res )
                {
                    std::stringstream strMsg("Could only successfully announce ");
                    strMsg << nFramesAnnounced << " of " <<  nSize  << " frames. Will continue with queuing those.";
                    LOG_FREE_TEXT( strMsg.str() );
                    break;
                }
                nFramesAnnounced = i+1;
            }

            res = StartCapture();
            if ( VmbErrorSuccess == res )
            {
                VmbUint32_t nFramesQueued = 0;
                for ( VmbUint32_t i=0; i<nFramesAnnounced; ++i )
                {
                    res = QueueFrame( pFrames[i] );
                    if ( VmbErrorSuccess != res )
                    {
                        std::stringstream strMsg("Could only successfully queue ");
                        strMsg << nFramesQueued << " of " << nSize << " frames. Will continue with filling those.";
                        LOG_FREE_TEXT( strMsg.str() );
                        break;
                    }
                    nFramesQueued = i+1;
                }
                
                res = GetFeatureByName( "AcquisitionStart", pFeature );
                if ( VmbErrorSuccess == res )
                {
                    res = SP_ACCESS( pFeature )->RunCommand();
                    if ( VmbErrorSuccess == res )
                    {
                        VmbUint32_t nFramesFilled = 0;
                        for ( VmbUint32_t i=0; i<nFramesQueued; ++i )
                        {
                            res = (VmbErrorType)VmbCaptureFrameWait( GetHandle(), &(SP_ACCESS( pFrames[i] )->m_pImpl->m_frame), nTimeout );
                            if ( VmbErrorSuccess != res )
                            {
                                std::stringstream strMsg("Could only successfully fill ");
                                strMsg << nFramesFilled << " of " << nSize << " frames. Will stop acquisition now.";
                                LOG_FREE_TEXT( strMsg.str() );
                                break;
                            }
                            else if ( NULL !=  pNumFramesCompleted )
                            {
                                ++(*pNumFramesCompleted);
                            }
                            nFramesFilled = i+1;
                        }

                        res = GetFeatureByName( "AcquisitionStop", pFeature );
                        if ( VmbErrorSuccess == res )
                        {
                            res = SP_ACCESS( pFeature )->RunCommand();

                            if ( VmbErrorSuccess != res )
                            {
                                // Do some logging
                                LOG_FREE_TEXT( "Could not stop acquisition" )
                            }
                        }
                        else // Couldn't get feature "AcquisitionStop"
                        {
                            // Do some logging
                            LOG_FREE_TEXT( "Could not get command feature AcquisitionStop" )
                        }
                    }
                    else // Couldn't run feature "AcquisitionStart"
                    {
                        // Do some logging
                        LOG_FREE_TEXT( "Could not start acquisition" )
                    }                            
                }
                else // Couldn't get feature "AcquisitionStart"
                {
                    // Do some logging
                    LOG_FREE_TEXT( "Could not get command feature AcquisitionStart" )
                }

                res = FlushQueue();
                if ( VmbErrorSuccess != res )
                {
                    // Do some logging
                    LOG_FREE_TEXT( "Could not flush frame queue")
                }                

                res = EndCapture();
                if ( VmbErrorSuccess != res )
                {
                    // Do some logging
                    LOG_FREE_TEXT( "Could not end capture" )
                }
            }
            else // Couldn't start capture
            {
                // Do some logging
                LOG_FREE_TEXT( "Could not start capture" )
            }

            res = RevokeAllFrames();
            if ( VmbErrorSuccess != res )
            {
                // Do some logging
                LOG_FREE_TEXT( "Could not revoke frame" )
            }
        }
        else // Couldn't get payload size value
        {
            // Do some logging
            LOG_FREE_TEXT( "Could not get value of feature PayloadSize");
        }
    }
    else // Couldn't get feature "PayloadSize"
    {
        // Do some logging
        LOG_FREE_TEXT( "Could not get feature PayloadSize");
    }

    return res;
}

VmbErrorType Camera::StartContinuousImageAcquisition( int nBufferCount, const IFrameObserverPtr &rObserver )
{
    VmbErrorType res;

    FramePtrVector frames;
    FeaturePtr pFeature;
    VmbInt64_t nPLS;

    res = GetFeatureByName( "PayloadSize", pFeature );
    if ( VmbErrorSuccess == res )
    {
        res = SP_ACCESS(pFeature)->GetValue( nPLS );
        if ( VmbErrorSuccess == res )
        {
            frames.resize( nBufferCount );
            for (   FramePtrVector::iterator iter = frames.begin();
                    frames.end() != iter; )
            {
                SP_SET( (*iter), new Frame( nPLS ));
                if ( VmbErrorSuccess == SP_ACCESS(( *iter ))->RegisterObserver( rObserver ) )
                {
                    if ( VmbErrorSuccess == AnnounceFrame( *iter ))
                    {
                        ++iter;
                    }
                    else
                    {
                        // Do some logging
                        LOG_FREE_TEXT( "Could not announce frame" )
                        frames.erase( iter++ );
                    }
                }
                else 
                {
                    frames.erase( iter++ );
                    // Do some logging
                    LOG_FREE_TEXT( "Could not register frame observer" )
                }
            }

            res = StartCapture();
            if ( VmbErrorSuccess == res )
            {
                for (   FramePtrVector::iterator iter = frames.begin();
                        frames.end() != iter;
                        ++iter )
                {
                    if ( VmbErrorSuccess != QueueFrame( *iter  ))
                    {
                        // Do some logging
                        LOG_FREE_TEXT( "Could not queue frame" )
                    }
                }

                res = GetFeatureByName( "AcquisitionStart", pFeature );
                if ( VmbErrorSuccess == res )
                {
                    res = SP_ACCESS( pFeature )->RunCommand();
                    if ( VmbErrorSuccess != res )
                    {
                        // Do some logging
                        LOG_FREE_TEXT( "Could not start acquisition" )
                    }
                }
                else // Could not get feature AcquisitionStart
                {
                    // Do some logging
                    LOG_FREE_TEXT( "Could not get feature AcqusitionStart" )
                }                
            }
            else // Could not start capture
            {
                // Do some logging
                LOG_FREE_TEXT( "Could not start capturing" )
            }
        }
        else // Could not get value of feature PayloadSize
        {
            // Do some logging
            LOG_FREE_TEXT( "Could not get value of feature PayloadSize" )
        }
    }
    else // Couldn't get feature PayloadSize
    {
        // Do some logging
        LOG_FREE_TEXT( "Could not get feature PayloadSize" )
    }

    return res;
}

VmbErrorType Camera::StopContinuousImageAcquisition()
{
    VmbErrorType res;

    FeaturePtr pFeature;

    res = GetFeatureByName( "AcquisitionStop", pFeature );
    if ( VmbErrorSuccess == res )
    {
        res = SP_ACCESS( pFeature )->RunCommand();
        if ( VmbErrorSuccess == res )
        {
            res = FlushQueue();
            if ( VmbErrorSuccess == res )
            {
                res = EndCapture();
                if ( VmbErrorSuccess == res )
                {
                    res = RevokeAllFrames();
                    if ( VmbErrorSuccess != res )
                    {
                        // Do some logging
                        LOG_FREE_TEXT( "Could not revoke frames" )
                    }
                }
                else // Could not end capture
                {
                    // Do some logging
                    LOG_FREE_TEXT( "Could not flush queue" )
                }
            }
            else // Could not flush queue
            {
                // Do some logging
                LOG_FREE_TEXT( "Could not end capture" )
            }
        }
        else // Could not run feature command AcquisitionStop
        {
            // Do some logging
            LOG_FREE_TEXT( "Could not run feature command AcquisitionStop")
        }
    }
    else // Could not get feature AcquisitionStop
    {
        // Do some logging
        LOG_FREE_TEXT( "Could not get feature AcquisitionStop" )
    }

    return res;
}

VmbErrorType Camera::AnnounceFrame( const FramePtr &frame ) 
{
    if (    true == SP_ACCESS( frame )->m_pImpl->m_bAlreadyAnnounced
         || true == SP_ACCESS( frame )->m_pImpl->m_bAlreadyQueued )
    {
        return VmbErrorInvalidCall;
    }

    VmbError_t res = VmbFrameAnnounce( GetHandle(), &(SP_ACCESS( frame )->m_pImpl->m_frame), sizeof SP_ACCESS( frame )->m_pImpl->m_frame );
    
    if ( VmbErrorSuccess == res )
    {
        // Begin write lock frame handler list
        if ( true == m_pImpl->m_conditionHelper.EnterWriteLock( m_pImpl->m_frameHandlers ))
        {
            m_pImpl->AppendFrameToVector( frame );
            SP_ACCESS( frame )->m_pImpl->m_bAlreadyAnnounced = true;

            // End write lock frame handler list
            m_pImpl->m_conditionHelper.ExitWriteLock( m_pImpl->m_frameHandlers );
        }
        else
        {
            LOG_FREE_TEXT( "Could not lock announced frame queue for appending frame." );
            res = VmbErrorResources;
        }
    }

    return (VmbErrorType)res;
}

VmbErrorType Camera::RevokeFrame( const FramePtr &frame ) 
{
    VmbError_t res = VmbFrameRevoke( GetHandle(), &(SP_ACCESS( frame )->m_pImpl->m_frame) );

    if ( VmbErrorSuccess == res )
    {
        // Begin (exclusive) write lock frame handler list
        if ( true == m_pImpl->m_conditionHelper.EnterWriteLock( m_pImpl->m_frameHandlers, true ))
        {
            // Dequeue, revoke and delete frame
            for(    FrameHandlerPtrVector::iterator iter = m_pImpl->m_frameHandlers.Vector.begin();
                    m_pImpl->m_frameHandlers.Vector.end() != iter;)
            {
                // Begin exclusive write lock frame handler
                if ( true == SP_ACCESS(( *iter ))->EnterWriteLock( true ))
                {
                    if ( SP_ISEQUAL( frame, SP_ACCESS(( *iter ))->GetFrame() ))
                    {
                        SP_ACCESS( frame )->m_pImpl->m_frame.context[FRAME_HDL] = NULL;
                        SP_ACCESS( frame )->m_pImpl->m_bAlreadyQueued = false;
                        SP_ACCESS( frame )->m_pImpl->m_bAlreadyAnnounced = false;
                        // End exclusive write lock frame handler
                        SP_ACCESS(( *iter ))->ExitWriteLock();
                        iter = m_pImpl->m_frameHandlers.Vector.erase( iter );
                        return VmbErrorSuccess;
                    }
                    else
                    {
                        // End exclusive write lock frame handler
                        SP_ACCESS(( *iter ))->ExitWriteLock();
                        
                        ++iter;
                    }
                }
            }

            // End (exclusive) write lock frame handler list
            m_pImpl->m_conditionHelper.ExitWriteLock( m_pImpl->m_frameHandlers );
        }
        else
        {
            LOG_FREE_TEXT( "Could not lock announced frame queue for removing frame." );
            res = VmbErrorResources;
        }
    }
    else
    {
        LOG_FREE_TEXT( "Could not revoke frames" )
    }

    return (VmbErrorType)res;
}

VmbErrorType Camera::RevokeAllFrames() 
{
    VmbError_t res;

    // HINT: We need to flush before we can revoke frames
    res = FlushQueue();

    res = VmbFrameRevokeAll( GetHandle() );

    if ( VmbErrorSuccess == res )
    {
        // Begin (exclusive) write lock frame handler list
        if ( true == m_pImpl->m_conditionHelper.EnterWriteLock( m_pImpl->m_frameHandlers, true ))
        {
            // Dequeue, revoke and delete frames
            for (   FrameHandlerPtrVector::iterator iter = m_pImpl->m_frameHandlers.Vector.begin();
                    m_pImpl->m_frameHandlers.Vector.end() != iter;
                    ++iter )
            {
                // Begin exclusive write lock frame handler
                if ( true == SP_ACCESS(( *iter ))->EnterWriteLock( true ))
                {
                    SP_ACCESS( SP_ACCESS(( *iter ))->GetFrame() )->m_pImpl->m_frame.context[FRAME_HDL] = NULL;
                    SP_ACCESS( SP_ACCESS(( *iter ))->GetFrame() )->m_pImpl->m_bAlreadyQueued = false;
                    SP_ACCESS (SP_ACCESS(( *iter ))->GetFrame() )->m_pImpl->m_bAlreadyAnnounced = false;
                    // End exclusive write lock frame handler
                    SP_ACCESS(( *iter ))->ExitWriteLock();
                }
                else
                {
                    LOG_FREE_TEXT( "Could not lock frame handler.")
                }
            }

            m_pImpl->m_frameHandlers.Vector.clear();
            
            // End exclusive write lock frame handler list
            m_pImpl->m_conditionHelper.ExitWriteLock( m_pImpl->m_frameHandlers );
        }
        else
        {
            LOG_FREE_TEXT( "Could not lock frame handler list.")
        }
    }

    return (VmbErrorType)res;
}

VmbErrorType Camera::QueueFrame( const FramePtr &frame )
{
    // HINT: The same frame cannot be queued twice (VmbErrorOther)
    VmbError_t res = VmbCaptureFrameQueue( GetHandle(), &(SP_ACCESS( frame )->m_pImpl->m_frame), FrameHandler::FrameDoneCallback );

    if (    VmbErrorSuccess == res
         && false == SP_ACCESS( frame )->m_pImpl->m_bAlreadyQueued )
    {
        if ( false == SP_ACCESS( frame )->m_pImpl->m_bAlreadyAnnounced )
        {
            // Begin write lock frame handler list
            if ( true == m_pImpl->m_conditionHelper.EnterWriteLock( m_pImpl->m_frameHandlers ))
            {
                m_pImpl->AppendFrameToVector( frame );
                SP_ACCESS( frame )->m_pImpl->m_bAlreadyQueued = true;
                
                // End write lock frame handler list
                m_pImpl->m_conditionHelper.ExitWriteLock( m_pImpl->m_frameHandlers );
            }
            else
            {
                LOG_FREE_TEXT( "Could not lock frame queue for appending frame." );
                res = VmbErrorResources;
            }
        }
    }

    return (VmbErrorType)res;
}

VmbErrorType Camera::FlushQueue()
{
    VmbError_t res = VmbCaptureQueueFlush( GetHandle() );

    if ( VmbErrorSuccess == res )
    {
        // Begin exclusive write lock frame handler list
        if ( true == m_pImpl->m_conditionHelper.EnterWriteLock( m_pImpl->m_frameHandlers, true ))
        {
            for (   FrameHandlerPtrVector::iterator iter = m_pImpl->m_frameHandlers.Vector.begin();
                    m_pImpl->m_frameHandlers.Vector.end() != iter;)
            {
                // Begin exclusive write lock of every single frame handler
                if ( true == SP_ACCESS(( *iter ))->EnterWriteLock( true ))
                {
                    // Dequeue frame
                    SP_ACCESS( SP_ACCESS(( *iter ))->GetFrame() )->m_pImpl->m_bAlreadyQueued = false;
                    if ( false == SP_ACCESS( SP_ACCESS(( *iter ))->GetFrame() )->m_pImpl->m_bAlreadyAnnounced )
                    {
                        // Delete frame if it was not announced / was revoked before
                        SP_ACCESS( SP_ACCESS(( *iter ))->GetFrame() )->m_pImpl->m_frame.context[FRAME_HDL] = NULL;
                        // End write lock frame handler
                        SP_ACCESS(( *iter ))->ExitWriteLock();
                        iter = m_pImpl->m_frameHandlers.Vector.erase( iter );
                    }
                    else
                    {
                        // End write lock frame handler
                        SP_ACCESS(( *iter ))->ExitWriteLock();
                        ++iter;
                    }
                }
                else
                {
                    LOG_FREE_TEXT( "Could not lock frame handler." );
                }
            }
            // End write lock frame handler list
            m_pImpl->m_conditionHelper.ExitWriteLock( m_pImpl->m_frameHandlers );
        }
        else
        {
            LOG_FREE_TEXT( "Could not lock frame handler list." )
        }
    }
    else
    {
        LOG_FREE_TEXT( "Could not flush frame queue" )
    }
    
    return (VmbErrorType)res;
}

VmbErrorType Camera::StartCapture() 
{
    return (VmbErrorType)VmbCaptureStart( GetHandle() );
}

VmbErrorType Camera::EndCapture() 
{
    VmbError_t res = VmbCaptureEnd( GetHandle() );

    if ( VmbErrorSuccess == res )
    {
        FlushQueue();
    }

    return (VmbErrorType)res;
}

void Camera::Impl::AppendFrameToVector( const FramePtr &rFrame )
{
    FrameHandlerPtr pFH( new FrameHandler( rFrame, SP_ACCESS( rFrame )->m_pImpl->m_pObserver ));
    SP_ACCESS( rFrame )->m_pImpl->m_frame.context[FRAME_HDL] = SP_ACCESS(pFH);    
    m_frameHandlers.Vector.push_back( pFH );
}

}} // namespace AVT::VmbAPI
