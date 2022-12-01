#include "interpreter.h"

/** ===================================================
 *
 *                  PCB manager
 *
 *  =================================================== */
#if defined(PCB_MANAGER)
struct Interpreter::PCB{
    ScopeLevel scope = {0,0};

    bool isWithinFunctionCall() const {
        return _withinFunctionCall;
    }

    std::string getLineNumber_s() const {
        return std::to_string(_lineNumber);
    }

    size_t getLineNumber() const {
        return _lineNumber;
    }

    size_t nextLineNumber(){
        return ++_lineNumber;
    }

    void invoke(){
        _withinFunctionCall = true;
        _invokeLineNumber = _lineNumber;
    }

    void scopeManager(const std::string& line, Interpreter * interpreter){
        if(line.find("else:") != std::string::npos) setWithinElseBlock(true);
        if(!isRunAllowed() &&!isWithinIfBlock() && !isWithinElseBlock()) setRunAllowed(true);

        if(line.find('{') != std::string::npos)
        {
            scope.first += 1;
            _curlyBraces++;
        }
        else if(line.find('}') != std::string::npos)
        {
            removeVariablesAllocatedInThisScope(interpreter);
            if(isWithinIfBlock()) {
                setRunAllowed(!isRunAllowed());
                setWithinIfBlock(false);
            }

            if(isWithinElseBlock()) {
                setRunAllowed(true);
                setWithinElseBlock(false);
            }

            scope.first -= 1;
            _curlyBraces--;
        }
    }

    void functionManager(const std::string& line){
        if(line.find('}') != std::string::npos)
        {
            if(_curlyBraces == 0){
                _withinFunctionCall = false;
                _lineNumber = _invokeLineNumber + 1;
            }
        }
    }

    std::vector<std::string> valueNameWithBackwardScopes(const std::string& name, Interpreter * interpreter) const{
        std::vector<std::string> names;
        for(int i = scope.first; i >= 0; i--)
            names.push_back(interpreter->scopizeName(name, {i,0}));

        return names;
    }

