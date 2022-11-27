#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <any>
#include <stack>

#define TEST 1

class Interpreter
{
public:
    Interpreter();
    Interpreter(const std::string& fileName);
    bool run(const std::string& code);

#if TEST == 1
    std::any getValue(const std::string& name){
        return _values[name].second.second;
    }
#endif

private: // types
    /// @brief first: Type, second: value
    typedef std::pair<std::string, std::any> TypedValue;

    /// @brief first: level (prent-child), second: sub-level (brotherhood)
    typedef std::pair<unsigned int, unsigned int> ScopeLevel;

    /** @brief first: ScopeLevel object, second: TypedValue
     *  @details Before accessing the variable, the scope must be checked
     */
    typedef std::pair<ScopeLevel, TypedValue> Value;

    /// @brief data types
    enum class DTS{InValid, Int, LInt, LLInt, UInt, F32, F64};

    /// @brief keywords
    enum class KWS{Declare, Syscall, If, Else, Loop, Return, BranchTo, Function};

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

    #define PCB_MANAGER
    /**
     * @brief process control block
     */
    struct PCB{
        ScopeLevel scope = {0,0};
        size_t lineNumber = 1;
        std::string getLineNumber() const { return std::to_string(lineNumber); }
    };

    struct calculationMetaData {
        std::string type;
        int index;
    };

private: // members
#ifdef DECLARE_VARIABLE
    bool declareVariable(const std::string& line);
#endif

    //#define ACCESS_VARIABLE
    //accessVariable()

    //#define CHANGE_VARIABLE
    //changeVariable()

    //#define CHECK_SCOPE_COMPATIBILITY
    //checkScopeCompatiblity()

    //#define MATH_OPERATION
    //mathOperation()

    //#define IF_ELSE_BLOCK
    //ifElseBlock()

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

//    #define DECLARE_FUNCTION
    /*cloudLinker() --> Due to the lack of resources in ESPs, we do not have the ability to precompile,
     *                  so we have to create a cloud link between calls and implementations of functions. */

    #define RUN_COMMAND
    bool runCommand(const std::string& line);

    void exceptionHandler(const std::string& what);

#ifdef PCB_MANAGER
    void pcbManager();
#endif


private: // helper memebers
    DTS stringTypeToEnumType(const std::string& type);
    KWS keywordStringTokeywordEnum(const std::string& type);
//    bool checkValue(const std::string& value);
    bool checkDataType(const std::string& type, const std::string& value);
    std::any stringToTypedAny(const std::string &type, const std::string& value);


private: // exceptions
    void invalidKeywordException() noexcept;

private: // variables
    /// first: name of value, second: Value Pair
    std::map<std::string, Value> _values;
//    std::map<std::string, FunctionType> _functions;
    PCB pcb;

    /// calclulation stacks
    std::stack<int> intStack;
    std::stack<long> lintStack;
    std::stack<long long> llintStack;
    std::stack<unsigned int> uintStack;
    std::stack<float> f32Stack;
    std::stack<double> f64Stack;
    std::map<std::string, calculationMetaData> metadata;
    std::stack<std::string> calculationStack;
//    std::stack<long double> f128Stack;

private: // constants
//    std::map<std::string, DTS> _dataTypes =
//        {{"int", DTS::Int}, {"lint", DTS::LInt}, {"llint", DTS::LLInt}, {"uint", DTS::UInt}, {"F32", DTS::F32}, {"F64", DTS::F64}};

//    std::map<std::string, KWS> _keywords =
//        {{"declare", KWS::Declare}, {"branch_to", KWS::BranchTo}, {"function", KWS::Function},
//         {"return", KWS::Return}, {"if", KWS::If}, {"Else", KWS::Else}, {"loop", KWS::Loop}, {"syscall", KWS::Syscall}};

};

#endif // INTERPRETER_H
