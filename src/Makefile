#
# @(#) Makefile
# @(License-CC0)
#
OBJ=node.o tree.o netaddr.o util.o merge_nets.o addr_range.o
CFLAGS=-g

all:	merge_nets

merge_nets:	$(OBJ)
	gcc -g -o $@ $(OBJ)

clean:	
	rm -f $(OBJ)

distclean:	clean
	rm -f merge_nets *.o a.out

addr_range.o: addr_range.c netaddr.h addr_range.h
addr_range.o: addr_range.h netaddr.h
constant.o: constant.h
netaddr.o: netaddr.c constant.h netaddr.h
netaddr.o: netaddr.h
node.o: node.c constant.h node.h netaddr.h
node.o: node.h constant.h netaddr.h
merge_nets.o: merge_nets.c constant.h netaddr.h addr_range.h tree.h node.h util.h
tree.o: tree.c constant.h node.h netaddr.h tree.h
tree.o: tree.h node.h constant.h netaddr.h
util.o: util.c
util.o: util.h
