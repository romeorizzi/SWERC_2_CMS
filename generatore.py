#!/usr/bin/env python2
from sys import argv, exit
import os, string

if len(argv) < 2:
    exit(1)

if argv[1] == "COPY":
    if len(argv) < 3:
        exit(1)
    os.system("cat " + argv[2])
elif argv[1] == "ZCOPY":
    if len(argv) < 3:
        exit(1)
    os.system("zcat " + argv[2])
else:
    os.system("echo " + ' '.join(argv[1:]))    

exit(0)

