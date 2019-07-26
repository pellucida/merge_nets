/*
// @(#) node.h
// @(License-CC0)
*/
# if	!defined	NODE_H
# define	NODE_H

# include	<stdio.h>
# include	<stdlib.h>

# include	"constant.h"
# include	"netaddr.h"

enum	{
	combine	= 'C',
};

typedef	struct	node	node_t;

struct	node	{
	node_t*	subtree[2];
};
enum	{
	N_0	= 0,
	N_1	= 1,
};
static	inline int	other (int zo) {
	return		zo ^ N_1;
}

static	node_t*	ALL	= (node_t*)(-1);
static	inline int	leaf (node_t* n) {
	return	n==0 || n==ALL;
}


int	node_Create (node_t** np);
int	node_free (node_t* n);
int	node_insert (node_t* n, haddr_t x, int height, int suffix);
int	node_apply (node_t* t, haddr_t x, int height, int (*f)(haddr_t,int,void*), void* args);

# endif
