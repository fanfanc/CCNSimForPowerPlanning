#include "lfu_cache.h"
#include "utils/ccn_utils.h"

LFU_Cache::LFU_Cache(uint32_t s)
    : Cache(s)
{
}

bool LFU_Cache::warmup(vector<file> &catalog)
{
    uint32_t s = 0,
             i = 1,
             F = catalog.size();
    uint64_t chunk;
    file file;

    //Size represents the chache size expressed in chunks
    cout << "Starting warmup..." << endl;
    while (s < size && i <= F)
    {
        file  = catalog[i];
        chunk = generateChunk(i, 0);

        storeData(chunk);
        i++;
        s++;
    }

    //Return if the cache has been filled or not
    if (i > F)
        return false;
    return true;

}

void LFU_Cache::storeData(uint64_t chunk)
{
    int64_t count = 0;
    map<uint64_t, uint64_t>::iterator finder = lfuCache.find(chunk);
    if (finder != lfuCache.end())
    {
        remove(finder->second, chunk);
        ++finder->second;
        refCountToChunk.insert(make_pair(finder->second, chunk));
    }
    else
    {
        lfuCache.insert(make_pair(chunk, 0));
        refCountToChunk.insert(make_pair(0, chunk));
    }


    if (lfuCache.size() > size)
    {
        lfuCache.erase(refCountToChunk.begin()->second);
        refCountToChunk.erase(refCountToChunk.begin());
    }
}


bool LFU_Cache::lookup(uint64_t chunk)
{
    multimap<uint64_t, uint64_t>::iterator finder = lfuCache.find(chunk);
    if (finder != lfuCache.end())
    {
        remove(finder->second, chunk);
        ++finder->second;
        refCountToChunk.insert(make_pair(finder->second, chunk));
        return true;
    }

    return false;
}

void LFU_Cache::remove(const uint64_t count, const uint64_t chunk)
{
    for (multimap<uint64_t, uint64_t>::iterator it = refCountToChunk.lower_bound(count);
            it != refCountToChunk.upper_bound(count); ++it)
    {
        if (it->second == chunk)
        {
            refCountToChunk.erase(it);
            return;
        }
    }
}

bool LFU_Cache::isFull()
{
    return lfuCache.size() >= size;  
}

deque<uint64_t> LFU_Cache::getCache()
{
    //The deque cache is filled and returned by this function for statistical purposes
    deq.clear();
    for (multimap<uint64_t, uint64_t>::const_iterator it = lfuCache.begin();
        it != lfuCache.end(); ++it)
    {
        deq.push_back(it->first);
    }
        
    return deq;
}
