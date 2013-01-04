message ccn_interest{
	@customize(true);
	unsigned long chunk;
	double hops = 0;
	bool closest=false;
	abstract int path[];
	unsigned long repository;
}
