
MERGE_NETS=../src/merge_nets

tests:
	$(MERGE_NETS) example-c.txt | diff expected-c.txt -
	$(MERGE_NETS) example-n.txt | diff expected-c.txt -
	$(MERGE_NETS) -m N example-n.txt | diff expected-n.txt -
	$(MERGE_NETS) -m C expected-c.txt | diff expected-c.txt -
	$(MERGE_NETS) -m N expected-n.txt | diff expected-n.txt -
	$(MERGE_NETS) -m C example-r.txt | grep -q -F -e '68.78.0.0/16'

.PHONEY:	tests
