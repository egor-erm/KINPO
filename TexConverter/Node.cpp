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
    return this->type == NodeType::Integer || this->type == NodeType::Float || this->type == NodeType::Variable;
}

bool Node::isOperator() const {
    return !this->isOperand();
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

bool Node::isSeparatingOperator() const {
    return this->getType() == NodeType::Assign
        || this->getType() == NodeType::Divide
        || this->getType() == NodeType::Ceil
        || this->getType() == NodeType::Floor
        || this->getType() == NodeType::TypeCast
        || this->getType() == NodeType::Round
        || this->getType() == NodeType::Abs
        || this->getType() == NodeType::Exp
        || this->getType() == NodeType::LogicalNot
        || this->getType() == NodeType::Sqrt;
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

int Node::getMultiplierPrecedence() const {
    switch (type) {
    case NodeType::Integer:
        return 0;
    case NodeType::Float:
        return 1;
    case NodeType::Variable:
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

        // ќпредел€ем систему счислени€
        if (str.size() > 1 && str[0] == '0') {
            if (tolower(str[1]) == 'b' && str.size() > 2) {
                str = "{" + str.substr(2) + "}_2";
            }
            else if (tolower(str[1]) == 'x' && str.size() > 2) {
                str = "{" + str.substr(2) + "}_{16}";
            }
            else {
                str = "{" + str.substr(1) + "}_8";
            }
        }

        break;
    case NodeType::Float:
        // ќбрабатываем каждую цифру, удал€€ суффикс 'f'
        for (char c : value) {
            if (isalpha(c) && tolower(c) != 'e') break;
            str += c;
        }

        if (str[0] == '.') {
            str = "0" + str;
        }

        break;
    default:
        str = value; // ƒл€ остальных типов возвращаем как есть
    }

    return str;
}

bool Node::needsParentheses(Node* parent, const bool parentIsFirst) {
    if (this->isOperand() || parent == NULL || parent->isSeparatingOperator()) return false;

    if (parent->isLogOrTrigonometricFunction()) {
        if (this->getType() == NodeType::Pow && this->getOperands().at(0)->isOperand()) return false;

        if (this->getType() == NodeType::Multiply && this->getOperands().size() == 2
            && (this->getOperands().at(0)->getType() == NodeType::Integer || this->getOperands().at(0)->getType() == NodeType::Float)
            && this->getOperands().at(1)->getType() == NodeType::Variable) return false;

        if (this->getType() == NodeType::Divide) return false;

        return true;
    }

    bool isFirst = parent->getOperands().front() == this;    
    if (this->getPrecedence() < parent->getPrecedence()) return true;
    if (this->getPrecedence() == parent->getPrecedence()) {
        if ((isFirst && this->getType() == NodeType::Pow && parent->getType() == NodeType::Pow) // todo проверка степеней
            || (this->getType() == NodeType::UnaryPlus || this->getType() == NodeType::UnaryMinus) && (parent->getType() == NodeType::UnaryPlus || parent->getType() == NodeType::UnaryMinus)
            ) return true;

        if (isFirst && parentIsFirst 
            || (parent->getType() == NodeType::Plus && this->getType() == NodeType::Plus)
            || parent->getType() == NodeType::LogicalAnd 
            || parent->getType() == NodeType::LogicalOr
            ) return false;

        return true;
    }

    if (this->getType() == NodeType::UnaryMinus || this->getType() == NodeType::UnaryPlus) {
        if (isFirst && parentIsFirst) return false;

        return true;
    }

    return false;
}

int Node::getPrecedence() const {
    switch (type) {
    case NodeType::LogicalNot:
        return 17;

    case NodeType::Pow:
    case NodeType::Abs:
    case NodeType::Ceil:
    case NodeType::Floor:
    case NodeType::Round:
    case NodeType::Log:
    case NodeType::Log2:
    case NodeType::Log10:
    case NodeType::Sin:
    case NodeType::Cos:
    case NodeType::Tan:
    case NodeType::ArcSin:
    case NodeType::ArcCos:
    case NodeType::ArcTan:
    case NodeType::Indexing:
    case NodeType::TypeCast:
        return 16;

    case NodeType::Sqrt:
    case NodeType::Exp:
        return 15;

    case NodeType::Divide:
        return 14;

    case NodeType::UnaryPlus:
    case NodeType::UnaryMinus:
        return 13;

    case NodeType::Multiply:
    case NodeType::Mod:
        return 12;

    case NodeType::Plus:
    case NodeType::Minus:
        return 11;

    case NodeType::GreaterThan:
    case NodeType::LessThan:
    case NodeType::EqualTo:
    case NodeType::NotEqualTo:
    case NodeType::GreaterOrEqual:
    case NodeType::LessOrEqual:
        return 10;

    case NodeType::LogicalAnd:
        return 7;
    case NodeType::LogicalOr:
        return 6;

    case NodeType::Ternary:
        return 4;

    case NodeType::Assign:
    case NodeType::AddAssign:
    case NodeType::SubtractAssign:
    case NodeType::MultiplyAssign:
    case NodeType::DivideAssign:
    case NodeType::ModAssign:
        return 2;

    default:
        return 0;
    }
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
        {NodeType::LogicalNot, 1},
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

const map<NodeType, const string> Node::operatorTypeToTexValue = {
        {NodeType::Assign, "{} \\gets {}"},
        {NodeType::Plus, "{} + {}"},
        {NodeType::Minus, "{} - {}"},
        {NodeType::Multiply, " \\bullet "},
        {NodeType::Divide, "\\frac{{{}}}{{{}}}"},
        {NodeType::Mod, "{} \\mod {}"},
        {NodeType::UnaryPlus, "+{}"},
        {NodeType::UnaryMinus, "-{}"},
        {NodeType::GreaterThan, "{} > {}"},
        {NodeType::LessThan, "{} < {}"},
        {NodeType::EqualTo, "{} = {}"},
        {NodeType::NotEqualTo, "{} \\neq {}"},
        {NodeType::GreaterOrEqual, "{} \\geq {}"},
        {NodeType::LessOrEqual, "{} \\leq {}"},
        {NodeType::LogicalAnd, "{} \\wedge {}"},
        {NodeType::LogicalOr, "{} \\vee {}"},
        {NodeType::Exp, "\\e^{{{}}}"},
        {NodeType::LogicalNot, "\\overline{{{}}}"},
        {NodeType::Abs, "\\abs{{{}}}"},
        {NodeType::Sqrt, "\\sqrt{{{}}}"},
        {NodeType::Ceil, "\\lceil {} \\rceil"},
        {NodeType::Floor, "\\lfloor {} \\rfloor"},
        {NodeType::TypeCast, "\\lfloor {} \\rfloor"},
        {NodeType::Round, "\\lfloor {} \\rceil"},
        {NodeType::Pow, "{{{}}}^{{{}}}"},
        {NodeType::Log, "\\ln{{{}}}{{{}}}"},
        {NodeType::Log2, "\\log_{{2}}{}{{{}}}"},
        {NodeType::Log10, "\\lg{}{{{}}}"},
        {NodeType::Sin, "\\sin{}{{{}}}"},
        {NodeType::Cos, "\\cos{}{{{}}}"},
        {NodeType::Tan, "\\tan{}{{{}}}"},
        {NodeType::ArcSin, "\\arcsin{}{{{}}}"},
        {NodeType::ArcCos, "\\arccos{}{{{}}}"},
        {NodeType::ArcTan, "\\arctan{}{{{}}}"},
        {NodeType::Indexing, "{{{}}}_{{{}}}"},
};