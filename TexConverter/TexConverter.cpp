#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <cctype>
#include <regex>
#include "Functions.h"
#include "Node.h"
#include "Error.h"

// using namespace std;

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

    string tex = convertNodeToTex(node, NULL);

    tex = "\\documentclass{article}\n\\begin{document}\n$ " + tex + " $\n\\end{document}";

    err = writeOutputFile(argv[2], tex);
    if (err != NULL) {
        cout << err->generateErrorMessage();
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
        if (type) *type = NodeType::Vаriable;
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
            // || (startNode->getType() == NodeType::Plus && operand->getType() == NodeType::Plus)
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
    return leftNode->getMultiplierTypePriority() < rightNode->getMultiplierTypePriority();
}

string convertNodeToTex(Node* node, Node* degreeNode) {
    if (node->isOperand()) {
        return node->getTexFormatedValue();
    }

    static const regex one_regex("^1[LlUu]?$|^1\.0[FfLl]?$");

    string tex;
    bool leftInParentheses, rightInParentheses, isPlus, isMinus;
    Node* leftOperator, * rightOperator;
    vector<Node*> operands = node->getOperands();
    switch (node->getType()) {
    case NodeType::Assign:
        tex = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->getType() == NodeType::Assign)
            + " \\gets "
            + putInParenthesesIfNeeded(convertNodeToTex(operands[1], NULL), operands[1]->getType() == NodeType::Assign);
        break;
    case NodeType::Plus:
        /*for (int i = 1; i < operands.size(); i++) {
            isPlus = true;
            if (operands[i]->getType() == NodeType::Multiply && (operands[i]->getOperands().at(0)->getType() == NodeType::UnaryPlus || operands[i]->getOperands().at(0)->getType() == NodeType::UnaryMinus)) {
                Node* unaryOperand = operands[i]->getOperands().at(0);
                if (unaryOperand->getType() == NodeType::UnaryMinus) isPlus = false;

                operands[i]->getOperands().at(0) = unaryOperand->getOperands().at(0);
                delete unaryOperand;
            }

            tex += (isPlus ? " + " : " - ") + putInParenthesesIfNeeded(convertNodeToTex(operands[i], NULL), operands[i]->getType() == NodeType::UnaryPlus || operands[i]->getType() == NodeType::UnaryMinus || operands[i]->getType() == NodeType::Ternary || operands[i]->getType() == NodeType::Assign || operands[i]->isLogicalOrCompareOperator());
        }*/

        tex = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->getType() == NodeType::Ternary || operands[0]->getType() == NodeType::Assign || operands[0]->isLogicalOrCompareOperator());

        isPlus = true;
        if (operands[1]->getType() == NodeType::Multiply && (operands[1]->getOperands().at(0)->getType() == NodeType::UnaryPlus || operands[1]->getOperands().at(0)->getType() == NodeType::UnaryMinus)) {
            Node* unaryOperand = operands[1]->getOperands().at(0);
            if (unaryOperand->getType() == NodeType::UnaryMinus) {
                isPlus = false;
                node->setType(NodeType::Minus);
            }

            operands[1]->getOperands().at(0) = unaryOperand->getOperands().at(0);
            delete unaryOperand;
        }

        tex += (isPlus ? " + " : " - ") + putInParenthesesIfNeeded(convertNodeToTex(operands[1], NULL), operands[1]->getType() == NodeType::UnaryPlus || operands[1]->getType() == NodeType::UnaryMinus || operands[1]->getType() == NodeType::Ternary || operands[1]->getType() == NodeType::Assign || operands[1]->isLogicalOrCompareOperator());
        break;
    case NodeType::Minus:
        tex = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->getType() == NodeType::Ternary || operands[0]->getType() == NodeType::Assign || operands[0]->isLogicalOrCompareOperator());

        isMinus = true;
        if (operands[1]->getType() == NodeType::Multiply && (operands[1]->getOperands().at(0)->getType() == NodeType::UnaryPlus || operands[1]->getOperands().at(0)->getType() == NodeType::UnaryMinus)) {
            Node* unaryOperand = operands[1]->getOperands().at(0);
            if (unaryOperand->getType() == NodeType::UnaryMinus) {
                isMinus = false;
                node->setType(NodeType::Plus);
            }

            operands[1]->getOperands().at(0) = unaryOperand->getOperands().at(0);
            delete unaryOperand;
        }

        tex += (isMinus ? " - " : " + ") + putInParenthesesIfNeeded(convertNodeToTex(operands[1], NULL), operands[1]->getType() == NodeType::UnaryPlus || operands[1]->getType() == NodeType::UnaryMinus || operands[1]->getType() == NodeType::Plus || operands[1]->getType() == NodeType::Minus || operands[1]->getType() == NodeType::Ternary || operands[1]->getType() == NodeType::Assign || operands[1]->isLogicalOrCompareOperator());
        break;
    case NodeType::Multiply:
        leftOperator = operands[0];
        leftInParentheses = leftOperator->getType() == NodeType::Plus || leftOperator->getType() == NodeType::Minus || leftOperator->getType() == NodeType::Ternary || leftOperator->getType() == NodeType::Assign || leftOperator->isLogicalOrCompareOperator();
        tex = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), leftInParentheses);

        for (int i = 1; i < operands.size(); i++) {
            rightOperator = operands[i];
            rightInParentheses = rightOperator->getType() == NodeType::UnaryPlus || rightOperator->getType() == NodeType::UnaryMinus || rightOperator->getType() == NodeType::Plus || rightOperator->getType() == NodeType::Minus || rightOperator->getType() == NodeType::Ternary || rightOperator->getType() == NodeType::Assign || rightOperator->isLogicalOrCompareOperator();

            bool omitBullet = ((leftOperator->getType() == NodeType::Integer || leftOperator->getType() == NodeType::Float || leftOperator->getType() == NodeType::Divide)
                && (rightOperator->getType() == NodeType::Vаriable || rightOperator->isLogOrTrigonometricFunction()))
                || leftInParentheses || rightInParentheses;

            if (!omitBullet) tex += " \\bullet ";

            tex += putInParenthesesIfNeeded(convertNodeToTex(rightOperator, NULL), rightInParentheses);

            leftInParentheses = rightInParentheses;
            leftOperator = rightOperator;
        }

        break;
    case NodeType::Divide:
        tex = "\\frac{" + convertNodeToTex(operands[0], NULL) + "}{" + convertNodeToTex(operands[1], NULL) + "}";
        break;
    case NodeType::Mod:
        tex = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->getType() == NodeType::Plus || operands[0]->getType() == NodeType::Minus || operands[0]->getType() == NodeType::Ternary || operands[0]->getType() == NodeType::Assign || operands[0]->isLogicalOrCompareOperator());
        +" \\mod "
            + putInParenthesesIfNeeded(convertNodeToTex(operands[1], NULL), operands[1]->getType() == NodeType::UnaryPlus || operands[1]->getType() == NodeType::UnaryMinus || operands[1]->getType() == NodeType::Plus || operands[1]->getType() == NodeType::Minus || operands[1]->getType() == NodeType::Ternary || operands[0]->getType() == NodeType::Assign || operands[0]->isLogicalOrCompareOperator());

        break;
    case NodeType::UnaryPlus:
        tex = "+" + putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->isOperator() && operands[0]->getOperands().size() > 1 && operands[0]->getType() == NodeType::Divide);
        break;
    case NodeType::UnaryMinus:
        tex = "-" + putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->isOperator() && operands[0]->getOperands().size() > 1 && operands[0]->getType() == NodeType::Divide);
        break;
    case NodeType::GreaterThan:
    case NodeType::LessThan:
    case NodeType::EqualTo:
    case NodeType::NotEqualTo:
    case NodeType::GreaterOrEqual:
    case NodeType::LessOrEqual:
    case NodeType::LogicalAnd:
    case NodeType::LogicalOr:
        tex = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->getType() == NodeType::Ternary || operands[0]->getType() == NodeType::Assign || operands[0]->isLogicalOrCompareOperator())
            + Node::operatorTypeToTexValue.at(node->getType())
            + putInParenthesesIfNeeded(convertNodeToTex(operands[1], NULL), operands[1]->getType() == NodeType::Ternary || operands[1]->getType() == NodeType::Assign || operands[1]->isLogicalOrCompareOperator());

        break;
    case NodeType::Exp:
        tex = "e^{" + convertNodeToTex(operands[0], NULL) + "}";
        break;
    case NodeType::Pow:
        if (operands[0]->isLogOrTrigonometricFunction()) {
            if (operands[1]->getType() == NodeType::Divide) {
                tex = convertNodeToTex(operands[0], operands[1]->getOperands().at(0)) + "}";

                if (!regex_match(operands[1]->getOperands().at(1)->getValue(), one_regex)) {
                    tex = "\\sqrt[" + convertNodeToTex(operands[1]->getOperands().at(1), NULL) + "]{" + tex + "}";
                }
            }
            else {
                tex = convertNodeToTex(operands[0], operands[1]);
            }
        }
        else {
            if (operands[1]->getType() == NodeType::Divide) {
                tex = "{" + putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->isOperator()) + "}"
                    + "^{" + convertNodeToTex(operands[1]->getOperands().at(0), NULL) + "}";

                if (!regex_match(operands[1]->getOperands().at(1)->getValue(), one_regex)) {
                    tex = "\\sqrt[" + convertNodeToTex(operands[1]->getOperands().at(1), NULL) + "]{" + tex + "}";
                }
            }
            else {
                tex = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->isOperator())
                    + "^{" + convertNodeToTex(operands[1], NULL) + "}";
            }
        }

        break;
    case NodeType::Ceil:
        tex = "\\lceil " + convertNodeToTex(operands[0], NULL) + " \\rceil";
        break;
    case NodeType::Floor:
    case NodeType::TypeCast:
        tex = "\\lfloor " + convertNodeToTex(operands[0], NULL) + " \\rfloor";
        break;
    case NodeType::Round:
        tex = "\\lfloor " + convertNodeToTex(operands[0], NULL) + " \\rceil";
        break;
    case NodeType::Log:
    case NodeType::Log2:
    case NodeType::Log10:
    case NodeType::Sin:
    case NodeType::Cos:
    case NodeType::Tan:
    case NodeType::ArcSin:
    case NodeType::ArcCos:
    case NodeType::ArcTan:
        tex = Node::operatorTypeToTexValue.at(node->getType());

        if (degreeNode != NULL)
            tex += "^{" + convertNodeToTex(degreeNode, NULL) + "}";

        tex += "{" + putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), isNeedToPutFuncArgsInParentheses(operands[0])) + "}";

        break;
    case NodeType::LogicalNot:
    case NodeType::Abs:
    case NodeType::Sqrt:
        tex = Node::operatorTypeToTexValue.at(node->getType()) + "{" + convertNodeToTex(operands[0], NULL) + "}";
        break;
    case NodeType::Indexing:
        tex = putInParenthesesIfNeeded(convertNodeToTex(operands[0], NULL), operands[0]->isOperator())
            + "_{" + convertNodeToTex(operands[1], NULL);

        for (int i = 2; i < operands.size(); i++) {
            tex += ", " + convertNodeToTex(operands[i], NULL);
        }

        tex += "}";
        break;
    case NodeType::Ternary:
        string condition = convertNodeToTex(operands[0], NULL);
        tex = "\\begin{cases}" + convertNodeToTex(operands[1], NULL) + ", " + condition + " = true\\\\"
            + convertNodeToTex(operands[2], NULL) + ", " + condition + " = false\\end{cases}";
        break;
    }

    return tex;
}

bool isNeedToPutFuncArgsInParentheses(Node* node) {
    if (node->isOperand()) return false;

    if (node->getType() == NodeType::Pow && node->getOperands().at(0)->isOperand()) return false;

    if (node->getType() == NodeType::Multiply && node->getOperands().size() == 2
        && (node->getOperands().at(0)->getType() == NodeType::Integer || node->getOperands().at(0)->getType() == NodeType::Float)
        && node->getOperands().at(1)->getType() == NodeType::Vаriable) return false;

    if (node->getType() == NodeType::Divide) return false;

    return true;
}

string putInParenthesesIfNeeded(string str, bool putInParentheses) {
    if (putInParentheses) return "(" + str + ")";

    return str;
}