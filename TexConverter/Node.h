#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

enum class NodeType {
	Assign, Plus, Minus, Multiply, Divide, Mod, UnaryPlus, UnaryMinus, GreaterThan, LessThan, EqualTo, NotEqualTo, GreaterOrEqual, LessOrEqual, LogicalAnd, LogicalOr, LogicalNot, AddAssign, SubtractAssign, MultiplyAssign, DivideAssign, ModAssign, Abs, Sqrt, Pow, Ceil, Floor, Round, Exp, Log, Log2, Log10, Sin, Cos, Tan, ArcSin, ArcCos, ArcTan, Indexing, Ternary, TypeCast, Integer, Float, Variable
};

class Node {
private:
	NodeType type;
	string value;
	vector<Node*> operands;

public:
	static const map<string, NodeType> operatorToNodeType;
	static const map<NodeType, int> operatorTypeToOperandCount;
	static const map<NodeType, const string> operatorTypeToTexValue;

	Node(NodeType type, string value);
	Node(NodeType type, vector<Node*> operands);

	bool isOperand() const;
	bool isOperator() const;
	bool isLogOrTrigonometricFunction() const;
	bool isSeparatingOperator() const;
	bool needsParentheses(Node* parent, const bool parentIsFirst);

	NodeType getType() const;
	void setType(NodeType type);

	int getPrecedence() const;
	int getMultiplierPrecedence() const;
	string getTexFormatedValue() const;

	vector<Node*>& getOperands();
	string getValue() const;
};