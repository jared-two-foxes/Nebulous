#ifndef __NEBULAE_AUDIO_COMMONHEADERS_H__
#define __NEBULAE_AUDIO_COMMONHEADERS_H__

//
// Specifies common headers required for sound driver.
//

// include OpenAL headers
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

bool CheckForAudioErrors( const char* message );

#endif // __NEBULAE_AUDIO_COMMONHEADERS_H__