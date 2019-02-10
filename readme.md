Screencast link:
https://www.youtube.com/watch?v=G32QMsgr-jM

### Last Class
* Measuring wall-time
* Calculating FLOP/s and MOP/s
* Compiler optimization flags: -Ox, -march=native, -g
* Brief overview of different kinds of parallelism:
  + instruction level (vectorization, pipelining)
  + shared memory (OpenMP)
  + distributd memory (MPI)
* Tools: ssh, modules

### Definitions
* Arithmetic intensity
* Latency
* Bandwidth
* Cache
* Cache line
* Virtual memory, Translation Lookaside Buffer (TLB), Memory pages, Page fault, First touch

### Valgrind

#### --tool=memcheck (default)
- Out-of-bound array access
- Uninitialized variables (--track-origins=yes)
- Memory leaks (--leak-check=full)
- Attach GDB (--vgdb-error=0)

### --tool=callgrind (for profiling/timing)
* kcachegrind <callgrind-output-file>
* Optional agruments --dump-instr=yes --collect-jumps=yes
* cg_annotate --auto=yes <output-file>
* On Mac: brew install qcachegrind --with-graphviz

### --tool=cachegrind (for cache misses)
* From callgrind --tool=callgrind --cache-sim=yes

### References
* http://igoro.com/archive/gallery-of-processor-cache-effects/
* https://www.aristeia.com/TalkNotes/ACCU2011_CPUCaches.pdf

### Takeaway
* Memory allocations are expensive (i.e. cost of first touch not malloc)
* Keep most frequently used data in registers, then L1 cache, L2 cache, L3 cache and main memory
* Regular memory access patterns (implicit prefetch) or explicit prefetch to hide memory latency
* Valgrind to debug memory errors, profile code and find cache misses

