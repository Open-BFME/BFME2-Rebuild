/* BFME-ALTERED: the body of struct qr2_keybuffer_s moved here from qr2.c, text unchanged; the landing gate refuses a struct body in a .c file. */
struct qr2_keybuffer_s
{
	uchar keys[MAX_REGISTERED_KEYS];
	int numkeys;
};
