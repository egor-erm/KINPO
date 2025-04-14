#include "Node.h"

using namespace std;

Node::Node(NodeType type, string value) {
    this->type = type;
    this->value = value;
}

Node::Node(NodeType type, const vector<Node*> operands) {
    this->type = type;
    this->operands = operands;
}

bool Node::isOperand() const {
    return this->type == NodeType::Integer || this->type == NodeType::Float || this->type == NodeType::Vаriable;
}

bool Node::isOperator() const {
    return !this->isOperand();
}

bool Node::isLogicalOrCompareOperator() const {
    return this->type == NodeType::GreaterThan
        || this->type == NodeType::LessThan
        || this->type == NodeType::EqualTo
        || this->type == NodeType::NotEqualTo
        || this->type == NodeType::GreaterOrEqual
        || this->type == NodeType::LessOrEqual
        || this->type == NodeType::LogicalAnd
        || this->type == NodeType::LogicalOr
        || this->type == NodeType::LogicalNot;
}

bool Node::isLogOrTrigonometricFunction() const {
    return this->type == NodeType::Log
        || this->type == NodeType::Log2
        || this->type == NodeType::Log10
        || this->type == NodeType::Sin
        || this->type == NodeType::Cos
        || this->type == NodeType::Tan
        || this->type == NodeType::ArcSin
        || this->type == NodeType::ArcCos
        || this->type == NodeType::ArcTan;
}

NodeType Node::getType() const {
    return this->type;
}

void Node::setType(NodeType type) {
    this->type = type;
}

string Node::getValue() const {
    return this->value;
}

vector<Node*>& Node::getOperands() {
    return this->operands;
}

int Node::getMultiplierTypePriority() const {
    switch (type) {
    case NodeType::Integer:
        return 0;
    case NodeType::Float:
        return 1;
    case NodeType::Vаriable:
        return 2;
    default:
        return 3;
    }
}

string Node::getTexFormatedValue() const {
    string str;

    switch (type) {
    case NodeType::Integer:
        for (char c : value) {
            if (isalpha(c) && tolower(c) != 'x' && tolower(c) != 'b') break;
            str += c;
        }

        // Определяем систему счисления
        if (str.size() > 1 && str[0] == '0') {
            if (tolower(str[1]) == 'b' && str.size() > 2) {
                str = "\\{" + str.substr(2) + "\\}_2";
            }
            else if (tolower(str[1]) == 'x' && str.size() > 2) {
                str = "\\{" + str.substr(2) + "\\}_{16}";
            }
            else {
                str = "\\{" + str.substr(1) + "\\}_8";
            }
        }

        break;
    case NodeType::Float:
        // Обрабатываем каждую цифру, удаляя суффикс 'f'
        for (char c : value) {
            if (isalpha(c) && tolower(c) != 'e') break;
            str += c;
        }

        if (str[0] == '.') {
            str = "0" + str;
        }
    default:
        str = value; // Для остальных типов возвращаем как есть
    }

    return str;
}

const map<string, NodeType> Node::operatorToNodeType = {
        {"=", NodeType::Assign},
        {"+", NodeType::Plus},
        {"-", NodeType::Minus},
        {"*", NodeType::Multiply},
        {"/", NodeType::Divide},
        {"%", NodeType::Mod},
        {"1+", NodeType::UnaryPlus},
        {"1-", NodeType::UnaryMinus},
        {">", NodeType::GreaterThan},
        {"<", NodeType::LessThan},
        {"==", NodeType::EqualTo},
        {"!=", NodeType::NotEqualTo},
        {">=", NodeType::GreaterOrEqual},
        {"<=", NodeType::LessOrEqual},
        {"&&", NodeType::LogicalAnd},
        {"||", NodeType::LogicalOr},
        {"!", NodeType::LogicalNot},
        {"+=", NodeType::AddAssign},
        {"-=", NodeType::SubtractAssign},
        {"*=", NodeType::MultiplyAssign},
        {"/=", NodeType::DivideAssign},
        {"%=", NodeType::ModAssign},
        {"abs", NodeType::Abs},
        {"sqrt", NodeType::Sqrt},
        {"pow", NodeType::Pow},
        {"ceil", NodeType::Ceil},
        {"floor", NodeType::Floor},
        {"round", NodeType::Round},
        {"exp", NodeType::Exp},
        {"log", NodeType::Log},
        {"log2", NodeType::Log2},
        {"log10", NodeType::Log10},
        {"sin", NodeType::Sin},
        {"cos", NodeType::Cos},
        {"tan", NodeType::Tan},
        {"asin", NodeType::ArcSin},
        {"acos", NodeType::ArcCos},
        {"atan", NodeType::ArcTan},
        {"[]", NodeType::Indexing},
        {"?:", NodeType::Ternary},
        {"(type)", NodeType::TypeCast}
};

