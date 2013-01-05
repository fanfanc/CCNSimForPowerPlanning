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
#include "ccn_node.h"
#include "local_m.h"
#include "statistics.h"
#include "statistics/stat_util.h"

#include "lru_cache.h"
#include "random_cache.h"
#include "fifo_cache.h"
#include "two_cache.h"

#include "always_policy.h"
#include "distance_policy.h"
#include "lcd_policy.h"
#include "never_policy.h"
#include "fix_policy.h"
#include "global_request_generator.h"


Register_Class(ccn_node);


static int unstable = 0;
static int not_full = 0;

ofstream ccn_node::node_stat;
ofstream ccn_node::node_local;

const int peakStabledDuration = 10; // s

int ccn_node::getNodeID()
{
    return getIndex();
}

int ccn_node::getNodeCount()
{
    vector<string> ccn_types;
    ccn_types.push_back("modules.ccn_node");
    cTopology ccn_topo;
    ccn_topo.extractByNedTypeName(ccn_types);
    return ccn_topo.getNumNodes();
}

void ccn_node::initializeStatisticsThroughput()
{
    chunkCountRecvFromEachNode.assign(getNodeCount(), 0);
}

void ccn_node::addChunkCount(const int source)
{
    chunkCountRecvFromEachNode[source]++;
}

void ccn_node::initialize()
{
    initializeStatisticsThroughput();
    _interests = 0;
    _data = 0;

    S = par("S"); //Cache size (in number of available chunks)
    M = par("M"); //Multipath
    D = par("D"); //Locality diameter
    //Replicas = par("replicas"); //Replicas
    type_strategy = par("strategy_layer").stringValue();
    local_popularity = (bool) par("local_popularity");
    beta = par("Beta"); //Beta parameter for strategy layer Adaptive_closest
    bool warmup = (bool) par("warmup");
    convergence_threshold = par("convergence_threshold_wc");
    time_steady_state = par("sim_time");
    sim_state = INIT; //state of node
    //cout<<ACCESS_DELAY_FIX<<"s +"<<ACCESS_DELAY_RAND<<"s"<<endl;

    content = (content_distribution *)this->getParentModule()->getModuleByRelativePath("content_distribution");
    stat = (statistics *)this->getParentModule()->getModuleByRelativePath("statistics");
    rout = (routing *)this->getParentModule()->getModuleByRelativePath("routing");

    Replicas = content->getNumberReplicas();//Replicas of file


    //Choose cache type (lru, random, power of two, fifo)
    string cache_replacement_policy = par("cache_replacement_policy");
    if (cache_replacement_policy.compare("rnd") == 0)
        ContentStore = new RND_Cache(S);
    else if (cache_replacement_policy.compare("fifo") == 0)
        ContentStore = new FIFO_Cache(S);
    else if (cache_replacement_policy.compare("two") == 0)
        ContentStore = new TWO_Cache(S);
    else
        ContentStore = new LRU_Cache(S);


    //and the decision policy (distance, always, never, fixed probability p)
    string dec_policy = par("decision_policy");
    if (dec_policy.compare("distance") == 0)
        decision_policy = new Distance(1);
    else if (dec_policy.compare("lcd") == 0)
        decision_policy = new LCD();
    else if (dec_policy.find("fix") == 0)
    {
        string p = dec_policy.substr(3);
        double _p = atof(p.c_str());
        decision_policy = new Fix(_p);
    }
    else if (dec_policy.compare("never") == 0)
        decision_policy = new Never();
    else
        decision_policy = new Always();


    //Building forwarding table. The distance set dset is composed by nodes
    //which minimum distance from current node is shorter then D
    for (uint32_t n = 0; n < (uint32_t) getAncestorPar("n") ; n++)
    {
        if (n != getIndex())
        {
            FIB[n] = rout->calculatePath(getIndex(), n, M);

            if (FIB[n].size() && FIB[n].at(0).size() <= D)//if D=-1 set is not_full
                dset[n] = FIB[n].at(0).size();
        }
    }

    //Node statistics on a file (rather than on a single chunk)
    //The process is more lightweight and it should not waste cpu resources
    if (!node_stat.is_open())
    {
        string node_stat_file = ev.getConfig()->getConfigValue("output-vector-file");
        node_stat_file.replace(node_stat_file.find(".vec"), 10, ".node_stat");

        remove(node_stat_file.c_str());
        node_stat.open((node_stat_file).c_str(), std::ios::app );
    }

    // local distribution, this file contain the local requests for each fileID(Zipf) and the hitrate of each node
    if (!node_local.is_open())
    {
        string node_local_file = ev.getConfig()->getConfigValue("output-vector-file");
        node_local_file.replace(node_local_file.find(".vec"), 10, ".node_local");
        remove(node_local_file.c_str());
        node_local.open(node_local_file.c_str(), std::ios::app );
    }

    //reverse order because compare of all and closest result true in adaptive_closest and adaptive_all
    if (type_strategy.compare("all") == 0) //AdaptiveClosest solution of paper
        fwd = ALL;
    else if (type_strategy.compare("closest") == 0) //second solution of paper
        fwd = CLOSEST;

    //extraction of number of nodes
    N = getNodeCount();
    not_full = N;
    unstable = N;

    string convergence_type = par("convergence_type");
    if (convergence_type.compare("wc") == 0)
        convergence_type_wc = true;
    else
        convergence_type_wc = false;


    double percentile = par("convergence_zipf_percentile");

    if (convergence_type_wc)
    {
        event = new cMessage("event");
        scheduleAt(simTime() + TC, event);
        phase_ended = false;
        //load the convergence zipf percentile
    }
    else
    {
        percentile = 1; //if convergence_type is avg
    }


    //load the convergence zipf percentile
    global_generator *glob_generator = (global_generator *)this->getParentModule()->getModuleByRelativePath("global_generator");

    zipf_limit = glob_generator->zipf_search(percentile);


    // Initialization ended


    //state of node
    sim_state = WARM_UP;


    //Cache warmup
    //if (warmup && ContentStore->warmup( content->getCatalog() )){
    if (warmup)
    {
        ContentStore->warmup(content->getCatalog());
        cout << getIndex() << "] warmed up!" << endl;
        if (!convergence_type_wc)
            sim_state = TRANSIENT;
        //else we set the state in stabilize function

        not_full = 0;
        phase_ended = true;
        stat->fullStamping(getIndex());
    }
}


