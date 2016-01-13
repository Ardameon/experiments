#include <iostream>
#include <string>


int main(void)
{
    std::string s1 = std::to_string(123456);
    std::string s2 = std::to_string(123.01);

    std::cout << "s1: " << s1 << "  s2: " << s2 << std::endl;
    std::cout << "s1: " << s1 << "  s2: " << s2 << std::endl;

    return 0;
}
