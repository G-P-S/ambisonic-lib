//
//  AmbisonicEncoderDistSansDelay.h
//  amblib
//
//  Created by Joseph Mariglio on 8/16/16.
//
//

#ifndef AmbisonicEncoderDistSansDelay_h
#define AmbisonicEncoderDistSansDelay_h

#include "AmbisonicEncoder.h"

// Ambisonic encoder with non-delaying (gain only) distance cues
//  ensures coherent playback with gain compensation, ideal for speaker array simulations

class CAmbisonicEncoderDistSansDelay : public CAmbisonicEncoder
{
public:
    CAmbisonicEncoderDistSansDelay();
    ~CAmbisonicEncoderDistSansDelay();
    virtual bool Create(AmbUInt nOrder, AmbBool b3D, AmbUInt nMisc);
    virtual void Refresh();
    void Process(AmbFloat* pfSrc, AmbUInt nSamples, CBFormat* pBFDst, bool replacing = true);
    void SetRoomRadius(AmbFloat fRoomRadius);
    AmbFloat GetRoomRadius();
protected:
    AmbFloat m_fRoomRadius;
    AmbFloat m_fInteriorGain;
    AmbFloat m_fExteriorGain;
};

#endif /* AmbisonicEncoderDistSansDelay_h */
