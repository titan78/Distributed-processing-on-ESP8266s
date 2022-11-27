#include "interpreter.h"
#include <stdexcept>
#include <iostream>

Interpreter::Interpreter()
{

}

Interpreter::Interpreter(const std::string &fileName)
{

}

bool Interpreter::run(const std::string &code)
{
    return this->runCommand(code);
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
    std::any value;
    ScopeLevel scope;

    std::pair<std::string, std::string> nameAndType(const std::string& line){
        std::string invalidChars = ".-*+/=+)(^%!~><";
        std::vector<std::string> typeName;
        std::string tempBuffer = "";
        bool withinWord = false;
        char sep = ' ';

        for(size_t i = 0; i < line.length(); i++){
            if(line[i] == sep && !withinWord) continue;

            if(line[i] != sep && !withinWord) {
                withinWord = true;
            }

            if(line[i] != sep && withinWord){
                tempBuffer += line[i];
                if(invalidChars.find(line[i]) != std::string::npos)
                    throw std::runtime_error("you use invalid chars to define your variable name or use an invalid type name!");
                if(i != line.size() -1) continue;
            }


            if((line[i] == sep &&  withinWord) || i == line.size() -1) {
                typeName.push_back(tempBuffer);
                tempBuffer.clear();
                withinWord = false;
            }
        }

        if(typeName.size() != 2) throw std::runtime_error("invalid instruction, in a line of patrick code, you can create only one variable!");

        return {typeName[0], typeName[1]};
    }

public:
    DeclarableValue(const std::string& line, Interpreter * interpreter) {
        size_t indexOfAssign = line.find('=');

        if(indexOfAssign != std::string::npos) {
            std::tie(this->type, this->name) = nameAndType(line.substr(0, indexOfAssign));
            this->value = interpreter->stringToTypedAny(this->type, line.substr(indexOfAssign + 1));
        } else {
            std::tie(this->type, this->name) = nameAndType(line);
            this->value = interpreter->stringToTypedAny(this->type, "0");
        }

        this->scope = interpreter->pcb.scope;
    }

    Value toValue(){
        return {this->scope, {this->type, this->value}};
    }

    std::string getName() const {
        return this->name;
    }
};

bool Interpreter::declareVariable(const std::string &line)
{
    bool result = true;

    try {

        DeclarableValue dv(line, this);
        Value newValue = dv.toValue();
        this->_values.insert({dv.getName(), newValue});

    } catch (std::exception& e) {

        result = false;
        exceptionHandler(e.what());

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
        const std::string command = line.substr(index + 1);

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

void Interpreter::exceptionHandler(const std::string &what)
{
    std::cout << what + " line: " + pcb.getLineNumber() + ". \n";
}
#endif

/** ===================================================
 *
 *                  PCB manager
 *
 *  =================================================== */
#if defined(PCB_MANAGER)
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

std::any Interpreter::stringToTypedAny(const std::string &type, const std::string &value)
{
    try {
        switch(stringTypeToEnumType(type)){
        case DTS::Int: return std::stoi(value);
        case DTS::LInt: return std::stol(value);
        case DTS::LLInt: return std::stoll(value);
        case DTS::UInt: return std::stoul(value);
        case DTS::F32: return std::stof(value);
        case DTS::F64: return std::stod(value);
        case DTS::InValid: throw std::runtime_error("Invalid Data Type");
        default: throw "Invalid Data Type";
        }
    } catch (std::invalid_argument& e) {
        throw std::runtime_error("in compatible value and data type!");
    }
}

//bool Interpreter::checkValue(const std::string &value)
//{
//    std::string validDigits = "0123456789.";
//    char sign = value[0];
//    size_t i = 0;
//    bool result = true;

//    if(sign != '+' || sign == '-') i++;

//    while(i < value.length())
//        if(validDigits.find(value[i]) == std::string::npos){
//            result = false;
//            break;
//        }

//    return result;
//}

void Interpreter::invalidKeywordException() noexcept
{

}
