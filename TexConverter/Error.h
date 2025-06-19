#pragma once

#include <string>

using namespace std;

//! Перечисление типов допустимых ошибок
enum class ErrorType {
	NoAccessToInputFile, ImpossibleToCreateOutputFile, InputFileIsEmpty, ExpressionParsingTreeTooLarge, MultipleInputLines, MissingOperand, ExtraOperand, InvalidExpressionParseTreeElement
};

//! Ошибка
class Error {
private:
	//! тип ошибки
	ErrorType type;
	//! путь до входного файла
	string errorInputFileWay;
	//! путь до выходного файла
	string errorOutputFileWay;
	//! недопустимое значение
	string errorValue;
	//! индекс недопустимого значени¤
	int errorIndex;
	
public:
	/*! Конструктор ошибки
	* \param[in] type - тип ошибки */
	Error(ErrorType type);

	/*! Генерирует текст ошибки
	* return - текст ошибки */
	string generateErrorMessage() const;

	/*! Устанавливает путь до входного файла
	* \param[in] path - путь */
	void setErrorInputFileWay(string path);

	/*! Устанавливает путь до выходного файла
	* \param[in] path - путь */
	void setErrorOutputFileWay(string path);

	/*! Устанавливает недопустимое значение
	* \param[in] value - путь */
	void setErrorValue(string value);

	/*! Устанавливает индекс недопустимого значения
	* \param[in] index - индекс */
	void setErrorIndex(int index);

	//! Перегрузка оператора <
	bool operator<(const Error& err) const;
};