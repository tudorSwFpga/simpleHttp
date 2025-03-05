# simpleHttp

## Description

### Generic info

This project proposes a simple C++ API for implementing an HTTP Client. The following features are proposed:
- HTTP Requests : GET
- HTTP Version : 1.0, 1.1
- OS : Linux

### Class descripton

- The class exposes a **default constructor** as well as a **move constructor**. No copy constructor is exposed since it uses an underlying tcp connection.
- The constructor allows to the user to **set a verbosity level** for logging purposes
- The the main function the class exposes is **get**. A Response struct is returned, expliciting network status (tcp/ip errors) as well as htpp request status
- The class exposes a method allowing to the user to **define a callback** that is called upon request answer reception

For more details regarding the exposed API please refer to the [class header](https://github.com/tudorSwFpga/simpleHttp/blob/main/include/http.hpp) that contains doxygen documentation.


## Build 

The only external dependency is spdlog; cmake will handle that for you

```
mkdir _build && cd _build
cmake .. -DBUILD_TESTS=ON
make -j $(nproc)
```

## Sample

A [sample](https://github.com/tudorSwFpga/simpleHttp/tree/main/sample/source) application is provided. 
The sample will send a get request to a host and path (given in the program's input) and will print the received answer

```
httpclient $host $path
```

## Tests

A series of unit tests are implemented in the [here](https://github.com/tudorSwFpga/simpleHttp/blob/main/tests/source/main.cpp). 
In order to launch them:

```
cd _build && ctest --test-dir tests
```




