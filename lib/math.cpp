
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

int itoa(int value, char *sp, int radix)
{
	char tmp[16];
	char *tp = tmp;
	int i;
	unsigned v;
	
	int sign = (radix == 10 && value < 0);
	if (sign)
		v = -value;
	else
		v = (unsigned)value;
	
	while (v || tp == tmp)
	{
		i = v % radix;
		v /= radix;
		if (i < 10)
			*tp++ = i+'0';
		else
			*tp++ = i + 'a' - 10;
	}
	
	int len = tp - tmp;
	
	if (sign)
	{
		*sp++ = '-';
		len++;
	}
	
	while (tp > tmp)
		*sp++ = *--tp;
	*sp++ = 0x0;
	return len;
}

int ipow(int base, int exp)
{
	if (exp < 0) {
		return 0;
	}
	int result = 1;
	while (exp)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}
	
	return result;
}