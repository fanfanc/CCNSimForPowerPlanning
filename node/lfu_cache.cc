#include "lfu_cache.h"

LFU_Cache::LFU_Cache(uint32_t s)
    : Cache(s)
{
}

void LFU_Cache::storeData(uint64_t chunk)
{
	uint64_t count = 0;
	hash_map<uint64_t, uint64_t>::iterator finder = lfuCache.find(chunk);
	if (finder != lfuCache.end())
	{
		count = finder->second;
		remove(count, chunk);
		++count;
	}
	lfuCache[chunk] = count;
	refCountToChunk.insert(make_pair(count, chunk));

    if (lfuCache.size() > size)
    {
    	lfuCache.erase(refCountToChunk.begin()->second);
        refCountToChunk.erase(refCountToChunk.begin());
    }
}


bool LFU_Cache::lookup(uint64_t chunk)
{
    return (lfuCache.find(chunk) != lfuCache.end());
}

void LFU_Cache::remove(const uint64_t count, const uint64_t chunk)
{
	for (multimap<uint64_t, uint64_t>::iterator it = refCountToChunk.lower_bound(count);
		it != refCountToChunk.upper_bound(count); ++it)
	{
		if(it->second == chunk)
		{
			refCountToChunk.erase(it);
			return;
		}
	}
}
