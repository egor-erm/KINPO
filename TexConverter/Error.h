#pragma once
#include <string>

using namespace std;

enum class ErrorType {
	NoAccessToInputFile, ImpossibleToCreateOutputFile, InputFileIsEmpty, ExpressionParsingTreeTooLarge, MultipleInputLines, MissingOperand, ExtraOperand, InvalidExpressionParseTreeElement
};

class Error {
private:
	ErrorType type;
	string errorInputFileWay;
	string errorOutputfileWay;
	string errorValue;
	int errorIndex;

public:
	Error(ErrorType type);
	string generateErrorMessage();
};