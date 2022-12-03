#include <string>
#include <stack>
#include <iostream>
#include <algorithm>
#include <tuple>

// + - * / ^

static const int priority = 4;

char priorityToCharacter(int p){
	switch(p){
		case 0: return '+';
		case 1: return '-';
		case 2: return '*';
		case 3: return '/';
		case 4: return '^'; 
	}
}

std::pair<size_t, char> findOperatorWithHigherPrioriy(const std::string& string){
	size_t index = std::string::npos;
	char op = '\0';
	for(int i = priority; i >= 0; i--)
		{
			index = string.find(priorityToCharacter(i));
			if(index != std::string::npos)
				goto ret;
		}

ret:
return {index, op};
}

struct Expression{
	std::string left;
	std::string right;
	char op;
};

std::string extractLeft(const std::string& string, size_t index){

}

std::string extractRight(const std::string& string, size_t index){

}

Expression extractOperands(const std::string& string, 
std::pair<size_t, char> op)
{
	return {
		extractLeft(string, op.first),
		extractRight(string, op.first),
		op.second
	}
}

std::string removeParantesses_copy(const std::string& string){

}

std::string removeParantesses(std::string& string){

}

bool hasOperator(std::string& string){
	const auto op = findOperatorWithHigherPrioriy(string);
	return op.first != std::string::npos;
}

std::string solveExpression(const std::string& string){
	std::stack<std::string> stack;
	const auto op = findOperatorWithHigherPrioriy(string);
	if(op.first == std::string::npos)
		return removeParantesses_copy(string);

	Expression ex = extractOperands(string, op);

	if(!hasOperator(ex.left) || !hasOperator(ex.right)){
		removeParantesses(ex.left);
		removeParantesses(ex.right);
		
	}

	if(hasOperator(ex.left) || hasOperator(ex.right))
	{
		solveExpression(ex.left);
		solveExpression(ex.right);
	}

	
	return extractOperands()
}

int main(){
	//std::stack<std::string> stack;
	//const std::string exp = "((2 - 5)*3)^(2 + 2) ^ 2 * 4";
	int a = 20;
	int * a2 = &a;
	std::cout << a << std::endl;
	*a2 = 10;
	std::cout << a << "  " << &a;
}
