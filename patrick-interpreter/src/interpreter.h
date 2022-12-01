#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <any>
#include <stack>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <limits>
#include <stdarg.h>
#include <algorithm>

#define TEST 1

#if TEST == 1
#include <iostream>
#endif

class Interpreter
{
public:
    Interpreter();
    Interpreter(const std::string& fileName);
    bool runOneCommand(const std::string& code);
    void runMultipleCommands(const std::string& code);

#if TEST == 1
    std::any getValue(const std::string& name){
        return _values[name].second;
    }

    std::vector<std::string> getValueNames(){
        std::vector<std::string> keys;
        for (const auto& [key, value] : _values)
        {
            keys.push_back(key);
        }
        return keys;
    }

    void printValues(){
        for (const auto& [key, value] : _values)
        {
            std::cout << key << " -> type: " << value.first << " and has value: " << value.second.has_value() << std::endl;
        }
    }

#endif

private: // types
    /// @brief first: Type, second: value
    typedef std::pair<std::string, std::any> TypedValue;

    /// @brief first: level (prent-child), second: sub-level (brotherhood)
    typedef std::pair<unsigned int, unsigned int> ScopeLevel;

//    /** @brief first: ScopeLevel object, second: TypedValue
//     *  @details Before accessing the variable, the scope must be checked
//     */
//    typedef std::pair<ScopeLevel, TypedValue> Value;

    /// @brief data types
    enum class DTS{InValid, Int, LInt, LLInt, UInt, F32, F64};

    /// @brief keywords
    enum class KWS{Declare, Syscall, If, Else, Loop, Return, BranchTo, Function, Invoke};

    /// @brief keywords
    enum class BOPS{EQ, GTEQ, LTEQ, GT, LT, NEQ};

    #define DECLARE_VARIABLE
    /**
     * @brief The DeclarableValue class; The fields of this structure are filled by the fromString function
     *  from the lines of code related to the variable declaration,
     *  and an instance of this structure is used as the input of the declareValue function.
     */
    struct DeclarableValue;

//    #define DECLARE_FUNCTION
//    /**
//     * @brief The FunctionType class; Each function, when implemented, gets an instance of this structure,
//     *  which holds the beginning and end of the function definition in the code.
//     *  Then, during execution, this object fills its other fields using the fromRawString function and is ready to execute.
//     *  Finally, by using the toExecutableString function,
//     *  the final code of the function that can be executed by the interpreter will be created.
//     */
//    struct FunctionType;

#define IF_ELSE_BLOCK
    struct IfElseBlock;

    #define PCB_MANAGER
    /**
     * @brief process control block
     */
    struct PCB;

    /**
     * @brief The calculationMetaData class:
     * This structure determines the index at which each of our variables is placed in the stack specific to its type
     */
    struct CalculationMetaData {
        std::string type;
        std::size_t index;
    };

private: // members
#ifdef DECLARE_VARIABLE
    bool declareVariable(const std::string& line);
#endif

#define CHECK_ACCESS_VARIABLE
    std::string checkAccessToVariableAndScopizeName(const std::string& name);

#define CHANGE_VARIABLE
    void changeVariable(const std::string& name, const std::string &value);

#ifdef IF_ELSE_BLOCK
    void ifElseBlock(const std::string &line);
#endif

    //#define LOOP_BLOCK
    //loop()

#ifdef DECLARE_FUNCTION
    bool declareFunction(const std::string& line);
#endif

    //#define INVOKE_FUNCTION
    //invokeFunction()

    //#define STORE_FUNCTION_RETURN_VALUE
    //storeFunctionReturnValue()

    //#define BRANCH_TO
    //branchTo()

    //#define SOLVE_EXPRESSION
    //solveExpression()

    /*cloudLinker() --> Due to the lack of resources in ESPs, we do not have the ability to precompile,
     *                  so we have to create a cloud link between calls and implementations of functions. */

    #define RUN_COMMAND
    bool runCommand(const std::string& line);

    void exceptionHandler(const std::string& what);

#ifdef PCB_MANAGER
    void pcbManager(const std::string &line, bool file);
#endif

private: // helper memebers
    void checkScopeCompatiblity(const std::string& name);
    DTS stringTypeToEnumType(const std::string& type);
    KWS keywordStringTokeywordEnum(const std::string& type);
    BOPS operatorStringToOperatorEnum(const std::string& type);
    bool checkValue(const std::string &value);
    void checkDataTypeOnOperation(int n, ...);
    std::any stringToTypedAny(const std::string &type, const std::string &value);
    ScopeLevel scopeFromName(const std::string& name) const noexcept;
    void goTo(size_t line);
    std::pair<std::string, size_t> getWordAndItsEnd(const std::string& line, int start = 0);
    void variableAndTypeNameValidator(const std::string& name);
    std::string scopizeName(const std::string &name, const ScopeLevel &scope);
    CalculationMetaData loadValueToItsStack(const std::string& name);
    bool booleanOperation(const std::string &type, const std::string &op);
    void trimLeft(std::string &string);
    void trimRight(std::string &string);
    void trim(std::string &string);
    std::string trimLeft_copy(std::string string);
    std::string trimRight_copy(std::string string);
    std::string trim_copy(std::string string);
    std::string removeSpaces(const std::string& string);
    bool isNumber(const std::string& string);

private: // exceptions
    void invalidKeywordException() noexcept;

private: // variables
    /// first: name of value, second: Value Pair
    std::map<std::string, TypedValue> _values;
//    std::map<std::string, FunctionType> _functions;
    PCB* pcb;

    /// calclulation stacks
    std::stack<int> intStack;
    std::stack<long> lintStack;
    std::stack<long long> llintStack;
    std::stack<unsigned int> uintStack;
    std::stack<float> f32Stack;
    std::stack<double> f64Stack;
    std::map<std::string, CalculationMetaData> metadata;
    std::stack<std::string> calculationStack;
//    std::stack<long double> f128Stack;
    std::ifstream * file;
    bool exceptionOrTerminate;

private: // constants

};

#endif // INTERPRETER_H
