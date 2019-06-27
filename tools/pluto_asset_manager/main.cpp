#include <iostream>

void PrintMenu()
{
    std::cout << std::endl;
    std::cout << "*** Menu ***" << std::endl;
    std::cout << "1: Build Text    2: Build Mesh    3: Build Shader    0: Exit" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    int option = 0;
    do
    {
        PrintMenu();

        std::cin >> option;
    }
    while (option != 0);
    return 0;
}
