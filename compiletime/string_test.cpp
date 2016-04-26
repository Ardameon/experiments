#include <iostream>
#include <string>
#include <map>


int main(void)
{
    std::string s1 = std::to_string(123456);
    std::string s2 = std::to_string(123.01);
    std::map<int, char *> m;
    char a[4] = "123";
    char b[4] = "456";
    int i, j;

    m[0] = a;
    m[1] = b;

    std::cout << "s1: " << s1 << "  s2: " << s2 << std::endl;
    std::cout << "s1: " << s1 << "  s2: " << s2 << std::endl;

    std::map<int, char *>::iterator itr;

    for(itr = m.begin(), j = 0; itr != m.end(); itr++, j++)
    {
        for(i = 0; i < 3; i++)
        {
            std::cout << itr->second[i] << std::endl;
        }
    }

    return 0;
}
