#pragma once

#include <string>

using namespace std;

//! ������������ ����� ���������� ������
enum class ErrorType {
	NoAccessToInputFile, ImpossibleToCreateOutputFile, InputFileIsEmpty, ExpressionParsingTreeTooLarge, MultipleInputLines, MissingOperand, ExtraOperand, InvalidExpressionParseTreeElement
};

//! ������
class Error {
private:
	//! ��� ������
	ErrorType type;
	//! ���� �� ������������� �������� �����
	string errorInputFileWay;
	//! ���� �� ��������� �����
	string errorOutputFileWay;
	//! ������������ ��������
	string errorValue;
	//! ������ ������������� ��������
	int errorIndex;
	
public:
	/*! ����������� ������
	* \param[in] type � ��� ������ */
	Error(ErrorType type);

	/*! ���������� ����� ������
	* return � ����� ������ */
	string generateErrorMessage() const;

	/*! ������������� ���� �� �������� �����
	* \param[in] path � ���� */
	void setErrorInputFileWay(string path);

	/*! ������������� ���� �� �������� �����
	* \param[in] path � ���� */
	void setErrorOutputFileWay(string path);

	/*! ������������� ������������ ��������
	* \param[in] value � ���� */
	void setErrorValue(string value);

	/*! ������������� ������ ������������� ��������
	* \param[in] index � ������ */
	void setErrorIndex(int index);

	//! ���������� ��������� <
	bool operator<(const Error& err) const;
};