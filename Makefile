package		= merge-nets
version		= 0.1
tarname		= merge-nets
WORK		= /var/tmp/$(package)
distdir		= $(tarname)-$(version)

prefix		= /usr/local
exec_prefix	= ${prefix}
bindir		= ${exec_prefix}/bin

all clean distclean:
	cd src && $(MAKE) $@

tests:	
	cd src && $(MAKE) merge_nets
	cd tests && $(MAKE) tests

dist: distclean $(distdir).tar.gz

$(distdir).tar.gz: $(distdir)
	tar -C $(WORK) -chof - $(distdir) | gzip -9 -c > $@
	rm -rf $(WORK)/$(distdir)

SOURCES=\
	Makefile\
	addr_range.c\
	addr_range.h\
	constant.h\
	merge_nets.c\
	netaddr.c\
	netaddr.h\
	node.c\
	node.h\
	tree.c\
	tree.h\
	util.c\
	util.h

TESTS=\
	Makefile\
	example-c.txt\
	example-n.txt\
	expected-c.txt\
	expected-n.txt\
	Makefile\
	output-c.txt

$(distdir): FORCE
	-mkdir	$(WORK)
	-mkdir -p $(WORK)/$(distdir)/src
	tar -C src -c -p -f - $(SOURCES) | tar -C $(WORK)/$(distdir)/src -x -p -f -
	-mkdir -p $(WORK)/$(distdir)/tests
	tar -C tests -c -p -f - $(TESTS) | tar -C $(WORK)/$(distdir)/tests -x -p -f -
	cp Makefile README.md $(WORK)/$(distdir)

FORCE:
	-rm $(distdir).tar.gz >/dev/null 2>&1
	-rm -rf $(WORK)/$(distdir) >/dev/null 2>&1
	-rmdir $(WORK) >/dev/null 2>&1


.PHONY: FORCE all clean check dist tests distclean
