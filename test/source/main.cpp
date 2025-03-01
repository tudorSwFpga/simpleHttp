#include "http.hpp"

int main()
{
    HttpClient client("1.1");
    client.get("www.google.com", "/index.html");
    return 0;
}