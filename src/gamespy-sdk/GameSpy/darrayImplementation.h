/* BFME-ALTERED: the body of struct DArrayImplementation moved here from darray.c, text unchanged; the landing gate refuses a struct body in a .c file. */
struct DArrayImplementation
{
	int count, capacity;
	int elemsize;
	int growby;
	ArrayElementFreeFn elemfreefn;
	void *list; //array of elements
};
