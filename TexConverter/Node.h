#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

//! ������������ ����� �����
enum class NodeType {
	Assign, Plus, Minus, Multiply, Divide, Mod, UnaryPlus, UnaryMinus, GreaterThan, LessThan, EqualTo, NotEqualTo, GreaterOrEqual, LessOrEqual, LogicalAnd, LogicalOr, LogicalNot, AddAssign, SubtractAssign, MultiplyAssign, DivideAssign, ModAssign, Abs, Sqrt, Pow, Ceil, Floor, Round, Exp, Log, Log2, Log10, Sin, Cos, Tan, ArcSin, ArcCos, ArcTan, Indexing, Ternary, TypeCast, Integer, Float, Variable
};

//! ���� ������ ������� ���������
class Node {
private:
	//! ��� ����
	NodeType type;
	//! �������� ����
	string value;
	//! �������� ����
	vector<Node*> operands;

public:
	//! ������� �������� �������� ��������� � ��� ���������
	static const map<string, NodeType> operatorToNodeType;
	//! ������� �������� ���� ��������� � ����� ��� ���������
	static const map<NodeType, int> operatorTypeToOperandCount;
	//! ������� �������� ���� �������� � ��� ������ �� ����� tex
	static const map<NodeType, const string> operatorTypeToTexValue;

    /*! ����������� ���� ��� ���������
    * \param[in] type - ��� ����
    * \param[in] value - �������� ���� */
    Node(NodeType type, string value);

    /*! ����������� ���� ��� ����������
     * \param[in] type - ��� ����
     * \param[in] operands - ������ ��������� */
    Node(NodeType type, vector<Node*> operands);

    /*! ���������, �������� �� ���� ���������
     * \return �������� �� ���� ��������� */
    bool isOperand() const;

    /*! ���������, �������� �� ���� ����������
     * \return �������� �� ���� ���������� */
    bool isOperator() const;

    /*! ���������, �������� �� ���� ��������������� ��� ������������������ ��������
     * \return �������� �� ���� ��������������� ��� ������������������ �������� */
    bool isLogOrTrigonometricFunction() const;

    /*! ���������, �������� �� ���� �������������� ����������, �������� �� ���������, ����� �������� ������� ��� � ������� �������
     * \return �������� �� ���� �������������� ���������� */
    bool isSeparatingOperator() const;

    /*! ���������, ����� �� ������ ������ �������� ���� ��� �������������� � TeX
     * \param[in] parent - ������������ ����
     * \param[in] parentIsFirst - ����, �������� �� ������������ ���� ������ ���������
     * \return true ���� ������ ����� */
    bool needsParentheses(Node* parent, const bool parentIsFirst);

    /*! �������� ��� ����
     * \return ��� ���� */
    NodeType getType() const;

    /*! ������������� ��� ����
     * \param[in] type - ����� ��� ���� */
    void setType(NodeType type);

    /*! ���������� ��������� ���������
     * \return ��������� ��������� */
    int getPrecedence() const;

    /*! ���������� ��������� ��� ����������
     * \return ��������� ��������� */
    int getMultiplierPrecedence() const;

    /*! �������� �������� ���� � TeX-�������
     * \return �������� � TeX-������� */
    string getTexFormatedValue() const;

    /*! �������� ������ �� ������ ���������
     * \return ������ �� ������ ��������� */
    vector<Node*>& getOperands();

    /*! �������� �������� ����
     * \return �������� ���� */
    string getValue() const;
};