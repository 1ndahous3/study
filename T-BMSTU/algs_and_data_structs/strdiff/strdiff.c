int strdiff(char *a, char *b) {
	int bit, byte;
	for (byte = 0; a[byte] != '\0' || b[byte] != '\0'; byte++) {
		char buff = a[byte] ^ b[byte];
		for (bit = 0; bit < 8; bit++)
			if (buff & (1 << bit) == 1 << bit)
				return byte * 8 + bit;
	}
	return -1;
}
