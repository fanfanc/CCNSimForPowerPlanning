/*
 * ccnSim
 *  ccnSim is a scalable chunk-level simulator of Content Centric Networks (CCN),
 *  that we developed in the context of ANR Connect (http://www.anr-connect.org/)
 *
 * Contact us:
 *  Email: ccnsim@lincs.fr
 *      Web: http://www.enst.fr/~drossi/ccnSim
 *
 * People:
 *      Giuseppe Rossini (lead developer)
 *  Raffele Chiocchetti (developer)
 *  Dario Rossi (occasional debugger)
 *
 *
 *                                                    #
 *                            #                     ;##
 *                         `#+##                   +##:
 *                         #` .#                 ### ##
 *                         + `#.                 ;   #+ .
 *                          ##,                    ,+#####
 *                         +#  `                  `++#;
 *                        ;#####`             ;+`
 *                        +#, `:             +###`
 *                              `.########; `####+
 *                      ###+################  +#+
 *                     `###########;.,,,:;#
 *                      :##:
 *                   `;.
 *            ` :####
 *       +#####`####                                    `###,
 *       +  #+ ,###.                                    ,###
 *         +#+ ####                                     ;###
 *     +    #+;###                                      ###`
 *     ##;;## ####                                     .###
 *       ###  ###.                                      ###`
 *                                     `.;###:,                 ###
 *           ###:                     ;########  .##                ###
 *          #####                    ##########  :##`               ###
 *          #####                    ###;    `;  ```                ###
 *           +##:          #####` ###`;###  `###,    ###  ###`###` ;###     ##;
 *              .        ,######  ########:  #######.    ##,  #############,       `###
 *           :#.        `###`  .  ###. `###  #########. .##`  ###` #### `###       .###
 *          `###        ###  `##;   ##:   ######### ;##  `##:  ###   ##:       .##+
 *           ###        ###  .##   `##` .;##### ###  ,##`  ###  `##`       .##,
 *         #:###        ###  ###   ,##     .### ###  ###   ##;  ,##    .##.
 *        ## ###`       ###  ###   ;##  ##`    ;### ##:  ###   ##.  ;##    .###
 *       ##+ ,##;       #######  ###   ###  ##########:.##`  ###  .##   ###
 *     :#,.#..###;       ######  ##,   ### `#########: ;##   ##,  ;##   ###
 *    +##+#+,,####        `          `,;##;`        `              +##+
 *        #:  :###.                                 ,#####
 *        +.   ;.                                   ;#####
 *               ,###                               .#####
 *               +###+                              `####`
 *               #####
 *               :###
 *                 . :#                              ###
 *                  ;##,                            +###;
 *                 `####.                         # ####+
 *                  #####;                     #### ####`
 *                    ######### ,#+,     `,########
 *                      ######: ####`###########;
 *                         ###. ####`########+.
 *                              +###. ##;.`         ;##,
 *                     `                   #
 *                  ####.             `#:``
 *                  +#`            ####+
 *                  #:             #;  ##
 *                     +###            +# +#
 *                   .#+                +##.
 *                     +
 *                    .` `#+
 *                    ####+
 *
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 */
#include "lru_cache.h"
#include "utils/ccn_utils.h"


//Insert an element within the cache
void LRU_Cache::storeData(uint64_t chunk)
{
    lru[chunk] = true;
}


//Lookup for an element
bool LRU_Cache::lookup(uint64_t chunk)
{
    return (lru.find(chunk) != lru.end());
}


deque<uint64_t> LRU_Cache::getCache()
{
    //The deque cache is filled and returned by this function for statistical purposes
    deq.clear();
    for (lru_cache::const_iterator it = lru.mru_begin(); it != lru.end(); it++)
        deq.push_back(it.key());
    return deq;
}


bool LRU_Cache::warmup(vector <file> &catalog)
{
    uint32_t s = 0,
             i = 1,
             F = catalog.size();
    uint64_t chunk;
    file file;

    deque<uint64_t> dummycache;
    dummycache.clear();
    dummycache.resize(size);

    while (s < size && i <= F)
    {
        file = catalog[i];
        chunk = generateChunk(i, 0);

        for (uint32_t j = 0; j < file.size && s < size; j++)
        {
            double x = dblrand ();
            if (x < 0.5)
            {
                dummycache[s] = chunk;
                s++;
            }
        }
        i++;
    }

    //cacha warmup is done in the opposite sense of a random cache. This to be compliant
    //with an LRU cache (the least recently used will be, reasonably, the less popular element
    //, i.e. the last element inserted within the cache)
    for (deque<uint64_t>::reverse_iterator i = dummycache.rbegin(); i != dummycache.rend(); i++)
        lru[*i] = true;

    if (i > F)
        return false;
    return true;
}
