#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestCompareNodes
{
	TEST_CLASS(TestCompareNodes)
	{
	public:

		// 1. ����� ����� ����� �� �������������
		TEST_METHOD(IntegerBeforeRealNumber)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Integer, "3"), new Node(NodeType::Float, "3.5")),
				true
			);
		}

		// 2. ����� ����� ����� ����� �������������
		TEST_METHOD(IntegerAfterRealNumber)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Float, "3.5"), new Node(NodeType::Integer, "3")),
				false
			);
		}

		// 3. ����� ����� ����� �� ����������
		TEST_METHOD(IntegerBeforeVariable)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Integer, "3"), new Node(NodeType::Variable, "sum")),
				true
			);
		}
		
		// 4. ����� ����� ����� ����� ����������
		TEST_METHOD(IntegerAfterVariable)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Variable, "sum"), new Node(NodeType::Integer, "3")),
				false
			);
		}

		// 5. ����� ����� ����� �� ��������
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

		// 6. ����� ����� ����� ����� ��������
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

		// 7. ������������ ����� ����� �� ����������
		TEST_METHOD(RealNumberBeforeVariable)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Float, "3.5"), new Node(NodeType::Variable, "sum")),
				true
			);
		}
		
		// 8. ������������ ����� ����� ����� ����������
		TEST_METHOD(RealNumberAfterVariable)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Variable, "sum"), new Node(NodeType::Float, "3.5")),
				false
			);
		}

		// 9. ������������ ����� ����� �� ��������
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

		// 10. ������������ ����� ����� ����� ��������
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

		// 11. ���������� ����� �� ��������
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

		// 12. ���������� ����� ����� ��������
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

		// 13. ��� ����� �����
		TEST_METHOD(TwoIntegers)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Integer, "3"), new Node(NodeType::Integer, "5")),
				true
			);
		}

		// 14. ��� ������������ �����
		TEST_METHOD(TwoRealNumbers)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Float, "3.5"), new Node(NodeType::Float, "5.3")),
				true
			);
		}

		// 15. ��� ����������
		TEST_METHOD(TwoVariables)
		{
			Assert::AreEqual(
				compareNodes(new Node(NodeType::Variable, "sum"), new Node(NodeType::Variable, "order")),
				true
			);
		}

		// 16. ��� ��������
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