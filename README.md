# ga-dao
Direct Aperture Optimization based on MOGAL by Jori Liesenborgs.

MOGAL: https://github.com/j0r1/MOGAL and http://research.edm.uhasselt.be/~jori/page/Misc/MOGAL.html  
Jori Liesenborgs: http://research.edm.uhasselt.be/~jori/page/Main/HomePage.html

## Installation
```
mkdir ga
cd ga
mkdir dependencies
cd dependencies
```

### Requirements
Download and extract (or git clone) the following into `dependencies`.
* ErrUt
* ENUt
* SerUT
* MOGAL

Install them e.g. like this:
```
cd ErrUt
cmake .
make
sudo make install
```

Depending on where the the shared libraries were installed, it can be necessary to tell cmake where to find them or e.g. update the LD_LIBRARY_PATH like this:
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/
```

### Installing ga-dao
```
git clone <this repository>
cd ga-dao
git submodule update --init --recursive (clone Eigen)
```

## Running ga-dao
```
cd bin
cmake .
make
mpirun -n <N> ./ga-dao
```

