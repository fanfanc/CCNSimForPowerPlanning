/*
 * ccnSim 
 *	ccnSim is a scalable chunk-level simulator of Content Centric Networks (CCN), 
 * 	that we developed in the context of ANR Connect (http://www.anr-connect.org/)
 * 
 * Contact us:
 * 	Email: ccnsim@lincs.fr 
 *  	Web: http://www.enst.fr/~drossi/ccnSim
 * 
 * People:
 *  	Giuseppe Rossini (lead developer)
 * 	Raffele Chiocchetti (developer)
 * 	Dario Rossi (occasional debugger) 
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
 *       +#####`####    							    `###,   									   
 *       +  #+ ,###.    							    ,###    										  
 *         +#+ ####     							    ;###    	    
 *     +    #+;###      							    ###`    	    
 *     ##;;## ####      							   .###     							 
 *       ###  ###.      							    ###`    								
 *                      		       `.;###:, 			    ###     								
 *           ###:       		      ;########  .##			    ###
 *          #####       		     ##########  :##`			    ###
 *          #####       		     ###;    `;  ```			    ###
 *           +##:          #####` ###`;###  `###,	 ###  ###`###` ;###	    ##;
 *              .        ,######  ########:  #######.	 ##,  #############,	   `###     	 
 *           :#.        `###`  .  ###. `###  #########. .##`  ###` #### `###	   .###     	 
 *          `###        ###	 `##;	##:   ######### ;##  `##:  ###   ##:	   .##+     	   
 *           ###        ###	 .##   `##`	.;##### ###  ,##`  ###  `##`	   .##,     	   
 *         #:###        ###	 ###   ,##	   .### ###  ###   ##;  ,##	   .##.     	   
 *        ## ###`       ###	 ###   ;##  ##`    ;### ##:  ###   ##.  ;##	   .###     	   
 *       ##+ ,##;       #######  ###   ###  ##########:.##`  ###  .##	###		    	   
 *     :#,.#..###;       ######  ##,   ### `#########: ;##   ##,  ;##	###		    	   
 *    +##+#+,,####        `		     `,;##;`	    `			   +##+     	    
 *        #:  :###.     							,#####      	    
 *        +.   ;.       							;#####      	  
 *               ,###   							.#####      	  
 *               +###+  							`####`      	  
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
 *                 	   `	 	           #  						  
 *                 	####.	 		  `#:`` 					    
 *                 	+#`	 		   ####+					    
 *                 	#:	 		   #;  ##					    
 *                     +###	 		   +# +#					    
 *                 	 .#+	 		    +##.					    
 *                 	   +	 								    
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
#include "routing.h"
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

Register_Class(routing);

typedef vector<cTopology::LinkOut *> vLink;
typedef vector<cTopology::Node *> vNode; 

void routing::initialize(){

    types.push_back("modules.ccn_node");
    ff = par("fromFile");

}

//Calculate a set of C paths from an origin node to a destination node
vector<deque <int> > routing::calculatePath( int origin, int destination, int C ){

    // Paths are read from files if corresponging flag is enabled
    if (ff)
	return getPathFromFile(origin,destination,C);
    
    //Otherwise calculate them on demand
    cTopology topo;
    topo.extractByNedTypeName( types );
    vector< deque<int> > paths;

    cTopology::Node *to = topo.getNode( destination ); //destination node
    cTopology::Node *from = topo.getNode( origin ); //origin node

    for ( int c = 0; c < C; c++ ){	

	cTopology::Node *node = topo.getNode( origin ); //iterator node
	cTopology::LinkOut *linkOut;

	//Dijkstra on the weighted graph (at first iteration all links have unitary weight and all nodes have zero cost)
	topo.weightedSingleShortestPathsTo( to ); 
	if ( from->getNumPaths() == 0 )
	    break;

	vNode nodes; 
	vLink olinks; //links to disable
	while ( node != to ){  //path storing
		nodes.push_back( node );
		linkOut = node -> getPath( 0 );
		olinks.push_back( linkOut );
		node = linkOut -> getRemoteNode();
	}

	deque<int> p;
	for (vLink::iterator i =olinks.begin();i != olinks.end();i++ )
	    p.push_back( (*i )->getLocalGate()->getIndex());

	//if ( find( paths.begin(),paths.end(),p ) == paths.end() )
	paths.push_back( p );


	olinks[0]->setWeight( 100 );
	for ( int n = 1; n<( int )nodes.size();n++ ){
		nodes[n]->setWeight( 100 );
		olinks[n]->setWeight( 100 );

	}

    }

    return paths;

}

//Get paths from file
//Each path stored within the file (sited in the /optimal directory) is of the type
//
//	origin:destination:o1:o2:...:on:
//
//where o1...on represent the output ports index
//
//
vector<deque <int> > routing::getPathFromFile( int origin, int destination, int C ){

    ifstream fi;
    char row[100];
    char *path;
    int o,d;

    deque<int> dpath;
    vector<deque <int> > bucket;
    string topology_name = ev.getConfig()->getConfigValue("network");

   /*
    int repet=par("repetition");
    std::stringstream sstm;
    sstm << repet;
    string result = sstm.str();
*/	
	fi.open( ("./optimal/"+topology_name).c_str() );

    while (fi >> row && C > 0){

	dpath.clear();

	path = strtok(row,":");
	o = atoi(path); //origin node
	path = strtok(NULL,":");
	d = atoi(path); //destination node

	if (o == origin && d == destination){
	    //get the output ports in the case the path is the right one
	    path = strtok(NULL,":");
	    while (path !=NULL ){
		dpath.push_back(atoi(path));
		path = strtok(NULL,":");
	    }
	    bucket.push_back(dpath);
	    C--;
	}
	
    }
    fi.close();
    return bucket;
}
