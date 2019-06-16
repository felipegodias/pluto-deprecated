#include <pluto/root.h>

int main(int argc, char* argv[])
{
    const Pluto::Root pluto("", "", "");
    const int code = pluto.Run();
    return code;
}
