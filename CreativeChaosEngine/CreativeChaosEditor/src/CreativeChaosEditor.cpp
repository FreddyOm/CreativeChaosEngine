#include <iostream>;

namespace CCECore
{
    __declspec(dllimport) void Test();
}


int main()
{
    CCECore::Test();

    std::cin.get();
}