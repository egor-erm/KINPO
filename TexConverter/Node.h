#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

//! ѕеречисление типов узлов
enum class NodeType {
	Assign, Plus, Minus, Multiply, Divide, Mod, UnaryPlus, UnaryMinus, GreaterThan, LessThan, EqualTo, NotEqualTo, GreaterOrEqual, LessOrEqual, LogicalAnd, LogicalOr, LogicalNot, AddAssign, SubtractAssign, MultiplyAssign, DivideAssign, ModAssign, Abs, Sqrt, Pow, Ceil, Floor, Round, Exp, Log, Log2, Log10, Sin, Cos, Tan, ArcSin, ArcCos, ArcTan, Indexing, Ternary, TypeCast, Integer, Float, Variable
};

//! ”зел дерева разбора выражени€
class Node {
private:
	//! тип узла
	NodeType type;
	//! значение узла
	string value;
	//! операнды узла
	vector<Node*> operands;

public:
	//! —ловарь перевода значени€ оператора в тип оператора
	static const map<string, NodeType> operatorToNodeType;
	//! —ловарь перевода типа оператора в число его операндов
	static const map<NodeType, int> operatorTypeToOperandCount;
	//! —ловарь перевода типа операнда в его аналог на €зыке tex
	static const map<NodeType, const string> operatorTypeToTexValue;

    /*!  онструктор узла дл€ операндов
    * \param[in] type - тип узла
    * \param[in] value - значение узла */
    Node(NodeType type, string value);

    /*!  онструктор узла дл€ операторов
     * \param[in] type - тип узла
     * \param[in] operands - вектор операндов */
    Node(NodeType type, vector<Node*> operands);

    /*! ѕровер€ет, €вл€етс€ ли узел операндом
     * \return €вл€етс€ ли узел операндом */
    bool isOperand() const;

    /*! ѕровер€ет, €вл€етс€ ли узел оператором
     * \return €вл€етс€ ли узел оператором */
    bool isOperator() const;

    /*! ѕровер€ет, €вл€етс€ ли узел логарифмической или тригонометрической функцией
     * \return €вл€етс€ ли узел логарифмической или тригонометрической функцией */
    bool isLogOrTrigonometricFunction() const;

    /*! ѕровер€ет, €вл€етс€ ли узел разделительным оператором, которому не требуетс€, чтобы дочерний элемент был в круглых скобках
     * \return €вл€етс€ ли узел разделительным оператором */
    bool isSeparatingOperator() const;

    /*! ѕровер€ет, нужны ли скобки вокруг текущего узла при преобразовании в TeX
     * \param[in] parent - родительский узел
     * \param[in] parentIsFirst - флаг, €вл€етс€ ли родительский узел первым операндом
     * \return true если скобки нужны */
    bool needsParentheses(Node* parent, const bool parentIsFirst);

    /*! ѕолучает тип узла
     * \return тип узла */
    NodeType getType() const;

    /*! ”станавливает тип узла
     * \param[in] type - новый тип узла */
    void setType(NodeType type);

    /*! ¬озвращает приоритет оператора
     * \return приоритет оператора */
    int getPrecedence() const;

    /*! ¬озвращает приоритет дл€ множителей
     * \return приоритет умножени€ */
    int getMultiplierPrecedence() const;

    /*! ѕолучает значение узла в TeX-формате
     * \return значение в TeX-формате */
    string getTexFormatedValue() const;

    /*! ѕолучает ссылку на вектор операндов
     * \return ссылка на вектор операндов */
    vector<Node*>& getOperands();

    /*! ѕолучает значение узла
     * \return значение узла */
    string getValue() const;
};