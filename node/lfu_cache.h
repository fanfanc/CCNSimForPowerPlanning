#ifndef LFU_CACHE_H_
#define LFU_CACHE_H_

#include "base_cache.h"

using namespace std;

//LFU replacement algorithm:
class LFU_Cache: public Cache
{
public:
    LFU_Cache(uint32_t s);
    
    virtual bool warmup(vector<file> &catalog);
    virtual void storeData(uint64_t);
    virtual bool lookup(uint64_t);
    virtual bool isFull();
    virtual deque<uint64_t> getCache();

private:
	map<uint64_t, uint64_t> lfuCache;
	multimap<uint64_t, uint64_t> refCountToChunk;
	void remove(const uint64_t count, const uint64_t chunk);
};

#endif
