#include <iostream>
#include <string>

int main()
{
    std::string myString = "Hello, World!";

    myString.erase(0, 2);
    ;

    std::cout << myString << std::endl;

    return 0;
}