#include <pluto/root.h>

int main(int argc, char* argv[])
{
    const Pluto::Root pluto("", "", "");
    pluto.Run();
    return 0;
}
