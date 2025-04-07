#pragma once
#include "Error.h"
#include "Node.h"

string readInputFile(string path, Error* error);

Error* writeOutputFile(string path, string line);

Node* parseExtensionParseTreeToNodeTree(string exensionParseTree, vector<Error> errors);

bool isOperator(string value, NodeType* type);

bool isOperand(string value, NodeType* type);

void prepareExtensionParseTree(Node* startNode);

void formatMultiplicationOrder(Node* startNode);

string convertNodeToTex(Node* node, Node* mainOperator, bool inBraskets);

bool compareNodes(const Node* leftNode, const Node* rightNode);

bool isNeedToPutFuncArgsInBraces(Node* node);