void ccn_node::stabilize()
{
    //collect hit rate only if the state is transient

    if (sim_state == WARM_UP)
    {
        if (not_full == 0)
        {
            sim_state = TRANSIENT;
            hit = 0; miss = 0;
            //cout<<"NODE["<<this->getIndex()<<"] TRANSIENT Phase"<<endl;
            phase_ended = false;
        }

    }
    else if (sim_state == TRANSIENT)
    {
        double hit_rate_global;
        hit_rate_global = hit / (hit + miss);

        hold = hit + miss;

        if (!phase_ended and sim_state == TRANSIENT)
        {
            samples.push_back(hit_rate_global);
            if (samples.size() == 600)  //sample each 60 seconds every 100ms
            {
                if (variance(samples) <= convergence_threshold)
                {
                    phase_ended = true;
                    unstable = unstable - 1;
                    stat->nodeStabilize(this->getIndex());
                    cout << "NODE[" << this->getIndex() << "] Stable, REMAINING:" << unstable << endl;

                }//stabilize
                samples.clear();
            }//samples
        }//!phase_ended

        if (unstable == 0)
        {
            sim_state = STEADY;
            //cout<<"CCN_NODE["<<this->getIndex()<<"] "<<" Hitrate: "<<hit/(hit+miss)<<" at second: "<<simTime()<<" STEADY PHASE"<<endl;
            //node_local<<"CCN_NODE["<<this->getIndex()<<"]"<<" Hitrate: "<<hit/(hit+miss)<<" at second: "<<simTime()<<" STEADY PHASE"<<endl;
            hit = 0;
            miss = 0;
        }

    }//sim_state
}


