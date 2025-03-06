# simpleHttp

## Description

### Generic info

This project proposes a simple C++ API for implementing an HTTP Client. The following features are proposed:
- HTTP Requests : GET
- HTTP Version : 1.0, 1.1
- OS : Linux
It is meant to be future-proof, either in terms of supporting later HTTP versions or adding other HTTP requests.

### Class descripton

- The class exposes a **default constructor** as well as a **move constructor**. No copy constructor is exposed since it uses an underlying tcp connection.
- The constructor allows to the user to **set a verbosity level** for logging purposes
- The the main function the class exposes is **get**. A Response struct is returned, expliciting network status (tcp/ip errors) as well as htpp request status
- The class exposes a method allowing to the user to **define a callback** that is called upon request answer reception

For more details regarding the exposed API please refer to the [class header](https://github.com/tudorSwFpga/simpleHttp/blob/main/include/http.hpp) that contains doxygen documentation.


## Build 

The only external dependencies are spdlog for logging and gtest for testing; cmake will handle that for you

```
mkdir _build && cd _build
cmake .. -DBUILD_TESTS=ON
make -j $(nproc)
```

## Sample

A [sample](https://github.com/tudorSwFpga/simpleHttp/tree/main/sample/source) application is provided. 
The sample will send a get request to a host and path (given in the program's input) and will print the received answer

```
cd sample
#./sample $host $path
./sample httpbin.io /html
```

## Tests

A series of unit tests are implemented in the [here](https://github.com/tudorSwFpga/simpleHttp/blob/main/tests/source/main.cpp). 
In order to have the list (suite name / test name):

```
cd _build/tests && ./test --gtest_list_tests
##output
get.
  version_1_0
  version_1_1
  host_connection_failed
  wrong_path
add_callback.
  v1_0
  v1_1
move_constructor.
  successful_request
headers.
  keepavalive
  other_headers

```

You can launch one of the tests, one of the suits or all of them:

```
#launch all
./test
#launch only get.version_1_0
./test --gtest_filter=get.version_1_0
#launch all get tests
./test --gtest_filter="get*"
```




