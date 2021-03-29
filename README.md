# 概要
DAMCREM (Dynamic Allocation Method of Computation REsource to Macro-Tasks for Fully Homomorphic Encryption Application)

# Requirements
* CentOS 7
  * It may work on Ubuntu 20.04
* CMake >= 3.13
* GCC >= 9.3.1

# ビルド
```terminal
$ git submodule update -i
$ mkdir build
$ cd build
$ cmake ../ -DCMAKE_BUILD_TYPE=Release
$ make && cd ../
```

# デモ
## 準備
```terminal
$ unzip scheme.zip
$ unzip const_data.zip
```
## サーバ
```terminal
$ ./build/example/server/test_online_single_fifo --option-file=options/1app/1-test.txt
```
## クライアント
```terminal
$ ./build/example/client/test_online 
```

## 静的解析
```terminal
$ ./build/bin/static_analysis --option-file=options/1app/1-static_analysis.txt --log-file=./app/1/app_exec_time_result.txt --data-file=./app/1/app_exec_time_data.txt --measure --generate --max-num-threads=4 --ignore-top-jobs=1
```



