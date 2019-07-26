/*
// @(#) node.c
// @(License-CC0)
*/
# include	<stdio.h>
# include	<stdlib.h>

# include	"constant.h"
# include	"node.h"
# include	"netaddr.h"

static	int	subtree_make (node_t** subtreep, haddr_t x, int top, int bot);

int	node_Create (node_t** np){
	int	result	= err;
	node_t*	n	= calloc (sizeof (*n),1);
	if (n) {
		*np	= n;
		result	= ok;
	}
	return	result;
}
/*
// Create a node initializing the subtrees with "under"
*/
static	int	node_Create_from (node_t** np, node_t* under){
	node_t*	n	= 0;
	int	result	= node_Create (&n);
	if (result == ok) {
		*n	= *under;
		*np	= n;
	}
	return	result;
}
/*
// Build subtree from bottom up.
// When node_insert() encounters an empty subtree
// it uses this more efficient code to construct
// the missing subtree.
*/
static	int	subtree_make (node_t** subtreep, haddr_t x, int top, int bot){
	int	result	= ok;
	int	l = 0;
	node_t*	last	= ALL;
	for (l=bot; l < top; ++l) {
		node_t	under = { .subtree = { 0, 0}};
		int	zo	= bit (l, x);
		under.subtree [zo]	= last;
		node_Create_from (&last, &under);
	}
	*subtreep	= last;
	return	result;
}

int	node_free (node_t* n) {
	if (!leaf (n)) {
		node_free (n->subtree[N_0]);
		node_free (n->subtree[N_1]);
		free (n);
	}
	return	ok;
}

int	node_insert (node_t* n, haddr_t x, int height, int suffix) {
	int	result	= ok;
	int	zo	= bit (height-1, x);	
	node_t**	subtree	= n->subtree;
	if (subtree [zo] == ALL) {
		/* Already covered */;
	}
	else if (height == (suffix+1)) {
		/* insert a /suffix range here */
		if (subtree [zo]) {
			/* remove the covered subtree if any */
			node_free (subtree [zo]);
		}
		subtree [zo]	= ALL;
		/* Check whether these two can combined into /suffix+1 */
		if (subtree [other (zo)] == ALL) {
			result	= combine;
		}
	}
	else {
		if (subtree [zo] == 0) {
			/* empty subtree - must construct */
			subtree_make (&subtree [zo], x, height-1, suffix);
		}
		else	{
		/* Recurse and check whether we can combine the two subtrees */
			if (node_insert (subtree [zo], x, height-1, suffix) == combine) {
				node_free (subtree [zo]);
				subtree [zo]	= ALL;
				if (subtree [other (zo)] == ALL) {
					result	= combine;
				}
			}
		}
	}
	return	result;
}

int	node_apply (node_t* t, haddr_t x, int  height, int (*f)(haddr_t,int,void*), void* args) {
	if (leaf (t)) {
		if (t == ALL) {
			f (x, height, args);
		}
	}
	else 	{
		int	zo	= 0;
		for (zo = N_0; zo <= N_1; ++zo) {
			node_t*	subtree	= t->subtree [zo];
			x	= x | zo << height;
			if (subtree) {	
				node_apply (subtree, x, height-1, f, args);
			}
		}
	}
	return	ok;
}
