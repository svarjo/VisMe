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

#include "GT1290Camera.h"

GT1290Camera::GT1290Camera (
    const char         *pCameraID,
    const char         *pCameraName,
    const char         *pCameraModel,
    const char         *pCameraSerialNumber,
    const char         *pInterfaceID,
    VmbInterfaceType    interfaceType,
    const char         *pInterfaceName,
    const char         *pInterfaceSerialNumber,
    VmbAccessModeType   interfacePermittedAccess 
    )
    : Camera (
        pCameraID, 
        pCameraName, 
        pCameraModel, 
        pCameraSerialNumber, 
        pInterfaceID, 
        interfaceType)
{
}


// Category /Acquisition
VmbErrorType GT1290Camera::AcquisitionAbort ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionAbortFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType GT1290Camera::GetAcquisitionAbortFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionAbortFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("AcquisitionAbort", m_AcquisitionAbortFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionAbortFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionAbortFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetAcquisitionFrameCount (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetAcquisitionFrameCount (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetAcquisitionFrameCountFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionFrameCountFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("AcquisitionFrameCount", m_AcquisitionFrameCountFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionFrameCountFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionFrameCountFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetAcquisitionFrameRateAbs (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameRateAbsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetAcquisitionFrameRateAbs (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameRateAbsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetAcquisitionFrameRateAbsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionFrameRateAbsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("AcquisitionFrameRateAbs", m_AcquisitionFrameRateAbsFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionFrameRateAbsFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionFrameRateAbsFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetAcquisitionFrameRateLimit (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameRateLimitFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetAcquisitionFrameRateLimitFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionFrameRateLimitFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("AcquisitionFrameRateLimit", m_AcquisitionFrameRateLimitFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionFrameRateLimitFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionFrameRateLimitFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetAcquisitionMode (AcquisitionModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (AcquisitionModeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetAcquisitionMode (AcquisitionModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetAcquisitionModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("AcquisitionMode", m_AcquisitionModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionModeFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::AcquisitionStart ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionStartFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType GT1290Camera::GetAcquisitionStartFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionStartFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("AcquisitionStart", m_AcquisitionStartFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionStartFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionStartFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::AcquisitionStop ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionStopFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType GT1290Camera::GetAcquisitionStopFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionStopFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("AcquisitionStop", m_AcquisitionStopFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionStopFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionStopFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetRecorderPreEventCount (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetRecorderPreEventCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetRecorderPreEventCount (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetRecorderPreEventCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetRecorderPreEventCountFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_RecorderPreEventCountFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("RecorderPreEventCount", m_RecorderPreEventCountFeature);
        if (result != VmbErrorSuccess)
        {
            m_RecorderPreEventCountFeature.reset();
            return result;
        }
    }
    feature = m_RecorderPreEventCountFeature;
    return VmbErrorSuccess;
}


// Category /Acquisition/Trigger
VmbErrorType GT1290Camera::GetTriggerActivation (TriggerActivationEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerActivationFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TriggerActivationEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetTriggerActivation (TriggerActivationEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerActivationFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetTriggerActivationFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerActivationFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("TriggerActivation", m_TriggerActivationFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerActivationFeature.reset();
            return result;
        }
    }
    feature = m_TriggerActivationFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetTriggerDelayAbs (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerDelayAbsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetTriggerDelayAbs (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerDelayAbsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetTriggerDelayAbsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerDelayAbsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("TriggerDelayAbs", m_TriggerDelayAbsFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerDelayAbsFeature.reset();
            return result;
        }
    }
    feature = m_TriggerDelayAbsFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetTriggerMode (TriggerModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TriggerModeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetTriggerMode (TriggerModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetTriggerModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("TriggerMode", m_TriggerModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerModeFeature.reset();
            return result;
        }
    }
    feature = m_TriggerModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetTriggerOverlap (TriggerOverlapEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerOverlapFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TriggerOverlapEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetTriggerOverlap (TriggerOverlapEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerOverlapFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetTriggerOverlapFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerOverlapFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("TriggerOverlap", m_TriggerOverlapFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerOverlapFeature.reset();
            return result;
        }
    }
    feature = m_TriggerOverlapFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetTriggerSelector (TriggerSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TriggerSelectorEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetTriggerSelector (TriggerSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetTriggerSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("TriggerSelector", m_TriggerSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerSelectorFeature.reset();
            return result;
        }
    }
    feature = m_TriggerSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::TriggerSoftware ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerSoftwareFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType GT1290Camera::GetTriggerSoftwareFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerSoftwareFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("TriggerSoftware", m_TriggerSoftwareFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerSoftwareFeature.reset();
            return result;
        }
    }
    feature = m_TriggerSoftwareFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetTriggerSource (TriggerSourceEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerSourceFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TriggerSourceEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetTriggerSource (TriggerSourceEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerSourceFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetTriggerSourceFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerSourceFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("TriggerSource", m_TriggerSourceFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerSourceFeature.reset();
            return result;
        }
    }
    feature = m_TriggerSourceFeature;
    return VmbErrorSuccess;
}


// Category /Controls
VmbErrorType GT1290Camera::GetGamma (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGammaFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGamma (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGammaFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGammaFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GammaFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("Gamma", m_GammaFeature);
        if (result != VmbErrorSuccess)
        {
            m_GammaFeature.reset();
            return result;
        }
    }
    feature = m_GammaFeature;
    return VmbErrorSuccess;
}


// Category /Controls/BlackLevelControl
VmbErrorType GT1290Camera::GetBlackLevelSelector (BlackLevelSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBlackLevelSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (BlackLevelSelectorEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetBlackLevelSelector (BlackLevelSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBlackLevelSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetBlackLevelSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_BlackLevelSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("BlackLevelSelector", m_BlackLevelSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_BlackLevelSelectorFeature.reset();
            return result;
        }
    }
    feature = m_BlackLevelSelectorFeature;
    return VmbErrorSuccess;
}


// Category /Controls/DSPSubregion
VmbErrorType GT1290Camera::GetDSPSubregionBottom (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDSPSubregionBottomFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetDSPSubregionBottom (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDSPSubregionBottomFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDSPSubregionBottomFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DSPSubregionBottomFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DSPSubregionBottom", m_DSPSubregionBottomFeature);
        if (result != VmbErrorSuccess)
        {
            m_DSPSubregionBottomFeature.reset();
            return result;
        }
    }
    feature = m_DSPSubregionBottomFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetDSPSubregionLeft (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDSPSubregionLeftFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetDSPSubregionLeft (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDSPSubregionLeftFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDSPSubregionLeftFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DSPSubregionLeftFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DSPSubregionLeft", m_DSPSubregionLeftFeature);
        if (result != VmbErrorSuccess)
        {
            m_DSPSubregionLeftFeature.reset();
            return result;
        }
    }
    feature = m_DSPSubregionLeftFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetDSPSubregionRight (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDSPSubregionRightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetDSPSubregionRight (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDSPSubregionRightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDSPSubregionRightFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DSPSubregionRightFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DSPSubregionRight", m_DSPSubregionRightFeature);
        if (result != VmbErrorSuccess)
        {
            m_DSPSubregionRightFeature.reset();
            return result;
        }
    }
    feature = m_DSPSubregionRightFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetDSPSubregionTop (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDSPSubregionTopFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetDSPSubregionTop (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDSPSubregionTopFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDSPSubregionTopFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DSPSubregionTopFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DSPSubregionTop", m_DSPSubregionTopFeature);
        if (result != VmbErrorSuccess)
        {
            m_DSPSubregionTopFeature.reset();
            return result;
        }
    }
    feature = m_DSPSubregionTopFeature;
    return VmbErrorSuccess;
}


// Category /Controls/Exposure
VmbErrorType GT1290Camera::GetExposureAuto (ExposureAutoEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (ExposureAutoEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetExposureAuto (ExposureAutoEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureAutoFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureAuto", m_ExposureAutoFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetExposureMode (ExposureModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (ExposureModeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetExposureMode (ExposureModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureMode", m_ExposureModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureModeFeature.reset();
            return result;
        }
    }
    feature = m_ExposureModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetExposureTimeAbs (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureTimeAbsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetExposureTimeAbs (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureTimeAbsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureTimeAbsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureTimeAbsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureTimeAbs", m_ExposureTimeAbsFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureTimeAbsFeature.reset();
            return result;
        }
    }
    feature = m_ExposureTimeAbsFeature;
    return VmbErrorSuccess;
}


// Category /Controls/Exposure/ExposureAutoControl
VmbErrorType GT1290Camera::GetExposureAutoAdjustTol (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoAdjustTolFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetExposureAutoAdjustTol (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoAdjustTolFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureAutoAdjustTolFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoAdjustTolFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureAutoAdjustTol", m_ExposureAutoAdjustTolFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoAdjustTolFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoAdjustTolFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetExposureAutoAlg (ExposureAutoAlgEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoAlgFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (ExposureAutoAlgEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetExposureAutoAlg (ExposureAutoAlgEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoAlgFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureAutoAlgFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoAlgFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureAutoAlg", m_ExposureAutoAlgFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoAlgFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoAlgFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetExposureAutoMax (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetExposureAutoMax (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureAutoMaxFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoMaxFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureAutoMax", m_ExposureAutoMaxFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoMaxFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoMaxFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetExposureAutoMin (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoMinFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetExposureAutoMin (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoMinFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureAutoMinFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoMinFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureAutoMin", m_ExposureAutoMinFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoMinFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoMinFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetExposureAutoOutliers (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoOutliersFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetExposureAutoOutliers (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoOutliersFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureAutoOutliersFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoOutliersFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureAutoOutliers", m_ExposureAutoOutliersFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoOutliersFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoOutliersFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetExposureAutoRate (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetExposureAutoRate (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureAutoRateFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoRateFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureAutoRate", m_ExposureAutoRateFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoRateFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoRateFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetExposureAutoTarget (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoTargetFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetExposureAutoTarget (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoTargetFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetExposureAutoTargetFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoTargetFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ExposureAutoTarget", m_ExposureAutoTargetFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoTargetFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoTargetFeature;
    return VmbErrorSuccess;
}


// Category /Controls/GainControl
VmbErrorType GT1290Camera::GetGain (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGain (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGainFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("Gain", m_GainFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainFeature.reset();
            return result;
        }
    }
    feature = m_GainFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGainAuto (GainAutoEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (GainAutoEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetGainAuto (GainAutoEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGainAutoFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainAutoFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GainAuto", m_GainAutoFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainAutoFeature.reset();
            return result;
        }
    }
    feature = m_GainAutoFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGainSelector (GainSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (GainSelectorEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetGainSelector (GainSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGainSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GainSelector", m_GainSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainSelectorFeature.reset();
            return result;
        }
    }
    feature = m_GainSelectorFeature;
    return VmbErrorSuccess;
}


// Category /Controls/GainControl/GainAutoControl
VmbErrorType GT1290Camera::GetGainAutoAdjustTol (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoAdjustTolFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGainAutoAdjustTol (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoAdjustTolFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGainAutoAdjustTolFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainAutoAdjustTolFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GainAutoAdjustTol", m_GainAutoAdjustTolFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainAutoAdjustTolFeature.reset();
            return result;
        }
    }
    feature = m_GainAutoAdjustTolFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGainAutoMax (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGainAutoMax (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGainAutoMaxFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainAutoMaxFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GainAutoMax", m_GainAutoMaxFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainAutoMaxFeature.reset();
            return result;
        }
    }
    feature = m_GainAutoMaxFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGainAutoMin (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoMinFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGainAutoMin (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoMinFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGainAutoMinFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainAutoMinFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GainAutoMin", m_GainAutoMinFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainAutoMinFeature.reset();
            return result;
        }
    }
    feature = m_GainAutoMinFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGainAutoOutliers (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoOutliersFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGainAutoOutliers (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoOutliersFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGainAutoOutliersFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainAutoOutliersFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GainAutoOutliers", m_GainAutoOutliersFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainAutoOutliersFeature.reset();
            return result;
        }
    }
    feature = m_GainAutoOutliersFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGainAutoRate (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGainAutoRate (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGainAutoRateFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainAutoRateFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GainAutoRate", m_GainAutoRateFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainAutoRateFeature.reset();
            return result;
        }
    }
    feature = m_GainAutoRateFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGainAutoTarget (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoTargetFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGainAutoTarget (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoTargetFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGainAutoTargetFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainAutoTargetFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GainAutoTarget", m_GainAutoTargetFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainAutoTargetFeature.reset();
            return result;
        }
    }
    feature = m_GainAutoTargetFeature;
    return VmbErrorSuccess;
}


// Category /Controls/Iris
VmbErrorType GT1290Camera::GetIrisAutoTarget (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIrisAutoTargetFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetIrisAutoTarget (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIrisAutoTargetFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetIrisAutoTargetFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IrisAutoTargetFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("IrisAutoTarget", m_IrisAutoTargetFeature);
        if (result != VmbErrorSuccess)
        {
            m_IrisAutoTargetFeature.reset();
            return result;
        }
    }
    feature = m_IrisAutoTargetFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetIrisMode (IrisModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIrisModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (IrisModeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetIrisMode (IrisModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIrisModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetIrisModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IrisModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("IrisMode", m_IrisModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_IrisModeFeature.reset();
            return result;
        }
    }
    feature = m_IrisModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetIrisVideoLevel (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIrisVideoLevelFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetIrisVideoLevelFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IrisVideoLevelFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("IrisVideoLevel", m_IrisVideoLevelFeature);
        if (result != VmbErrorSuccess)
        {
            m_IrisVideoLevelFeature.reset();
            return result;
        }
    }
    feature = m_IrisVideoLevelFeature;
    return VmbErrorSuccess;
}


// Category /Controls/Iris/LensDCIris
VmbErrorType GT1290Camera::GetLensDCDriveStrength (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLensDCDriveStrengthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetLensDCDriveStrength (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLensDCDriveStrengthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetLensDCDriveStrengthFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LensDCDriveStrengthFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("LensDCDriveStrength", m_LensDCDriveStrengthFeature);
        if (result != VmbErrorSuccess)
        {
            m_LensDCDriveStrengthFeature.reset();
            return result;
        }
    }
    feature = m_LensDCDriveStrengthFeature;
    return VmbErrorSuccess;
}


// Category /Controls/Iris/LensPIris
VmbErrorType GT1290Camera::GetLensPIrisFrequency (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLensPIrisFrequencyFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetLensPIrisFrequency (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLensPIrisFrequencyFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetLensPIrisFrequencyFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LensPIrisFrequencyFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("LensPIrisFrequency", m_LensPIrisFrequencyFeature);
        if (result != VmbErrorSuccess)
        {
            m_LensPIrisFrequencyFeature.reset();
            return result;
        }
    }
    feature = m_LensPIrisFrequencyFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetLensPIrisNumSteps (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLensPIrisNumStepsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetLensPIrisNumSteps (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLensPIrisNumStepsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetLensPIrisNumStepsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LensPIrisNumStepsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("LensPIrisNumSteps", m_LensPIrisNumStepsFeature);
        if (result != VmbErrorSuccess)
        {
            m_LensPIrisNumStepsFeature.reset();
            return result;
        }
    }
    feature = m_LensPIrisNumStepsFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetLensPIrisPosition (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLensPIrisPositionFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetLensPIrisPosition (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLensPIrisPositionFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetLensPIrisPositionFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LensPIrisPositionFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("LensPIrisPosition", m_LensPIrisPositionFeature);
        if (result != VmbErrorSuccess)
        {
            m_LensPIrisPositionFeature.reset();
            return result;
        }
    }
    feature = m_LensPIrisPositionFeature;
    return VmbErrorSuccess;
}


// Category /Controls/SubstrateVoltage
VmbErrorType GT1290Camera::GetVsubValue (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetVsubValueFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetVsubValueFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_VsubValueFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("VsubValue", m_VsubValueFeature);
        if (result != VmbErrorSuccess)
        {
            m_VsubValueFeature.reset();
            return result;
        }
    }
    feature = m_VsubValueFeature;
    return VmbErrorSuccess;
}


// Category /DeviceStatus
VmbErrorType GT1290Camera::GetDeviceTemperature (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceTemperatureFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDeviceTemperatureFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceTemperatureFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DeviceTemperature", m_DeviceTemperatureFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceTemperatureFeature.reset();
            return result;
        }
    }
    feature = m_DeviceTemperatureFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetDeviceTemperatureSelector (DeviceTemperatureSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceTemperatureSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (DeviceTemperatureSelectorEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetDeviceTemperatureSelector (DeviceTemperatureSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceTemperatureSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDeviceTemperatureSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceTemperatureSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DeviceTemperatureSelector", m_DeviceTemperatureSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceTemperatureSelectorFeature.reset();
            return result;
        }
    }
    feature = m_DeviceTemperatureSelectorFeature;
    return VmbErrorSuccess;
}


// Category /EventControl
VmbErrorType GT1290Camera::GetEventNotification (EventNotificationEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventNotificationFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (EventNotificationEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetEventNotification (EventNotificationEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventNotificationFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventNotificationFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventNotificationFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventNotification", m_EventNotificationFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventNotificationFeature.reset();
            return result;
        }
    }
    feature = m_EventNotificationFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventSelector (EventSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (EventSelectorEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetEventSelector (EventSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventSelector", m_EventSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventSelectorFeature.reset();
            return result;
        }
    }
    feature = m_EventSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventsEnable1 (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventsEnable1Feature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetEventsEnable1 (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventsEnable1Feature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventsEnable1Feature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventsEnable1Feature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventsEnable1", m_EventsEnable1Feature);
        if (result != VmbErrorSuccess)
        {
            m_EventsEnable1Feature.reset();
            return result;
        }
    }
    feature = m_EventsEnable1Feature;
    return VmbErrorSuccess;
}


// Category /EventControl/EventData
VmbErrorType GT1290Camera::GetEventLine2FallingEdgeFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine2FallingEdgeFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine2FallingEdgeFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine2FallingEdgeFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine2FallingEdgeFrameID", m_EventLine2FallingEdgeFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine2FallingEdgeFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventLine2FallingEdgeFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine2FallingEdgeTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine2FallingEdgeTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine2FallingEdgeTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine2FallingEdgeTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine2FallingEdgeTimestamp", m_EventLine2FallingEdgeTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine2FallingEdgeTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventLine2FallingEdgeTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine2RisingEdgeFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine2RisingEdgeFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine2RisingEdgeFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine2RisingEdgeFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine2RisingEdgeFrameID", m_EventLine2RisingEdgeFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine2RisingEdgeFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventLine2RisingEdgeFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine2RisingEdgeTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine2RisingEdgeTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine2RisingEdgeTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine2RisingEdgeTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine2RisingEdgeTimestamp", m_EventLine2RisingEdgeTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine2RisingEdgeTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventLine2RisingEdgeTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine3FallingEdgeFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine3FallingEdgeFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine3FallingEdgeFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine3FallingEdgeFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine3FallingEdgeFrameID", m_EventLine3FallingEdgeFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine3FallingEdgeFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventLine3FallingEdgeFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine3FallingEdgeTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine3FallingEdgeTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine3FallingEdgeTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine3FallingEdgeTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine3FallingEdgeTimestamp", m_EventLine3FallingEdgeTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine3FallingEdgeTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventLine3FallingEdgeTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine3RisingEdgeFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine3RisingEdgeFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine3RisingEdgeFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine3RisingEdgeFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine3RisingEdgeFrameID", m_EventLine3RisingEdgeFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine3RisingEdgeFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventLine3RisingEdgeFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine3RisingEdgeTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine3RisingEdgeTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine3RisingEdgeTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine3RisingEdgeTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine3RisingEdgeTimestamp", m_EventLine3RisingEdgeTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine3RisingEdgeTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventLine3RisingEdgeTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine4FallingEdgeFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine4FallingEdgeFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine4FallingEdgeFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine4FallingEdgeFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine4FallingEdgeFrameID", m_EventLine4FallingEdgeFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine4FallingEdgeFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventLine4FallingEdgeFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine4FallingEdgeTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine4FallingEdgeTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine4FallingEdgeTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine4FallingEdgeTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine4FallingEdgeTimestamp", m_EventLine4FallingEdgeTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine4FallingEdgeTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventLine4FallingEdgeTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine4RisingEdgeFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine4RisingEdgeFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine4RisingEdgeFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine4RisingEdgeFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine4RisingEdgeFrameID", m_EventLine4RisingEdgeFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine4RisingEdgeFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventLine4RisingEdgeFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine4RisingEdgeTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine4RisingEdgeTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine4RisingEdgeTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine4RisingEdgeTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine4RisingEdgeTimestamp", m_EventLine4RisingEdgeTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine4RisingEdgeTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventLine4RisingEdgeTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventOverflowFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventOverflowFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventOverflowFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventOverflowFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventOverflowFrameID", m_EventOverflowFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventOverflowFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventOverflowFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventOverflowTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventOverflowTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventOverflowTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventOverflowTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventOverflowTimestamp", m_EventOverflowTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventOverflowTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventOverflowTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventPtpSyncLockedFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventPtpSyncLockedFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventPtpSyncLockedFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventPtpSyncLockedFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventPtpSyncLockedFrameID", m_EventPtpSyncLockedFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventPtpSyncLockedFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventPtpSyncLockedFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventPtpSyncLockedTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventPtpSyncLockedTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventPtpSyncLockedTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventPtpSyncLockedTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventPtpSyncLockedTimestamp", m_EventPtpSyncLockedTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventPtpSyncLockedTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventPtpSyncLockedTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventPtpSyncLostFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventPtpSyncLostFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventPtpSyncLostFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventPtpSyncLostFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventPtpSyncLostFrameID", m_EventPtpSyncLostFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventPtpSyncLostFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventPtpSyncLostFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventPtpSyncLostTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventPtpSyncLostTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventPtpSyncLostTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventPtpSyncLostTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventPtpSyncLostTimestamp", m_EventPtpSyncLostTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventPtpSyncLostTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventPtpSyncLostTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventAcquisitionEndFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventAcquisitionEndFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventAcquisitionEndFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventAcquisitionEndFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventAcquisitionEndFrameID", m_EventAcquisitionEndFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventAcquisitionEndFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventAcquisitionEndFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventAcquisitionEndTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventAcquisitionEndTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventAcquisitionEndTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventAcquisitionEndTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventAcquisitionEndTimestamp", m_EventAcquisitionEndTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventAcquisitionEndTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventAcquisitionEndTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventAcquisitionRecordTriggerFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventAcquisitionRecordTriggerFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventAcquisitionRecordTriggerFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventAcquisitionRecordTriggerFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventAcquisitionRecordTriggerFrameID", m_EventAcquisitionRecordTriggerFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventAcquisitionRecordTriggerFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventAcquisitionRecordTriggerFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventAcquisitionRecordTriggerTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventAcquisitionRecordTriggerTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventAcquisitionRecordTriggerTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventAcquisitionRecordTriggerTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventAcquisitionRecordTriggerTimestamp", m_EventAcquisitionRecordTriggerTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventAcquisitionRecordTriggerTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventAcquisitionRecordTriggerTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventAcquisitionStartFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventAcquisitionStartFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventAcquisitionStartFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventAcquisitionStartFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventAcquisitionStartFrameID", m_EventAcquisitionStartFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventAcquisitionStartFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventAcquisitionStartFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventAcquisitionStartTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventAcquisitionStartTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventAcquisitionStartTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventAcquisitionStartTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventAcquisitionStartTimestamp", m_EventAcquisitionStartTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventAcquisitionStartTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventAcquisitionStartTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventErrorFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventErrorFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventErrorFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventErrorFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventErrorFrameID", m_EventErrorFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventErrorFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventErrorFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventErrorTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventErrorTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventErrorTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventErrorTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventErrorTimestamp", m_EventErrorTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventErrorTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventErrorTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventExposureEndFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventExposureEndFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventExposureEndFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventExposureEndFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventExposureEndFrameID", m_EventExposureEndFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventExposureEndFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventExposureEndFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventExposureEndTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventExposureEndTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventExposureEndTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventExposureEndTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventExposureEndTimestamp", m_EventExposureEndTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventExposureEndTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventExposureEndTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventFrameTriggerFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventFrameTriggerFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventFrameTriggerFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventFrameTriggerFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventFrameTriggerFrameID", m_EventFrameTriggerFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventFrameTriggerFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventFrameTriggerFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventFrameTriggerReadyFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventFrameTriggerReadyFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventFrameTriggerReadyFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventFrameTriggerReadyFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventFrameTriggerReadyFrameID", m_EventFrameTriggerReadyFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventFrameTriggerReadyFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventFrameTriggerReadyFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventFrameTriggerReadyTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventFrameTriggerReadyTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventFrameTriggerReadyTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventFrameTriggerReadyTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventFrameTriggerReadyTimestamp", m_EventFrameTriggerReadyTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventFrameTriggerReadyTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventFrameTriggerReadyTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventFrameTriggerTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventFrameTriggerTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventFrameTriggerTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventFrameTriggerTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventFrameTriggerTimestamp", m_EventFrameTriggerTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventFrameTriggerTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventFrameTriggerTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine1FallingEdgeFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine1FallingEdgeFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine1FallingEdgeFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine1FallingEdgeFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine1FallingEdgeFrameID", m_EventLine1FallingEdgeFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine1FallingEdgeFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventLine1FallingEdgeFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine1FallingEdgeTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine1FallingEdgeTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine1FallingEdgeTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine1FallingEdgeTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine1FallingEdgeTimestamp", m_EventLine1FallingEdgeTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine1FallingEdgeTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventLine1FallingEdgeTimestampFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine1RisingEdgeFrameID (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine1RisingEdgeFrameIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine1RisingEdgeFrameIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine1RisingEdgeFrameIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine1RisingEdgeFrameID", m_EventLine1RisingEdgeFrameIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine1RisingEdgeFrameIDFeature.reset();
            return result;
        }
    }
    feature = m_EventLine1RisingEdgeFrameIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine1RisingEdgeTimestamp (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine1RisingEdgeTimestampFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine1RisingEdgeTimestampFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine1RisingEdgeTimestampFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine1RisingEdgeTimestamp", m_EventLine1RisingEdgeTimestampFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine1RisingEdgeTimestampFeature.reset();
            return result;
        }
    }
    feature = m_EventLine1RisingEdgeTimestampFeature;
    return VmbErrorSuccess;
}


// Category /EventControl/EventID
VmbErrorType GT1290Camera::GetEventPtpSyncLost (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventPtpSyncLostFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventPtpSyncLostFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventPtpSyncLostFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventPtpSyncLost", m_EventPtpSyncLostFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventPtpSyncLostFeature.reset();
            return result;
        }
    }
    feature = m_EventPtpSyncLostFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventPtpSyncLocked (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventPtpSyncLockedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventPtpSyncLockedFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventPtpSyncLockedFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventPtpSyncLocked", m_EventPtpSyncLockedFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventPtpSyncLockedFeature.reset();
            return result;
        }
    }
    feature = m_EventPtpSyncLockedFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventOverflow (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventOverflowFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventOverflowFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventOverflowFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventOverflow", m_EventOverflowFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventOverflowFeature.reset();
            return result;
        }
    }
    feature = m_EventOverflowFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine4RisingEdge (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine4RisingEdgeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine4RisingEdgeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine4RisingEdgeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine4RisingEdge", m_EventLine4RisingEdgeFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine4RisingEdgeFeature.reset();
            return result;
        }
    }
    feature = m_EventLine4RisingEdgeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine4FallingEdge (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine4FallingEdgeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine4FallingEdgeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine4FallingEdgeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine4FallingEdge", m_EventLine4FallingEdgeFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine4FallingEdgeFeature.reset();
            return result;
        }
    }
    feature = m_EventLine4FallingEdgeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine3RisingEdge (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine3RisingEdgeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine3RisingEdgeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine3RisingEdgeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine3RisingEdge", m_EventLine3RisingEdgeFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine3RisingEdgeFeature.reset();
            return result;
        }
    }
    feature = m_EventLine3RisingEdgeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine3FallingEdge (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine3FallingEdgeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine3FallingEdgeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine3FallingEdgeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine3FallingEdge", m_EventLine3FallingEdgeFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine3FallingEdgeFeature.reset();
            return result;
        }
    }
    feature = m_EventLine3FallingEdgeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine2RisingEdge (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine2RisingEdgeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine2RisingEdgeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine2RisingEdgeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine2RisingEdge", m_EventLine2RisingEdgeFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine2RisingEdgeFeature.reset();
            return result;
        }
    }
    feature = m_EventLine2RisingEdgeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine2FallingEdge (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine2FallingEdgeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine2FallingEdgeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine2FallingEdgeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine2FallingEdge", m_EventLine2FallingEdgeFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine2FallingEdgeFeature.reset();
            return result;
        }
    }
    feature = m_EventLine2FallingEdgeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine1RisingEdge (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine1RisingEdgeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine1RisingEdgeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine1RisingEdgeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine1RisingEdge", m_EventLine1RisingEdgeFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine1RisingEdgeFeature.reset();
            return result;
        }
    }
    feature = m_EventLine1RisingEdgeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventLine1FallingEdge (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventLine1FallingEdgeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventLine1FallingEdgeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventLine1FallingEdgeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventLine1FallingEdge", m_EventLine1FallingEdgeFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventLine1FallingEdgeFeature.reset();
            return result;
        }
    }
    feature = m_EventLine1FallingEdgeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventFrameTriggerReady (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventFrameTriggerReadyFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventFrameTriggerReadyFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventFrameTriggerReadyFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventFrameTriggerReady", m_EventFrameTriggerReadyFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventFrameTriggerReadyFeature.reset();
            return result;
        }
    }
    feature = m_EventFrameTriggerReadyFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventFrameTrigger (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventFrameTriggerFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventFrameTriggerFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventFrameTriggerFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventFrameTrigger", m_EventFrameTriggerFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventFrameTriggerFeature.reset();
            return result;
        }
    }
    feature = m_EventFrameTriggerFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventExposureEnd (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventExposureEndFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventExposureEndFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventExposureEndFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventExposureEnd", m_EventExposureEndFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventExposureEndFeature.reset();
            return result;
        }
    }
    feature = m_EventExposureEndFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventError (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventErrorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventErrorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventErrorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventError", m_EventErrorFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventErrorFeature.reset();
            return result;
        }
    }
    feature = m_EventErrorFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventAcquisitionStart (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventAcquisitionStartFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventAcquisitionStartFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventAcquisitionStartFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventAcquisitionStart", m_EventAcquisitionStartFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventAcquisitionStartFeature.reset();
            return result;
        }
    }
    feature = m_EventAcquisitionStartFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventAcquisitionRecordTrigger (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventAcquisitionRecordTriggerFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventAcquisitionRecordTriggerFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventAcquisitionRecordTriggerFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventAcquisitionRecordTrigger", m_EventAcquisitionRecordTriggerFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventAcquisitionRecordTriggerFeature.reset();
            return result;
        }
    }
    feature = m_EventAcquisitionRecordTriggerFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetEventAcquisitionEnd (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetEventAcquisitionEndFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetEventAcquisitionEndFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_EventAcquisitionEndFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("EventAcquisitionEnd", m_EventAcquisitionEndFeature);
        if (result != VmbErrorSuccess)
        {
            m_EventAcquisitionEndFeature.reset();
            return result;
        }
    }
    feature = m_EventAcquisitionEndFeature;
    return VmbErrorSuccess;
}


// Category /GigE
VmbErrorType GT1290Camera::GetBandwidthControlMode (BandwidthControlModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBandwidthControlModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (BandwidthControlModeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetBandwidthControlMode (BandwidthControlModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBandwidthControlModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetBandwidthControlModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_BandwidthControlModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("BandwidthControlMode", m_BandwidthControlModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_BandwidthControlModeFeature.reset();
            return result;
        }
    }
    feature = m_BandwidthControlModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetChunkModeActive (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetChunkModeActiveFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetChunkModeActive (bool value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetChunkModeActiveFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetChunkModeActiveFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ChunkModeActiveFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ChunkModeActive", m_ChunkModeActiveFeature);
        if (result != VmbErrorSuccess)
        {
            m_ChunkModeActiveFeature.reset();
            return result;
        }
    }
    feature = m_ChunkModeActiveFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGevDeviceMACAddress (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevDeviceMACAddressFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevDeviceMACAddressFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevDeviceMACAddressFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevDeviceMACAddress", m_GevDeviceMACAddressFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevDeviceMACAddressFeature.reset();
            return result;
        }
    }
    feature = m_GevDeviceMACAddressFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGevSCPSPacketSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevSCPSPacketSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGevSCPSPacketSize (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevSCPSPacketSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevSCPSPacketSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevSCPSPacketSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevSCPSPacketSize", m_GevSCPSPacketSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevSCPSPacketSizeFeature.reset();
            return result;
        }
    }
    feature = m_GevSCPSPacketSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetNonImagePayloadSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetNonImagePayloadSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetNonImagePayloadSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_NonImagePayloadSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("NonImagePayloadSize", m_NonImagePayloadSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_NonImagePayloadSizeFeature.reset();
            return result;
        }
    }
    feature = m_NonImagePayloadSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetPayloadSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPayloadSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetPayloadSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_PayloadSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("PayloadSize", m_PayloadSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_PayloadSizeFeature.reset();
            return result;
        }
    }
    feature = m_PayloadSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStreamBytesPerSecond (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamBytesPerSecondFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetStreamBytesPerSecond (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamBytesPerSecondFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStreamBytesPerSecondFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamBytesPerSecondFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StreamBytesPerSecond", m_StreamBytesPerSecondFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamBytesPerSecondFeature.reset();
            return result;
        }
    }
    feature = m_StreamBytesPerSecondFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStreamFrameRateConstrain (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamFrameRateConstrainFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetStreamFrameRateConstrain (bool value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamFrameRateConstrainFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStreamFrameRateConstrainFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamFrameRateConstrainFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StreamFrameRateConstrain", m_StreamFrameRateConstrainFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamFrameRateConstrainFeature.reset();
            return result;
        }
    }
    feature = m_StreamFrameRateConstrainFeature;
    return VmbErrorSuccess;
}


// Category /GigE/Configuration
VmbErrorType GT1290Camera::GetGevIPConfigurationMode (GevIPConfigurationModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevIPConfigurationModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (GevIPConfigurationModeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::GetGevIPConfigurationModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevIPConfigurationModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevIPConfigurationMode", m_GevIPConfigurationModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevIPConfigurationModeFeature.reset();
            return result;
        }
    }
    feature = m_GevIPConfigurationModeFeature;
    return VmbErrorSuccess;
}


// Category /GigE/Current
VmbErrorType GT1290Camera::GetGevCurrentDefaultGateway (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevCurrentDefaultGatewayFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevCurrentDefaultGatewayFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevCurrentDefaultGatewayFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevCurrentDefaultGateway", m_GevCurrentDefaultGatewayFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevCurrentDefaultGatewayFeature.reset();
            return result;
        }
    }
    feature = m_GevCurrentDefaultGatewayFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGevCurrentIPAddress (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevCurrentIPAddressFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevCurrentIPAddressFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevCurrentIPAddressFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevCurrentIPAddress", m_GevCurrentIPAddressFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevCurrentIPAddressFeature.reset();
            return result;
        }
    }
    feature = m_GevCurrentIPAddressFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGevCurrentSubnetMask (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevCurrentSubnetMaskFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevCurrentSubnetMaskFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevCurrentSubnetMaskFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevCurrentSubnetMask", m_GevCurrentSubnetMaskFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevCurrentSubnetMaskFeature.reset();
            return result;
        }
    }
    feature = m_GevCurrentSubnetMaskFeature;
    return VmbErrorSuccess;
}


// Category /GigE/GVCP
VmbErrorType GT1290Camera::GetGVCPCmdRetries (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVCPCmdRetriesFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVCPCmdRetries (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVCPCmdRetriesFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVCPCmdRetriesFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVCPCmdRetriesFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVCPCmdRetries", m_GVCPCmdRetriesFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVCPCmdRetriesFeature.reset();
            return result;
        }
    }
    feature = m_GVCPCmdRetriesFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVCPCmdTimeout (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVCPCmdTimeoutFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVCPCmdTimeout (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVCPCmdTimeoutFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVCPCmdTimeoutFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVCPCmdTimeoutFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVCPCmdTimeout", m_GVCPCmdTimeoutFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVCPCmdTimeoutFeature.reset();
            return result;
        }
    }
    feature = m_GVCPCmdTimeoutFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVCPHBInterval (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVCPHBIntervalFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVCPHBInterval (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVCPHBIntervalFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVCPHBIntervalFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVCPHBIntervalFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVCPHBInterval", m_GVCPHBIntervalFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVCPHBIntervalFeature.reset();
            return result;
        }
    }
    feature = m_GVCPHBIntervalFeature;
    return VmbErrorSuccess;
}


// Category /GigE/PTP
VmbErrorType GT1290Camera::GetPtpAcquisitionGateTime (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPtpAcquisitionGateTimeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetPtpAcquisitionGateTime (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPtpAcquisitionGateTimeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetPtpAcquisitionGateTimeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_PtpAcquisitionGateTimeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("PtpAcquisitionGateTime", m_PtpAcquisitionGateTimeFeature);
        if (result != VmbErrorSuccess)
        {
            m_PtpAcquisitionGateTimeFeature.reset();
            return result;
        }
    }
    feature = m_PtpAcquisitionGateTimeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetPtpMode (PtpModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPtpModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (PtpModeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetPtpMode (PtpModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPtpModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetPtpModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_PtpModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("PtpMode", m_PtpModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_PtpModeFeature.reset();
            return result;
        }
    }
    feature = m_PtpModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetPtpStatus (PtpStatusEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPtpStatusFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (PtpStatusEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::GetPtpStatusFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_PtpStatusFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("PtpStatus", m_PtpStatusFeature);
        if (result != VmbErrorSuccess)
        {
            m_PtpStatusFeature.reset();
            return result;
        }
    }
    feature = m_PtpStatusFeature;
    return VmbErrorSuccess;
}


// Category /GigE/Persistent
VmbErrorType GT1290Camera::GetGevPersistentDefaultGateway (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevPersistentDefaultGatewayFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevPersistentDefaultGatewayFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevPersistentDefaultGatewayFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevPersistentDefaultGateway", m_GevPersistentDefaultGatewayFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevPersistentDefaultGatewayFeature.reset();
            return result;
        }
    }
    feature = m_GevPersistentDefaultGatewayFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGevPersistentIPAddress (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevPersistentIPAddressFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevPersistentIPAddressFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevPersistentIPAddressFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevPersistentIPAddress", m_GevPersistentIPAddressFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevPersistentIPAddressFeature.reset();
            return result;
        }
    }
    feature = m_GevPersistentIPAddressFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGevPersistentSubnetMask (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevPersistentSubnetMaskFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevPersistentSubnetMaskFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevPersistentSubnetMaskFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevPersistentSubnetMask", m_GevPersistentSubnetMaskFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevPersistentSubnetMaskFeature.reset();
            return result;
        }
    }
    feature = m_GevPersistentSubnetMaskFeature;
    return VmbErrorSuccess;
}


// Category /GigE/StreamHold
VmbErrorType GT1290Camera::GetStreamHoldCapacity (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamHoldCapacityFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStreamHoldCapacityFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamHoldCapacityFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StreamHoldCapacity", m_StreamHoldCapacityFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamHoldCapacityFeature.reset();
            return result;
        }
    }
    feature = m_StreamHoldCapacityFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStreamHoldEnable (StreamHoldEnableEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamHoldEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (StreamHoldEnableEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetStreamHoldEnable (StreamHoldEnableEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamHoldEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStreamHoldEnableFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamHoldEnableFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StreamHoldEnable", m_StreamHoldEnableFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamHoldEnableFeature.reset();
            return result;
        }
    }
    feature = m_StreamHoldEnableFeature;
    return VmbErrorSuccess;
}


// Category /GigE/Timestamp
VmbErrorType GT1290Camera::GevTimestampControlLatch ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevTimestampControlLatchFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType GT1290Camera::GetGevTimestampControlLatchFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevTimestampControlLatchFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevTimestampControlLatch", m_GevTimestampControlLatchFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevTimestampControlLatchFeature.reset();
            return result;
        }
    }
    feature = m_GevTimestampControlLatchFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GevTimestampControlReset ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevTimestampControlResetFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType GT1290Camera::GetGevTimestampControlResetFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevTimestampControlResetFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevTimestampControlReset", m_GevTimestampControlResetFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevTimestampControlResetFeature.reset();
            return result;
        }
    }
    feature = m_GevTimestampControlResetFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGevTimestampTickFrequency (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevTimestampTickFrequencyFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevTimestampTickFrequencyFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevTimestampTickFrequencyFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevTimestampTickFrequency", m_GevTimestampTickFrequencyFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevTimestampTickFrequencyFeature.reset();
            return result;
        }
    }
    feature = m_GevTimestampTickFrequencyFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGevTimestampValue (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGevTimestampValueFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGevTimestampValueFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GevTimestampValueFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GevTimestampValue", m_GevTimestampValueFeature);
        if (result != VmbErrorSuccess)
        {
            m_GevTimestampValueFeature.reset();
            return result;
        }
    }
    feature = m_GevTimestampValueFeature;
    return VmbErrorSuccess;
}


