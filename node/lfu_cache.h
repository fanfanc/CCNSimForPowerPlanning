#ifndef LFU_CACHE_H_
#define LFU_CACHE_H_

#include "base_cache.h"

using namespace std;

//LFU replacement algorithm:
class LFU_Cache: public Cache
{
public:
    LFU_Cache(uint32_t s);

    virtual void storeData(uint64_t);

    virtual bool lookup(uint64_t);
};

#endif
