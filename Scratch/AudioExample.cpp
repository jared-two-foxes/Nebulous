#include <iostream>
#include <fstream>
#include <cstring>
#include <AL/al.h>
#include <AL/alc.h>
#include <SDL/SDL.h>
 
bool isBigEndian()
{
  int a=1;
  return !((char*)&a)[0];
}

int convertToInt(char* buffer,int len)
{
    int a=0;
    if(!isBigEndian())
        for(int i=0;i<len;i++)
            ((char*)&a)[i]=buffer[i];
    else
        for(int i=0;i<len;i++)
            ((char*)&a)[3-i]=buffer[i];    
    return a;
}

char* loadWAV(const char* fn,int& chan,int& samplerate,int& bps,int& size)
{
    char buffer[4];
    std::ifstream in(fn,std::ios::binary);
    in.read(buffer,4);
    if(strncmp(buffer,"RIFF",4)!=0)
    {
        std::cout << "this is not a valid WAVE file"  << std::endl;
        return NULL;
    }
    in.read(buffer,4);
    in.read(buffer,4);      //WAVE
    in.read(buffer,4);      //fmt
    in.read(buffer,4);      //16
    in.read(buffer,2);      //1
    in.read(buffer,2);
    chan=convertToInt(buffer,2);
    in.read(buffer,4);
    samplerate=convertToInt(buffer,4);
    in.read(buffer,4);
    in.read(buffer,2);
    in.read(buffer,2);
    bps=convertToInt(buffer,2);
    in.read(buffer,4);      //data
    in.read(buffer,4);
    size=convertToInt(buffer,4);
    char* data=new char[size];
    in.read(data,size);
    return data;   
}



Ref<WavSound> WavSound::LoadWAV(ting::fs::File& fi)
{
    ting::fs::File::Guard fileGuard(fi, ting::fs::File::READ);//make sure we close the file even in case of exception is thrown

    //Start reading Wav-file header
    {
        StaticBuffer<u8, 4> riff;
        fi.Read(riff);//Read 'RIFF' signature
        if(std::string(reinterpret_cast<char*>(riff.Begin()), riff.Size()) != "RIFF"){
                throw Exc("WavSound::LoadWAV(): 'RIFF' signature not found");
        }
    }

    fi.SeekForward(4);//Skip "Wav-file size minus 7". We are not intrested in this information

    {
        StaticBuffer<u8, 4> wave;
        fi.Read(wave);//Read 'WAVE' signature
        if(std::string(reinterpret_cast<char*>(wave.Begin()), wave.Size()) != "WAVE"){
            throw Exc("WavSound::LoadWAV(): 'WAVE' signature not found");
        }
    }

    {
        StaticBuffer<u8, 4> fmt;
        fi.Read(fmt);//Read 'fmt ' signature
        if(std::string(reinterpret_cast<char*>(fmt.Begin()), fmt.Size()) != "fmt "){
            throw Exc("WavSound::LoadWAV(): 'fmt ' signature not found");
        }
    }

    fi.SeekForward(4);//Skip 4 bytes. Their purpose is unknown to me. 

    unsigned chans;
    {
        StaticBuffer<u8, 4> pcmBuf;
        fi.Read(pcmBuf);
        u32 pcm = ting::util::Deserialize32LE(pcmBuf.Begin());
        if((pcm & 0x0000ffff) != 1){//Low word indicates whether the file is in PCM format
            throw Exc("C_PCM_NonStreamedSound::LoadWAV(): not a PCM format, only PCM format is supported");
        }

        chans = unsigned(pcm >> 16);//High word contains the number of channels (1 for mono, 2 for stereo)
        if(chans != 1 && chans != 2){
            //only mono or stereo is supported and nothing other
            throw Exc("WavSound::LoadWAV(): unsupported number of channels");
        }
    }

    //Read in the sound quantization frequency
    u32 frequency;
    {
        StaticBuffer<u8, 4> buf;
        fi.Read(buf);
        frequency = ting::util::Deserialize32LE(buf.Begin());
    }

    fi.SeekForward(4);//Playback speed (freq * PCMSampleSize). We don't need this info.

    u32 bitDepth;
    {
        StaticBuffer<u8, 4> buf;
        fi.Read(buf);
        bitDepth = ting::util::Deserialize32LE(buf.Begin());
        bitDepth >>= 16;//High word contains the sound bit depth
    }

    {
        StaticBuffer<u8, 4> data;
        fi.Read(data);//Read 'data' signature
        if(std::string(reinterpret_cast<char*>(data.Begin()), data.Size()) != "data"){
                throw Exc("WavSound::LoadWAV(): 'data' signature not found");
        }
    }

    u32 dataSize;
    {
        StaticBuffer<u8, 4> buf;
        fi.Read(buf);//read the size of the sound data
        dataSize = ting::util::Deserialize32LE(buf.Begin());
    }
    
    //read in the sound data
    ting::Array<ting::u8> data(dataSize);
    {
        unsigned bytesRead = fi.Read(data);//Load Sound data

        if(bytesRead != dataSize){
            throw Exc("WavSound::LoadWAV(): sound data size is incorrect");
        }
    }
    
    //Now we have Wav-file info
    Ref<WavSound> ret;
    if(bitDepth == 8){
        //@todo: support it
        throw Exc("WavSound::LoadWAV(): unsupported bit depth (8 bit) wav file");
    }
    else if(bitDepth == 16){
        //set the format
        switch(chans){
            case 1://mono
                switch(frequency){
                    case 11025:
                        ret = WavSoundImpl<ting::s16, 1, 11025>::New(data);
                        break;
                    case 22050:
                        ret = WavSoundImpl<ting::s16, 1, 22050>::New(data);
                        break;
                    case 44100:
                        ret = WavSoundImpl<ting::s16, 1, 44100>::New(data);
                        break;
                    default:
                        throw Exc("WavSound::LoadWAV(): unsupported sampling rate");
                }
                break;
            case 2://stereo
                switch(frequency){
                    case 11025:
                        ret = WavSoundImpl<ting::s16, 2, 11025>::New(data);
                        break;
                    case 22050:
                        ret = WavSoundImpl<ting::s16, 2, 22050>::New(data);
                        break;
                    case 44100:
                        ret = WavSoundImpl<ting::s16, 2, 44100>::New(data);
                        break;
                    default:
                        throw Exc("WavSound::LoadWAV(): unsupported sampling rate");
                }
                break;
            default:
                throw aumiks::Exc("WavSound::LoadWAV():  unsupported number of channels");
        }//~switch(chans)
    }else{
        throw Exc("WavSound::LoadWAV(): unsupported bit depth");
    }
    
    return ret;
}




























