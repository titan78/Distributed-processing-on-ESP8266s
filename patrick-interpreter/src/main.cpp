#include"interpreter.h"
#include <iostream>

void test_1_vriable_declaration(){
    Interpreter i;
    i.runOneCommand("declare: lint num1 = 10.1");
    i.runOneCommand("declare: lint num2 = 20");
    i.runOneCommand("declare: lint num3=10");
    i.runOneCommand("declare: lint num4 = 100");

    std::cout << std::any_cast<long>(i.getValue("num1:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num2:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num3:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num4:0.0")) << std::endl;

    std::cout << "\n\n\n" << std::endl;
}

void test_2_vriable_declaration(){
    Interpreter i;
    i.runMultipleCommands("declare: lint num1 = 10\n"
                          "declare: lint num2 = 20\n"
                          "{\n"
                          "declare: lint num3=10\n"
                          "}\n"
                          "declare: lint num4 = 100\n"
                          );

    i.printValues();

    std::cout << "\n\n\n" << std::endl;
}

void test_3_if_else(){
    Interpreter i;
    i.runMultipleCommands("declare: lint num1 = 10 \n"
                          "declare: lint num2 = 20 \n"
                          "if: num1 == num2 \n"
                          "{\n"
                          "declare: F64 d = 20.20 \n"
                          "}\n"
                          "declare: lint num4 = 100\n"
                          );

    i.printValues();

    std::cout << std::any_cast<long>(i.getValue("num1:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num2:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num4:0.0")) << std::endl;
    //    std::cout << std::any_cast<double>(i.getValue("d:1.0")) << std::endl;

    std::cout << "\n\n\n" << std::endl;
}

void test_4_if_else(){
    Interpreter i;
    i.runMultipleCommands("declare: lint num1 = 10 \n"
                          "declare: lint num2 = 20 \n"
                          "if: num1 == num2 \n"
                          "{\n"
                          "declare: F64 d1 = 20.20 \n"
                          "}\n"
                          "else: \n"
                          "{ \n"
                          "declare: F32 d2 = 10.10 \n"
                          "} \n"
                          "declare: lint num4 = 100\n"
                          );

    i.printValues();

    std::cout << std::any_cast<long>(i.getValue("num1:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num2:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num4:0.0")) << std::endl;
    std::cout << std::any_cast<float>(i.getValue("d2:1.0")) << std::endl;
    //    std::cout << std::any_cast<double>(i.getValue("d1:1.0")) << std::endl;
    std::cout << "\n\n\n" << std::endl;
}

void test_5_if_else(){
    Interpreter i;
    i.runMultipleCommands("declare: lint num1 = 10 \n"
                          "declare: lint num2 = 20 \n"
                          "if: num1 != num2 \n"
                          "{\n"
                          "declare: F64 d1 = 20.20 \n"
                          "}\n"
                          "else: \n"
                          "{ \n"
                          "declare: F32 d2 = 10.10 \n"
                          "} \n"
                          "declare: lint num4 = 100\n"
                          );

    i.printValues();

    std::cout << std::any_cast<long>(i.getValue("num1:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num2:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num4:0.0")) << std::endl;
    std::cout << std::any_cast<double>(i.getValue("d1:1.0")) << std::endl;
    //    std::cout << std::any_cast<float>(i.getValue("d2:1.0")) << std::endl;

    std::cout << "\n\n\n" << std::endl;
}

void test_6_if_else_file(){
    Interpreter i("F:/UT/Term 1/Computer Achitecture/Project/Distributed-processing-on-ESP8266s/patrick-interpreter/tests/02-if-else-blocks/01-if-else-block.pt");
    i.printValues();

    std::cout << std::any_cast<long>(i.getValue("num1:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num2:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num4:0.0")) << std::endl;
    std::cout << std::any_cast<double>(i.getValue("d1:1.0")) << std::endl;
    //    std::cout << std::any_cast<float>(i.getValue("d2:1.0")) << std::endl;

    std::cout << "\n\n\n" << std::endl;
}

void test_7_change_variable(){
    Interpreter i;
    i.runMultipleCommands("declare: lint num1 = 10 \n"
                          "declare: lint num2 = 20 \n"
                          "declare: F64 num3 \n"
                          "if: num1 != num2 \n"
                          "{\n"
                          "num3 = 0.0001 \n"
                          "}\n"
                          "else: \n"
                          "{ \n"
                          "num3 = 0.0002 \n"
                          "} \n"
                          );

    i.printValues();

    std::cout << std::any_cast<long>(i.getValue("num1:0.0")) << std::endl;
    std::cout << std::any_cast<long>(i.getValue("num2:0.0")) << std::endl;
    std::cout << std::any_cast<double>(i.getValue("num3:0.0")) << std::endl;

    std::cout << "\n\n\n" << std::endl;
}

int main()
{
    //    test_1_vriable_declaration();
    //    test_2_vriable_declaration();
    //    test_3_if_else();
    //    test_4_if_else();
    //    test_5_if_else();
//        test_6_if_else_file();
    test_7_change_variable();


//    Interpreter i;
//    std::cout << sizeof(i) << "\n";
}
















