#ifndef __NEBULAE_COMMON_SINGLETON_H__
#define __NEBULAE_COMMON_SINGLETON_H__

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{

template <class T>
class Singleton
{
private:
  static T* ms_instance;

  public:
    Singleton()
    {
      NE_ASSERT( !ms_instance, "Attempted to create an instance to a singleton that already exists." )();
      ms_instance = (T*)this;
    }

    virtual ~Singleton() 
    {
      NE_ASSERT( ms_instance, "Attempted to destroy a singleton that was not initialized." )();
      ms_instance = nullptr;      
    }

    static bool IsValid() { return (ms_instance != 0); }

    static T* GetPtr() { NE_ASSERT( ms_instance, "Attempting to access uninitialized singleton." )(); return ms_instance; }
    static T& Get()    { NE_ASSERT( ms_instance, "Attempting to access uninitialized singleton." )(); return *ms_instance; }
    static T& GetLazy() 
    {
      if( ms_instance == nullptr )
      {
        new T();
      }

      return *ms_instance;
    }

};

template <typename T> T* Singleton <T>::ms_instance = nullptr;

}

#endif // __NEBULAE_COMMON_SINGLETON_H__