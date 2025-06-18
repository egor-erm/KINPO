#pragma once

#include <string>

using namespace std;

//! ѕеречисление типов допустимых ошибок
enum class ErrorType {
	NoAccessToInputFile, ImpossibleToCreateOutputFile, InputFileIsEmpty, ExpressionParsingTreeTooLarge, MultipleInputLines, MissingOperand, ExtraOperand, InvalidExpressionParseTreeElement
};

//! ќшибка
class Error {
private:
	//! тип ошибки
	ErrorType type;
	//! путь до некорректного входного файла
	string errorInputFileWay;
	//! путь до выходного файла
	string errorOutputFileWay;
	//! недопустимое значение
	string errorValue;
	//! индекс недопустимого значени€
	int errorIndex;
	
public:
	/*!  онструктор ошибки
	* \param[in] type Ц тип ошибки */
	Error(ErrorType type);

	/*! √енерирует текст ошибки
	* return Ц текст ошибки */
	string generateErrorMessage() const;

	/*! ”станавливает путь до входного файла
	* \param[in] path Ц путь */
	void setErrorInputFileWay(string path);

	/*! ”станавливает путь до входного файла
	* \param[in] path Ц путь */
	void setErrorOutputFileWay(string path);

	/*! ”станавливает недопустимое значение
	* \param[in] value Ц путь */
	void setErrorValue(string value);

	/*! ”станавливает индекс недопустимого значени€
	* \param[in] index Ц индекс */
	void setErrorIndex(int index);

	//! ѕерегрузка оператора <
	bool operator<(const Error& err) const;
};