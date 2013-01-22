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
#include "net_inspector.h"
Define_Module(NetworkInspector);


void NetworkInspector::initialize()
{
    cTopology topo;
    string topologyName = ev.getConfig()->getConfigValue("network");

    types.push_back("modules.ccn_node");
    topo.extractByNedTypeName(types);

    f.open( ("./optimal/" + topologyName).c_str(), fstream::out );

    int N = topo.getNumNodes();
    cout << N << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == j) continue;
            calculatePath(i, j, 1);
        }
    }
    endSimulation();

}

void NetworkInspector::calculatePath(int origin, int destination, int C)
{

    cTopology topo;
    topo.extractByNedTypeName( types );
    std::vector< std::vector<int> > paths;
    cTopology::Node *to = topo.getNode( destination ); //destination node
    cTopology::Node *from = topo.getNode( origin ); //origin nod

    for ( int c = 0; c < C; c++ )
    {

        f << origin << ":" << destination << ":";
        cTopology::Node *node = topo.getNode( origin ); //iterator node
        cTopology::LinkOut *linkOut;
        topo.weightedSingleShortestPathsTo( to );
        if ( from->getNumPaths() == 0 )
            break;

        vNode nodes;
        vLink olinks;//links to disable
        while ( node != to )  //path storing
        {

            nodes.push_back( node );
            linkOut = node -> getPath( 0 );
            olinks.push_back( linkOut );
            node = linkOut -> getRemoteNode();

        }

        std::vector<int> p;
        for ( vLink::iterator i = olinks.begin(); i != olinks.end(); i++ )
            p.push_back( (*i )->getLocalGate()->getIndex() );


        for (vInt::iterator i = p.begin(); i != p.end(); i++)
            f << *i << ":";
        f << endl;

        olinks[0]->setWeight( 100 );
        for ( int n = 1; n < ( int )nodes.size(); n++ )
        {
            nodes[n]->setWeight( 100 );
            olinks[n]->setWeight( 100 );
        }
    }
}
