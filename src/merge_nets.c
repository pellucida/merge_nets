/*
//	@(#) merge_nets.c
//	@(License-CC0)
*/
# include	<unistd.h>
# include	<stdio.h>
# include	<stdlib.h>
# include	<string.h>
# include	"constant.h"
# include	"netaddr.h"
# include	"addr_range.h"

# include	"tree.h"
# include	"util.h"

void	Usage() {
	fprintf (stderr, "Usage: %s [-m N|C|W] file...]\n", progname());
	fprintf (stderr, " -m ?  - netmask output style\n");
	fprintf (stderr, "    C  - xx.xx.xx.xx/nn\n");
	fprintf (stderr, "    N  - xx.xx.xx.xx/mm.mm.mm.mm\n");
	fprintf (stderr, "    W  - like N but tcpwrapper compatible.\n");
	exit (EXIT_FAILURE);
}
const	static	char	STYLES[]	= "CNW";

static	int	process_file (FILE* input, tree_t* tree);

main (int argc, char* argv[]) {
	tree_t*	tree	= 0;
	haddr_t	u	= 0;
	FILE*	input	= stdin;
	int	opt	= EOF;
	int	m_flag	= 0;
	int	style	= 'C';

	setprogname (argv[0]);

	opterr	= 0;
	while ((opt = getopt (argc, argv, "f:m:D"))!=EOF) {
		switch (opt) {
		case	'm':
			if (m_flag++)
				Usage ();
			style	= optarg[0];
		break;
		default:
			Usage ();
		}
	}
	
	if (strchr (STYLES, style)==0) {
		fatal ("style %c not in <%s>\n", style, STYLES);
	}
	tree_Create (&tree, IPV4_ADDR_BITS);
	if (optind == argc) {
		process_file (input, tree);
	}
	else for (; optind < argc; optind++) {
		input	=  fopen (argv [optind], "r");
		if (input==0) {
			error ("Cannot open file: \"%s\"\n", argv [optind]);
		}
		else	{
			process_file (input, tree);
			fclose (input);
		}
	}
	tree_print (tree, style);
	tree_Delete (&tree);
}

/*
// Each line of input contains one network range in form of:
// a)	xxx.xxx.xxx.xxx				# host address
// b)	xxx.xxx.xxx.xxx/mmm.mmm.mmm.mmm		# network/netmask
// c)	xxx.xxx.xxx.xxx/nn			# network/cidr
// d)	xxx.xxx.xxx.xxx-yyy.yyy.yyy.yyy		# start-addr..end-address (inclusive)
//
// A vector of address/mask pairs stores a single range or up to 63 ranges (d)
// which is then inserted into the tree structure.
// The binary tree summarizes adjacent subnets eg x/n y/n => x/n-1 (where x+2^n == y)
// and removes redundant subranges eg adding x/n-1 remove x/n, y/n
*/
static	int	process_file (FILE* input, tree_t* tree) {
	char	line [BUFSIZ];
	while (fgetline (input, line, sizeof(line)) != EOF) {
		range_list_t	r	= {.nranges = 0, };
		int	i	= 0;
		haddr_t	addr	= 0;
		unsigned	mask	= 32;
		char*		mptr	= 0;
		char*	t	= strchr (line, '/');
		if (t) {
			*t	= '\0';
			mptr	= t+1;
			if (strchr (mptr, '.')==0) {
				mask	= atoi (mptr);
			}
			else	{
				mask_to_cidr (&mask, mptr);
			}
			if (mask <= IPV4_ADDR_BITS && inet_ptoh (AF_INET, line, &addr) == ADDR_OK) {
				range_list_append (&r, addr, mask);
			}
		}
		else if ((t = strchr (line, '-')) != 0) {
			haddr_t	start	= 0;
			haddr_t	finish	= 0;
			*t++	= '\0';
			if (inet_ptoh (AF_INET, line, &start) == ADDR_OK) {
				if (inet_ptoh (AF_INET, t, &finish) == ADDR_OK) {
					if (start > finish) {
						haddr_t	tmp	= start;
						warn ("Address range END < START %s-%s - swapping\n", line, t);
						start	= finish;
						finish	= tmp;
					}
					range_list_make (start, finish, &r);
				}
				else	{
					error ("Address range error (END) %s-%s\n", line, t);
				}
			}
			else	{
				error ("Address range error (START) %s-%s\n", line, t);
			}
		}
		else if (mask <= IPV4_ADDR_BITS && inet_ptoh (AF_INET, line, &addr) == ADDR_OK) {
			range_list_append (&r, addr, mask);
		}
		for (i=0; i < r.nranges; ++i) {
			addr_range_t	ar	= r.ranges [i];
			tree_insert (tree, ar.addr, ar.mask);
		}
	}
	return	ok;
}
