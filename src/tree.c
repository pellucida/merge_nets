/*
// @(#) tree.c
// @(License-CC0)
*/
# include	<arpa/inet.h>
# include	<stdio.h>

# include	"constant.h"
# include	"node.h"
# include	"tree.h"
# include	"netaddr.h"

int	tree_Create (tree_t** tpp, int height) {
	int	result	= err;
	tree_t*	tp	= calloc (sizeof(*tp), 1);
	if (tp) {
		tp->height	= height;
		tp->root	= 0;
		*tpp	= tp;
		result	= ok;
	}
	return	result;
}
int	tree_Delete (tree_t** tpp) {
	if (tpp && *tpp) {
		tree_t*	t	= *tpp;
		node_free (t->root);
		free (t);
		*tpp	= 0;
	}
	return	ok;
}

int	tree_insert (tree_t* t, haddr_t x, int mask) {
	int	result	= ok;
	if (t->root == ALL) 	// Special case 0.0.0.0/0 already inserted
		;
	else if (mask==0) {	// Special case 0.0.0.0/0 to be inserted
		node_free (t->root);
		t->root	= ALL;
	}
	else	{
		 if (t->root == 0) {
			result	= node_Create (&t->root);
		}
		if (result==ok) {
			int	suffix	= t->height - mask;
			result	= node_insert (t->root, x, t->height, suffix);
		}
	}
	return	result;
}

struct	print_args	{
	FILE*	output;
	int	height;
	int	style;
};
static	int	printer (haddr_t x, int height, void* args) {
	struct	print_args*	a	= args;
	FILE*	output	= a->output;
	char    address [INET_ADDRSTRLEN];
	int	style	= a->style;
	int	cidr	= a->height - height-1;

	inet_htop (AF_INET, &x, address, sizeof(address));

	fprintf (output, "%s", address);
	switch (style) {
	case	'C': // CIDR
		fprintf (output, "/%d", cidr);
	break;
	case	'W':
	case	'N':
		if (style=='N' || height > 0) {
			haddr_t m       = netmask (cidr);
			char    mask [INET_ADDRSTRLEN];

			inet_htop (AF_INET, &m, mask, sizeof(mask));

			fprintf (output, "/%s", mask);
		}
	break;
	}
	fprintf (output, "\n");
}
	

int	tree_print (tree_t* t, const int style) {
	struct	print_args args	= { .output = stdout, .height = t->height, .style = style, };
	if (t->root) // Not empty tree
		node_apply (t->root, 0, t->height-1, printer, &args);
}
