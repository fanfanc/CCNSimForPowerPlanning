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
#include "request_generator.h"
#include "local_m.h"
Register_Class(request_generator);
vector<double> request_generator::cdfZipf(0);

void request_generator::initialize(){
    vector <string> ccn_types;
    cTopology topo;
	
    limit=0;

    ccn_types.push_back("modules.ccn_node");
    topo.extractByNedTypeName (ccn_types);
    int N = topo.getNumNodes();
    int node = intrand(N);

    if ( !gate("reqPort")->isConnected() ) { //If you are not already connected

	//try to connect with a random node of the network
	cGate *g =topo.getNode(node)->getModule()->gate("reqPort");
	while (g->isConnected()){
	    node = intrand(N);
	    g = topo.getNode(node)->getModule()->gate("reqPort");
	}
	gate("reqPort")->connectTo(g);

    }



}


void request_generator::activity(){
    //Initialize base parameters and modules
    content = (content_distribution*)this->getParentModule()->getModuleByRelativePath("content_distribution");
    ccn = (ccn_node*) gate("reqPort")->getNextGate()->getOwnerModule();

    //Initialize generator parameters
    F = content->getCatalogSize();
    
    lambda = par ("lambda");
    alpha = par ("alpha");
    q = par ("q");

    //Initialize zipf distribution cdf
    initializeZipf();

    uint32_t file_name = 0;
    int i = 0;
    //while ( (true && R == 0) || (i < R && R != 0) ){
    while(true){	
    //File requests are distributed through a zipf function
        file_name = zipf_local();

	//Local request to the ccn_node associated
        local *localReq = new local("localReq",CCN_L);
	localReq->setFile_name(file_name);
        send (localReq, "reqPort");
	i++;

	//Poisson arrival process
        wait (exponential(1/lambda));
    }

}




//Initialize the vector representing the zipf cdf distribution. As this vector can be of really high dimension 
//the initialization is done just one time for every request generator who needs it
void request_generator::initializeZipf(){

    //Return if the Zipf cdf has been already initialized
    if (cdfZipf.size() != 0)
	return;

    //Otherwise initialize it
    cout << "Start Zipf cdf initialization..." << endl;
    cdfZipf.resize(F + 1);
    cdfZipf[0] = -1;

    //Normalization constant computation
    c = 0;          
    for (unsigned long i=1; i<=F; i++){
	c += (1.0 /  pow(i+q,alpha));
	cdfZipf[i] = c; 
    }
    c = 1.0 / c;

    //Normalize Zipf distribution
    for (vector<double>::iterator it=cdfZipf.begin() + 1;it!=cdfZipf.end();it++){
    //for (unsigned long i=1; i<=F; i++){
	(*it) *= c;
    }
    cout << "Zipf initialized" << endl;

}


//Return a random value following a zipf distribution
uint32_t request_generator::zipf_local(){

    
    double x; // Uniform random number (0 < x < 1)
    uint32_t upper, lower,atry, last_try;
    
    x = dblrand();
    lower = -1;
    upper = cdfZipf.size()-1;
    atry = -1;
    last_try = -1;

    //binary search to find the nearest value y whose cdf is x
    while (1){
	atry = floor((lower+upper+1)/2);

	if (last_try == atry)
	    break;

	if (cdfZipf[atry] >= x)
	    upper=atry;
	else
	    lower = atry-1;

	last_try = atry;

    }

    return upper;
}
//Search the zipf value 
uint32_t request_generator::zipf_search(double p){


    if (limit!=0)// in this case we have received another request from another node before
	return limit;

     if (cdfZipf.size() == 0)//intialize the zipf
	 initializeZipf();



    uint32_t upper, lower,atry, last_try;
    

    lower = -1;
    upper = cdfZipf.size()-1;
    atry = -1;
    last_try = -1;

    //binary search to find the nearest value y whose cdf is x
    while (1){
	atry = floor((lower+upper+1)/2);

	if (last_try == atry)
	    break;

	if (cdfZipf[atry] >= p)
	    upper=atry;
	else
	    lower = atry-1;

	last_try = atry;

    }

    return upper;



}