void ccn_node::local_cache_miss(uint64_t chunk)
{

    uint32_t name =::getName(chunk);

    if (name < zipf_limit or sim_state == STEADY)
    {
        miss++;
    }
}

void ccn_node::local_cache_hit(uint64_t chunk)
{
    uint32_t name =::getName(chunk);
    if (name < zipf_limit or sim_state == STEADY)
    {
        hit++;
    }
}

//Core function of a ccn_node
void ccn_node::handleMessage(cMessage *in)
{
    //only if convergence_type_wc
    if (in == event)
    {
        if (sim_state != STEADY)
        {
            stabilize();
            if (sim_state == STEADY)
                scheduleAt(simTime() + time_steady_state, event);
            else
                scheduleAt(simTime() + TC, event);
        }
        else
        {
            delete in;
            stat->nodeFinalize(this->getIndex());
        }
        return;
    }

    //in!=event
    ccn_data *data_msg;
    ccn_interest *int_msg;

    int type = in->getKind();
    switch (type)
    {
        //On receiving interest
    case CCN_I:
        _interests++;

        int_msg = (ccn_interest *) in;

        int_msg->setHops(int_msg->getHops() + 1);//beta^d to calculate the probability(adaptive all)

        manage_interest (int_msg);
        delete in;
        break;

        //On receiving data
    case CCN_D:
        _data++;

        data_msg = (ccn_data * ) in;
        //One hop more from the last caching node (useful for distance policy)
        data_msg->setD(data_msg->getD() + 1);
        //On hop more from the origin of the node (useful for stretching metrics)
        data_msg->setHops(data_msg->getHops() + 1);

        manage_data(data_msg);

        addChunkCount(data_msg->getSenderModule()->getIndex());
        delete in;
        break;

        //On receiving local request for a content
    case CCN_L:
        generate_interest(((local *)in)->getFile_name());
        delete in;
        break;

        //
    case CCN_A:
        manage_localChunk(((access_data *)in)->getChunk());
        delete in;

    }//case
}

void ccn_node::recordThroughput()
{
    for (int source = 0; source != chunkCountRecvFromEachNode.size(); ++source)
    {
        stat->throughput(source, getNodeID(), chunkCountRecvFromEachNode[source] / simTime());
    }
}

//Per node statistics printing
void ccn_node::finish()
{
    recordThroughput();

    char name[10];
    sprintf ( name, "INT[%d]", getIndex() );
    recordScalar(name, _interests);

    sprintf ( name, "DATA[%d]", getIndex() );
    recordScalar(name, _data);
    stat->inspectCache(ContentStore, getIndex());

    if ( node_stat.is_open() )
        node_stat.close();

    if (getIndex() == 0) //only ccn_node[0] print repositories node
    {
        vector<uint32_t> repositories = content->getRepositories();
        node_local << "List of Repositories: " << repositories.size() << '\t' << endl;

        cout << endl << "List of Repositories: " << repositories.size() << '\t' << endl;
        for (int i = 0; i < repositories.size(); i++)
        {
            node_local << "ccn_node " << repositories[i] << '\t' << endl;
            cout << "CCN_NODE[" << repositories[i] << "]" << endl;
        }
        cout << endl;
    }
    vector<uint32_t> repositories = content->getRepositories();

    //print the hit rate per node
    node_local << endl << "CCN_NODE[" << getIndex() << "]\t HitRate " << hit / (hit + miss);
    cout << "CCN_NODE[" << getIndex() << "]\t HitRate " << hit / (hit + miss) << endl;
    if (getIndex() == N - 1)
    {
        node_local.close();
    }
}

