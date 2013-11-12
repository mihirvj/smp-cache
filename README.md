smp-cache
=========

multiprocessor cache simulator

This project is developed as a part of CSC506 course at North Carolina State University. It simulates working of
multiprocessor caches.

Please read MP_3.pdf for detailed problem description.

files:<br>
cache.cc: working of a single cache<br>
msi.cc  : working of MSI protocol<br>
mesi.cc : working of MESI protocol<br>
moesi.cc: working of MOESI protocol<br>

proto.out: program output of <proto> for below configuration<br>
L1_SIZE:                        32678<br>
L1_ASSOC:                       8<br>
L1_BLOCKSIZE:                   64<br>
NUMBER OF PROCESSORS:           16<br>

CGad_VALIDATION_ <PROTO> and CGaw_VALIDATION_ <PROTO>: ideal output for PROTO for above configuration
