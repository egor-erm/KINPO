#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

//! Перечисление типов узлов
enum class NodeType {
	Assign, Plus, Minus, Multiply, Divide, Mod, UnaryPlus, UnaryMinus, GreaterThan, LessThan, EqualTo, NotEqualTo, GreaterOrEqual, LessOrEqual, LogicalAnd, LogicalOr, LogicalNot, AddAssign, SubtractAssign, MultiplyAssign, DivideAssign, ModAssign, Abs, Sqrt, Pow, Ceil, Floor, Round, Exp, Log, Log2, Log10, Sin, Cos, Tan, ArcSin, ArcCos, ArcTan, Indexing, Ternary, TypeCast, Integer, Float, Variable
};

//! Узел дерева разбора выражения
class Node {
private:
	//! тип узла
	NodeType type;
	//! значение узла
	string value;
	//! операнды узла
	vector<Node*> operands;

public:
	//! Словарь перевода значения оператора в тип оператора
	static const map<string, NodeType> operatorToNodeType;
	//! Словарь перевода типа оператора в число его операндов
	static const map<NodeType, int> operatorTypeToOperandCount;
	//! Словарь перевода типа операнда в его аналог на языке TeX
	static const map<NodeType, const string> operatorTypeToTexValue;

    /*! Конструктор узла для операндов
    * \param[in] type - тип узла
    * \param[in] value - значение узла */
    Node(NodeType type, string value);

    /*! Конструктор узла для операторов
     * \param[in] type - тип узла
     * \param[in] operands - вектор операндов */
    Node(NodeType type, vector<Node*> operands);

    /*! Проверяет, является ли узел операндом
     * \return является ли узел операндом */
    bool isOperand() const;

    /*! Проверяет, является ли узел оператором
     * \return является ли узел оператором */
    bool isOperator() const;

    /*! Проверяет, является ли узел логарифмической или тригонометрической функцией
     * \return является ли узел логарифмической или тригонометрической функцией */
    bool isLogOrTrigonometricFunction() const;

    /*! Проверяет, является ли узел разделительным оператором, которому не требуется, чтобы дочерний элемент был в круглых скобках
     * \return является ли узел разделительным оператором */
    bool isSeparatingOperator() const;

    /*! Проверяет, нужны ли скобки вокруг текущего узла при преобразовании в TeX
     * \param[in] parent - родительский узел
     * \param[in] parentIsFirst - флаг, является ли родительский узел первым операндом
     * \return true если скобки нужны */
    bool needsParentheses(Node* parent, const bool parentIsFirst);

    /*! Получает тип узла
     * \return тип узла */
    NodeType getType() const;

    /*! Устанавливает тип узла
     * \param[in] type - новый тип узла */
    void setType(NodeType type);

    /*! Возвращает приоритет оператора
     * \return приоритет оператора */
    int getPrecedence() const;

    /*! Возвращает приоритет для множителей
     * \return приоритет умножения */
    int getMultiplierPrecedence() const;

    /*! Получает значение узла в TeX-формате
     * \return значение в TeX-формате */
    string getTexFormatedValue() const;

    /*! Получает ссылку на вектор операндов
     * \return ссылка на вектор операндов */
    vector<Node*>& getOperands();

    /*! Получает значение узла
     * \return значение узла */
    string getValue() const;
};