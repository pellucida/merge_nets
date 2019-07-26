/*
//	@(#) tree.h
//	@(License-CC0)
*/
# if	!defined(TREE_H)
# define	TREE_H

# include	"node.h"
# include	"netaddr.h"

typedef	struct	tree	tree_t;
struct	tree	{
	int	height;
	node_t*	root;
};

int     tree_Create (tree_t** t, int height);
int     tree_Delete (tree_t** t);
int     tree_insert (tree_t* t, haddr_t x, int suffix);
int	tree_print (tree_t* t, const int style);

# endif
