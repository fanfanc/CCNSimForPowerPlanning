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
#ifndef STATISTICS_H_
#define STATISTICS_H_
#include <omnetpp.h>
#include "base_cache.h"
#include <ext/hash_map>
#include "lru_cache.h"
#include "utils/ccn_utils.h"



#include <set>
#include <vector>

using namespace std;
using namespace __gnu_cxx;

class statistics : public cSimpleModule
{
public:

    statistics() : cSimpleModule( 16384 )
    {
        ;
    }

private:
    /*! Activity function
    *  \sa Version 0.0
     *  */
    void activity();

    map<pair<int, int>, double> throughputBetweenEachNode;


    double distance;
    double shortest;

    double div_size;
    double tot_size;
    //std::set<int> div_cache;
    hash_map<unsigned long int, bool> div_cache;

    double miss;
    double hit;

    map<int, double> hit_level;
    map<int, double> miss_level;
    map<int, simtime_t> full_time;
    map<uint32_t, double> hit_stat;
    map<uint32_t, double> miss_stat;
    bool enable_stat;
    cOutVector hit_time;

    int N;

    simtime_t ft;
    simtime_t _sim_time;
    int node_unstable;
    int node_steady;
    double convergence_threshold;
    bool convergence_type_avg;
public:

	void throughput(const int source, const int sink, const double chunkCountPerSec);
	void recordThroughput();

    //Standard node function

    /*! Cache Hit function.
     *  \param nodeID
     *    \sa Version 0.0
     *  */
    void cache_hit(int, uint64_t);

    /*! Cache Miss function
    *  \param nodeID
     *   \sa Version 0.0
     *  */
    void cache_miss(int, uint64_t);

    //Standard node function

    /*! Cache inspection.
     *
     *    \sa Version 0.0
     *  */
    void inspectCache(Cache *, int);

    //Standard node function

    /*! Stretch function.
     *
     *    \sa Version 0.0
     *  */
    void stretch(int, int);

    //Standard node function

    /*! Function called by node when it's cache is filled for the first time.
     *
     *    \sa Version 0.0
     *  */
    void fullStamping(int);

    //Standard node function

    /*! Function that check the stabilization of average Hitrate.
     * \param "Hit Rate"
     *    \sa Version 0.0
     *  */
    bool checkStabilization(double);

    //Standard node function

    /*! Function called by node when is stabilized.
     * \param "ID Node"
     *    \sa Version 0.0
     *  */
    void nodeStabilize(int node);

    //Standard node function

    /*! Function called by node at the end of steady phase.
     *    \param "ID Node"
     *    \sa Version 0.0
     *  */
    void nodeFinalize(int node);

protected:
    /*! Initialization function
    *   \sa Version 0.0
     *  */
    virtual void initialize();
    virtual void finish();

private:
    void recordSimTime();
};

#endif
