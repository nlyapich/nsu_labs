#!/bin/bash

gcc -c hello4.S -o hello4.o
ld hello4.o -o hello4
strace ./hello4
