# simpleHttp

## Description

This project proposes a simple C++ API for implementing an HTTP Client. The following features are proposed:
- HTTP Requests : GET
- HTTP Version : 1.0, 1.1
- OS : Linux

For more details regarding the exposed API please refer to the [class header](https://github.com/tudorSwFpga/simpleHttp/blob/main/include/http.hpp)

A [sample](https://github.com/tudorSwFpga/simpleHttp/tree/main/sample/source) is provided. It will simply print to stdout the content received from the server:
```
httpclient $host $path
```

## Build 

```
mkdir _build && cd _build
cmake ..
make
```

## Sample

## Tests

```
cmake -B build -DBUILD_TESTS=ON
```




