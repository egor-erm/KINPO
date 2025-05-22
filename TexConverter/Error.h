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
	string errorOutputFileWay;
	string errorValue;
	int errorIndex;
	
public:
	Error(ErrorType type);
	string generateErrorMessage();
	void setErrorInputFileWay(string path);
	void setErrorOutputFileWay(string path);
	void setErrorValue(string value);
	void setErrorIndex(int index);
	bool operator<(const Error& err) const;
};