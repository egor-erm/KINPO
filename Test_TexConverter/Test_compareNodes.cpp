#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestCompareNodes
{
	TEST_CLASS(TestCompareNodes)
	{
	public:

		// 1. Целое число стоит до вещественного
		TEST_METHOD(IntegerBeforeRealNumber)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Integer, "3"), new Node(NodeType::Float, "3.5")),
				true
			);
		}

		// 2. Целое число стоит после вещественного
		TEST_METHOD(IntegerAfterRealNumber)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Float, "3.5"), new Node(NodeType::Integer, "3")),
				false
			);
		}

		// 3. Целое число стоит до переменной
		TEST_METHOD(IntegerBeforeVariable)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Integer, "3"), new Node(NodeType::Variable, "sum")),
				true
			);
		}
		
		// 4. Целое число стоит после переменной
		TEST_METHOD(IntegerAfterVariable)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Variable, "sum"), new Node(NodeType::Integer, "3")),
				false
			);
		}

		// 5. Целое число стоит до операции
		TEST_METHOD(IntegerBeforeOperation)
		{
			Node* node = new Node(NodeType::Plus, vector<Node*>{
				new Node(NodeType::Variable, "a"),
				new Node(NodeType::Variable, "b")
			});

			Assert::AreEqual(
				compareNodes(new Node(NodeType::Integer, "3"), node),
				true
			);
		}

		// 6. Целое число стоит после операции
		TEST_METHOD(IntegerAfterOperation)
		{
			Node* node = new Node(NodeType::Plus, vector<Node*>{
				new Node(NodeType::Variable, "a"),
				new Node(NodeType::Variable, "b")
			});

			Assert::AreEqual(
				compareNodes(node, new Node(NodeType::Integer, "3")),
				false
			);
		}

		// 7. Вещественное число стоит до переменной
		TEST_METHOD(RealNumberBeforeVariable)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Float, "3.5"), new Node(NodeType::Variable, "sum")),
				true
			);
		}
		
		// 8. Вещественное число стоит после переменной
		TEST_METHOD(RealNumberAfterVariable)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Variable, "sum"), new Node(NodeType::Float, "3.5")),
				false
			);
		}

		// 9. Вещественное число стоит до операции
		TEST_METHOD(RealNumberBeforeOperation)
		{
			Node* node = new Node(NodeType::Plus, vector<Node*>{
				new Node(NodeType::Variable, "a"),
				new Node(NodeType::Variable, "b")
			});

			Assert::AreEqual(
				compareNodes(new Node(NodeType::Float, "3.5"), node),
				true
			);
		}

		// 10. Вещественное число стоит после операции
		TEST_METHOD(RealNumberAfterOperation)
		{
			Node* node = new Node(NodeType::Plus, vector<Node*>{
				new Node(NodeType::Variable, "a"),
				new Node(NodeType::Variable, "b")
			});

			Assert::AreEqual(
				compareNodes(node, new Node(NodeType::Float, "3.5")),
				false
			);
		}

		// 11. Переменная стоит до операции
		TEST_METHOD(VariableBeforeOperation)
		{
			Node* node = new Node(NodeType::Plus, vector<Node*>{
				new Node(NodeType::Variable, "a"),
				new Node(NodeType::Variable, "b")
			});

			Assert::AreEqual(
				compareNodes(new Node(NodeType::Variable, "sum"), node),
				true
			);
		}

		// 12. Переменная стоит после операции
		TEST_METHOD(VariableAfterOperation)
		{
			Node* node = new Node(NodeType::Plus, vector<Node*>{
				new Node(NodeType::Variable, "a"),
				new Node(NodeType::Variable, "b")
			});

			Assert::AreEqual(
				compareNodes(node, new Node(NodeType::Variable, "sum")),
				false
			);
		}

		// 13. Два целых числа
		TEST_METHOD(TwoIntegers)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Integer, "3"), new Node(NodeType::Integer, "5")),
				true
			);
		}

		// 14. Два вещественных числа
		TEST_METHOD(TwoRealNumbers)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Float, "3.5"), new Node(NodeType::Float, "5.3")),
				true
			);
		}

		// 15. Две переменные
		TEST_METHOD(TwoVariables)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Variable, "sum"), new Node(NodeType::Variable, "order")),
				true
			);
		}

		// 16. Две операции
		TEST_METHOD(TwoOperations)
		{
			Node* node1 = new Node(NodeType::Plus, vector<Node*>{
				new Node(NodeType::Variable, "a"),
				new Node(NodeType::Variable, "b")
			});

			Node* node2 = new Node(NodeType::Minus, vector<Node*>{
				new Node(NodeType::Variable, "c"),
				new Node(NodeType::Variable, "d")
			});

			Assert::AreEqual(
				compareNodes(node1, node2),
				true
			);
		}
	};
}