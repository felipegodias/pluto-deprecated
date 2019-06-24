#include <pluto/root.h>
#include <iostream>

int main(int argc, char* argv[])
{
    const pluto::Root pluto("config.yml", "sandbox.log", "C:/Users/felipe.dias/Repos/sandbox");
    int exitCode;
    try
    {
        exitCode = pluto.Run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        exitCode = 1;
    }
    return exitCode;
}
