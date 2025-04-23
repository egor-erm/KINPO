#pragma once

#include <set>
#include "Error.h"
#include "Node.h"

string readInputFile(string path, Error* error);

Error* writeOutputFile(string path, string text);

Node* parseExtensionParseTreeToNodeTree(string exensionParseTree, set<Error>& errors);

bool isOperator(string value, NodeType* type);

bool isOperand(string value, NodeType* type);

void prepareExtensionParseTree(Node* startNode);

void formatMultiplicationOrder(Node* startNode);

string convertNodeToTex(Node* node, Node* degreeNode, const bool& isFirstOperand);

bool compareNodes(const Node* leftNode, const Node* rightNode);

string putInParenthesesIfNeeded(string str, bool putInParentheses);