    void goTo(size_t line, Interpreter * interpreter)
    {
        interpreter->file->seekg(std::ios::beg);
        for(size_t i = 0; i < line - 1; ++i){
            interpreter->file->ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }

    bool isWithinIfBlock() const
    {
        return _withinIfBlock;
    }

    void setWithinIfBlock(bool newWithinIfBlock)
    {
        _withinIfBlock = newWithinIfBlock;
    }

    bool isWithinElseBlock() const
    {
        return _withinElseBlock;
    }

    void setWithinElseBlock(bool newWithinIfBlock)
    {
        _withinElseBlock = newWithinIfBlock;
    }

    bool isRunAllowed() const {
        return _runAllowed;
    }

    void setRunAllowed(bool c){
        _runAllowed = c;
    }

    void setIfElseMetaData(bool condition){
        _condition = condition;
        setRunAllowed(condition);
    }

    size_t condition() const { return _condition; }

private:
    void removeVariablesAllocatedInThisScope(Interpreter * interpreter){
        std::vector<std::string> forErase;
        for (const auto& [key, value] : interpreter->_values)
        {
            auto const scp = interpreter->scopeFromName(key);
            if(scp.first == scope.first) forErase.push_back(key);
        }

        for(const auto& key : forErase)
            interpreter->_values.erase(key);
    }

private:
    bool _withinFunctionCall;
    bool _withinIfBlock = false;
    bool _withinElseBlock = false;
    bool _condition;
    bool _runAllowed = true;
    size_t _invokeLineNumber;
    size_t _lineNumber = 1;
    size_t _curlyBraces = 0;
};

void Interpreter::pcbManager(const std::string& line, bool file = false)
{
    if(!file) {
        pcb->nextLineNumber();
        pcb->scopeManager(line, this);
    }

    if(file) {
        pcb->goTo(pcb->nextLineNumber(), this);
        if(pcb->isWithinFunctionCall())
            pcb->functionManager(line);
    }
}
#endif

/** ===================================================
 *
 *                  Constructors
 *
 *  =================================================== */
Interpreter::Interpreter() : pcb(new PCB)
{

}

Interpreter::Interpreter(const std::string &fileName) : pcb(new PCB), file(new std::ifstream(fileName))
{
    file->close();
    file->open(fileName, std::fstream::in);

    if(!file->is_open()){
        std::cout << "file closed!\n";
        return;
    }

    std::string line;
    while(std::getline(*this->file, line)){
        pcbManager(line);
        if(pcb->isRunAllowed()) runCommand(line);
    }
}

/** ===================================================
 *
 *              Public Function Members
 *
 *  =================================================== */
bool Interpreter::runOneCommand(const std::string &code)
{
    return this->runCommand(code);
}

void Interpreter::runMultipleCommands(const std::string &code)
{
    std::vector<std::string> lines;
    std::string line;

    for(auto& c : code){
        if(c != '\n')
            line += c;
        else {
            lines.push_back(line);
            line.clear();
        }
    }

    for(auto& line : lines){
        pcbManager(line);
        if(pcb->isRunAllowed()) runCommand(line);
    }
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

    std::pair<std::string, std::string> nameAndType(const std::string& line, Interpreter * interpreter){
        std::vector<std::string> typeName;
        std::string word; size_t index;

        for(size_t i = 0; i < line.length(); i++){
            std::tie(word, index) = interpreter->getWordAndItsEnd(line, i);
            interpreter->variableAndTypeNameValidator(word);
            if(!word.empty()) typeName.push_back(word);
            i = index;
        }

        if(typeName.size() != 2)
            throw std::runtime_error("invalid instruction, in a line of patrick code, you can create only one variable!");

        return {typeName[0], interpreter->scopizeName(typeName[1], interpreter->pcb->scope)};
    }

public:
    DeclarableValue(const std::string& line, Interpreter * interpreter) {
        size_t indexOfAssign = line.find('=');

        if(indexOfAssign != std::string::npos) {
            std::tie(this->type, this->name) = nameAndType(line.substr(0, indexOfAssign), interpreter);
            this->value = interpreter->stringToTypedAny(this->type, interpreter->trim_copy(line.substr(indexOfAssign + 1)));
        } else {
            std::tie(this->type, this->name) = nameAndType(line, interpreter);
            this->value = interpreter->stringToTypedAny(this->type, "0");
        }

        this->scope = interpreter->pcb->scope;
    }

    TypedValue toValue(){
        return {this->type, this->value};
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
        TypedValue newValue = dv.toValue();
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
 *                  access variable
 *
 *  =================================================== */
#ifdef CHECK_ACCESS_VARIABLE
std::string Interpreter::checkAccessToVariableAndScopizeName(const std::string &name)
{
    std::string nameWithScope;

    const auto names = pcb->valueNameWithBackwardScopes(name, this);

    for(const auto &n : names){
        try {
            _values.at(n);
            nameWithScope = n;
        } catch (...) {
            if(n == names.back()){
                throw std::runtime_error(name + " is undefined!");
            }
        }
    }

    return nameWithScope;

    //    try {
    //        checkScopeCompatiblity(nameWithScope);
    //    } catch (std::runtime_error& e) {
    //        throw e.what();
    //    }
}

#endif

#if defined(CHANGE_VARIABLE)
void Interpreter::changeVariable(const std::string &name, const std::string& value)
{
    ///exception
    checkValue(value);
    auto& v = _values[checkAccessToVariableAndScopizeName(name)];

    switch(stringTypeToEnumType(v.first)){
    case DTS::Int:
        v.second = std::any_cast<int>(std::stoi(value));
        break;

    case DTS::LInt:
        v.second = std::any_cast<long>(std::stol(value));
        break;

    case DTS::LLInt:
        v.second = std::any_cast<long long>(std::stoll(value));
        break;

    case DTS::UInt:
        v.second = std::any_cast<unsigned int>(std::stoul(value));
        break;

    case DTS::F32:
        v.second = std::any_cast<float>(std::stof(value));
        break;

    case DTS::F64:
        v.second = std::any_cast<double>(std::stod(value));
        break;

    case DTS::InValid: throw std::runtime_error("Invalid Data Type");
    default: throw "Invalid Data Type";
    }
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
 *                  if-else block
 *
 *  =================================================== */
#ifdef IF_ELSE_BLOCK
void Interpreter::ifElseBlock(const std::string& line)
{
    pcb->setWithinIfBlock(true);

    // check condition
    bool badOperator = true;
    const std::vector<std::string> operators = {"==", "!=", ">=", "<=", ">", "<"};
    for(auto& op : operators){
        const size_t index = line.find(op);

        if(index != std::string::npos){
            badOperator = false;
            std::string fop = getWordAndItsEnd(line.substr(0, index)).first;
            variableAndTypeNameValidator(fop);
            std::string sop = getWordAndItsEnd(line.substr(index + op.length())).first;
            variableAndTypeNameValidator(sop);
            CalculationMetaData fop_m = loadValueToItsStack(fop);
            fop.clear();
            CalculationMetaData sop_m = loadValueToItsStack(sop);
            sop.clear();
            checkDataTypeOnOperation(2, fop_m.type.c_str(), sop_m.type.c_str());
            pcb->setIfElseMetaData(booleanOperation(fop_m.type, op));
            break;
        }
    }

    if(badOperator) throw std::runtime_error("Bad Operator");
}
#endif

Interpreter::CalculationMetaData Interpreter::loadValueToItsStack(const std::string &name)
{
    const auto& value = _values[checkAccessToVariableAndScopizeName(name)];

    switch (stringTypeToEnumType(value.first)) {
    case DTS::Int:
        intStack.push(std::any_cast<int>(value.second));
        return {value.first, intStack.size() - 1};

    case DTS::LInt:
        lintStack.push(std::any_cast<long>(value.second));
        return {value.first, lintStack.size() - 1};

    case DTS::LLInt:
        llintStack.push(std::any_cast<long long>(value.second));
        return {value.first, llintStack.size() - 1};

    case DTS::UInt:
        uintStack.push(std::any_cast<unsigned int>(value.second));
        return {value.first, uintStack.size() - 1};

    case DTS::F32:
        f32Stack.push(std::any_cast<float>(value.second));
        return {value.first, f32Stack.size() - 1};

    case DTS::F64:
        f64Stack.push(std::any_cast<double>(value.second));
        return {value.first, f64Stack.size() - 1};

    case DTS::InValid: throw std::runtime_error("Invalid Data Type");
    default: throw "Invalid Data Type";
    }
}

bool Interpreter::booleanOperation(const std::string& type, const std::string& op)
{

    auto helper = [this, op] (auto oper1, auto oper2){
        switch (operatorStringToOperatorEnum(op)) {
        case BOPS::EQ: return oper1 == oper2;
        case BOPS::NEQ: return oper1 != oper2;
        case BOPS::GTEQ: return oper1 >= oper2;
        case BOPS::LTEQ: return oper1 <= oper2;
        case BOPS::GT: return oper1 > oper2;
        case BOPS::LT: return oper1 < oper2;
        default: throw std::runtime_error("Invalid Operator");
        }
    };

    // oper2, oper1, az chap be rast operator ha miyan!
    switch (stringTypeToEnumType(type)) {
    case DTS::Int:{
        int oper1 = intStack.top(); intStack.pop();
        int oper2 = intStack.top(); intStack.pop();
        return helper(oper2, oper1);
    }

    case DTS::LInt:{
        long oper1 = lintStack.top(); lintStack.pop();
        long oper2 = lintStack.top(); lintStack.pop();
        return helper(oper2, oper1);
    }

    case DTS::LLInt:{
        long long oper1 = llintStack.top(); llintStack.pop();
        long long oper2 = llintStack.top(); llintStack.pop();
        return helper(oper2, oper1);
    }

    case DTS::UInt:{
        unsigned oper1 = uintStack.top(); uintStack.pop();
        unsigned oper2 = uintStack.top(); uintStack.pop();
        return helper(oper2, oper1);
    }

    case DTS::F32:{
        float oper1 = f32Stack.top(); f32Stack.pop();
        float oper2 = f32Stack.top(); f32Stack.pop();
        return helper(oper2, oper1);
    }

    case DTS::F64:{
        double oper1 = f64Stack.top(); f64Stack.pop();
        double oper2 = f64Stack.top(); f64Stack.pop();
        return helper(oper2, oper1);
    }

    case DTS::InValid: throw std::runtime_error("Invalid Data Type");
    default: throw "Invalid Data Type";
    }
}

void Interpreter::trimLeft(std::string &string)
{
    string.erase(string.begin(), std::find_if(string.begin(), string.end(),
                                              [](unsigned char ch) {
                                                  return !std::isspace(ch);
                                              }));
}

void Interpreter::trimRight(std::string &string)
{
    string.erase(std::find_if(string.rbegin(), string.rend(),
                              [](unsigned char ch) {
                                  return !std::isspace(ch);
                              }).base(), string.end());
}

void Interpreter::trim(std::string &string)
{
    trimRight(string);
    trimLeft(string);
}

std::string Interpreter::trimLeft_copy(std::string string)
{
    string.erase(string.begin(), std::find_if(string.begin(), string.end(),
                                              [](unsigned char ch) {
                                                  return !std::isspace(ch);
                                              }));
    return string;
}

std::string Interpreter::trimRight_copy(std::string string)
{
    string.erase(std::find_if(string.rbegin(), string.rend(),
                              [](unsigned char ch) {
                                  return !std::isspace(ch);
                              }).base(), string.end());
    return string;
}

std::string Interpreter::trim_copy(std::string string)
{
    trimRight(string);
    trimLeft(string);
    return string;
}

std::string Interpreter::removeSpaces(const std::string &string)
{
    std::string result;
    std::copy_if(string.begin(), string.end(),
                 std::back_inserter(result),
                 [](char ch) { return !std::isspace(ch); });

    return result;
}

bool Interpreter::isNumber(const std::string &string)
{
    return !string.empty() && std::find_if(string.begin(), string.end(),
                                           [](unsigned char c)
                                           { return !std::isdigit(c); }) == string.end();
}


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
        const auto index = line.find('=');
        if(index != std::string::npos)
        {
            //executionReult = solveExpression(line);
            std::string executionReult = line.substr(index + 1);
            std::string name = line.substr(0, index);
            trim(executionReult);
            trim(name);
            changeVariable(name, executionReult);
        }

    } else { //keyword
        const std::string keyword = line.substr(0, index);
        const std::string command = line.substr(index + 1);

        switch (keywordStringTokeywordEnum(keyword)) {
        case KWS::BranchTo: break;
        case KWS::Declare: executionReult = declareVariable(command); break;
        case KWS::Else: pcb->setWithinElseBlock(true); break;
        case KWS::If: ifElseBlock(command); break;
        case KWS::Function: break;
        case KWS::Loop: break;
        case KWS::Return: break;
        case KWS::Syscall: break;
        case KWS::Invoke: pcb->invoke(); break;

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
 *               exception handler
 *
 *  =================================================== */
void Interpreter::exceptionHandler(const std::string &what)
{
    std::cout << what + " line: " + pcb->getLineNumber_s() + ". \n";
}

/** ===================================================
 *
 *              check scope compatibilty
 *
 *  =================================================== */
void Interpreter::checkScopeCompatiblity(const std::string& name)
{
    std::runtime_error error = std::runtime_error("Invalid Variable Access! different scops.");
    const ScopeLevel& scope = scopeFromName(name);
    if(pcb->scope.first < scope.first)
        throw error;

    if(pcb->scope.first > scope.first && !pcb->isWithinFunctionCall())
        return;

    if(pcb->scope.first == scope.first)
    {
        if(pcb->scope.second == scope.second)
            return;
        else
            throw error;
    }

    throw std::runtime_error("Undefined Behavior! function -> accessVariable, args -> " + name);
}


/** ===================================================
 *
 *                  helper functions
 *
 *  =================================================== */
Interpreter::DTS Interpreter::stringTypeToEnumType(const std::string &type)
{
    std::map<std::string, DTS> dataTypes =
        {{"int", DTS::Int}, {"lint", DTS::LInt}, {"llint", DTS::LLInt},
         {"uint", DTS::UInt}, {"F32", DTS::F32}, {"F64", DTS::F64}};

    return dataTypes[type];
}

Interpreter::KWS Interpreter::keywordStringTokeywordEnum(const std::string &type)
{
    std::map<std::string, KWS> keywords =
        {{"declare", KWS::Declare}, {"branch_to", KWS::BranchTo}, {"function", KWS::Function}, {"return", KWS::Return},
         {"if", KWS::If}, {"else", KWS::Else}, {"loop", KWS::Loop}, {"syscall", KWS::Syscall}, {"invoke", KWS::Invoke}};

    return keywords[type];
}

Interpreter::BOPS Interpreter::operatorStringToOperatorEnum(const std::string &type)
{
    std::map<std::string, BOPS> ops =
        {{"==", BOPS::EQ}, {">=", BOPS::GTEQ}, {"<=", BOPS::LTEQ}, {"<", BOPS::LT}, {">", BOPS::GT}, {"!=", BOPS::NEQ}};

    return ops[type];
}

bool Interpreter::checkValue(const std::string &value)
{
    return isNumber(value);
}

void Interpreter::checkDataTypeOnOperation(int n, ...)
{
    va_list pointer;
    va_start(pointer, n);

    std::string defType = va_arg(pointer, char*);
    auto e = std::runtime_error("Operatiorn of Incompatible Data types, all operands must have " +
                                defType + " data type in this expression.");

    for(int i = 1; i < n; i++){
        std::string tmp = va_arg(pointer, char*);
        if(tmp != defType) throw e;
    }

    va_end(pointer);
}

std::any Interpreter::stringToTypedAny(const std::string &type, const std::string &value)
{
    ///exception
    checkValue(value);

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

Interpreter::ScopeLevel Interpreter::scopeFromName(const std::string &name) const noexcept
{
    const std::string scope = name.substr(name.find(':') + 1);
    size_t lvl = std::stoul(scope.substr(0, scope.find('.')));
    size_t slvl = std::stoul(scope.substr(scope.find('.') + 1));
    return {lvl, slvl};
}

std::pair<std::string, size_t> Interpreter::getWordAndItsEnd(const std::string &line, int start)
{
    std::string word = "";
    bool withinWord = false;
    char sep = ' ';
    size_t index;

    for(size_t i = start; i < line.length(); i++){
        if(line[i] == sep && !withinWord) continue;

        if(line[i] != sep && !withinWord) {
            withinWord = true;
        }

        if(line[i] != sep && withinWord){
            word += line[i];
            if(i != line.size() -1) continue;
        }

        if((line[i] == sep &&  withinWord) || i == line.size() -1) {
            //            withinWord = false;
            index = i;
            break;
        }
    }

    return {word, index};
}

void Interpreter::variableAndTypeNameValidator(const std::string &name)
{
    std::string invalidChars = ".-*+/=+)(^%!~>< ";
    for(auto& c : name)
        if(invalidChars.find(c) != std::string::npos)
            throw std::runtime_error("you use invalid chars to define your variable name or use an invalid type name!");
}

std::string Interpreter::scopizeName(const std::string &name, const ScopeLevel &scope)
{
    return name + ":" + std::to_string(scope.first) + "." + std::to_string(scope.second);
}

void Interpreter::invalidKeywordException() noexcept
{

}