// Category /IO/Strobe
VmbErrorType GT1290Camera::GetStrobeDelay (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStrobeDelayFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetStrobeDelay (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStrobeDelayFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStrobeDelayFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StrobeDelayFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StrobeDelay", m_StrobeDelayFeature);
        if (result != VmbErrorSuccess)
        {
            m_StrobeDelayFeature.reset();
            return result;
        }
    }
    feature = m_StrobeDelayFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStrobeDuration (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStrobeDurationFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetStrobeDuration (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStrobeDurationFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStrobeDurationFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StrobeDurationFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StrobeDuration", m_StrobeDurationFeature);
        if (result != VmbErrorSuccess)
        {
            m_StrobeDurationFeature.reset();
            return result;
        }
    }
    feature = m_StrobeDurationFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStrobeDurationMode (StrobeDurationModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStrobeDurationModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (StrobeDurationModeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetStrobeDurationMode (StrobeDurationModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStrobeDurationModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStrobeDurationModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StrobeDurationModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StrobeDurationMode", m_StrobeDurationModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_StrobeDurationModeFeature.reset();
            return result;
        }
    }
    feature = m_StrobeDurationModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStrobeSource (StrobeSourceEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStrobeSourceFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (StrobeSourceEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetStrobeSource (StrobeSourceEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStrobeSourceFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStrobeSourceFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StrobeSourceFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StrobeSource", m_StrobeSourceFeature);
        if (result != VmbErrorSuccess)
        {
            m_StrobeSourceFeature.reset();
            return result;
        }
    }
    feature = m_StrobeSourceFeature;
    return VmbErrorSuccess;
}


// Category /IO/SyncIn
VmbErrorType GT1290Camera::GetSyncInGlitchFilter (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncInGlitchFilterFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetSyncInGlitchFilter (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncInGlitchFilterFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSyncInGlitchFilterFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SyncInGlitchFilterFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SyncInGlitchFilter", m_SyncInGlitchFilterFeature);
        if (result != VmbErrorSuccess)
        {
            m_SyncInGlitchFilterFeature.reset();
            return result;
        }
    }
    feature = m_SyncInGlitchFilterFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetSyncInLevels (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncInLevelsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSyncInLevelsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SyncInLevelsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SyncInLevels", m_SyncInLevelsFeature);
        if (result != VmbErrorSuccess)
        {
            m_SyncInLevelsFeature.reset();
            return result;
        }
    }
    feature = m_SyncInLevelsFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetSyncInSelector (SyncInSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncInSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SyncInSelectorEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetSyncInSelector (SyncInSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncInSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSyncInSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SyncInSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SyncInSelector", m_SyncInSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_SyncInSelectorFeature.reset();
            return result;
        }
    }
    feature = m_SyncInSelectorFeature;
    return VmbErrorSuccess;
}


// Category /IO/SyncOut
VmbErrorType GT1290Camera::GetSyncOutLevels (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncOutLevelsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetSyncOutLevels (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncOutLevelsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSyncOutLevelsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SyncOutLevelsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SyncOutLevels", m_SyncOutLevelsFeature);
        if (result != VmbErrorSuccess)
        {
            m_SyncOutLevelsFeature.reset();
            return result;
        }
    }
    feature = m_SyncOutLevelsFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetSyncOutPolarity (SyncOutPolarityEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncOutPolarityFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SyncOutPolarityEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetSyncOutPolarity (SyncOutPolarityEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncOutPolarityFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSyncOutPolarityFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SyncOutPolarityFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SyncOutPolarity", m_SyncOutPolarityFeature);
        if (result != VmbErrorSuccess)
        {
            m_SyncOutPolarityFeature.reset();
            return result;
        }
    }
    feature = m_SyncOutPolarityFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetSyncOutSelector (SyncOutSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncOutSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SyncOutSelectorEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetSyncOutSelector (SyncOutSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncOutSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSyncOutSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SyncOutSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SyncOutSelector", m_SyncOutSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_SyncOutSelectorFeature.reset();
            return result;
        }
    }
    feature = m_SyncOutSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetSyncOutSource (SyncOutSourceEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncOutSourceFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SyncOutSourceEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetSyncOutSource (SyncOutSourceEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSyncOutSourceFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSyncOutSourceFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SyncOutSourceFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SyncOutSource", m_SyncOutSourceFeature);
        if (result != VmbErrorSuccess)
        {
            m_SyncOutSourceFeature.reset();
            return result;
        }
    }
    feature = m_SyncOutSourceFeature;
    return VmbErrorSuccess;
}


// Category /ImageFormat
VmbErrorType GT1290Camera::GetHeight (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetHeightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetHeight (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetHeightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetHeightFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_HeightFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("Height", m_HeightFeature);
        if (result != VmbErrorSuccess)
        {
            m_HeightFeature.reset();
            return result;
        }
    }
    feature = m_HeightFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetHeightMax (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetHeightMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetHeightMaxFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_HeightMaxFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("HeightMax", m_HeightMaxFeature);
        if (result != VmbErrorSuccess)
        {
            m_HeightMaxFeature.reset();
            return result;
        }
    }
    feature = m_HeightMaxFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetImageSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetImageSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetImageSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ImageSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("ImageSize", m_ImageSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_ImageSizeFeature.reset();
            return result;
        }
    }
    feature = m_ImageSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetOffsetX (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetOffsetXFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetOffsetX (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetOffsetXFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetOffsetXFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_OffsetXFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("OffsetX", m_OffsetXFeature);
        if (result != VmbErrorSuccess)
        {
            m_OffsetXFeature.reset();
            return result;
        }
    }
    feature = m_OffsetXFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetOffsetY (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetOffsetYFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetOffsetY (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetOffsetYFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetOffsetYFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_OffsetYFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("OffsetY", m_OffsetYFeature);
        if (result != VmbErrorSuccess)
        {
            m_OffsetYFeature.reset();
            return result;
        }
    }
    feature = m_OffsetYFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetPixelFormat (PixelFormatEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPixelFormatFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (PixelFormatEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetPixelFormat (PixelFormatEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPixelFormatFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetPixelFormatFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_PixelFormatFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("PixelFormat", m_PixelFormatFeature);
        if (result != VmbErrorSuccess)
        {
            m_PixelFormatFeature.reset();
            return result;
        }
    }
    feature = m_PixelFormatFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetWidth (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetWidthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetWidth (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetWidthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetWidthFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_WidthFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("Width", m_WidthFeature);
        if (result != VmbErrorSuccess)
        {
            m_WidthFeature.reset();
            return result;
        }
    }
    feature = m_WidthFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetWidthMax (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetWidthMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetWidthMaxFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_WidthMaxFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("WidthMax", m_WidthMaxFeature);
        if (result != VmbErrorSuccess)
        {
            m_WidthMaxFeature.reset();
            return result;
        }
    }
    feature = m_WidthMaxFeature;
    return VmbErrorSuccess;
}


// Category /ImageMode
VmbErrorType GT1290Camera::GetBinningHorizontal (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBinningHorizontalFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetBinningHorizontal (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBinningHorizontalFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetBinningHorizontalFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_BinningHorizontalFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("BinningHorizontal", m_BinningHorizontalFeature);
        if (result != VmbErrorSuccess)
        {
            m_BinningHorizontalFeature.reset();
            return result;
        }
    }
    feature = m_BinningHorizontalFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetBinningVertical (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBinningVerticalFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetBinningVertical (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBinningVerticalFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetBinningVerticalFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_BinningVerticalFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("BinningVertical", m_BinningVerticalFeature);
        if (result != VmbErrorSuccess)
        {
            m_BinningVerticalFeature.reset();
            return result;
        }
    }
    feature = m_BinningVerticalFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetSensorHeight (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSensorHeightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSensorHeightFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SensorHeightFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SensorHeight", m_SensorHeightFeature);
        if (result != VmbErrorSuccess)
        {
            m_SensorHeightFeature.reset();
            return result;
        }
    }
    feature = m_SensorHeightFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetSensorWidth (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSensorWidthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSensorWidthFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SensorWidthFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SensorWidth", m_SensorWidthFeature);
        if (result != VmbErrorSuccess)
        {
            m_SensorWidthFeature.reset();
            return result;
        }
    }
    feature = m_SensorWidthFeature;
    return VmbErrorSuccess;
}


// Category /Info
VmbErrorType GT1290Camera::GetDeviceFirmwareVersion (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceFirmwareVersionFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDeviceFirmwareVersionFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceFirmwareVersionFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DeviceFirmwareVersion", m_DeviceFirmwareVersionFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceFirmwareVersionFeature.reset();
            return result;
        }
    }
    feature = m_DeviceFirmwareVersionFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetDeviceID (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDeviceIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DeviceID", m_DeviceIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceIDFeature.reset();
            return result;
        }
    }
    feature = m_DeviceIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetDeviceModelName (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceModelNameFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDeviceModelNameFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceModelNameFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DeviceModelName", m_DeviceModelNameFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceModelNameFeature.reset();
            return result;
        }
    }
    feature = m_DeviceModelNameFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetDevicePartNumber (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDevicePartNumberFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDevicePartNumberFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DevicePartNumberFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DevicePartNumber", m_DevicePartNumberFeature);
        if (result != VmbErrorSuccess)
        {
            m_DevicePartNumberFeature.reset();
            return result;
        }
    }
    feature = m_DevicePartNumberFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetDeviceScanType (DeviceScanTypeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceScanTypeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (DeviceScanTypeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::GetDeviceScanTypeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceScanTypeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DeviceScanType", m_DeviceScanTypeFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceScanTypeFeature.reset();
            return result;
        }
    }
    feature = m_DeviceScanTypeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetDeviceVendorName (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceVendorNameFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetDeviceVendorNameFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceVendorNameFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("DeviceVendorName", m_DeviceVendorNameFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceVendorNameFeature.reset();
            return result;
        }
    }
    feature = m_DeviceVendorNameFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetFirmwareVerBuild (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFirmwareVerBuildFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetFirmwareVerBuildFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FirmwareVerBuildFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("FirmwareVerBuild", m_FirmwareVerBuildFeature);
        if (result != VmbErrorSuccess)
        {
            m_FirmwareVerBuildFeature.reset();
            return result;
        }
    }
    feature = m_FirmwareVerBuildFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetFirmwareVerMajor (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFirmwareVerMajorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetFirmwareVerMajorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FirmwareVerMajorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("FirmwareVerMajor", m_FirmwareVerMajorFeature);
        if (result != VmbErrorSuccess)
        {
            m_FirmwareVerMajorFeature.reset();
            return result;
        }
    }
    feature = m_FirmwareVerMajorFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetFirmwareVerMinor (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFirmwareVerMinorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetFirmwareVerMinorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FirmwareVerMinorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("FirmwareVerMinor", m_FirmwareVerMinorFeature);
        if (result != VmbErrorSuccess)
        {
            m_FirmwareVerMinorFeature.reset();
            return result;
        }
    }
    feature = m_FirmwareVerMinorFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetSensorBits (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSensorBitsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetSensorBitsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SensorBitsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SensorBits", m_SensorBitsFeature);
        if (result != VmbErrorSuccess)
        {
            m_SensorBitsFeature.reset();
            return result;
        }
    }
    feature = m_SensorBitsFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetSensorType (SensorTypeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSensorTypeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SensorTypeEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::GetSensorTypeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SensorTypeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("SensorType", m_SensorTypeFeature);
        if (result != VmbErrorSuccess)
        {
            m_SensorTypeFeature.reset();
            return result;
        }
    }
    feature = m_SensorTypeFeature;
    return VmbErrorSuccess;
}


// Category /SavedUserSets
VmbErrorType GT1290Camera::GetUserSetDefaultSelector (UserSetDefaultSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetDefaultSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (UserSetDefaultSelectorEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetUserSetDefaultSelector (UserSetDefaultSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetDefaultSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetUserSetDefaultSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetDefaultSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("UserSetDefaultSelector", m_UserSetDefaultSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetDefaultSelectorFeature.reset();
            return result;
        }
    }
    feature = m_UserSetDefaultSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::UserSetLoad ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetLoadFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType GT1290Camera::GetUserSetLoadFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetLoadFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("UserSetLoad", m_UserSetLoadFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetLoadFeature.reset();
            return result;
        }
    }
    feature = m_UserSetLoadFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::UserSetSave ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetSaveFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType GT1290Camera::GetUserSetSaveFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetSaveFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("UserSetSave", m_UserSetSaveFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetSaveFeature.reset();
            return result;
        }
    }
    feature = m_UserSetSaveFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetUserSetSelector (UserSetSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (UserSetSelectorEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetUserSetSelector (UserSetSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetUserSetSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("UserSetSelector", m_UserSetSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetSelectorFeature.reset();
            return result;
        }
    }
    feature = m_UserSetSelectorFeature;
    return VmbErrorSuccess;
}


// Category /Stream/Info
VmbErrorType GT1290Camera::GetGVSPFilterVersion (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPFilterVersionFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPFilterVersionFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPFilterVersionFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPFilterVersion", m_GVSPFilterVersionFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPFilterVersionFeature.reset();
            return result;
        }
    }
    feature = m_GVSPFilterVersionFeature;
    return VmbErrorSuccess;
}


// Category /Stream/Multicast
VmbErrorType GT1290Camera::GetMulticastEnable (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetMulticastEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetMulticastEnable (bool value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetMulticastEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetMulticastEnableFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_MulticastEnableFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("MulticastEnable", m_MulticastEnableFeature);
        if (result != VmbErrorSuccess)
        {
            m_MulticastEnableFeature.reset();
            return result;
        }
    }
    feature = m_MulticastEnableFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetMulticastIPAddress (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetMulticastIPAddressFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetMulticastIPAddress (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetMulticastIPAddressFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetMulticastIPAddressFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_MulticastIPAddressFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("MulticastIPAddress", m_MulticastIPAddressFeature);
        if (result != VmbErrorSuccess)
        {
            m_MulticastIPAddressFeature.reset();
            return result;
        }
    }
    feature = m_MulticastIPAddressFeature;
    return VmbErrorSuccess;
}


// Category /Stream/Settings
VmbErrorType GT1290Camera::GVSPAdjustPacketSize ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPAdjustPacketSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType GT1290Camera::GetGVSPAdjustPacketSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPAdjustPacketSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPAdjustPacketSize", m_GVSPAdjustPacketSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPAdjustPacketSizeFeature.reset();
            return result;
        }
    }
    feature = m_GVSPAdjustPacketSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPBurstSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPBurstSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVSPBurstSize (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPBurstSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPBurstSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPBurstSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPBurstSize", m_GVSPBurstSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPBurstSizeFeature.reset();
            return result;
        }
    }
    feature = m_GVSPBurstSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPDriver (GVSPDriverEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPDriverFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (GVSPDriverEnum) nValue;
    return result;
}
VmbErrorType GT1290Camera::SetGVSPDriver (GVSPDriverEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPDriverFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPDriverFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPDriverFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPDriver", m_GVSPDriverFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPDriverFeature.reset();
            return result;
        }
    }
    feature = m_GVSPDriverFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPHostReceiveBuffers (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPHostReceiveBuffersFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVSPHostReceiveBuffers (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPHostReceiveBuffersFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPHostReceiveBuffersFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPHostReceiveBuffersFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPHostReceiveBuffers", m_GVSPHostReceiveBuffersFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPHostReceiveBuffersFeature.reset();
            return result;
        }
    }
    feature = m_GVSPHostReceiveBuffersFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPMaxLookBack (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPMaxLookBackFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVSPMaxLookBack (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPMaxLookBackFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPMaxLookBackFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPMaxLookBackFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPMaxLookBack", m_GVSPMaxLookBackFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPMaxLookBackFeature.reset();
            return result;
        }
    }
    feature = m_GVSPMaxLookBackFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPMaxRequests (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPMaxRequestsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVSPMaxRequests (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPMaxRequestsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPMaxRequestsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPMaxRequestsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPMaxRequests", m_GVSPMaxRequestsFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPMaxRequestsFeature.reset();
            return result;
        }
    }
    feature = m_GVSPMaxRequestsFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPMaxWaitSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPMaxWaitSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVSPMaxWaitSize (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPMaxWaitSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPMaxWaitSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPMaxWaitSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPMaxWaitSize", m_GVSPMaxWaitSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPMaxWaitSizeFeature.reset();
            return result;
        }
    }
    feature = m_GVSPMaxWaitSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPMissingSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPMissingSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVSPMissingSize (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPMissingSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPMissingSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPMissingSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPMissingSize", m_GVSPMissingSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPMissingSizeFeature.reset();
            return result;
        }
    }
    feature = m_GVSPMissingSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPPacketSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPPacketSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVSPPacketSize (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPPacketSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPPacketSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPPacketSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPPacketSize", m_GVSPPacketSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPPacketSizeFeature.reset();
            return result;
        }
    }
    feature = m_GVSPPacketSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPTiltingSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPTiltingSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVSPTiltingSize (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPTiltingSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPTiltingSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPTiltingSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPTiltingSize", m_GVSPTiltingSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPTiltingSizeFeature.reset();
            return result;
        }
    }
    feature = m_GVSPTiltingSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetGVSPTimeout (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPTimeoutFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::SetGVSPTimeout (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGVSPTimeoutFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetGVSPTimeoutFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GVSPTimeoutFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("GVSPTimeout", m_GVSPTimeoutFeature);
        if (result != VmbErrorSuccess)
        {
            m_GVSPTimeoutFeature.reset();
            return result;
        }
    }
    feature = m_GVSPTimeoutFeature;
    return VmbErrorSuccess;
}


