#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <any>

class Interpreter
{
public:
    Interpreter(const std::string& fileName);
    bool run(const std::string& code);

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
    enum class DTS{InValid = 0x0, Int, LInt, LLInt, UInt, F32, F64};

    /**
     * @brief The DeclarableValue class; The fields of this structure are filled by the fromString function
     *  from the lines of code related to the variable declaration,
     *  and an instance of this structure is used as the input of the declareValue function.
     */
    struct DeclarableValue;

    /**
     * @brief The FunctionType class; Each function, when implemented, gets an instance of this structure,
     *  which holds the beginning and end of the function definition in the code.
     *  Then, during execution, this object fills its other fields using the fromRawString function and is ready to execute.
     *  Finally, by using the toExecutableString function,
     *  the final code of the function that can be executed by the interpreter will be created.
     */
    struct FunctionType;

private: // members
    bool declareVariable(const std::string& line);
    //accessVariable()
    //changeVariable()
    //checkScopeCompatiblity()
    //checkDataType()
    //mathOperation()
    //if-else blocks
    //loop()
    bool declareFunction(const std::string& line);
    //invokeFunction()
    //storeFunctionReturnValue()
    //branchTo()
    /*cloudLinker() --> Due to the lack of resources in ESPs, we do not have the ability to precompile,
     *                  so we have to create a cloud link between calls and implementations of functions. */

    bool runCommand(const std::string& line);

private: // variables
    /// first: name of value, second: Value Pair
    std::map<std::string, Value> _values;
    std::map<std::string, FunctionType> _functions;

private: // constants
    std::map<std::string, DTS> _dataTypes =
        {{"int", DTS::Int}, {"lint", DTS::LInt}, {"llint", DTS::LLInt}, {"uint", DTS::UInt}, {"F32", DTS::F32}, {"F64", DTS::F64}};

};

#endif // INTERPRETER_H
