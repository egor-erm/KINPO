#include "Error.h"
#include "Node.h"
#include <sstream>

Error::Error(ErrorType type) {
	this->type = type;
}

string Error::generateErrorMessage() const {
    ostringstream message; // текст сообщения

    // Для каждого типа ошибки формируем сообщение
    switch (type) {
    case ErrorType::NoAccessToInputFile:
        message << "Неверно указан файл с входными данными. Возможно, файл не существует. Путь: '" << errorInputFileWay << "'";
        break;

    case ErrorType::ImpossibleToCreateOutputFile:
        message << "Неверно указан файл для выходных данных. Возможно, указанного расположения не существует или нет прав на запись. Путь: '" << errorOutputFileWay << "'";
        break;

    case ErrorType::InputFileIsEmpty:
        message << "Входной файл пуст. " + (!errorInputFileWay.empty() ? "Файл: '" + errorInputFileWay + "'" : "");
        break;

    case ErrorType::ExpressionParsingTreeTooLarge:
        message << "Во входной строке слишком много элементов. Число элементов не может быть больше 1000.";
        break;

    case ErrorType::MultipleInputLines:
        message << "Входной файл не может содержать более 1 строки. Разместите каждую строчку в отдельном файле. Файл: '" << errorInputFileWay << "'";
        break;

    case ErrorType::MissingOperand:
        message << "Недостаточное количество операндов. Оператор '" << errorValue << "' с индексом " << errorIndex << " должен принимать "
            << Node::operatorTypeToOperandCount.at(Node::operatorToNodeType.at(errorValue))
            << " операнда.";
        break;

    case ErrorType::ExtraOperand:
        message << "Обнаружен лишний операнд " << errorValue << " с индексом " << errorIndex << ". Возможно, пропущен оператор.";
        break;

    case ErrorType::InvalidExpressionParseTreeElement:
        message << "Элемент " << errorValue << " с индексом " << errorIndex << " не является операндом или оператором.";
        break;

    default:
        message << "Неизвестная ошибка.";
    }

    return message.str();
}

void Error::setErrorInputFileWay(string path) {
	this->errorInputFileWay = path;
}

void Error::setErrorOutputFileWay(string path) {
	this->errorOutputFileWay = path;
}

void Error::setErrorValue(string value) {
	this->errorValue = value;
}

void Error::setErrorIndex(int index) {
	this->errorIndex = index;
}

bool Error::operator<(const Error& err) const {
	return this->generateErrorMessage() < err.generateErrorMessage();
}