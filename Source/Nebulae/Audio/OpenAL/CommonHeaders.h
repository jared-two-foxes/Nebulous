#ifndef NEBULAE_AUDIO_OPENAL_COMMONHEADERS_H_
#define NEBULAE_AUDIO_OPENAL_COMMONHEADERS_H_

//
// Specifies common headers required for sound driver.
//

// include OpenAL headers
#if defined( __APPLE__ ) && defined( __MACH__ )
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

bool CheckForAudioErrors( const char* message );

#endif // NEBULAE_AUDIO_OPENAL_COMMONHEADERS_H_