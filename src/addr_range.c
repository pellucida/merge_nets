/*
// @(#) addr_range.c - cover [begin..end] addresses (IPv4) with net/mask pairs.
// @(License-CC0)
*/
# include	"netaddr.h"
# include	"addr_range.h"

static  inline  long    powerof2 (int n) {
        return  (1ul)<<n;
}

static	int	z (long  a, long  b) {
	int	i	= 0;
	while ((a % powerof2 (i+1)) == 0 && (a + powerof2 (i+1) - 1) <= b) {
		++i;
	}
	return	i;
}

int	range_list_make (haddr_t A, haddr_t B, range_list_t* r) {
	long	b	= (long)A - 1;

	do	{
		long	a	= b+1;
		int	n	= z (a, B); 
		range_list_append (r, (haddr_t)a, IPV4_ADDR_BITS - n);
		b	= b + powerof2 (n);
	} while (b != B);
}
