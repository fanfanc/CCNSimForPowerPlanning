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
#ifndef TWO_CACHE_H_
#define TWO_CACHE_H_

#include "base_cache.h"
#include "utils/ccn_utils.h"

using namespace std;

//Power of two replacement: elements are pushed back into the cache
//If the cache if filled replacement is fulfilled in this way:
// a) two random elements are taken from the cache 
// b) the "more popular" element is replaced
//
class TWO_Cache: public Cache{
    public:
	TWO_Cache (uint32_t s) : Cache(s){;}
	/*! Store chunk in cache
	*   \param chunkID
        *   \sa Version 0.0 
        *  */
	virtual void storeData(uint64_t);
	/*! Lookup chunk
	*   \param chunkID
        *   \sa Version 0.0 
        *  */
	virtual bool lookup(uint64_t);

    private:
	map<uint64_t,uint64_t> freq;
};



void TWO_Cache::storeData(uint64_t chunk){

   cache[chunk] = true;

   if (deq.size() == size){

       //Random extraction of two elements
       uint32_t pos1 = intrand( deq.size() );
       uint32_t pos2 = intrand( deq.size() );
       uint32_t pos;

       uint64_t toErase1 = deq.at(pos1);
       uint64_t toErase2 = deq.at(pos2);
       uint64_t toErase;

       uint32_t name1 = getName(toErase1);
       uint32_t name2 = getName(toErase2);


       //Comparing content popularity (a realistic implementation can employ a the freq map
       if (name1 > name2){

	   toErase = toErase2;
	   pos = pos2;

       }else if (name1 == name2){
	   if ( intrand(2) == 0 ){
	       toErase = toErase1;
	       pos=pos1;
	   }else{
	       toErase=toErase2;
	       pos=pos2;
	   }
       }else{
	   toErase = toErase1;
	   pos = pos1;
       }

       //Erase the more popular elements among the two
       deq.at(pos)=chunk;
       cache.erase(toErase);
   }else
       deq.push_back(chunk);


}


bool TWO_Cache::lookup(uint64_t chunk){
    return (cache.find(chunk)!=cache.end());
}


#endif
