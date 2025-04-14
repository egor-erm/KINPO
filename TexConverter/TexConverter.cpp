#include <iostream>
#include <fstream>
#include <sstream>

#include "Functions.h"

using namespace std;

int main()
{
    
}

string readInputFile(string path, Error* error) {
    ifstream file(path);
    // Проверяем, удалось ли открыть файл
    if (!file.is_open()) {
        error = new Error(ErrorType::NoAccessToInputFile);
        return "";
    }

    // Читаем содержимое файла в строку
    stringstream buffer;
    buffer << file.rdbuf();

    // Проверяем, не установлены ли флаги ошибок после чтения
    if (!file) {
        error = new Error(ErrorType::NoAccessToInputFile);
        return "";
    }

    string line = buffer.str();
    if (line.empty()) {
        error = new Error(ErrorType::InputFileIsEmpty);
        return "";
    }

    if (line.find("\n") != string::npos) {
        error = new Error(ErrorType::MultipleInputLines);
        return "";
    }

    // Если всё успешно - возвращаем содержимое
    return line;
}

Error* writeOutputFile(string path, string text) {
    ofstream file(path);

    // Проверяем, удалось ли открыть файл для записи
    if (!file.is_open()) {
        return new Error(ErrorType::ImpossibleToCreateOutputFile);
    }

    // Пытаемся записать строку в файл
    file << text;

    // Проверяем, не произошла ли ошибка при записи
    if (!file) {
        return new Error(ErrorType::ImpossibleToCreateOutputFile);
    }

    // Закрываем файл и проверяем, не было ли ошибки
    file.close();
    if (!file) {
        return new Error(ErrorType::ImpossibleToCreateOutputFile);
    }

    // Если всё успешно - возвращаем NULL
    return NULL;
}

Node* parseExtensionParseTreeToNodeTree(string extensionParseTree, set<Error>& errors) {
    return new Node(NodeType::Integer, "1");
}

bool isOperator(string value, NodeType* type) {
    return false;
}

bool isOperand(const string value, NodeType* type) {
    return false;
}

void prepareExtensionParseTree(Node* startNode) {
 
}

void formatMultiplicationOrder(Node* startNode) {
    
}

bool compareNodes(const Node* leftNode, const Node* rightNode) {
    return leftNode->getMultiplierTypePriority() < rightNode->getMultiplierTypePriority();
}

string convertNodeToTex(Node* node, Node* degreeNode) {
    return "";
}

bool isNeedToPutFuncArgsInParentheses(Node* node) {
    return false;
}

string putInParenthesesIfNeeded(string str, bool putInParentheses) {
    if (putInParentheses) return "(" + str + ")";

    return str;
}