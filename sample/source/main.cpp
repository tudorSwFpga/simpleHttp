#include "http.hpp"
#include <iostream>

//this sample uses the HttpClient class to make an HTTP GET request to a specified host and path.
//The response is printed to the console.
int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <host> <path>" << std::endl;
        return 1;
    }
    HttpClient client("1.1","debug");
    client.addGetCb([](const std::string& response) {
        std::cout << response << std::endl;
    });
    client.get(argv[1], argv[2],{ {"User-Agent", "HttpClient/1.0"}, {"Accept", "*/*"} });
    
    return 0;
}