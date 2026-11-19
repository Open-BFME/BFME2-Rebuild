/* BFME-ALTERED: the body of struct qr2_buffer_s moved here from qr2.c, text unchanged; the landing gate refuses a struct body in a .c file. */
struct qr2_buffer_s
{
	char buffer[MAX_DATA_SIZE];
	int len;
};
