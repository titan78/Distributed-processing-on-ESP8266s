#include "interpreter.h"

Interpreter::Interpreter(const std::string &fileName)
{

}

/** ===================================================
 *
 *                  declare variable
 *
 *  =================================================== */
#if defined(DECLARE_VARIABLE)
struct Interpreter::DeclarableValue {
private:
    std::string type;
    std::string name;
    std::string value = "";
    ScopeLevel scope;
public:
    DeclarableValue(const std::string& line) {

    }

    Value toValue(const ScopeLevel& scope){

    }

    std::string getName() const {

    }
};

bool Interpreter::declareVariable(const std::string &line)
{
    bool result = true;

    try {

        DeclarableValue dv(line);
        Value newValue = dv.toValue({0,0});
        this->_values.insert({dv.getName(), newValue});

    } catch (...) {

        result = false;
        //exception handler

    }

    return result;
}
#endif


/** ===================================================
 *
 *                  declare function
 *
 *  =================================================== */
#if defined(DECLARE_FUNCTION)
struct Interpreter::FunctionType {
private:
    size_t end;
    size_t start;
    std::string type;
    std::string body;
    std::vector<DeclarableValue> arguments;
    std::vector<std::string> parameters;
public:
    std::string toExecutableString() const;
    void fromRawString(const std::string& functionString);
    bool checkParameters();
};

bool Interpreter::declareFunction(const std::string &line)
{

}
#endif

/** ===================================================
 *
 *                  run command
 *
 *  =================================================== */
#if defined(RUN_COMMAND)
bool Interpreter::runCommand(const std::string &line)
{
    bool executionReult = true;
    const auto index = line.find(':');

    if(index == std::string::npos){ //expression

        //executionReult = solveExpression(line);

    } else { //keyword
        const std::string keyword = line.substr(0, index);
        const std::string command = line.substr(index);

        switch (keywordStringTokeywordEnum(keyword)) {
        case KWS::BranchTo: break;
        case KWS::Declare: executionReult = declareVariable(command); break;
        case KWS::Else: break;
        case KWS::If: break;
        case KWS::Function: break;
        case KWS::Loop: break;
        case KWS::Return: break;
        case KWS::Syscall: break;

        default:
            executionReult = false;
            invalidKeywordException();
            break;
        }
    }

    return executionReult;
}
#endif

/** ===================================================
 *
 *                  PCB manager
 *
 *  =================================================== */
#if defined(PCB_MANAGER)
struct Interpreter::PCB{
    ScopeLevel scope;
};

void Interpreter::pcbManager()
{

}
#endif

Interpreter::DTS Interpreter::stringTypeToEnumType(const std::string &type)
{
    std::map<std::string, DTS> dataTypes =
        {{"int", DTS::Int}, {"lint", DTS::LInt}, {"llint", DTS::LLInt}, {"uint", DTS::UInt}, {"F32", DTS::F32}, {"F64", DTS::F64}};

    return dataTypes[type];
}

Interpreter::KWS Interpreter::keywordStringTokeywordEnum(const std::string &type)
{
    std::map<std::string, KWS> keywords =
        {{"declare", KWS::Declare}, {"branch_to", KWS::BranchTo}, {"function", KWS::Function},
         {"return", KWS::Return}, {"if", KWS::If}, {"Else", KWS::Else}, {"loop", KWS::Loop}, {"syscall", KWS::Syscall}};

    return keywords[type];
}

void Interpreter::invalidKeywordException() noexcept
{

}
