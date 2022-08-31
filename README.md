# rolling-hash

Sample implementation rolling hash using checksum and strong hash like MD5 (for example) [Spec](https://github.com/eqlabs/recruitment-exercises/blob/8e49a7b8cf9c415466876e852fbd862f74105ec6/rolling-hash.md)

Requirements
```
Boost 1.74 (tested on this version)
Cmake 3.22
g++
```

Build 
```
mkdir build
cd build
cmake ..
make
```
Run test

```
cd ./test
./test -l all
```

Run software
```
rolling_hash -s old_file signature_file
rolling_hash -d signature_file new_file delta_file
```
