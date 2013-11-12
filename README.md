smp-cache
=========

multiprocessor cache simulator

This project is developed as a part of CSC506 course at North Carolina State University. It simulates working of
multiprocessor caches.

Please read MP_3.pdf for detailed problem description.

files:
cache.cc: working of a single cache
msi.cc  : working of MSI protocol
mesi.cc : working of MESI protocol
moesi.cc: working of MOESI protocol

proto.out: program output of <proto> for below configuration
L1_SIZE:                        32678
L1_ASSOC:                       8
L1_BLOCKSIZE:                   64
NUMBER OF PROCESSORS:           16

CGad_VALIDATION_<PROTO> and CGaw_VALIDATION_<PROTO>: ideal output for PROTO for above configuration