//Manage incoming interests:
//if an interest for a data file arrives:
//a) check in your cache.
//b) check if you are the source for that data.
//c) Put the interface within the PIT.
//
void ccn_node::manage_interest(ccn_interest *int_msg)
{
    uint64_t chunk = int_msg ->getChunk();
    uint32_t name = ::getName(chunk);
    file f = content->getData(name);
    request_file[name]++;
    uint32_t P = minimum_distance_from_source(name, f.repository);

    if (ContentStore->lookup(chunk)) //a) Check in your cache
    {

        stat->cache_hit(P, chunk); //stat->cache_hit(P);
        local_cache_hit(chunk);
        ccn_data *data_msg = composeCData(chunk);
        data_msg->setRepository(int_msg->getRepository());
        send(data_msg, "face$o", int_msg->getArrivalGate()->getIndex());
    }
    else if ( P == 0 )  //b) Look locally (only if you own a repository)
    {
        stat->cache_miss(P, chunk); //stat->cache_miss(P);
        local_cache_miss(chunk);//local stabilize
        ccn_data *data_msg = composeCData(chunk);

        if (decision_policy->isDataToCache( data_msg )) //Decide if message contains data to cache
        {

            ContentStore->storeData(chunk);
            if ( !phase_ended and ContentStore->isFull() and sim_state == WARM_UP)
            {
                stat->fullStamping(getIndex());
                not_full--;
                cout << "CCN_NODE[" << this->getIndex() << "]: FULL CACHE, REMAINING: " << not_full << endl;
                phase_ended = true;
            }
        }

        data_msg->setRepository(int_msg->getRepository());
        send(data_msg, "face$o", int_msg->getArrivalGate()->getIndex());
    }
    else   //c) Put the interface within the PIT
    {

        stat->cache_miss(P, chunk); //stat->cache_miss(P);
        local_cache_miss(chunk);//local stabilize
        uint32_t pit_size = PIT[chunk].size();

        //Check if there are other interest from the same face for the same chunk
        if ( find( PIT[chunk].begin(), PIT[chunk].end(), int_msg->getArrivalGate()->getIndex() ) == PIT[chunk].end() )
            PIT[chunk].push_back(int_msg->getArrivalGate()->getIndex());

        //If you are adding an entry in the PIT "send" the interest
        if ( pit_size == 0 )
        {
            //send(int_msg)
            //if multipath second solution of paper send only with probability beta^d
            send( int_msg -> dup() , "face$o", int_msg->popPath() );
        }
    }
}


//Manage incoming data
void ccn_node::manage_data(ccn_data *data_msg)
{
    uint64_t chunk = data_msg ->getChunk();
    uint32_t name = ::getName(chunk);//filename
    file f = content->getData(name);

    hash_map < uint64_t, vector<int> >::iterator pitIt = PIT.find(chunk);
    uint32_t P = minimum_distance_from_source(name, f.repository);

    //If someone had previously requested the data
    if ( pitIt != PIT.end() )
    {

        if (decision_policy->isDataToCache( data_msg )) //Decide if message contains data to cache
        {

            ContentStore->storeData(chunk);
            if ( !phase_ended && ContentStore->isFull() && sim_state == WARM_UP)
            {
                not_full--;
                cout << "CCN_NODE[" << this->getIndex() << "]: FULL CACHE, REMAINING: " << not_full << endl;
                stat->fullStamping(getIndex());
                phase_ended = true;

            }
            data_msg->setD(0);

        }

        vector<int> interfaces = pitIt->second;//get interface list
        PIT.erase(chunk); //erase pending interests for that data file

        for (vector<int>::iterator i = interfaces.begin(); i != interfaces.end(); i++)
        {

            if ( (*i) != -1) //-1 local dummy interface
            {

                send (data_msg->dup(), "face$o", *i ); // if not local follow bread crumbs back

            }
            else    //in this case we have request that chunk
            {
                //saving the arrival gate if this is the best gate(minimum of hops)

                pending_map[name].missing_chunks--; //waiting for the next chunk (window size = 1 )
                pending_map[name].tot_distance += (data_msg->getHops() > P ? P : data_msg->getHops());

                stat -> stretch(P, data_msg->getHops());

                if (pending_map[name].missing_chunks) //if the file is not yet completed
                {
                    chunk++;//next chunk

                    if (ContentStore->lookup(chunk))
                    {
                        stat->cache_hit(P, chunk); //stat->cache_hit(P);
                        local_cache_hit(chunk);//local stabilize
                        //stat->stretch(P,0);
                        //pending_map[name].missing_chunks--;


                        access_data *acc_data = new access_data("access_data", CCN_A);
                        acc_data->setChunk(chunk);
                        scheduleAt(simTime() + ACCESS_DELAY_FIX + ACCESS_DELAY_RAND, acc_data); //1ms + uniform(0,0.1)ms
                    }
                    else
                    {
                        stat->cache_miss(P, chunk); //stat->cache_miss(P);
                        local_cache_miss(chunk);//local stabilize
                        PIT[chunk].push_back(-1);
                        sendInterests(chunk, f.repository); //send Interests for new chunk
                    }


                }
                else     //if the file is completed
                {

                    node_stat << name << '\t' << pending_map[name].tot_distance * ( 1. / (f.size * P) ) << '\t' << getIndex() << endl;
                    pending_map.erase(name);//delete the entry from the list of pendent files

                }

            }
        }
    }
}

