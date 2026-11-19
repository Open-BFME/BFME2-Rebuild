/* BFME-ALTERED: the body of struct statsgame_s moved here from gstats.c, text unchanged; the landing gate refuses a struct body in a .c file. */
struct statsgame_s 
{
	int connid;
	int sesskey;
	int usebuckets;
	bucketset_t buckets;
	char challenge[9];
	DArray playernums; //for player number translation
	DArray teamnums; //for team number translation
	int totalplayers, totalteams;
	gsi_time sttime;

};
