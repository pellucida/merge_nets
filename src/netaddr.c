/*
//	@(#) netaddr.c
//	@(License-CC0)
*/
# include	<ctype.h>
# include	<string.h>
# include	<stdio.h>
# include	<errno.h>
# include	<arpa/inet.h>
# include	"constant.h"
# include	"netaddr.h"

// _pton and _ntop versions that use host byte order. (IPV4 only) 
int     inet_ptoh (int family, const char* src, void* dst) {
        in_addr_t       addr    = 0;
        int     result  = -1;
        if (family != AF_INET) {
                errno   = EAFNOSUPPORT;
        }
        else    {
                result  = inet_pton (family, src, &addr);
                if (result == ADDR_OK) {
                        *((haddr_t*)(dst))      = ntohl(addr);
                }
        }
        return  result;
}
const char*  inet_htop (int family,const void *src,char *dst,socklen_t size) {
        const   char*   result  = 0;
        if (family != AF_INET) {
                errno   = EAFNOSUPPORT;
        }
        else    {       
                haddr_t addr    = *((haddr_t*)(src));
                in_addr_t       naddr   = htonl (addr);
                result  = inet_ntop (family, &naddr, dst, size);
        }
        return  result;
}
// Convert netmask to /n prefix.
int	mask_to_cidr (unsigned* cidrp, const char* mask) {
	int	result	= ok;
	haddr_t	u	= 0;
	unsigned	cidr	= IPV4_ADDR_BITS;
	int	i	= IPV4_ADDR_BITS;
	inet_ptoh (AF_INET, mask, &u);
	while (i>0 && bit(i-1, u)==1) {
		--i;
	}
	cidr	= IPV4_ADDR_BITS - i;
	if ( (~netmask (cidr) & u)!=0) {
		result	= err; /* non contiguous netmask */
	}
	*cidrp	= cidr;
	return	result;
}