//
void ccn_node:: manage_localChunk(uint64_t chunk)
{

    uint32_t name = ::getName(chunk);//filename
    file f = content->getData(name);
    uint32_t P = minimum_distance_from_source(name, f.repository);
    //in this case we have request that chunk
    //saving the arrival gate if this is the best gate(minimum of hops)
    pending_map[name].missing_chunks--; //waiting for the next chunk (window size = 1 )
    pending_map[name].tot_distance += 0;
    stat -> stretch(P, 0);
    if (pending_map[name].missing_chunks) //if the file is not yet completed
    {
        chunk++;//next chunk
        if (ContentStore->lookup(chunk))
        {
            stat->cache_hit(P, chunk); //stat->cache_hit(P);
            local_cache_hit(chunk);//local stabilize
            //stat->stretch(P,0);
            ////pending_map[name].missing_chunks--;
            //
            access_data *acc_data = new access_data("access_data", CCN_A);
            acc_data->setChunk(chunk);
            scheduleAt(simTime() + ACCESS_DELAY_FIX + ACCESS_DELAY_RAND, acc_data); // 1ms + uniform(0,0.1)ms
        }
        else
        {
            stat->cache_miss(P, chunk); //stat->cache_miss(P);
            local_cache_miss(chunk);//local stabilize
            PIT[chunk].push_back(-1);
            sendInterests(chunk, f.repository); //send Interests for new chunk
        }

    }
    else     //if the file is completed
    {
        node_stat << name << '\t' << pending_map[name].tot_distance * ( 1. / (f.size * P) ) << '\t' << getIndex() << endl;
        pending_map.erase(name);//delete the entry from the list of pendent files
    }

}

//Generate interest requests coming from local or global request generator
void ccn_node::generate_interest(uint32_t name)
{
    uint64_t chunk = generateChunk(name, 0);
    file f = content->getData(name);

    uint32_t P = minimum_distance_from_source(name, f.repository);

    if (pending_map.find(name) == pending_map.end() )  //if the content has not been requested yet
    {
        pending_map[name] = file_property(f.size, 0, 0, 0, false); //0 rep 0 hops not valid

        //Check if you are attached to the source of the file
        if ( P == 0)
        {
            node_local << "CCN Error " << this->getIndex() << " repo: " << f.repository << endl;
            cout << endl << endl << endl << "ERROR" << endl << endl << endl;
            return;
        }

        if (ContentStore->lookup(chunk))
        {
            stat->cache_hit(P, chunk); //stat->cache_hit(P);
            local_cache_hit(chunk);//local stabilize
            //stat->stretch(P,0);
            //pending_map[name].missing_chunks--;

            access_data *acc_data = new access_data("access_data", CCN_A);
            acc_data->setChunk(chunk);
            scheduleAt(simTime() + ACCESS_DELAY_FIX + ACCESS_DELAY_RAND, acc_data); //1ms + uniform(0,0.1)ms
        }
        else
        {
            stat->cache_miss(P, chunk); //stat->cache_miss(P);//Cache miss for the last not completed chunk
            local_cache_miss(chunk);//local stabilize
            PIT[chunk].push_back(-1); //Insert a local dummy interface (-1) within PIT
            sendInterests(chunk, f.repository); //Send interests through the network
        }
    }
}



