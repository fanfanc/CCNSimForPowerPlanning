#include "lfu_cache.h"

LFU_Cache::LFU_Cache(uint32_t s)
: Cache(s)
{
}

void LFU_Cache::storeData(uint64_t chunk)
{
    cache[chunk] = true;
    deq.push_back(chunk);

    if (deq.size() > size)
    {
        uint64_t toErase = deq.front();
        deq.pop_front();
        cache.erase(toErase);
    }
}


bool LFU_Cache::lookup(uint64_t chunk)
{
    return (cache.find(chunk) != cache.end());
}