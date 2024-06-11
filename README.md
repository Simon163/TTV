


# Introduction

Like [Protobuf](https://protobuf.dev/) and [flatbuffer](https://flatbuffers.dev/), TTV is a lightweight data communication tool that can be used to serialize or deserialize data, which stands for Tag-Type-Value.

TTV has self-describing capabilities, which means that after the sender serializes data using the TTV protocol and completes the transmission, the receiver can deserialize the data according to the TTV protocol and reconstruct the data sent by the sender.

To a certain extent, TTV is equivalent to [TLV](https://en.wikipedia.org/wiki/Type-length-value)  (Tag-Length-Value).  An object with basic data types (such as int, float, double) is stored in memory in the following order: Tag, Type, Value rather than Tag, Length, Value. So I rename it using TTV.
  
This repo is to provide an easy-to-use API for serialization and deserialization in C++.

TTV is implemented in C/C++, and the data types it currently supports are:

1.  `uint8_t` (Unsigned 8-bit integer)
2.  `int8_t` (Signed 8-bit integer)
3.  `uint16_t` (Unsigned 16-bit integer)
4.  `int16_t` (Signed 16-bit integer)
5.  `uint32_t` (Unsigned 32-bit integer)
6.  `int32_t` (Signed 32-bit integer)
7.  `uint64_t` (Unsigned 64-bit integer)
8.  `int64_t` (Signed 64-bit integer)
9.  `string` (String)
10. `char*` (Char*)

# Usage
Please see 
```
test/TtvBox.cpp
test/testTtvBuffer.cpp
```

Build:
```
mkdir build
cd build
cmake ..
make
```
Run:
```
cd build
./testTtvBox.out
./testTtvBuffer.out
```

# Application
When performing deep learning inference, the input images are usually needed to do preprocessing before feeding to the backbone network to do inference processing.

TTV can be used to store the preprocessing configuration parameters such as input channel, width, height, mean, and variant. This repo gives a demo of: 
   1. how to serialize parameters to the .bin file
   2. how to deserialize parameters from the .bin file

The contents of the serialized .bin file can be integrated into the model file before releasing and then ensure preprocessing parameters is self-explanatory when performing deep learning inference, which means you can store these parameters in the model file when compiling and you can parse them and do preprocessing based on them before performing inference. 

The preprocessing parameters can be configured in a Yaml [file](demo/modelPreCfg.yaml), 

step 1:  convert the preprocessing parameters from .yaml file to .txt file.
```
cd demo
python3 genPreCfgFile.py "./modelPreCfg.yaml"
```
Then [modelPreCfg.txt](demo/modelPreCfg.txt) is generated.

step 2: run serialization
```
cd build
./testModelPreCfgDemo.out
```
The source code can be seen in [testModelPreCfgDemo.cpp](demo/modelPreCfg.txt).

