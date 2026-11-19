/* BFME-ALTERED: the body of struct HashImplementation moved here from hashtable.c, text unchanged; the landing gate refuses a struct body in a .c file. */
struct HashImplementation 
{
	DArray *buckets;
	int nbuckets;
	TableElementFreeFn freefn;
	TableHashFn hashfn;
	TableCompareFn compfn;
};
