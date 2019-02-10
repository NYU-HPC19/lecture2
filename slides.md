

-> Advanced Topics in Numerical Analysis: <-
============================================
-> High Performance Computing <-
================================

-> Georg Stadler, Dhairya Malhotra <-
-> Courant Institute, NYU <-

-> Lecture 2 <-
-> Feb 4, 2019 <-

-> *Memory, Caches, Valgrind* <-
-> https://github.com/NYU-HPC19/lecture2 <-

-------------------------------------------------------------------------------

-> # Goals <-


<br>
Measure performance

<br>
Maximum/expected performance

<br>
Understand bottlenecks

<br>
Mitigate bottlenecks

-------------------------------------------------------------------------------

-> Computer Memory <-
=====================

_Single core:_

->       `C|R`       <-
->         |         <-
->       ` L1`       <-
->         |         <-
->      `  L2 `      <-
->         |         <-
->   `     L3    `   <-
->         |         <-
-> ................. <-
-> :      RAM      : <-
-> :...............: <-

-------------------------------------------------------------------------------

-> Computer Memory <-
=====================

_Multi core:_

->   `C|R`   `C|R`   <-
->      |     |      <-
->   ` L1`   ` L1`   <-
->      |     |      <-
->  `  L2 ` `  L2 `  <-
->      |     |      <-
->  `      L3     `  <-
->         |         <-
-> ................. <-
-> :      RAM      : <-
-> :...............: <-

-------------------------------------------------------------------------------

-> Computer Memory <-
=====================

_Multi socket:_ Non-Uniform Memory Access

->   `C|R`   `C|R`        `C|R`   `C|R`   <-
->        |     |          |     |        <-
->   ` L1`   ` L1`        ` L1`   ` L1`   <-
->        |     |          |     |        <-
->  `  L2 ` `  L2 `      `  L2 ` `  L2 `  <-
->        |     |          |     |        <-
->  `      L3     ` <=>  `      L3     `  <-
->         |                    |         <-
-> .................    ................. <-
-> :      RAM      :    :      RAM      : <-
-> :...............:    :...............: <-

-------------------------------------------------------------------------------

-> Computer Memory <-
=====================

Getting the cache size

```
getconf -a | grep CACHE
```

-------------------------------------------------------------------------------

-> Computer Memory <-
=====================

*Memory allocation* 'malloc' returns
virtual memory address

<br>
True cost of new memory allocation is
in the initialization or *first touch*

*Memeory pages* generally 4KB, but can be
larger (2MB, 1GB), not in programmers
control.

-------------------------------------------------------------------------------

-> Computer Memory <-
=====================

*Cache line* smallest unit of memory for
reading/writing data between main memory and
caches

* More efficient to read/write consecutive
  array elements than strided accesses

-------------------------------------------------------------------------------

-> Computer Memory <-
=====================

*Reading* TLB lookup, virtual address
translation to physical address, find
cache line in L1 cache
* cache hit: load data to register
* cache miss: look further down in
  memory hierarchy

<br>
*Writing* bring in entire cache line to
L1 and update the cache line

-------------------------------------------------------------------------------

-> Benchmarking Memory Bandwidth <-
===================================

-------------------------------------------------------------------------------

-> Benchmarking Memory Latency <-
=================================

*Sequential access:*
-> [ 1  2  3  4  5  6  7  8  9  0 ] <-

<br>
*Strided access:*
-> [ 3  4  5  6  7  8  9  0  1  2 ] <-

<br>
*Random access:*
-> [ 9  4  1  8  5  2  7  0  6  3 ] <-

-------------------------------------------------------------------------------

-> Valgrind <-
================

-------------------------------------------------------------------------------

-> Valgrind <-
================

A set of tools for debugging and
profiling

*Memcheck*: debugging memory errors

*Cachegrind*: simulate two levels of
cache hierarchy and show cache misses

*Callgrind*: profiling and displaying
the callgraph

-------------------------------------------------------------------------------

-> Memcheck <-
==============

```
valgrind --tool=memcheck ./a.out
```

<br>
*Uninitialized variables*
                   --track-origins=yes

<br>
*Out-of-bound array access*
       attach debugger: --vgdb-error=0

<br>
*Memory leaks*
                     --leak-check=full

-------------------------------------------------------------------------------

-> Callgrind <-
===============

```
valgrind --tool=callgrind ./a.out
```

*Optional agruments to see assembly code*
-> --dump-instr=yes --collect-jumps=yes <-

*Graphical output*
Linux:   kcachegrind <output-file>
Mac:     qcachegrind <output-file>

-------------------------------------------------------------------------------

-> Cachegrind <-
================

```
valgrind --tool=cachegrind ./a.out
```

<br>
Alternatively, run with callgrind
```
valgrind --tool=callgrind --cache-sim=yes ./a.out
```

*Graphical output*
Linux:   kcachegrind <output-file>
Mac:     qcachegrind <output-file>

-------------------------------------------------------------------------------

-> References <-
=================

_What every programmer should know_
_about memory_ by Ulrich Drepper


_Gallery of Processor Cache Effects_
by Igor Ostrovsky

