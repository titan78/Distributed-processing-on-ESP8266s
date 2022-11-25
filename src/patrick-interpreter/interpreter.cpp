#include "interpreter.h"

Interpreter::Interpreter(const std::string &fileName)
{

}

/** ===================================================
 *
 *                  declare variable
 *
 *  =================================================== */

struct Interpreter::DeclarableValue {
private:
    std::string type;
    std::string name;
    std::string value = "";
    ScopeLevel scope;
public:
    DeclarableValue(const std::string& line);
    Value toValue(const ScopeLevel& scope);
    std::string getName() const;
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

    }

    return result;
}

/** ===================================================
 *
 *                  declare function
 *
 *  =================================================== */

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

