#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestPrepareExtensionParseTree
{
	TEST_CLASS(TestPrepareExtensionParseTree)
	{
	public:

		// 1. Раскрытие операции +=
		TEST_METHOD(DisclosureAddAssign)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Variable, "b");
			Node* expectedNode3 = new Node(NodeType::Plus, vector<Node*>{ expectedNode1, expectedNode2 });
			
			Node* expectedNode4 = new Node(NodeType::Variable, "a");
			Node* expectedNode = new Node(NodeType::Assign, vector<Node*>{ expectedNode4, expectedNode3 });

			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");
			Node* node = new Node(NodeType::AddAssign, vector<Node*>{ node1, node2 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 1 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}
			
			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}

		// 2. Раскрытие операции -=
		TEST_METHOD(DisclosureSubtractAssign)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Variable, "b");
			Node* expectedNode3 = new Node(NodeType::Minus, vector<Node*>{ expectedNode1, expectedNode2 });

			Node* expectedNode4 = new Node(NodeType::Variable, "a");
			Node* expectedNode = new Node(NodeType::Assign, vector<Node*>{ expectedNode4, expectedNode3 });

			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");
			Node* node = new Node(NodeType::SubtractAssign, vector<Node*>{ node1, node2 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 2 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}

			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}

		// 3. Раскрытие операции *=
		TEST_METHOD(DisclosureMultiplyAssign)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Variable, "b");
			Node* expectedNode3 = new Node(NodeType::Multiply, vector<Node*>{ expectedNode1, expectedNode2 });

			Node* expectedNode4 = new Node(NodeType::Variable, "a");
			Node* expectedNode = new Node(NodeType::Assign, vector<Node*>{ expectedNode4, expectedNode3 });

			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");
			Node* node = new Node(NodeType::MultiplyAssign, vector<Node*>{ node1, node2 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 3 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}

			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}

		// 4. Раскрытие операции /=
		TEST_METHOD(DisclosureDivideAssign)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Variable, "b");
			Node* expectedNode3 = new Node(NodeType::Divide, vector<Node*>{ expectedNode1, expectedNode2 });

			Node* expectedNode4 = new Node(NodeType::Variable, "a");
			Node* expectedNode = new Node(NodeType::Assign, vector<Node*>{ expectedNode4, expectedNode3 });

			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");
			Node* node = new Node(NodeType::DivideAssign, vector<Node*>{ node1, node2 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 4 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}

			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}

		// 5. Раскрытие операции %=
		TEST_METHOD(DisclosureModAssign)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Variable, "b");
			Node* expectedNode3 = new Node(NodeType::Mod, vector<Node*>{ expectedNode1, expectedNode2 });

			Node* expectedNode4 = new Node(NodeType::Variable, "a");
			Node* expectedNode = new Node(NodeType::Assign, vector<Node*>{ expectedNode4, expectedNode3 });

			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");
			Node* node = new Node(NodeType::ModAssign, vector<Node*>{ node1, node2 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 5 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}

			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}

		// 6. Левый оператор является обращением к массиву при раскрытии операции +=
		TEST_METHOD(ComplexOperand)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Variable, "i");
			Node* expectedNode3 = new Node(NodeType::Indexing, vector<Node*>{ expectedNode1, expectedNode2 });

			Node* expectedNode4 = new Node(NodeType::Variable, "b");
			Node* expectedNode5 = new Node(NodeType::Plus, vector<Node*>{ expectedNode3, expectedNode4 });

			Node* expectedNode6 = new Node(NodeType::Variable, "a");
			Node* expectedNode7 = new Node(NodeType::Variable, "i");
			Node* expectedNode8 = new Node(NodeType::Indexing, vector<Node*>{ expectedNode6, expectedNode7 });

			Node* expectedNode = new Node(NodeType::Assign, vector<Node*>{ expectedNode8, expectedNode5 });


			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "i");
			Node* node3 = new Node(NodeType::Indexing, vector<Node*>{ node1, node2 });
			Node* node4 = new Node(NodeType::Variable, "b");

			Node* node = new Node(NodeType::AddAssign, vector<Node*>{ node3, node4 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 6 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}

			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}

		// 7. Объединение множителей под одним узлом
		TEST_METHOD(CombiningMultipliers)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Variable, "b");
			Node* expectedNode3 = new Node(NodeType::Variable, "c");
			Node* expectedNode4 = new Node(NodeType::Variable, "d");

			Node* expectedNode = new Node(NodeType::Multiply, vector<Node*>{ expectedNode1, expectedNode2, expectedNode3, expectedNode4 });


			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");
			Node* node3 = new Node(NodeType::Multiply, vector<Node*>{ node1, node2 });

			Node* node4 = new Node(NodeType::Variable, "c");
			Node* node5 = new Node(NodeType::Variable, "d");
			Node* node6 = new Node(NodeType::Multiply, vector<Node*>{ node4, node5 });

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node3, node6 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 7 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}

			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}

		// 8. Дерево содержит операторы, препятствующие объединению множителей
		TEST_METHOD(CombiningMultipliersWithInterruptions)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Variable, "b");
			
			Node* expectedNode3 = new Node(NodeType::Multiply, vector<Node*>{ expectedNode1, expectedNode2 });
			Node* expectedNode4 = new Node(NodeType::Variable, "c");
			
			Node* expectedNode5 = new Node(NodeType::Plus, vector<Node*>{ expectedNode3, expectedNode4 });
			Node* expectedNode6 = new Node(NodeType::Variable, "d");
			Node* expectedNode7 = new Node(NodeType::Variable, "f");

			Node* expectedNode = new Node(NodeType::Multiply, vector<Node*>{ expectedNode5, expectedNode6, expectedNode7 });


			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");

			Node* node3 = new Node(NodeType::Multiply, vector<Node*>{ node1, node2 });
			Node* node4 = new Node(NodeType::Variable, "c");
			Node* node5 = new Node(NodeType::Variable, "d");
			Node* node6 = new Node(NodeType::Variable, "f");

			Node* node7 = new Node(NodeType::Plus, vector<Node*>{ node3, node4 });
			Node* node8 = new Node(NodeType::Multiply, vector<Node*>{ node5, node6 });

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node7, node8 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 8 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}

			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}

		// 9. Объединение множественных обращений к элементу массива под одним узлом
		TEST_METHOD(CombiningIndexes)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Integer, "1");
			Node* expectedNode3 = new Node(NodeType::Integer, "2");
			Node* expectedNode4 = new Node(NodeType::Integer, "3");

			Node* expectedNode = new Node(NodeType::Indexing, vector<Node*>{ expectedNode1, expectedNode2, expectedNode3, expectedNode4 });


			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Integer, "1");

			Node* node3 = new Node(NodeType::Indexing, vector<Node*>{ node1, node2 });
			Node* node4 = new Node(NodeType::Integer, "2");

			Node* node5 = new Node(NodeType::Indexing, vector<Node*>{ node3, node4 });
			Node* node6 = new Node(NodeType::Integer, "3");

			Node* node = new Node(NodeType::Indexing, vector<Node*>{ node5, node6 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 9 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}

			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}

		// 10. Дерево содержит обращения к элементу массива и множители
		TEST_METHOD(MultipliersAndArrayAccesses)
		{
			Node* expectedNode1 = new Node(NodeType::Variable, "b");
			Node* expectedNode2 = new Node(NodeType::Variable, "c");
			Node* expectedNode3 = new Node(NodeType::Variable, "d");
			Node* expectedNode4 = new Node(NodeType::Variable, "f");
			Node* expectedNode5 = new Node(NodeType::Variable, "g");

			Node* expectedNode6 = new Node(NodeType::Variable, "a");
			Node* expectedNode7 = new Node(NodeType::Plus, vector<Node*>{ expectedNode1, expectedNode2});
			Node* expectedNode8 = new Node(NodeType::Multiply, vector<Node*>{ expectedNode3, expectedNode4, expectedNode5 });

			Node* expectedNode = new Node(NodeType::Indexing, vector<Node*>{ expectedNode6, expectedNode7, expectedNode8 });


			Node* node1 = new Node(NodeType::Variable, "b");
			Node* node2 = new Node(NodeType::Variable, "c");
			Node* node3 = new Node(NodeType::Variable, "f");
			Node* node4 = new Node(NodeType::Variable, "g");

			Node* node5 = new Node(NodeType::Variable, "a");
			Node* node6 = new Node(NodeType::Plus, vector<Node*>{ node1, node2 });
			Node* node7 = new Node(NodeType::Variable, "d");
			Node* node8 = new Node(NodeType::Multiply, vector<Node*>{ node3, node4 });

			Node* node9 = new Node(NodeType::Indexing, vector<Node*>{ node5, node6 });
			Node* node10 = new Node(NodeType::Multiply, vector<Node*>{ node7, node8 });

			Node* node = new Node(NodeType::Indexing, vector<Node*>{ node9, node10 });
			prepareExtensionParseTree(node);

			string path = "/";
			set<string>differences;
			std::wstringstream wss;
			if (!compareTrees(expectedNode, node, path, differences)) {
				wss << stringToWstring("Тест 10 провален: \n");

				wss << stringToWstring("Деревья не совпадают: \n");
				for (string diff : differences) {
					wss << stringToWstring(diff) << "\n";
				}
			}

			Assert::IsTrue(differences.empty(), wss.str().c_str());
		}
	};
}