// Initialization
Device = alcOpenDevice(NULL); // select the "preferred device"
if (Device) 
{
    Context=alcCreateContext(Device,NULL);
    alcMakeContextCurrent(Context);
}

// Check for EAX 2.0 support
g_bEAX = alIsExtensionPresent("EAX2.0");

// Generate Buffers
alGetError(); // clear error code
alGenBuffers(NUM_BUFFERS, g_Buffers);
if ((error = alGetError()) != AL_NO_ERROR)
{
    DisplayALError("alGenBuffers :", error);
    return;
}

// Load test.wav
loadWAVFile("test.wav",&format,&data,&size,&freq,&loop);
if ((error = alGetError()) != AL_NO_ERROR)
{
    DisplayALError("alutLoadWAVFile test.wav : ", error);
    alDeleteBuffers(NUM_BUFFERS, g_Buffers);
    return;
}

// Copy test.wav data into AL Buffer 0
alBufferData(g_Buffers[0],format,data,size,freq);
if ((error = alGetError()) != AL_NO_ERROR)
{
    DisplayALError("alBufferData buffer 0 : ", error);
    alDeleteBuffers(NUM_BUFFERS, g_Buffers);
    return;
}

// Unload test.wav
unloadWAV(format,data,size,freq);
if ((error = alGetError()) != AL_NO_ERROR)
{
    DisplayALError("alutUnloadWAV : ", error);
    alDeleteBuffers(NUM_BUFFERS, g_Buffers);
    return;
}

// Generate Sources
alGenSources(1,source);
if ((error = alGetError()) != AL_NO_ERROR)
{
    DisplayALError("alGenSources 1 : ", error);
    return;
}

