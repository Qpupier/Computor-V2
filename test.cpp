#include "InfiniteDouble.hpp"

int main(void)
{
    // InfiniteDouble	a("342", "7201");
    // InfiniteDouble	b("71", "981");
    // InfiniteDouble	c("3");

    // std::cout << "a: " << a << std::endl;
    // std::cout << "b: " << b << std::endl;
    // std::cout << "a + b: " << a + b << std::endl;
    // std::cout << "7201 + 4.02 = " << InfiniteDouble("7201") + InfiniteDouble("4", "02") << std::endl;
    // std::cout << "a - b: " << a - b << std::endl;
    // std::cout << "7201 - 4.02 = " << InfiniteDouble("7201") - InfiniteDouble("4", "02") << std::endl;
    // std::cout << "7201.02 - 4.02 = " << InfiniteDouble("7201", "02") - InfiniteDouble("4", "02") << std::endl;
    // std::cout << "a * b: " << a * b << std::endl;
    // std::cout << "7201 * 4.02 = " << InfiniteDouble("7201") * InfiniteDouble("4", "02") << std::endl;
    // std::cout << "4.02 * 10 * 10 = " << InfiniteDouble("4", "02") * InfiniteDouble("10") * InfiniteDouble("10") << std::endl;
    // std::cout << "4.02 * 1000 = " << InfiniteDouble("4", "02") * InfiniteDouble("1000") << std::endl;
    // std::cout << "a / b: " << a / b << std::endl;
    // std::cout << "a / b * b: " << a / b * b << std::endl;
    // std::cout << "a * b / b: " << a * b / b << std::endl;
    // std::cout << "a % b: " << a % b << std::endl;
    // std::cout << "a ^ c: " << (a ^ c) << std::endl;



    std::cout << "sqrt(25): " << InfiniteInt("25").sqrt() << std::endl;
    std::cout << "sqrt(1472): " << InfiniteInt("1472").sqrt() << std::endl;
    std::cout << "sqrt(24598): " << InfiniteInt("24598").sqrt() << std::endl;
    std::cout << "sqrt(154651343136565615156161651651652424): " << InfiniteInt("154651343136565615156161651651652424").sqrt() << std::endl;
    return (0);
}
