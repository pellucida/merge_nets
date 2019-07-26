
### Summarize, condense or merge IPv4 network ranges

merge_nets uses a simple binary tree to store the network addresses
or ranges which allow simple merging of adjacent network ranges.

Input also permits arbitrary address ranges which are converted into
network, prefix pairs before insertion into the tree. 

### SYNOPSIS

```
    merge_nets  [-m C|N|W] [ files-of-ranges ...]
               -m	mask style for list output
                   C	xx.xx.xx.0/cc		[C]idr
                   N	xx.xx.xx.0/mm.mm.mm.0	[N]etmask
                   W	xx.xx.xx.xx		tcp_[W]rapper
```

### INPUT FORMAT
A single network range per line of input from specified files (or stdin.)
The ranges can be in any of the following and freely mixed.

1. [dotted-quad]/[network mask] 
2. [dotted-quad]/[network prefix]
3. [dotted-quad-address-start]-[dotted-quad-address-end]

eg (1) 8.8.8.8/255.255.255.0, (2) 192.168.96.0/20, (3) 172.16.54.22-172.16.97.33

NB the address ranges (3) *are not* restricted to /n boundaries.

```
172.16.54.22-172.16.97.33 is expanded to before merging

	172.16.54.22/31
	172.16.54.24/29
	172.16.54.32/27
	172.16.54.64/26
	172.16.54.128/25
	172.16.55.0/24
	172.16.56.0/21
	172.16.64.0/19
	172.16.96.0/24
	172.16.97.0/27
	172.16.97.32/31
```

	### EXAMPLES

Given an input file containing this list

	10.78.76.0/24
	10.78.77.0/24

the program should write to stdout

	10.78.76.0/23

The input could equivalently be given as

	10.78.76.0/255.255.255.0
	10.78.77.0/255.255.255.0
or
	10.78,76.0-10.78.77.255

To have the output printed as

	10.78.76.0/255.255.254.0

use the `-m N` switch.  The `-m W` switch is identical 
except for 255.255.255.255 masks for host addresses.

eg 10.78.80.17 instead of 10.78.80.17/255.255.255.255 

If the list is too big for the program to handle in one go
sort the list and split the output into a number of pieces
Run subnet_merge on the pieces separately and then on the output.

### PREREQUISITES

The code uses <arpa/inet.h> and dependent headers for inet_pton(3)
inet_ntop(3) but otherwise is fairly ordinary and should be
compilable by any C89 compiler.

### SEE ALSO

[http://cidrmerge.sourceforge.net/]
(http://cidrmerge.sourceforge.net/)

subnet_merge and cidr_cover are precursors.

### LICENSE
Creative Commons CC0
[http://creativecommons.org/publicdomain/zero/1.0/legalcode]
(http://creativecommons.org/publicdomain/zero/1.0/legalcode)


### AUTHOR
[James Sainsbury](mailto:toves@sdf.lonestar.org)
