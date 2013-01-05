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
#ifndef CCN_NODE_H
#define CCN_NODE_H

#include <omnetpp.h>
#include <ext/hash_map>
#include <fstream>
#include "ccn_data_m.h"
#include "ccn_interest.h"
#include "access_data_m.h"
#include "content_distribution.h"
#include "caching_policy.h"
#include "repository.h"
#include "statistics.h"
#include "routing.h"
#include "utils/ccn_utils.h"


#define CCN_A 200
#define CCN_I -100
#define CCN_D -200
#define CCN_L 100
#define ALL 3
#define CLOSEST 4
#define INIT 10 //PHASE
#define WARM_UP 20
#define TRANSIENT 30
#define STEADY 40
#define TC 0.1 //Sampling Time
#define VERY_BIG_VALUE 100000
#define ACCESS_DELAY_FIX 0.001 //1 ms
#define ACCESS_DELAY_RAND dblrand()/10000// (1/10)ms

using namespace __gnu_cxx;
using namespace std;

struct file_property //state of file
{
    uint32_t tot_distance;
    uint32_t missing_chunks;
    uint32_t repository;
    double  hops;
    bool valid;
    file_property(uint32_t m = 0, uint32_t td = 0, uint32_t rep = 0, double h = 0, bool v = false): tot_distance(td), missing_chunks(m), repository(rep), hops(h), valid(v)
    {
        ;
    }
};


class ccn_node : public cSimpleModule
{
public:

    //external function
    const map<uint32_t, uint32_t> &getDistanceSet();

protected:
    //Standard node function

    /*! Initialization function.
     *
     *    \sa Version 0.0
     *  */
    virtual void initialize();

    /*! Handling message function
      *     \param message
      *     \sa Version 0.0
      *  */
    virtual void handleMessage(cMessage *);

    /*! That function, collect results at the end of simulation.
    *     \sa Version 0.0
    *    */
    virtual void finish();

    //Custom function

    /*! Generate the interest message
     *    \param FileID
     *    \sa Version 0.0
     * */
    void generate_interest(uint32_t);
    /*! Manage the Data message
     *    \param data message
     *    \sa Version 0.0
     * */
    void manage_data(ccn_data *);

    /*! Manage the Data message
     *          *    \param data message
     *                   *    \sa Version 0.0
     *                            * */
    void manage_localChunk(uint64_t);


    /*! Manage the interest message
    *   \param data message
     *   \sa Version 0.0
     *  */
    void manage_interest(ccn_interest *);



    /*! Calculate the minimum distance from the source
    *   \param FileID
     *   \param Repositories list
     *   \sa Version 0.0
     *  */
    int minimum_distance_from_source(uint32_t, uint32_t *);


    /*! Send the interest message according to the chosen strategy layer
     *   \param ChunkID
     *   \param Repositories list
     *   \sa Version 0.0
     *  */
    void sendInterests(uint64_t, uint32_t *);



    void sendSortedInterests(vector < deque <int> >, uint64_t);


    /*! Send the interest only to closer repository
     *   \param Repository list
     *   \param chunkID
     *   \sa Version 0.0
     *  */
    void sendSinglePath(uint32_t *, uint64_t );


    /*! Compose Interest message
    *   \param chunkID
    *   \sa Version 0.0
    *   */
    ccn_interest *composeInterest(uint64_t);

    /*! Compose Data message
    *   \param chunkID
     *   \sa Version 0.0
     *   */
    ccn_data *composeCData(uint64_t);





    /*! check stabilization of HitRate
     *   \sa Version 0.0
     *  */
    void stabilize();

    /*! Local Cache miss
    *   \sa Version 0.0
    *  */
    void local_cache_miss(uint64_t);
    /*! Local Cache hit
    *   \sa Version 0.0
     *  */
    void local_cache_hit(uint64_t);



private:
    cMessage *event; // pointer to the event object which we'll use for timing

    cMessage *statisticsPeak;
    vector<int> peakAtEachNode;
    vector<int> chunkCountRecvFromEachNodePerSecond;
    vector<int> peakDuration;
    bool isPeakStabled;

    int getNodeID();
    int getNodeCount();
    void initializeStatisticsPeak();
    void handleStatisticsPeak();
    void addChunkCount(const int source);
    void updatePeakStabled();
    void updatePeakDuration();
    void recordPeak();

    content_distribution *content;
    statistics *stat;
    routing *rout;
    static ofstream node_stat;
    static ofstream node_local;

    Policy *decision_policy;

    hash_map<uint64_t, vector<int> > PIT;
    hash_map<uint32_t, vector< deque <int> > > FIB;
    Cache *ContentStore;

    //  hash_map<uint32_t,state> SLP; //for forwarding
    int fwd;
    double miss;//
    double hit;//

    vector<double> samples;//contain hitrate
    uint32_t zipf_limit;
    bool phase_ended;

    double hold; //used only for calculate the hit+miss(100ms)
    int sim_state; //state of node
    double time_steady_state; //time to spend in steady state
    double convergence_threshold;

    map<uint32_t, uint32_t> dset;
    hash_map<uint32_t, file_property> pending_map;
    double beta;// beta strategy layer adaptive all
    uint32_t S;
    uint32_t M;
    int D;
    uint32_t Replicas;
    int N;
    string type_strategy;// strategyLayer
    bool local_popularity;// collect data for Zipf
    bool convergence_type_wc;
    uint32_t  _interests;
    uint32_t  _data;

    hash_map<uint32_t, uint32_t> request_file;
};

#endif
