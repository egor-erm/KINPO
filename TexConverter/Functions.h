#pragma once

#include <set>
#include "Error.h"
#include "Node.h"

/*! Читает содержимое файла
* \param[in] path – путь до входного файла
* \param[out] error – ошибка чтения файла
* \return – содержимое файла
*/
string readInputFile(string path, Error*& error);

/*! Записывает текст в файл
* \param[in] path – путь до выходного файла
* \param[in] text – текст для записи в файл
* \return – ошибка записи файла
*/
Error* writeOutputFile(string path, string text);

/*! Преобразует выражение из обратной польской записи в дерево
* \param[in] extensionParseTree – дерево разбора выражения в обратной польской записи
* \param[out] errors – возвращаемый список ошибок
* \return - указатель на начальный узел дерева
*/
Node* parseExtensionParseTreeToNodeTree(string exensionParseTree, set<Error>& errors);

/*! Определяет, является ли значение оператором
* \param[in] value – значение
* \param[out] type – тип оператора
* \return - является ли значение оператором
*/
bool isOperator(string value, NodeType* type);

/*! Определяет, является ли значение операндом
* \param[in] value – значение
* \param[out] type – тип оператора
* \return - является ли значение операндом
*/
bool isOperand(string value, NodeType* type);

/*! Преобразует дерево к TeX формату
* \param[in|out] startNode – указатель на начальный узел дерева
*/
void prepareExtensionParseTree(Node* startNode);

/*! Расставляет множители в нужном порядке
* \param[in|out] startNode – указатель на начальный узел дерева
*/
void formatMultiplicationOrder(Node* startNode);

/*! Переводит дерево разбора выражения в формат TeX
* \param[in] node - указатель на начальный узел
* \param[in] degreeNode - указатель на узел показателя степени
* \param[in] isFirstOperand – флаг нахождения операнда в начале выражения
* \return – строка, содержащая выражение в формате Tex
*/
string convertNodeToTex(Node* node, Node* degreeNode, const bool isFirstOperand);

/*! Сравнивает два узла по приоритетам множителей
* \param[in] leftNode - указатель на первый узел
* \param[in] rightNode - указатель на второй узел
* \return – первый множитель должен стоять раньше второго
*/
bool compareNodes(const Node* leftNode, const Node* rightNode);

/*! Оборачивает строку в круглые скобки, если требуется
* \param[in] str - строка
* \param[in] putInParentheses - необходимо ли обернуть строку в круглые скобки
* \return – строка в круглых скобках при необходимости
*/
string putInParenthesesIfNeeded(string str, const bool putInParentheses);