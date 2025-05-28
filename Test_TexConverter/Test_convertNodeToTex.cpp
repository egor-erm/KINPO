#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestConvertNodeToTex
{
	TEST_CLASS(TestConvertNodeToTex)
	{
	public:

		// 1. Дерево разбора выражения с арифметическими операторами
		TEST_METHOD(ExpressionParseTreeWithArithmeticOperators)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");

			Node* node3 = new Node(NodeType::UnaryMinus, vector<Node*>{ node1 });
			Node* node4 = new Node(NodeType::UnaryPlus, vector<Node*>{ node2 });

			Node* node5 = new Node(NodeType::Plus, vector<Node*>{ node3, node4 });
			Node* node6 = new Node(NodeType::Variable, "c");

			Node* node7 = new Node(NodeType::Minus, vector<Node*>{ node5, node6 });
			Node* node8 = new Node(NodeType::Variable, "d");
			Node* node9 = new Node(NodeType::Variable, "f");
			Node* node10 = new Node(NodeType::Variable, "g");

			Node* node11 = new Node(NodeType::Multiply, vector<Node*>{ node7, node8 });
			Node* node12 = new Node(NodeType::Divide, vector<Node*>{ node9, node10 });

			Node* node13 = new Node(NodeType::Variable, "result");
			Node* node14 = new Node(NodeType::Mod, vector<Node*>{ node11, node12 });

			Node* node = new Node(NodeType::Assign, vector<Node*>{ node13, node14 });

			string expectedTex = "result \\gets (-a + (+b) - c)d \\mod \\frac{f}{g}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 2. Дерево разбора выражения с операторами сравнения и различными операндами
		TEST_METHOD(ExpressionParseTreeWithComparisonOperatorsAndVariousOperands)
		{
			Node* node1 = new Node(NodeType::Variable, "count");
			Node* node2 = new Node(NodeType::Integer, "5L");

			Node* node3 = new Node(NodeType::Integer, "48");
			Node* node4 = new Node(NodeType::Variable, "dif");

			Node* node5 = new Node(NodeType::Float, ".5f");
			Node* node6 = new Node(NodeType::Variable, "c");

			Node* node7 = new Node(NodeType::GreaterOrEqual, vector<Node*>{ node1, node2 });
			Node* node8 = new Node(NodeType::LessOrEqual, vector<Node*>{ node3, node4 });
			Node* node9 = new Node(NodeType::Float, "3.6");
			Node* node10 = new Node(NodeType::NotEqualTo, vector<Node*>{ node5, node6 });

			Node* node11 = new Node(NodeType::GreaterThan, vector<Node*>{ node7, node8 });
			Node* node12 = new Node(NodeType::LessThan, vector<Node*>{ node9, node10 });

			Node* node = new Node(NodeType::EqualTo, vector<Node*>{ node11, node12 });

			string expectedTex = "count \\geq 5 > (48 \\leq dif) = (3.6 < (0.5 \\neq c))";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 3. Дерево разбора выражения с логическими операторами
		TEST_METHOD(ExpressionParseTreeWithLogicalOperators)
		{
			Node* node1 = new Node(NodeType::Variable, "ab_");
			Node* node2 = new Node(NodeType::Integer, "1");
			Node* node3 = new Node(NodeType::Variable, "bo2");

			Node* node4 = new Node(NodeType::LogicalOr, vector<Node*>{ node1, node2 });
			Node* node5 = new Node(NodeType::LogicalNot, vector<Node*>{ node3 });

			Node* node = new Node(NodeType::LogicalAnd, vector<Node*>{ node4, node5 });

			string expectedTex = "(ab\\_ \\vee 1) \\wedge \\overline{bo2}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 4. Дерево разбора выражения с тригонометрическими функциями
		TEST_METHOD(ExpressionParseTreeWithTrigonometricFunctions)
		{
			Node* node1 = new Node(NodeType::Variable, "x");
			Node* node2 = new Node(NodeType::ArcTan, vector<Node*>{ node1 });
			Node* node3 = new Node(NodeType::Tan, vector<Node*>{ node2 });
			Node* node4 = new Node(NodeType::ArcCos, vector<Node*>{ node3 });
			Node* node5 = new Node(NodeType::Cos, vector<Node*>{ node4 });
			Node* node6 = new Node(NodeType::ArcSin, vector<Node*>{ node5 });

			Node* node = new Node(NodeType::Sin, vector<Node*>{ node6 });

			string expectedTex = "\\sin{(\\arcsin{(\\cos{(\\arccos{(\\tan{(\\arctan{x})})})})})}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 5. Дерево разбора выражения с математическими функциями
		TEST_METHOD(ExpressionParseTreeWithMathematicalFunctions)
		{
			Node* node1 = new Node(NodeType::Float, "5.3f");
			Node* node2 = new Node(NodeType::Variable, "b");

			Node* node3 = new Node(NodeType::Round, vector<Node*>{ node1 });
			Node* node4 = new Node(NodeType::Log10, vector<Node*>{ node2 });

			Node* node5 = new Node(NodeType::Floor, vector<Node*>{ node3 });
			Node* node6 = new Node(NodeType::Log2, vector<Node*>{ node4 });

			Node* node7 = new Node(NodeType::Ceil, vector<Node*>{ node5 });
			Node* node8 = new Node(NodeType::Log, vector<Node*>{ node6 });

			Node* node9 = new Node(NodeType::Abs, vector<Node*>{ node7 });
			Node* node10 = new Node(NodeType::Exp, vector<Node*>{ node8 });

			Node* node = new Node(NodeType::Pow, vector<Node*>{ node9, node10 });

			string expectedTex = "{\\abs{\\lceil \\lfloor \\lfloor 5.3 \\rceil \\rfloor \\rceil}}^{e^{\\ln{(\\log_{2}{(\\lg{b})})}}}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 6. Дерево разбора выражения с особыми операторами (тернарный, обращение к элементу массива, приведение типа)
		TEST_METHOD(ExpressionParseTreeWithSpecialOperators)
		{
			Node* node1 = new Node(NodeType::Variable, "b");
			Node* node2 = new Node(NodeType::Variable, "i");
			Node* node3 = new Node(NodeType::Variable, "c");

			Node* node4 = new Node(NodeType::Variable, "a");
			Node* node5 = new Node(NodeType::Indexing, vector<Node*>{ node1, node2 });
			Node* node6 = new Node(NodeType::TypeCast, vector<Node*>{ node3 });

			Node* node = new Node(NodeType::Ternary, vector<Node*>{ node4, node5, node6 });

			string expectedTex = "\\begin{cases}{b}_{i}, a = true\\\\\\lfloor c \\rfloor, a = false\\end{cases}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 7. Дерево разбора выражения содержит тернарный оператор, который является правым операндом операции присваивания
		TEST_METHOD(ExpressionParseTreeContainsTernaryOperatorAsRightOperandOfAssignment)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Integer, "5");
			Node* node3 = new Node(NodeType::Variable, "arr");
			Node* node4 = new Node(NodeType::Variable, "i");
			Node* node5 = new Node(NodeType::Variable, "c");

			Node* node6 = new Node(NodeType::GreaterThan, vector<Node*>{ node1, node2 });
			Node* node7 = new Node(NodeType::Indexing, vector<Node*>{ node3, node4 });
			Node* node8 = new Node(NodeType::TypeCast, vector<Node*>{ node5 });

			Node* node9 = new Node(NodeType::Variable, "count");
			Node* node10 = new Node(NodeType::Ternary, vector<Node*>{ node6, node7, node8 });
			
			Node* node = new Node(NodeType::Assign, vector<Node*>{ node9, node10 });

			string expectedTex = "count \\gets \\begin{cases}{arr}_{i}, a > 5\\\\\\lfloor c \\rfloor, a \\leq 5\\end{cases}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 8. Дерево разбора выражения содержит тернарный оператор, который является частью другой операции
		TEST_METHOD(ExpressionParseTreeContainsTernaryOperatorAsPartOfAnotherOperation)
		{
			Node* node1 = new Node(NodeType::Variable, "arr");
			Node* node2 = new Node(NodeType::Variable, "i");
			Node* node3 = new Node(NodeType::Variable, "c");

			Node* node4 = new Node(NodeType::Variable, "a");
			Node* node5 = new Node(NodeType::Indexing, vector<Node*>{ node1, node2 });
			Node* node6 = new Node(NodeType::TypeCast, vector<Node*>{ node3 });
			Node* node7 = new Node(NodeType::Float, "3.5");
			Node* node8 = new Node(NodeType::Variable, "dif");

			Node* node9 = new Node(NodeType::Ternary, vector<Node*>{ node4, node5, node6 });
			Node* node10 = new Node(NodeType::Multiply, vector<Node*>{ node7, node8 });

			Node* node = new Node(NodeType::Plus, vector<Node*>{ node9, node10 });

			string expectedTex = "(\\begin{cases}{arr}_{i}, a = true\\\\\\lfloor c \\rfloor, a = false\\end{cases}) + 3.5dif";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 9. Сложение целочисленных операндов в различном формате
		TEST_METHOD(AdditionOfIntegerOperandsInDifferentFormats)
		{
			Node* node1 = new Node(NodeType::Integer, "3U");
			Node* node2 = new Node(NodeType::Integer, "0b11");
			Node* node3 = new Node(NodeType::Integer, "03");
			Node* node4 = new Node(NodeType::Integer, "0x3");

			Node* node5 = new Node(NodeType::Integer, "3");
			Node* node6 = new Node(NodeType::Integer, "3L");
			Node* node7 = new Node(NodeType::Plus, vector<Node*>{ node1, node2 });
			Node* node8 = new Node(NodeType::Plus, vector<Node*>{ node3, node4 });

			Node* node9 = new Node(NodeType::Plus, vector<Node*>{ node5, node6 });
			Node* node10 = new Node(NodeType::Plus, vector<Node*>{ node7, node8 });

			Node* node = new Node(NodeType::Plus, vector<Node*>{ node9, node10 });

			string expectedTex = "3 + 3 + 3 + 11_2 + 3_8 + 3_{16}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 10. Вычитание целочисленных операндов в различном формате
		TEST_METHOD(SubtractionOfIntegerOperandsInDifferentFormats)
		{
			Node* node1 = new Node(NodeType::Integer, "3U");
			Node* node2 = new Node(NodeType::Integer, "0b11");
			Node* node3 = new Node(NodeType::Integer, "03");
			Node* node4 = new Node(NodeType::Integer, "0x3");

			Node* node5 = new Node(NodeType::Integer, "3");
			Node* node6 = new Node(NodeType::Integer, "3L");
			Node* node7 = new Node(NodeType::Minus, vector<Node*>{ node1, node2 });
			Node* node8 = new Node(NodeType::Minus, vector<Node*>{ node3, node4 });

			Node* node9 = new Node(NodeType::Minus, vector<Node*>{ node5, node6 });
			Node* node10 = new Node(NodeType::Minus, vector<Node*>{ node7, node8 });

			Node* node = new Node(NodeType::Minus, vector<Node*>{ node9, node10 });

			string expectedTex = "3 - 3 - (3 - 11_2 - (3_8 - 3_{16}))";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 11. Различный формат записи вещественных операндов
		TEST_METHOD(DifferentFormatsOfRealOperands)
		{
			Node* node1 = new Node(NodeType::Float, "5.3");
			Node* node2 = new Node(NodeType::Float, ".3");
			Node* node3 = new Node(NodeType::Float, "5.2e3");
			Node* node4 = new Node(NodeType::Float, "3.2f");

			Node* node5 = new Node(NodeType::Plus, vector<Node*>{ node1, node2 });
			Node* node6 = new Node(NodeType::Plus, vector<Node*>{ node3, node4 });

			Node* node = new Node(NodeType::Plus, vector<Node*>{ node5, node6 });

			string expectedTex = "5.3 + 0.3 + 5.2 \\bullet 10^{3} + 3.2";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 12. Вещественные числа в экспоненциальной записи
		TEST_METHOD(RealNumbersInExponentialNotation)
		{
			Node* node1 = new Node(NodeType::Float, "3.5");
			Node* node2 = new Node(NodeType::Integer, "7");
			Node* node3 = new Node(NodeType::Float, "5.4");
			Node* node4 = new Node(NodeType::Float, "3.4e3");
			Node* node5 = new Node(NodeType::Integer, "3");
			Node* node6 = new Node(NodeType::Float, "5.4e3");

			Node* node7 = new Node(NodeType::Pow, vector<Node*>{ node1, node2 });
			Node* node8 = new Node(NodeType::Sin, vector<Node*>{ node3 });
			Node* node9 = new Node(NodeType::Pow, vector<Node*>{ node4, node5 });
			Node* node10 = new Node(NodeType::Sin, vector<Node*>{ node6 });

			Node* node11 = new Node(NodeType::Minus, vector<Node*>{ node7, node8 });
			Node* node12 = new Node(NodeType::Minus, vector<Node*>{ node9, node10 });

			Node* node = new Node(NodeType::Plus, vector<Node*>{ node11, node12 });

			string expectedTex = "{3.5}^{7} - \\sin{5.4} + ({(3.5 \\bullet 10^{3})}^{7} - \\sin{(5.4 \\bullet 10^{3})})";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 13. Дерево с объединёнными обращениями к массиву и множителями
		TEST_METHOD(TreeWithCombinedArrayAccessAndMultipliers)
		{
			Node* node1 = new Node(NodeType::Variable, "b");
			Node* node2 = new Node(NodeType::Variable, "c");
			Node* node3 = new Node(NodeType::Variable, "d");
			Node* node4 = new Node(NodeType::Variable, "f");
			Node* node5 = new Node(NodeType::Variable, "g");

			Node* node6 = new Node(NodeType::Variable, "a");
			Node* node7 = new Node(NodeType::Plus, vector<Node*>{ node1, node2 });
			Node* node8 = new Node(NodeType::Multiply, vector<Node*>{ node3, node4, node5 });

			Node* node = new Node(NodeType::Indexing, vector<Node*>{ node6, node7, node8 });

			string expectedTex = "{a}_{b + c, d \\bullet f \\bullet g}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 14. Возведение в дробную степень
		TEST_METHOD(ExponentiationToFractionalPower)
		{
			Node* node1 = new Node(NodeType::Variable, "b");
			Node* node2 = new Node(NodeType::Variable, "c");

			Node* node3 = new Node(NodeType::Variable, "a");
			Node* node4 = new Node(NodeType::Divide, vector<Node*>{ node1, node2 });

			Node* node = new Node(NodeType::Pow, vector<Node*>{ node3, node4 });

			string expectedTex = "\\sqrt[c]{{a}^{b}}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 15. Возведение в дробную степень (знаменатель - единица)
		TEST_METHOD(ExponentiationToFractionalPowerDenominatorOne)
		{
			Node* node1 = new Node(NodeType::Variable, "b");
			Node* node2 = new Node(NodeType::Integer, "1");

			Node* node3 = new Node(NodeType::Variable, "a");
			Node* node4 = new Node(NodeType::Divide, vector<Node*>{ node1, node2 });

			Node* node = new Node(NodeType::Pow, vector<Node*>{ node3, node4 });

			string expectedTex = "{a}^{b}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 16. Возведение функции в степень
		TEST_METHOD(FunctionExponentiation)
		{
			Node* node1 = new Node(NodeType::Variable, "x");

			Node* node2 = new Node(NodeType::Sin, vector<Node*>{ node1 });
			Node* node3 = new Node(NodeType::Integer, "2");

			Node* node = new Node(NodeType::Pow, vector<Node*>{ node2, node3 });

			string expectedTex = "\\sin^{2}{x}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 17. Возведение функции со сложным аргументом в степень
		TEST_METHOD(FunctionWithComplexArgumentExponentiation)
		{
			Node* node1 = new Node(NodeType::Variable, "b");
			Node* node2 = new Node(NodeType::Variable, "c");

			Node* node3 = new Node(NodeType::Variable, "a");
			Node* node4 = new Node(NodeType::Minus, vector<Node*>{ node1, node2 });

			Node* node5 = new Node(NodeType::Plus, vector<Node*>{ node3, node4 });
			Node* node6 = new Node(NodeType::Variable, "d");
			Node* node7 = new Node(NodeType::Variable, "f");

			Node* node8 = new Node(NodeType::Sin, vector<Node*>{ node5 });
			Node* node9 = new Node(NodeType::Multiply, vector<Node*>{ node6, node7 });

			Node* node = new Node(NodeType::Pow, vector<Node*>{ node8, node9 });

			string expectedTex = "\\sin^{d \\bullet f}{(a + b - c)}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 18. Множители разных типов
		TEST_METHOD(MultipliersOfDifferentTypes)
		{
			Node* node1 = new Node(NodeType::Variable, "a");
			Node* node2 = new Node(NodeType::Variable, "b");
			Node* node3 = new Node(NodeType::Variable, "d");
			Node* node4 = new Node(NodeType::Variable, "f");
			Node* node5 = new Node(NodeType::Variable, "x");

			Node* node6 = new Node(NodeType::Integer, "3");
			Node* node7 = new Node(NodeType::Float, "5.7");
			Node* node8 = new Node(NodeType::Divide, vector<Node*>{ node1, node2 });
			Node* node9 = new Node(NodeType::Variable, "c");
			Node* node10 = new Node(NodeType::Plus, vector<Node*>{ node3, node4 });
			Node* node11 = new Node(NodeType::Sin, vector<Node*>{ node5 });

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node6, node7, node8, node9, node10, node11 });

			string expectedTex = "3 \\bullet 5.7 \\bullet \\frac{a}{b}c(d + f)\\sin{x}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}

		// 19. Множителями являются вещественное число, переменная и оператор-функция
		TEST_METHOD(MultipliersAreRealNumberVariableAndFunctionOperator)
		{
			Node* node1 = new Node(NodeType::Variable, "x");

			Node* node2 = new Node(NodeType::Float, "3.3");
			Node* node3 = new Node(NodeType::Variable, "ab");
			Node* node4 = new Node(NodeType::Tan, vector<Node*>{ node1 });

			Node* node = new Node(NodeType::Multiply, vector<Node*>{ node2, node3, node4 });

			string expectedTex = "3.3ab \\bullet \\tan{x}";
			string tex = convertNodeToTex(node, NULL, true);

			Assert::AreEqual(expectedTex, tex);
		}
	};
}