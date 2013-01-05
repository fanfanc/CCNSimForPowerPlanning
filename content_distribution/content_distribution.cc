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
#include "content_distribution.h"
#include <algorithm>

Register_Class(content_distribution);



//Initialize the catalog, the repository, and distributes contents among them.
void content_distribution::initialize()
{
    //Initialize node parameters
    uint32_t N = (uint32_t) getAncestorPar("n"); //Number of nodes
    uint32_t F = par("F"); //Number of files within the system
    //uint32_t R = par("replicas"); //Degree of replication among different repositories
    uint32_t avgC = par("avgC"); //Average chunk size
    int repository_num = par("repository_num"); //Number of repositories (specifically ccn_node(s) who a repository is connected to)
    string repository_policy = par("repository_policy");

    catalog.resize(F + 1); // initialize content catalog

    // vector<uint32_t> repositories; // this vector contains the repositories within the system

    if (repository_policy.compare("ugc") == 0)
    {
        replicas = 1;
        int cardinality = N;
        int new_rep;
        while ( cardinality > 0 )
        {
            new_rep = intrand(N);
            if (find (repositories.begin(), repositories.end(), new_rep) == repositories.end())
            {
                repositories.push_back(new_rep);
                cardinality--;
            }
        }
    }
    else //pop with cardinality r
    {
        replicas = repository_num;
        int cardinality = repository_num;
        int new_rep;
        while ( cardinality > 0 )
        {
            new_rep = intrand(N);
            if (find (repositories.begin(), repositories.end(), new_rep) == repositories.end())
            {
                repositories.push_back(new_rep);
                cardinality--;
            }
        }
    }

    cout << "Start content initialization..." << endl;
    for (uint32_t f = 1; f <= F && N != 0 ; f++)
    {
        if (avgC > 1)
            catalog[f].size = geometric(1.0 / avgC) + 1;
        else
            catalog[f].size = 1;

        catalog[f].polarity = -1;
    }

    cout << "Start repository allocation..." << endl;
    big_rep = (uint32_t *) malloc (sizeof (uint32_t) * replicas * (F + 1)); //create a big R * (F+1) array (we use malloc in order to minimize the overhead)
    for (uint32_t f = 1; f <= F && N != 0 ; f++)
    {
        catalog[f].repository =
            compose_random_vector(big_rep + f * replicas, replicas, repositories); //initialize that part of memory with a ranodom vector containing the repositories where is possible to find the file
        cout << catalog[f].repository[0] << endl;
    }

    cout << "Content initialized" << endl;
}


int content_distribution::getNumberReplicas()
{
    return replicas;
}


//generate a random vector R-dimensioned, starting at address random_v whose components are taken by vector rep_set
uint32_t *content_distribution::compose_random_vector(uint32_t *random_v, uint32_t R, const vector<uint32_t> &rep_set)
{
    for (uint32_t j = 0; j < R; j++)
        random_v[j] = -1; //Initialize vector

    uint32_t repository;
    uint32_t i = R;

    while (i)
    {
        repository = rep_set[intrand(rep_set.size())];
        if ( find(random_v, random_v + R, repository) == random_v + R )
            random_v[ --i ] = repository;
    }
    return random_v;

}

//Return the f-th file within the the catalog
file content_distribution::getData(uint32_t f)
{
    return catalog[f];
}

//Return the cataolg size
uint32_t content_distribution::getCatalogSize()
{
    return catalog.size() - 1;
}

//Return the entire catalog (as a reference in order to avoid memory overhead in copying big data structures)
vector<file> &content_distribution::getCatalog()
{
    return catalog;
}

// Return all repositories
vector<uint32_t> content_distribution::getRepositories()
{
    return repositories;
}