//Given a content name, return the minimum (if there are more) distance from the repository who possesses such that content
int ccn_node::minimum_distance_from_source(uint32_t content_name, uint32_t *who_has)
{
    int P = 0;
    int cur_len = 0;

    //If you are (connected to) the repository for content_name return 0
    if (find(who_has, who_has + Replicas, getIndex()) != who_has + Replicas)
        return 0;

    //Otherwise
    if (Replicas > 1)
    {
        P = VERY_BIG_VALUE;
        //Find shortest path to the repository if there are more replicas of the same content
        for (uint32_t i = 0; i < Replicas; i++)
        {
            cur_len = FIB[ who_has[i] ].at(0).size();
            if (cur_len < P)
                P = cur_len;
        }
    }
    else
    {
        int fibsize = FIB.size();
        int fibwhosize = FIB[who_has[0]].size();
        P = FIB[who_has[0]].at(0).size();//Return the single shortest path
    }

    return P;
}

//Return the distance set of the node
const map<uint32_t, uint32_t> &ccn_node::getDistanceSet()
{
    return dset;
}


//Send interests for a chunk, knowing where the source for the
//content file is located
void ccn_node::sendInterests(uint64_t chunk, uint32_t *who_has)
{

    vector<deque <int> > _unsorted_paths;
    switch (fwd)
    {
        //Singlepath  | closest
    case CLOSEST:
        sendSinglePath(who_has, chunk);
        break;

        //Multipath ALL
    case ALL:
        for (uint32_t i = 0; i < Replicas; i++)
            _unsorted_paths.push_back( FIB[ who_has[i] ].at(0) );
        sendSortedInterests(_unsorted_paths, chunk);
        break;
    }
}

bool _lencmp (deque <int> a, deque <int> b)
{
    return (a.size() < b.size());
}

//
void ccn_node::sendSortedInterests(vector < deque <int> > unsorted_paths, uint64_t chunk )
{
    sort(unsorted_paths.begin(), unsorted_paths.end(), _lencmp);

    for (int p = 0; p < unsorted_paths.size(); p++)
    {

        ccn_interest *interest = composeInterest(chunk);
        interest->setPath ( unsorted_paths[p] );
        int outif = interest->popPath();
        sendDelayed (interest, 0.0000001 * (simtime_t)(p), "face$o", outif);

    }
}



//
void ccn_node::sendSinglePath(uint32_t *who_has, uint64_t chunk )
{
    //We send each chunk only on the shortest path among different paths
    //toward different content server
    int outif = 0;
    uint32_t pat_len = 10000;
    deque <int> path;

    ccn_interest *interest = composeInterest(chunk);


    for (uint32_t i = 0; i < Replicas; i++)
    {
        if (FIB[ who_has[i] ].at(0).size() < pat_len)
        {
            pat_len = FIB[ who_has[i] ].at(0).size();
            path = FIB[ who_has[i] ].at(0);
            interest->setRepository(who_has[i]);
        }
    }
    interest->setPath( path );//
    outif = interest->popPath();
    send(interest, "face$o", outif);
}

//Compose an interest packet
ccn_interest *ccn_node::composeInterest(uint64_t chunk)
{
    ccn_interest *interest = new ccn_interest("interest", CCN_I);
    interest->setChunk(chunk);
    interest->setHops(0);//adaptive_all
    return interest;
}


//Compose a data response packet
ccn_data *ccn_node::composeCData(uint64_t response_data)
{
    ccn_data *data = new ccn_data("data", CCN_D);
    data -> setChunk (response_data);
    data -> setD(0);
    data -> setHops(0);
    return data;
}