// Category /Stream/Statistics
VmbErrorType GT1290Camera::GetStatFrameDelivered (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatFrameDeliveredFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatFrameDeliveredFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatFrameDeliveredFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatFrameDelivered", m_StatFrameDeliveredFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatFrameDeliveredFeature.reset();
            return result;
        }
    }
    feature = m_StatFrameDeliveredFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatFrameDropped (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatFrameDroppedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatFrameDroppedFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatFrameDroppedFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatFrameDropped", m_StatFrameDroppedFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatFrameDroppedFeature.reset();
            return result;
        }
    }
    feature = m_StatFrameDroppedFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatFrameRate (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatFrameRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatFrameRateFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatFrameRateFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatFrameRate", m_StatFrameRateFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatFrameRateFeature.reset();
            return result;
        }
    }
    feature = m_StatFrameRateFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatFrameRescued (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatFrameRescuedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatFrameRescuedFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatFrameRescuedFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatFrameRescued", m_StatFrameRescuedFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatFrameRescuedFeature.reset();
            return result;
        }
    }
    feature = m_StatFrameRescuedFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatFrameShoved (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatFrameShovedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatFrameShovedFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatFrameShovedFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatFrameShoved", m_StatFrameShovedFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatFrameShovedFeature.reset();
            return result;
        }
    }
    feature = m_StatFrameShovedFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatFrameUnderrun (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatFrameUnderrunFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatFrameUnderrunFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatFrameUnderrunFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatFrameUnderrun", m_StatFrameUnderrunFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatFrameUnderrunFeature.reset();
            return result;
        }
    }
    feature = m_StatFrameUnderrunFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatLocalRate (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatLocalRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatLocalRateFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatLocalRateFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatLocalRate", m_StatLocalRateFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatLocalRateFeature.reset();
            return result;
        }
    }
    feature = m_StatLocalRateFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatPacketErrors (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatPacketErrorsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatPacketErrorsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatPacketErrorsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatPacketErrors", m_StatPacketErrorsFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatPacketErrorsFeature.reset();
            return result;
        }
    }
    feature = m_StatPacketErrorsFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatPacketMissed (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatPacketMissedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatPacketMissedFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatPacketMissedFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatPacketMissed", m_StatPacketMissedFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatPacketMissedFeature.reset();
            return result;
        }
    }
    feature = m_StatPacketMissedFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatPacketReceived (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatPacketReceivedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatPacketReceivedFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatPacketReceivedFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatPacketReceived", m_StatPacketReceivedFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatPacketReceivedFeature.reset();
            return result;
        }
    }
    feature = m_StatPacketReceivedFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatPacketRequested (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatPacketRequestedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatPacketRequestedFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatPacketRequestedFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatPacketRequested", m_StatPacketRequestedFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatPacketRequestedFeature.reset();
            return result;
        }
    }
    feature = m_StatPacketRequestedFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatPacketResent (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatPacketResentFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatPacketResentFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatPacketResentFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatPacketResent", m_StatPacketResentFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatPacketResentFeature.reset();
            return result;
        }
    }
    feature = m_StatPacketResentFeature;
    return VmbErrorSuccess;
}

VmbErrorType GT1290Camera::GetStatTimeElapsed (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStatTimeElapsedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType GT1290Camera::GetStatTimeElapsedFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StatTimeElapsedFeature.get() == NULL)
    {
        VmbErrorType result;
        result = GetFeatureByName ("StatTimeElapsed", m_StatTimeElapsedFeature);
        if (result != VmbErrorSuccess)
        {
            m_StatTimeElapsedFeature.reset();
            return result;
        }
    }
    feature = m_StatTimeElapsedFeature;
    return VmbErrorSuccess;
}

