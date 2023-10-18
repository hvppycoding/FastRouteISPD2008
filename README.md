# FastRouteISPD2008
A standalone version of FastRoute for ISPD2008 benchmarks

## How to Run

0. (Optional) Run the dev docker
```bash
cd /go/to/working/directory
docker pull hvppycoding/centos7-dev:v1
docker run -it -v $(pwd):/RUN_FastRoute hvppycoding/centos7-dev:v1
cd /RUN_FastRoute
```

1. Download sources
```bash
git clone https://github.com/hvppycoding/FastRouteISPD2008.git
cd FastRouteISPD2008
```

2. Build source
```bash
mkdir build
cd build
cmake ../CMakeLists.txt
make
```

3. Run a benchmark
```bash
./FastRouteISPD2008 ../examples/adaptec1.capo70.3d.35.50.90.gr adaptec1.out
```

```text
NOSPACE: num net 219794
NUMNETS: 219794
ADJUSTMENTS: 84731

Reading Lookup Table ...

Done reading table

...

Post Processsing finished, starting via filling
via related to pin nodes 1348390
via related stiner nodes 122485
total Usage   : 3638366
Total Capacity: 6646288
Max H Overflow: 0
Max V Overflow: 0
Max Overflow  : 0
H   Overflow  : 0
V   Overflow  : 0
Final Overflow: 0

Final routing length : 3638366
Final number of via  : 1734160
Final total length 1 : 5372526
Final total length 3 : 8840846
3D runtime: 173.539993 sec
Getting results... Done!
```

4. Evaluate the output
```bash
../scripts/eval2008.pl ../examples/adaptec1.capo70.3d.35.50.90.gr adaptec1.out
```

```text
File Names(In, Out)                         Tot OF       Max OF             WL
../examples/adaptec1.capo70.3d.35.50             0            0        5372524
```
