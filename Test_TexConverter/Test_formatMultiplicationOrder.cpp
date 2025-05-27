#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFormatMultiplicationOrder
{
	TEST_CLASS(TestFormatMultiplicationOrder)
	{
	public:

		TEST_METHOD(MultipliersAreSorted)
		{
			Node* expectedNode1 = new Node(NodeType::Integer, "3");
			Node* expectedNode2 = new Node(NodeType::Float, "5.7");
			Node* expectedNode3 = new Node(NodeType::Variable, "a");
			Node* expectedNode4 = new Node(NodeType::Variable, "x");
			Node* expectedNode5 = new Node(NodeType::Sin, vector<Node*>{ expectedNode4 });

			Node* expectedNode = new Node(NodeType::Multiply, vector<Node*>{ expectedNode1, expectedNode2, expectedNode3, expectedNode5 });


			Node* node1 = new Node(NodeType::Integer, "3");
			Node* node2 = new Node(NodeType::Float, "5.7");
			Node* node3 = new Node(NodeType::Variable, "a");
			Node* node4 = new Node(NodeType::Variable, "x");
			Node* node5 = new Node(NodeType::Sin, vector<Node*>{ node4 });

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node1, node2, node3, node5 });
			formatMultiplicationOrder(node);

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

		TEST_METHOD(MultipliersInReverseOrder)
		{
			Node* expectedNode1 = new Node(NodeType::Integer, "3");
			Node* expectedNode2 = new Node(NodeType::Float, "5.7");
			Node* expectedNode3 = new Node(NodeType::Variable, "a");
			Node* expectedNode4 = new Node(NodeType::Variable, "x");
			Node* expectedNode5 = new Node(NodeType::Sin, vector<Node*>{ expectedNode4 });

			Node* expectedNode = new Node(NodeType::Multiply, vector<Node*>{ expectedNode1, expectedNode2, expectedNode3, expectedNode5 });


			Node* node1 = new Node(NodeType::Integer, "3");
			Node* node2 = new Node(NodeType::Float, "5.7");
			Node* node3 = new Node(NodeType::Variable, "a");
			Node* node4 = new Node(NodeType::Variable, "x");
			Node* node5 = new Node(NodeType::Sin, vector<Node*>{ node4 });

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node5, node3, node2, node1 });
			formatMultiplicationOrder(node);

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

		TEST_METHOD(MultipliersInRandomOrder)
		{
			Node* expectedNode1 = new Node(NodeType::Integer, "3");
			Node* expectedNode2 = new Node(NodeType::Float, "5.7");
			Node* expectedNode3 = new Node(NodeType::Variable, "a");
			Node* expectedNode4 = new Node(NodeType::Variable, "x");
			Node* expectedNode5 = new Node(NodeType::Sin, vector<Node*>{ expectedNode4 });

			Node* expectedNode = new Node(NodeType::Multiply, vector<Node*>{ expectedNode1, expectedNode2, expectedNode3, expectedNode5 });


			Node* node1 = new Node(NodeType::Integer, "3");
			Node* node2 = new Node(NodeType::Float, "5.7");
			Node* node3 = new Node(NodeType::Variable, "a");
			Node* node4 = new Node(NodeType::Variable, "x");
			Node* node5 = new Node(NodeType::Sin, vector<Node*>{ node4 });

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node2, node5, node1, node3 });
			formatMultiplicationOrder(node);

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

		TEST_METHOD(IndependentMultipliers)
		{
			
			Node* expectedNode1 = new Node(NodeType::Variable, "x");

			Node* expectedNode2 = new Node(NodeType::Integer, "3"); 
			Node* expectedNode3 = new Node(NodeType::Variable, "a");
			Node* expectedNode4 = new Node(NodeType::Float, "5.7");
			Node* expectedNode5 = new Node(NodeType::Sin, vector<Node*>{ expectedNode1 }); 

			Node* expectedNode6 = new Node(NodeType::Multiply, vector<Node*>{ expectedNode2, expectedNode3 });
			Node* expectedNode7 = new Node(NodeType::Multiply, vector<Node*>{ expectedNode4, expectedNode5 });
			
			Node* expectedNode = new Node(NodeType::Plus, vector<Node*>{ expectedNode6, expectedNode7 });


			Node* node1 = new Node(NodeType::Variable, "x");

			Node* node2 = new Node(NodeType::Variable, "a");
			Node* node3 = new Node(NodeType::Integer, "3");
			Node* node4 = new Node(NodeType::Sin, vector<Node*>{ node1 });
			Node* node5 = new Node(NodeType::Float, "5.7");

			Node* node6 = new Node(NodeType::Multiply, vector<Node*>{ node2, node3 });
			Node* node7 = new Node(NodeType::Multiply, vector<Node*>{ node4, node5 });

			Node* node = new Node(NodeType::Plus, vector<Node*>{ node6, node7 });
			formatMultiplicationOrder(node);

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
	};
}