const map<NodeType, int> Node::operatorTypeToOperandCount = {
        {NodeType::Assign, 2},
        {NodeType::Plus, 2},
        {NodeType::Minus, 2},
        {NodeType::Multiply, 2},
        {NodeType::Divide, 2},
        {NodeType::Mod, 2},
        {NodeType::UnaryPlus, 1},
        {NodeType::UnaryMinus, 1},
        {NodeType::GreaterThan, 2},
        {NodeType::LessThan, 2},
        {NodeType::EqualTo, 2},
        {NodeType::NotEqualTo, 2},
        {NodeType::GreaterOrEqual, 2},
        {NodeType::LessOrEqual, 2},
        {NodeType::LogicalAnd, 2},
        {NodeType::LogicalOr, 2},
        {NodeType::LogicalNot, 2},
        {NodeType::AddAssign, 2},
        {NodeType::SubtractAssign, 2},
        {NodeType::MultiplyAssign, 2},
        {NodeType::DivideAssign, 2},
        {NodeType::ModAssign, 2},
        {NodeType::Abs, 1},
        {NodeType::Sqrt, 1},
        {NodeType::Pow, 2},
        {NodeType::Ceil, 1},
        {NodeType::Floor, 1},
        {NodeType::Round, 1},
        {NodeType::Exp, 1},
        {NodeType::Log, 1},
        {NodeType::Log2, 1},
        {NodeType::Log10, 1},
        {NodeType::Sin, 1},
        {NodeType::Cos, 1},
        {NodeType::Tan, 1},
        {NodeType::ArcSin, 1},
        {NodeType::ArcCos, 1},
        {NodeType::ArcTan, 1},
        {NodeType::Indexing, 2},
        {NodeType::Ternary, 3},
        {NodeType::TypeCast, 1}
};

const map<NodeType, string> Node::operatorTypeToTexValue = {
        {NodeType::UnaryPlus, "+"},
        {NodeType::UnaryMinus, "-"},
        {NodeType::GreaterThan, " > "},
        {NodeType::LessThan, " < "},
        {NodeType::EqualTo, " = "},
        {NodeType::NotEqualTo, " \\neq "},
        {NodeType::GreaterOrEqual, " \\geqslant "},
        {NodeType::LessOrEqual, " \\leqslant "},
        {NodeType::LogicalAnd, " \\wedge "},
        {NodeType::LogicalOr, " \\vee "},
        {NodeType::LogicalNot, " \\overline"},
        {NodeType::Abs, "\\abs"},
        {NodeType::Sqrt, "\\sqrt"},
        {NodeType::Log, "\\ln"},
        {NodeType::Log2, "\\log_{2}"},
        {NodeType::Log10, "\\lg"},
        {NodeType::Sin, "\\sin"},
        {NodeType::Cos, "\\cos"},
        {NodeType::Tan, "\\tan"},
        {NodeType::ArcSin, "\\arcsin"},
        {NodeType::ArcCos, "\\arccos"},
        {NodeType::ArcTan, "\\arctan"},
};