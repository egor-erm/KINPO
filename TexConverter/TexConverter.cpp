#define FMT_HEADER_ONLY
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <cctype>
#include <regex>

#include <fmt/core.h>
#include <fmt/args.h>

#include "Functions.h"
#include "Node.h"
#include "Error.h"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cout << "Check arguments count!";
        return 1;
    }

    Error* err = NULL;
    string tree = readInputFile(argv[1], err);
    if (err != NULL) {
        cout << err->generateErrorMessage();
        return 1;
    }

    set<Error> errors;
    Node* node = parseExtensionParseTreeToNodeTree(tree, errors);
    if (errors.size() > 0) {
        for (Error error : errors) {
            cout << error.generateErrorMessage() << endl;
        }

        return 1;
    }

    prepareExtensionParseTree(node);
    formatMultiplicationOrder(node);

    string tex = convertNodeToTex(node, NULL, true);
    tex = "\\documentclass{article}\n\\begin{document}\n$ " + tex + " $\n\\end{document}";

    err = writeOutputFile(argv[2], tex);
    if (err != NULL) {
        cout << err->generateErrorMessage();
        return 1;
    }

    return 0;
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
    stack<Node*> nodeStack;
    istringstream stringStream(extensionParseTree);

    string token;
    int operandsCount, index;
    map<Node*, int> nodesIndex;
    while (stringStream >> token) {
        NodeType type;
        Node* node;
        if (isOperator(token, &type)) {
            operandsCount = Node::operatorTypeToOperandCount.at(type);

            // Проверяем количество операндов в стеке
            if (nodeStack.size() < operandsCount) {
                errors.insert(Error(ErrorType::MissingOperand));
                return nullptr;
            }

            // Извлекаем операнды
            vector<Node*> operands;
            for (int i = 0; i < operandsCount; i++) {
                operands.insert(operands.begin(), nodeStack.top());
                nodeStack.pop();
            }

            // Создаем новый узел-оператор
            node = new Node(type, operands);
        }
        else if (isOperand(token, &type)) {
            // Создаем новый узел-операнд
            node = new Node(type, token);
        }
        else {
            errors.insert(Error(ErrorType::InvalidExpressionParseTreeElement));
            return nullptr;
        }

        nodeStack.push(node);
        nodesIndex.insert({ node, index });
        index++;
    }

    if (nodeStack.size() == 0) {
        errors.insert(Error(ErrorType::InputFileIsEmpty));
        return nullptr;
    }

    if (nodeStack.size() > 1) {
        nodeStack.pop();
        errors.insert(Error(ErrorType::ExtraOperand));
        return nullptr;
    }

    return nodeStack.top();
}

bool isOperator(string value, NodeType* type) {
    if (Node::operatorToNodeType.count(value)) {
        *type = Node::operatorToNodeType.at(value);
        return true;
    }

    return false;
}

bool isOperand(const string value, NodeType* type) {
    // Регулярное выражение для переменной
    regex variable_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
    if (regex_match(value, variable_regex)) {
        if (type) *type = NodeType::Variable;
        return true;
    }

    // Регулярные выражения для разных форматов чисел
    regex integer_regex("^(0b[01]+|0[0-7]+|0x[0-9a-fA-F]+|\\d+)[LlUu]?$");
    if (regex_match(value, integer_regex)) {
        if (type) *type = NodeType::Integer;
        return true;
    }

    regex float_regex("^(\\d+\\.\\d*|\\.\\d+|\\d+)([eE][-+]?\\d+)?[FfLl]?$");
    if (regex_match(value, float_regex)) {
        if (type) *type = NodeType::Float;
        return true;
    }

    return false;
}

void prepareExtensionParseTree(Node* startNode) {
    if (startNode->isOperand()) return;

    static map<NodeType, NodeType> operators = {
        {NodeType::AddAssign, NodeType::Plus},
        {NodeType::SubtractAssign, NodeType::Minus},
        {NodeType::MultiplyAssign, NodeType::Multiply},
        {NodeType::DivideAssign, NodeType::Divide},
        {NodeType::ModAssign, NodeType::Mod},
    };

    if (operators.count(startNode->getType())) {
        NodeType operatorType = operators.at(startNode->getType());
        Node* operationNode = new Node(operatorType, startNode->getOperands());

        startNode->getOperands().pop_back();
        startNode->getOperands().push_back(operationNode);

        startNode->setType(NodeType::Assign);
    }

    Node* operand;
    for (int i = 0; i < startNode->getOperands().size(); i++) {
        operand = startNode->getOperands().at(i);
        prepareExtensionParseTree(operand);

        if ((startNode->getType() == NodeType::Multiply && operand->getType() == NodeType::Multiply)
            || (i == 0 && startNode->getType() == NodeType::Indexing && operand->getType() == NodeType::Indexing)) {
            startNode->getOperands().erase(startNode->getOperands().begin() + i);

            startNode->getOperands().insert(startNode->getOperands().begin() + i, operand->getOperands().begin(), operand->getOperands().end());

            i += operand->getOperands().size() - 1;
            delete operand;
        }
    }
}

void formatMultiplicationOrder(Node* startNode) {
    if (startNode->getType() == NodeType::Multiply) {
        sort(startNode->getOperands().begin(), startNode->getOperands().end(), compareNodes);
    }

    for (Node* operand : startNode->getOperands()) {
        formatMultiplicationOrder(operand);
    }
}

bool compareNodes(const Node* leftNode, const Node* rightNode) {
    return leftNode->getMultiplierPrecedence() < rightNode->getMultiplierPrecedence();
}

