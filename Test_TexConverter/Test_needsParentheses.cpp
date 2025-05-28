#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Node.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestNeedsParentheses
{
	TEST_CLASS(TestNeedsParentheses)
	{
	public:

		// 1. Родительский узел не задан
		TEST_METHOD(ParentNodeNotSpecified)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node1, node2 });

			bool needsParentheses1 = node->needsParentheses(NULL, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(NULL, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 1 провален: \n");
				if (needsParentheses1 != expectedValue1) 
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");
				
				if (needsParentheses2 != expectedValue2) 
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 2. Текущий узел является операндом
		TEST_METHOD(CurrentNodeIsOperand)
		{
			Node* node = new Node(NodeType::Variable, "a");
			Node* node1 = new Node(NodeType::Variable, "b");

			Node* parent = new Node(NodeType::Plus, vector<Node*>{ node, node1 });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 2 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 3. Родительский операнд является делением
		TEST_METHOD(ParentOperandIsDivision)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");
			Node* node3 = new Node(NodeType::Variable, "a");
			Node* node4 = new Node(NodeType::Variable, "b");

			Node* node5 = new Node(NodeType::Multiply, vector<Node*>{ node1, node2 });
			Node* node = new Node(NodeType::Plus, vector<Node*>{ node3, node4 });

			Node* parent = new Node(NodeType::Divide, vector<Node*>{ node5, node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 3 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 4. Родительский операнд является округлением
		TEST_METHOD(ParentOperandIsRounding)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");
			
			Node* node = new Node(NodeType::Plus, vector<Node*>{ node1, node2 });

			Node* parent = new Node(NodeType::Round, vector<Node*>{ node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 4 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 5. Родительский операнд является экспонентой
		TEST_METHOD(ParentOperandIsExponent)
		{
			Node* node1 = new Node(NodeType::Integer, "3");
			Node* node2 = new Node(NodeType::Integer, "5");

			Node* node = new Node(NodeType::Plus, vector<Node*>{ node1, node2 });

			Node* parent = new Node(NodeType::Exp, vector<Node*>{ node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 5 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 6. Аргумент функции является оператором
		TEST_METHOD(FunctionArgumentIsOperator)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node1, node2 });

			Node* parent = new Node(NodeType::Sin, vector<Node*>{ node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = true;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = true;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 6 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 7. Аргументом функции является умножение числа на переменную
		TEST_METHOD(FunctionArgumentIsNumberMultipliedByVariable)
		{
			Node* node1 = new Node(NodeType::Integer, "3");
			Node* node2 = new Node(NodeType::Variable, "b");

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node1, node2 });

			Node* parent = new Node(NodeType::Cos, vector<Node*>{ node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 7 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 8. Аргументом функции является операнд в степени 
		TEST_METHOD(FunctionArgumentIsOperandInPower)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Integer, "2");

			Node* node = new Node(NodeType::Pow, vector<Node*>{ node1, node2 });

			Node* parent = new Node(NodeType::Tan, vector<Node*>{ node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 8 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 9. Аргументом функции является деление
		TEST_METHOD(FunctionArgumentIsDivision)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");

			Node* node = new Node(NodeType::Divide, vector<Node*>{ node1, node2 });

			Node* parent = new Node(NodeType::ArcSin, vector<Node*>{ node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 9 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 10. Текущий элемент является вещественным числом
		TEST_METHOD(CurrentElementIsRealNumber)
		{
			Node* node = new Node(NodeType::Float, "3.5");

			Node* parent = new Node(NodeType::ArcCos, vector<Node*>{ node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 10 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 11. Текущий элемент является вещественным числом в экспоненциальной записи
		TEST_METHOD(CurrentElementIsRealNumberInExponentialNotation)
		{
			Node* node = new Node(NodeType::Float, "3.5e3");

			Node* parent = new Node(NodeType::ArcCos, vector<Node*>{ node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = true;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = true;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 11 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 12. Текущий оператор имеет приоритет меньше, чем у родителя
		TEST_METHOD(CurrentOperatorHasLowerPriorityThanParent)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");

			Node* node3 = new Node(NodeType::Variable, "c");
			Node* node = new Node(NodeType::Plus, vector<Node*>{ node1, node2 });

			Node* parent = new Node(NodeType::Multiply, vector<Node*>{ node3, node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = true;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = true;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 12 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 13. Текущий и родительский узел являются унарными операторами
		TEST_METHOD(CurrentAndParentNodesAreUnaryOperators)
		{
			Node* node1 = new Node(NodeType::Variable, "a");

			Node* node = new Node(NodeType::UnaryPlus, vector<Node*>{ node1 });

			Node* parent = new Node(NodeType::UnaryMinus, vector<Node*>{ node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = true;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = true;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 13 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 14. Текущий и родительский узел являются сложениями
		TEST_METHOD(CurrentAndParentNodesAreAdditions)
		{
			Node* node1 = new Node(NodeType::Variable, "b");
			Node* node2 = new Node(NodeType::Variable, "c");

			Node* node3 = new Node(NodeType::Variable, "a");
			Node* node = new Node(NodeType::Plus, vector<Node*>{ node1, node2 });

			Node* parent = new Node(NodeType::Plus, vector<Node*>{ node3, node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 14 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 15. Текущий и родительский узел имеют одинаковый приоритет
		TEST_METHOD(CurrentAndParentNodesHaveSamePriority)
		{
			Node* node1 = new Node(NodeType::Variable, "b");
			Node* node2 = new Node(NodeType::Variable, "c");

			Node* node3 = new Node(NodeType::Variable, "a");
			Node* node = new Node(NodeType::Minus, vector<Node*>{ node1, node2 });

			Node* parent = new Node(NodeType::Minus, vector<Node*>{ node3, node });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = true;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = true;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 15 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 16. Унарный оператор на первом месте
		TEST_METHOD(UnaryOperatorOnFirstPosition)
		{
			Node* node1 = new Node(NodeType::Variable, "a");

			Node* node = new Node(NodeType::UnaryMinus, vector<Node*>{ node1 });
			Node* node2 = new Node(NodeType::Variable, "b");

			Node* parent = new Node(NodeType::Multiply, vector<Node*>{ node, node2 });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = true;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 16 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}

		// 17. Текущий и родительский узел имеют одинаковый приоритет и стоят на первом месте
		TEST_METHOD(CurrentAndParentNodesHaveSamePriorityAndOnFirstPosition)
		{
			Node* node1 = new Node(NodeType::Variable, "b");
			Node* node2 = new Node(NodeType::Variable, "c");

			Node* node = new Node(NodeType::Mod, vector<Node*>{ node1, node2 });
			Node* node3 = new Node(NodeType::Variable, "a");

			Node* parent = new Node(NodeType::Mod, vector<Node*>{ node, node3 });

			bool needsParentheses1 = node->needsParentheses(parent, false);
			bool expectedValue1 = false;

			bool needsParentheses2 = node->needsParentheses(parent, true);
			bool expectedValue2 = false;

			std::wstringstream wss;
			if (needsParentheses1 != expectedValue1 || needsParentheses2 != expectedValue2) {
				wss << stringToWstring("Тест 17 провален: \n");
				if (needsParentheses1 != expectedValue1)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = false: ожидается '" + to_string(expectedValue1) + "', получено '" + to_string(needsParentheses1) + "'\n");

				if (needsParentheses2 != expectedValue2)
					wss << stringToWstring("Ошибка при вызове функции с parentIsFirst = true: ожидается '" + to_string(expectedValue2) + "', получено '" + to_string(needsParentheses2) + "'\n");
			}

			Assert::IsTrue(needsParentheses1 == expectedValue1 && needsParentheses2 == expectedValue2, wss.str().c_str());
		}
	};
}