package modules;
simple content_distribution {
    parameters:
	double avgC = default (10);
    	int F =default (100);
	int repository_num = default(1);	
	string repository_policy = default("pop");
	
}

simple request_generator{
    parameters:
    	double lambda = default(1);
	double alpha = default (1);
	double q = default(0);
	int F =default(100);
	
    gates:
    	output reqPort;
}

simple global_generator{
    parameters:
    	double lambda = default(1);
	double alpha = default (1);
	double q = default(0);
	int F =default(100);
	
	int D = default (1);
	string repository_policy = default("pop");
    gates:
    	output globPort[];

}

simple repository{
    gates:
    	output repPort;
}

simple ccn_node {
    parameters:
	int S = default (100);
    	int F = default (100);
	double alpha = default (1);
	int D = default (1);
	string convergence_type = default("wc");
	
	double Beta = default(90);
	string cache_replacement_policy = default("lru");
	string decision_policy = default("always");
	string strategy_layer = default("closest");
	bool local_popularity = default(false);
	int M = default(1);
	bool warmup = default(false);

	double convergence_threshold_wc = default(0.001);
	double sim_time = default (60);
	double convergence_zipf_percentile = default(0.9);
    gates:
    	input reqPort;
	input globPort;

	inout face[];
}


simple statistics{
    parameters:
            double avgC = default (10);
	    double lambda = default(1);
	    int S = default (100);
	    
	    double sim_time = default(60);
	    double convergence_threshold_avg = default(0.00001);
	    string convergence_type = default("wc");
	}




simple NetworkInspector{
    parameters:
	double avgC = default (10);
	double lambda = default(1);
	int S = default (100);
	int r = default(1);
	double sim_time = default(60);

}

simple routing{
    parameters:
	bool fromFile = default (false);
    	string ccnName= default("");
	bool justDiscovery= default(false);
	
}

simple NetworkInspector{
}

