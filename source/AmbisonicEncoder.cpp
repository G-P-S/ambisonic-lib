/*############################################################################*/
/*#                                                                          #*/
/*#  Ambisonic C++ Library                                                   #*/
/*#  CAmbisonicEncoder - Ambisonic Encoder                                   #*/
/*#  Copyright � 2007 Aristotel Digenis                                      #*/
/*#                                                                          #*/
/*#  Filename:      AmbisonicEncoder.cpp                                     #*/
/*#  Version:       0.1                                                      #*/
/*#  Date:          19/05/2007                                               #*/
/*#  Author(s):     Aristotel Digenis                                        #*/
/*#  Licence:       MIT                                                      #*/
/*#                                                                          #*/
/*############################################################################*/

#include <iostream>
#include "AmbisonicEncoder.h"


CAmbisonicEncoder::CAmbisonicEncoder()
{
    Create(DEFAULT_ORDER, DEFAULT_HEIGHT, 0);
	Refresh();
}

CAmbisonicEncoder::~CAmbisonicEncoder()
{
	
}

bool CAmbisonicEncoder::Create(AmbUInt nOrder, AmbBool b3D, AmbUInt nMisc)
{
    bool success = CAmbisonicSource::Create(nOrder, b3D, nMisc);
    if(!success)
        return false;
    SetOrderWeight(0, 1.f / sqrtf(2.f));
    
    return true;
}

void CAmbisonicEncoder::Refresh()
{
	CAmbisonicSource::Refresh();
}

void CAmbisonicEncoder::Process(AmbFloat* pfSrc, AmbUInt nSamples, CBFormat* pfDst, bool replacing)
{
	AmbUInt niChannel = 0;
	AmbUInt niSample = 0;
	for(niChannel = 0; niChannel < m_nChannelCount; niChannel++)
	{
     //   std::cout<<"channel: "<<niChannel<<"\n"<<"[";
		for(niSample = 0; niSample < nSamples; niSample++)
		{
            AmbFloat tmpCoef = m_pfCoeff[niChannel];
            AmbFloat tmpSrcS = pfSrc[niSample];
            AmbFloat tmpDstS = tmpSrcS * tmpCoef;
            if(replacing)
            {
                pfDst->m_ppfChannels[niChannel][niSample] = tmpDstS;
            } else
            {
                pfDst->m_ppfChannels[niChannel][niSample] += tmpDstS;
            }
            /*std::cout<<tmpDstS;
            if(niSample < nSamples-1)
            {
                std::cout<<", ";
            } */
		}
//        std::cout<<"]"<<"\n";
	}
}