/*
//	@(#) netaddr.h
//	@(License-CC0)
*/
# if	!defined(NETADDR_H)
# define	NETADDR_H
# include	<limits.h>
# include	<errno.h>
# include	<arpa/inet.h>


// Just to remind us that we only work with host byte order
typedef	in_addr_t	haddr_t;

enum    { IPV4_ADDR_BITS  = sizeof(haddr_t)*CHAR_BIT, };
enum	{ ADDR_OK = 1, ADDR_BAD = 0, ADDR_ERR = -1, };

static  inline haddr_t  netmask (int n) {
        return  (~0u) << (IPV4_ADDR_BITS-n);
}

static inline int bit (int n, haddr_t x) {
	return	x & (1u<<n)?1:0;
}

int    	inet_ptoh (int family, const char* src, void* dst);
const char*  inet_htop (int family,const void *src,char *dst,socklen_t size);


# endif
