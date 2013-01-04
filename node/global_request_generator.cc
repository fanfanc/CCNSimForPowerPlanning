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
#include <vector>

#include "content_distribution/content_distribution.h"
#include "messages/local_m.h"

#include "global_request_generator.h"

Define_Module(global_generator);

void global_generator::initialize(){

    lambda = par ("lambda");
    
    string  repo_policy=par ("repository_policy");
    repository_policy=repo_policy;
    //repository_policy=par("repository_policy");
    alpha  = par ("alpha");
    
    q = par ("q");
    D = par ("D");
    F = par ("F");


    if (lambda<=0)
	return;

    vector<string> ccn_types;
    ccn_types.push_back("modules.ccn_node");
    topo.extractByNedTypeName (ccn_types);

    N = topo.getNumNodes();
    setGateSize("globPort",N);

    //Connect to all global request gate of each node
    cGate *g;

    for (uint32_t n = 0; n < N; n++){
	g = topo.getNode(n)->getModule()->gate("globPort"); //ccn_node gate (globport)
	gate("globPort",n)->connectTo(g); //connect local globport to the ccn_node one
    }

    //Initialize zipf distribution cdf
    initializeZipf();


//cout<<"FINISH REQUEST"<<endl;

}

void global_generator::activity(){//activity utilizzato una sola volta e non come while sempre attivo

    if (lambda<=0)
	return;

    content = (content_distribution*)this->getParentModule()->getModuleByRelativePath("content_distribution");


   
   // F = content->getCatalogSize();
    vector<file> catalog = content->getCatalog();


    //clients is used if send an interest only to clients
    vector<int> clients;
    vector<uint32_t> repositories=content->getRepositories();
    int j=N;
    while(j>0){
	j--;
	if(find (repositories.begin(),repositories.end(),j) == repositories.end()) 
	    clients.push_back(j);
    }



    uint32_t k, 
	     i=0;

    map<uint32_t, uint32_t> dset;


    //while ( (true && R == 0) || (i < R && R != 0) ){
    while(true){
	uint32_t id = zipf_local();

	local *global_req = new local("localReq",CCN_L);
	global_req->setFile_name( id );

	if ( D == -1 || catalog[id].polarity==-1 ){

	   // k=clients[intrand(clients.size())];
	    //k = intrand(N);
	    (repository_policy.compare("ugc")==0) ? k=extract_client_ugc(id) : k=clients[intrand(clients.size())];
	    
	    catalog[id].polarity = (int) k;

	} else {

	    //obtain the distance set of the node (nodes located at a distance less than D hops)
	    dset = ( (ccn_node *)gate("globPort",catalog[id].polarity)->getNextGate()->getOwnerModule() )->getDistanceSet(); 

	    //uint32_t candidate =clients[intrand(clients.size())];
	    uint32_t candidate = 0;
	    //candidate = intrand(N);
	    (repository_policy.compare("ugc")==0) ? 
		candidate=extract_client_ugc(id) : 
		candidate=clients[intrand(clients.size())];

	    while ( true ){
		if (dset.find(candidate) != dset.end()){
		    if (dblrand()< p(dset[candidate]) )
			    break;
		}else if (candidate == catalog[id].polarity)
		    break;
		//candidate =clients[intrand(clients.size())];
		//candidate = intrand(N);
		(repository_policy.compare("ugc")==0) ? 
		    candidate=extract_client_ugc(id) : 
		    candidate=clients[intrand(clients.size())];
		
	    }
	    k = candidate;

	}


	send(global_req, "globPort", k);
        wait (exponential(1/lambda));
	i++; //number of requests generated

    }
}

//Case User generated content
//this function extract a client that not is repository for this file
uint32_t global_generator::extract_client_ugc( uint32_t id_file ){
	
    file f=content->getData(id_file);
    uint32_t id_client;
    bool search_complete=false;
    int Replicas=content->getNumberReplicas();//Replicas of file
    while(!search_complete) {
	id_client=intrand(N);
	for (int i=0; i < Replicas;i++){
	    if (f.repository[i]==id_client){
		    break;
	     }else if ( i == Replicas-1 && f.repository[i] != id_client)
		 search_complete=true;

	}
    }
    return id_client;
}

//return the probability of sending a request to a node d-distance from node k
double global_generator::p(uint32_t d){

    return ( 1 - d * 1./(D+1) );

}
