# PSVbench
A small benchmarking homebrew for PSVita.

### Tests included:
- CPU (1T/3T)
  - Dhrystone v2.2
  - Single-precision Linpack C (08/27/09)
- Memory throughput
  - memcpy() performance for LPDDR2/PHYCONT/CDRAM
  - DMA copy performance for LPDDR2/PHYCONT/CDRAM
- Memory latency (dword load to cpu)
  - L1/L2 cache, LPDDR2, CDRAM
- Storage (ux0/uma0/imc0 only)
  - 4K random read/write speed
  - Sequential read/write speed (50 MB blocks)

### Screenshots
![2019-10-20-214945](https://user-images.githubusercontent.com/12598379/67165324-f0ef7100-f383-11e9-9fbd-f9b3bbbbb690.png)