// Attach buffer 0 to source
alSourcei(source[0], AL_BUFFER, g_Buffers[0]);
if ((error = alGetError()) != AL_NO_ERROR)
{
    DisplayALError("alSourcei AL_BUFFER 0 : ", error);
}

// Exit
Context=alcGetCurrentContext();
Device=alcGetContextsDevice(Context);
alcMakeContextCurrent(NULL);
alcDestroyContext(Context);
alcCloseDevice(Device);




void SetupListener()
{
    ALfloat listenerPos[]={0.0,0.0,0.0};
    ALfloat listenerVel[]={0.0,0.0,0.0};
    ALfloat listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};

    // Position ...
    alListenerfv(AL_POSITION,listenerPos);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        DisplayALError("alListenerfv POSITION : ", error);
        return;
    }

    // Velocity ...
    alListenerfv(AL_VELOCITY,listenerVel);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        DisplayALError("alListenerfv VELOCITY : ", error);
        return;
    }

    // Orientation ...
    alListenerfv(AL_ORIENTATION,listenerOri);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        DisplayALError("alListenerfv ORIENTATION : ", error);
        return;
    }
}

int main(int argc,char** argv)
{
    int channel,sampleRate,bps,size;
    char* data=loadWAV("test.wav",channel,sampleRate,bps,size);

    ALCdevice* device=alcOpenDevice(NULL);
    if(device==NULL)
    {
        std::cout << "cannot open sound card" << std::endl;
        return 0;
    }

    ALCcontext* context=alcCreateContext(device,NULL);
    if(context==NULL)
    {
        std::cout << "cannot open context" << std::endl;
        return 0;
    }
    alcMakeContextCurrent(context);

    unsigned int bufferid,format;
    alGenBuffers(1,&bufferid);
    if(channel==1)
    {
        if(bps==8)
            format=AL_FORMAT_MONO8;
        else
            format=AL_FORMAT_MONO16;    
    }
    else
    {
        if(bps==8)
            format=AL_FORMAT_STEREO8;
        else
            format=AL_FORMAT_STEREO16;          
    }
    alBufferData(bufferid,format,data,size,sampleRate);

    unsigned int sourceid;
    alGenSources(1,&sourceid);
    alSourcei(sourceid,AL_BUFFER,bufferid);

    uint32 start;
    bool running;
    bool b[4]={0,0,0,0};
    float x=0,z=0;
    while(running)
    {
        // switch(event.type)
        // {
        // case SDL_QUIT:
        //     running=false;
        //     break;
        // case SDL_KEYDOWN:
        //     switch(event.key.keysym.sym)
        //     {
        //         case SDLK_UP:
        //             b[0]=true;
        //             break;
        //         case SDLK_RIGHT:
        //             b[1]=true;
        //             break;
        //         case SDLK_DOWN:
        //             b[2]=true;
        //             break;
        //         case SDLK_LEFT:
        //             b[3]=true;
        //             break;
        //         case SDLK_SPACE:
        //             alSourcePlay(sourceid);
        //             break;                                         
        //     }
        //     break;
        // case SDL_KEYUP:
        //     switch(event.key.keysym.sym)
        //     {
        //         case SDLK_UP:
        //             b[0]=false;
        //             break;
        //         case SDLK_RIGHT:
        //             b[1]=false;
        //             break;
        //         case SDLK_DOWN:
        //             b[2]=false;
        //             break;
        //         case SDLK_LEFT:
        //             b[3]=false;
        //             break;
        //     }
        //     break;
        // }
       
        if(b[0])
            z+=0.3;
        if(b[1])
            x+=0.3;
        if(b[2])
            z-=0.3;
        if(b[3])
            x-=0.3;

        alSource3f(sourceid,AL_POSITION,x,0,z);
        alSourcei(sourceid,AL_LOOPING,AL_TRUE);
       
       
        float f[]={1,0,0,0,1,0};
        alListenerfv(AL_ORIENTATION,f);
       
       
        // if(1000/30.0>SDL_GetTicks()-start)
        //     SDL_Delay(1000/30.0-(SDL_GetTicks()-start));
    }

    alDeleteSources(1,&sourceid);
    alDeleteBuffers(1,&bufferid);

    alcDestroyContext(context);
    alcCloseDevice(device);

    delete[] data;

    return 0;
}