string convertNodeToTex(Node* node, Node* degreeNode, const bool& isFirstOperand) {
    if (node->isOperand()) {
        return node->getTexFormatedValue();
    }

    vector<Node*> operands = node->getOperands();
    static const regex one_regex("^1[LlUu]?$|^1\.0[FfLl]?$");
    if (node->getType() == NodeType::Multiply) {
        bool leftInParentheses, rightInParentheses;
        Node * leftOperator, * rightOperator;

        leftOperator = operands[0];
        leftInParentheses = leftOperator->isNeedParentheses(node, isFirstOperand);

        string tex = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL, isFirstOperand || leftInParentheses), leftInParentheses);
        for (int i = 1; i < operands.size(); i++) {
            rightOperator = operands[i];
            rightInParentheses = rightOperator->isNeedParentheses(node, isFirstOperand);

            bool omitBullet = ((leftOperator->getType() == NodeType::Integer || leftOperator->getType() == NodeType::Float || leftOperator->getType() == NodeType::Divide)
                && (rightOperator->getType() == NodeType::Variable || rightOperator->isLogOrTrigonometricFunction()))
                || leftInParentheses || rightInParentheses;

            if (!omitBullet) tex += Node::operatorTypeToTexValue.at(node->getType());

            tex += putInParenthesesIfNeeded(convertNodeToTex(rightOperator, NULL, right), rightInParentheses);

            leftInParentheses = rightInParentheses;
            leftOperator = rightOperator;
        }

        return tex;
    }
    else if (node->getType() == NodeType::Pow) {
        if (operands[1]->getType() == NodeType::Divide) {
            string tex;
            if (operands[0]->isLogOrTrigonometricFunction()) {
                tex = convertNodeToTex(operands[0], operands[1]->getOperands().at(0), true) + "}";
            }
            else {
                string operand = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL, true), operands[0]->isNeedParentheses(node, isFirstOperand));
                string degree = convertNodeToTex(operands[1]->getOperands().at(0), NULL, true);
               
                tex = fmt::vformat(
                    Node::operatorTypeToTexValue.at(node->getType()),
                    fmt::make_format_args(operand, degree)
                );
            }

            if (!regex_match(operands[1]->getOperands().at(1)->getValue(), one_regex)) {
                tex = "\\sqrt[" + convertNodeToTex(operands[1]->getOperands().at(1), NULL, true) + "]{" + tex + "}";
            }

            return tex;
        }
        else {
            if (operands[0]->isLogOrTrigonometricFunction()) {
                return convertNodeToTex(operands[0], operands[1], true);
            }
            else {
                string operand = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL, true), operands[0]->isNeedParentheses(node, isFirstOperand));
                string degree = convertNodeToTex(operands[1], NULL, true);
                
                return fmt::vformat(
                    Node::operatorTypeToTexValue.at(node->getType()),
                    fmt::make_format_args(operand, degree)
                );
            }
        }
    }
    else if (node->isLogOrTrigonometricFunction()) {
        string operand = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL, true), operands[0]->isNeedParentheses(node, isFirstOperand));

        string degree = "";
        if (degreeNode != NULL && !regex_match(degreeNode->getValue(), one_regex))
            degree = "^{" + convertNodeToTex(degreeNode, NULL, true) + "}";

        return fmt::vformat(
            Node::operatorTypeToTexValue.at(node->getType()),
            fmt::make_format_args(degree, operand)
        );
    }
    else if (node->getType() == NodeType::Indexing) {
        string operand = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL, true), operands[0]->isOperator());
        string indexes = convertNodeToTex(operands[1], NULL, true);

        for (int i = 2; i < operands.size(); i++) {
            indexes += ", " + convertNodeToTex(operands[i], NULL, true);
        }

        return fmt::vformat(
            Node::operatorTypeToTexValue.at(node->getType()),
            fmt::make_format_args(operand, indexes)
        );
    }
    else if (node->getType() == NodeType::Ternary) {
        string condition = convertNodeToTex(operands[0], NULL, true);
        return "\\begin{cases}" + convertNodeToTex(operands[1], NULL, true) + ", " + condition + " = true\\\\"
            + convertNodeToTex(operands[2], NULL, true) + ", " + condition + " = false\\end{cases}";
    }
    else if (operands.size() == 2) {
        bool firstOpearandInParentheses = operands[0]->isNeedParentheses(node, isFirstOperand);
        bool secondOpearandInParentheses = operands[1]->isNeedParentheses(node, isFirstOperand);
        string operand1 = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL, isFirstOperand || node->isSeparatingOperator() || firstOpearandInParentheses), firstOpearandInParentheses);
        string operand2 = putInParenthesesIfNeeded(convertNodeToTex(operands[1], NULL, node->isSeparatingOperator() || secondOpearandInParentheses), secondOpearandInParentheses);

        return fmt::vformat(
            Node::operatorTypeToTexValue.at(node->getType()),
            fmt::make_format_args(operand1, operand2)
        );
    }
    else {
        bool opearandInParentheses = operands[0]->isNeedParentheses(node, isFirstOperand);
        string operand = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL, isFirstOperand || node->isSeparatingOperator() || opearandInParentheses), opearandInParentheses);
        return fmt::vformat(
            Node::operatorTypeToTexValue.at(node->getType()),
            fmt::make_format_args(operand)
        );
    }

    return "";
}

string putInParenthesesIfNeeded(string str, bool putInParentheses) {
    if (putInParentheses) return "(" + str + ")";

    return str;
}