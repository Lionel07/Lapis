
const char *itoh_map = "0123456789abcdef";

int getBits();

char* itoh(int i, char *buf) {
	int		n;
	int		b;
	int		z;
	int		s;
	
	s = getBits() / 4;
	
	for (z = 0, n = (s - 1); n > -1; --n)
	{
		b = (i >> (n * 4)) & 0xf;
		buf[z] = itoh_map[b];
		++z;
	}
	buf[z] = 0;
	return buf;
}