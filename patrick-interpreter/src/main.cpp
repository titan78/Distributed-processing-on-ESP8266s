#include"interpreter.h"
#include <iostream>

int main()
{
    Interpreter i;
//    i.run("declare: lint num1 = 10");
//    i.run("declare: lint num2 = 20");
    i.run("declare: lint num3=10");
//    i.run("declare: lint num4 = 100");

//    std::cout << std::any_cast<long>(i.getValue("num1")) << std::endl;
//    std::cout << std::any_cast<long>(i.getValue("num2")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num3")) << std::endl;
//    std::cout << std::any_cast<long>(i.getValue("num4")) << std::endl;

//    std::cout << std::stoi("10 - 40") << std::endl;

//    try {
//    throw std::runtime_error("sag");
//    } catch (std::exception& e) {
//        std::cout << e.what() << std::endl;
//    }
}
