#include "InfiniteDouble.hpp"

int main(void)
{
    InfiniteDouble	a("342", "7201");
    // InfiniteDouble	a(342.7201);
    InfiniteDouble	b("71", "981");

    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "a + b: " << a + b << std::endl;
    std::cout << "7201 + 4.02 = " << InfiniteDouble("7201") + InfiniteDouble("4", "02") << std::endl;
    std::cout << "a - b: " << a - b << std::endl;
    std::cout << "7201 - 4.02 = " << InfiniteDouble("7201") - InfiniteDouble("4", "02") << std::endl;
    std::cout << "7201.02 - 4.02 = " << InfiniteDouble("7201", "02") - InfiniteDouble("4", "02") << std::endl;
    std::cout << "a * b: " << a * b << std::endl;
    std::cout << "7201 * 4.02 = " << InfiniteDouble("7201") * InfiniteDouble("4", "02") << std::endl;
    // std::cout << "a / b: " << a / b << std::endl;
    // std::cout << "a % b: " << a % b << std::endl;
    // std::cout << "a ^ b: " << a ^ b << std::endl;
    return (0);
}
