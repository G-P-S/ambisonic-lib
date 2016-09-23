//
//  AmbisonicEncoderDistSansDelay.cpp
//  amblib
//
//  Created by Joseph Mariglio on 8/16/16.
//
//

#include "AmbisonicEncoderDistSansDelay.h"

CAmbisonicEncoderDistSansDelay::CAmbisonicEncoderDistSansDelay()
{
    m_fRoomRadius = 0.25f;
    m_fInteriorGain = 1.f;
    m_fExteriorGain = 0.2f;
    Create(DEFAULT_ORDER, true, 0);
}

CAmbisonicEncoderDistSansDelay::~CAmbisonicEncoderDistSansDelay()
{
    
}

bool CAmbisonicEncoderDistSansDelay::Create(AmbUInt nOrder, AmbBool b3D, AmbUInt nMisc)
{
    return CAmbisonicSource::Create(nOrder, b3D, nMisc);
}

void CAmbisonicEncoderDistSansDelay::Refresh()
{
    CAmbisonicEncoder::Refresh();
    
    if(fabs(m_polPosition.fDistance) >= m_fRoomRadius)
    {
        m_fInteriorGain = (m_fRoomRadius / fabs(m_polPosition.fDistance)) / 2.f;
        m_fExteriorGain = m_fInteriorGain;
    }
    else
    {
        m_fInteriorGain = (2.f - fabs(m_polPosition.fDistance) / m_fRoomRadius) / 2.f;
        m_fExteriorGain = (fabs(m_polPosition.fDistance) / m_fRoomRadius) / 2.f;
    }
}

void CAmbisonicEncoderDistSansDelay::Process(AmbFloat* pfSrc, AmbUInt nSamples, CBFormat* pfDst, bool replacing)
{

    AmbUInt niChannel = 0;
    AmbUInt niSample = 0;
    AmbFloat tmpCoef = 1;
    AmbFloat tmpSrcS = 0;
    AmbFloat tmpDstS = 0;
    
    tmpCoef = m_pfCoeff[0];
    tmpCoef *= m_fExteriorGain;
    if(replacing)
    {
#pragma omp parallel for
        for(niSample = 0; niSample < nSamples; niSample++)
        {
            tmpSrcS = pfSrc[niSample];
            tmpDstS = tmpSrcS * tmpCoef;
            pfDst->m_ppfChannels[0][niSample] = tmpDstS;
        }
#pragma omp parallel for
        for(niChannel = 1; niChannel < m_nChannelCount; niChannel++)
        {
            tmpCoef = m_pfCoeff[niChannel] *  m_fInteriorGain;
            for(niSample = 0; niSample < nSamples; niSample++)
            {
                tmpSrcS = pfSrc[niSample];
                tmpDstS = tmpSrcS * tmpCoef;
                pfDst->m_ppfChannels[niChannel][niSample] = tmpDstS;
            }
        }
    }
    else
    {
#pragma omp parallel for
        for(niSample = 0; niSample < nSamples; niSample++)
        {
            tmpSrcS = pfSrc[niSample];
            tmpDstS = tmpSrcS * tmpCoef;
            pfDst->m_ppfChannels[0][niSample] += tmpDstS;
        }
#pragma omp parallel for
        for(niChannel = 1; niChannel < m_nChannelCount; niChannel++)
        {
            tmpCoef = m_pfCoeff[niChannel] *  m_fInteriorGain;
            for(niSample = 0; niSample < nSamples; niSample++)
            {
                tmpSrcS = pfSrc[niSample];
                tmpDstS = tmpSrcS * tmpCoef;
                pfDst->m_ppfChannels[niChannel][niSample] += tmpDstS;
            }
        }
    }
}

void CAmbisonicEncoderDistSansDelay::SetRoomRadius(AmbFloat fRoomRadius)
{
    m_fRoomRadius = fRoomRadius;
}

AmbFloat CAmbisonicEncoderDistSansDelay::GetRoomRadius()
{
    return m_fRoomRadius;
}