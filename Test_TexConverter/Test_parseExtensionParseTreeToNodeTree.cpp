#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestParseExtensionParseTreeToNodeTree
{
	TEST_CLASS(TestParseExtensionParseTreeToNodeTree)
	{
	public:

		// 1. Недостаточное число операндов
		TEST_METHOD(MissingOperand)
		{
			string tree = "a b ?:";

			set<Error> expectedErrors;
			Error err = Error(ErrorType::MissingOperand);
			err.setErrorIndex(2);
			err.setErrorValue("?:");
			expectedErrors.insert(err);

			set<Error> errors;
			parseExtensionParseTreeToNodeTree(tree, errors);

			set<Error> missingErrors, excessErrors;
			bool isOk = compareErrors(expectedErrors, errors, missingErrors, excessErrors);

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 1 провален: \n");

				if (!missingErrors.empty()) {
					wss << stringToWstring("Отсутствующие ошибки: \n");
					for (Error error : missingErrors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}

				if (!excessErrors.empty()) {
					wss << stringToWstring("Лишние ошибки: \n");
					for (Error error : excessErrors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 2. Неопознанный оператор
		TEST_METHOD(InvalidExpressionParseTreeElement)
		{
			string tree = "a b c +-";

			set<Error> expectedErrors;
			Error err = Error(ErrorType::InvalidExpressionParseTreeElement);
			err.setErrorIndex(3);
			err.setErrorValue("+-");
			expectedErrors.insert(err);

			set<Error> errors;
			parseExtensionParseTreeToNodeTree(tree, errors);

			set<Error> missingErrors, excessErrors;
			bool isOk = compareErrors(expectedErrors, errors, missingErrors, excessErrors);

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 2 провален: \n");

				if (!missingErrors.empty()) {
					wss << stringToWstring("Отсутствующие ошибки: \n");
					for (Error error : missingErrors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}

				if (!excessErrors.empty()) {
					wss << stringToWstring("Лишние ошибки: \n");
					for (Error error : excessErrors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 3. Строка состоит из пробелов
		TEST_METHOD(EmptyString)
		{
			string tree = " ";

			set<Error> expectedErrors;
			Error err = Error(ErrorType::InputFileIsEmpty);
			expectedErrors.insert(err);

			set<Error> errors;
			parseExtensionParseTreeToNodeTree(tree, errors);

			set<Error> missingErrors, excessErrors;
			bool isOk = compareErrors(expectedErrors, errors, missingErrors, excessErrors);

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 3 провален: \n");

				if (!missingErrors.empty()) {
					wss << stringToWstring("Отсутствующие ошибки: \n");
					for (Error error : missingErrors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}

				if (!excessErrors.empty()) {
					wss << stringToWstring("Лишние ошибки: \n");
					for (Error error : excessErrors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 4. Лишний операнд
		TEST_METHOD(ExtraOperand)
		{
			string tree = "a b + c d - e f *";

			set<Error> expectedErrors;

			Error err1 = Error(ErrorType::ExtraOperand);
			err1.setErrorIndex(2);
			err1.setErrorValue("+");
			expectedErrors.insert(err1);

			Error err2 = Error(ErrorType::ExtraOperand);
			err2.setErrorIndex(5);
			err2.setErrorValue("-");
			expectedErrors.insert(err2);

			set<Error> errors;
			parseExtensionParseTreeToNodeTree(tree, errors);

			set<Error> missingErrors, excessErrors;
			bool isOk = compareErrors(expectedErrors, errors, missingErrors, excessErrors);

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 4 провален: \n");

				if (!missingErrors.empty()) {
					wss << stringToWstring("Отсутствующие ошибки: \n");
					for (Error error : missingErrors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}

				if (!excessErrors.empty()) {
					wss << stringToWstring("Лишние ошибки: \n");
					for (Error error : excessErrors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 5. Дерево разбора выражения с арифметическими операторами
		TEST_METHOD(ArithmeticOperands)
		{
			string tree = "result a 1- b 1+ + c - d * f g / % =";

			Node* expectedNode1 = new Node(NodeType::Variable, "a");
			Node* expectedNode2 = new Node(NodeType::Variable, "b");

			Node* expectedNode3 = new Node(NodeType::UnaryMinus, vector<Node*>{ expectedNode1 });
			Node* expectedNode4 = new Node(NodeType::UnaryPlus, vector<Node*>{ expectedNode2 });

			Node* expectedNode5 = new Node(NodeType::Plus, vector<Node*>{ expectedNode3, expectedNode4 });
			Node* expectedNode6 = new Node(NodeType::Variable, "c");

			Node* expectedNode7 = new Node(NodeType::Minus, vector<Node*>{ expectedNode5, expectedNode6 });
			Node* expectedNode8 = new Node(NodeType::Variable, "d");
			Node* expectedNode9 = new Node(NodeType::Variable, "f");
			Node* expectedNode10 = new Node(NodeType::Variable, "g");

			Node* expectedNode11 = new Node(NodeType::Multiply, vector<Node*>{ expectedNode7, expectedNode8 });
			Node* expectedNode12 = new Node(NodeType::Divide, vector<Node*>{ expectedNode9, expectedNode10 });

			Node* expectedNode13 = new Node(NodeType::Variable, "result");
			Node* expectedNode14 = new Node(NodeType::Mod, vector<Node*>{ expectedNode11, expectedNode12 });

			Node* expectedNode = new Node(NodeType::Assign, vector<Node*>{ expectedNode13, expectedNode14 });

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			string path = "/";
			set<string> differences;
			bool isOk = compareTrees(expectedNode, node, path, differences) && errors.empty();

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 5 провален: \n");

				if (!differences.empty()) {
					wss << stringToWstring("Деревья не совпадают: \n");
					for (string diff : differences) {
						wss << stringToWstring(diff) << "\n";
					}
				}

				if (!errors.empty()) {
					wss << stringToWstring("Список содержит ошибки, которых быть не должно: \n");
					for (Error error : errors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 6. Дерево разбора выражения с операторами сравнения и различными операндами
		TEST_METHOD(ComparisonOperators)
		{
			string tree = "count 5L >= 48 dif <= > 3.6 .5f c != < ==";

			Node* expectedNode1 = new Node(NodeType::Variable, "count");
			Node* expectedNode2 = new Node(NodeType::Integer, "5L");

			Node* expectedNode3 = new Node(NodeType::Integer, "48");
			Node* expectedNode4 = new Node(NodeType::Variable, "dif");

			Node* expectedNode5 = new Node(NodeType::Float, ".5f");
			Node* expectedNode6 = new Node(NodeType::Variable, "c");

			Node* expectedNode7 = new Node(NodeType::GreaterOrEqual, vector<Node*>{ expectedNode1, expectedNode2 });
			Node* expectedNode8 = new Node(NodeType::LessOrEqual, vector<Node*>{ expectedNode3, expectedNode4 });
			Node* expectedNode9 = new Node(NodeType::Float, "3.6");
			Node* expectedNode10 = new Node(NodeType::NotEqualTo, vector<Node*>{ expectedNode5, expectedNode6 });

			Node* expectedNode11 = new Node(NodeType::GreaterThan, vector<Node*>{ expectedNode7, expectedNode8 });
			Node* expectedNode12 = new Node(NodeType::LessThan, vector<Node*>{ expectedNode9, expectedNode10 });

			Node* expectedNode = new Node(NodeType::EqualTo, vector<Node*>{ expectedNode11, expectedNode12 });

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			string path = "/";
			set<string> differences;
			bool isOk = compareTrees(expectedNode, node, path, differences) && errors.empty();

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 6 провален: \n");

				if (!differences.empty()) {
					wss << stringToWstring("Деревья не совпадают: \n");
					for (string diff : differences) {
						wss << stringToWstring(diff) << "\n";
					}
				}

				if (!errors.empty()) {
					wss << stringToWstring("Список содержит ошибки, которых быть не должно: \n");
					for (Error error : errors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 7. Дерево разбора выражения с логическими операторами
		TEST_METHOD(LogicalOperands)
		{
			string tree = "ab_ 1 || bo2 ! &&";

			Node* expectedNode1 = new Node(NodeType::Variable, "ab_");
			Node* expectedNode2 = new Node(NodeType::Integer, "1");
			Node* expectedNode3 = new Node(NodeType::Variable, "bo2");

			Node* expectedNode4 = new Node(NodeType::LogicalOr, vector<Node*>{ expectedNode1, expectedNode2 });
			Node* expectedNode5 = new Node(NodeType::LogicalNot, vector<Node*>{ expectedNode3 });

			Node* expectedNode = new Node(NodeType::LogicalAnd, vector<Node*>{ expectedNode4, expectedNode5 });

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			string path = "/";
			set<string> differences;
			bool isOk = compareTrees(expectedNode, node, path, differences) && errors.empty();

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 7 провален: \n");

				if (!differences.empty()) {
					wss << stringToWstring("Деревья не совпадают: \n");
					for (string diff : differences) {
						wss << stringToWstring(diff) << "\n";
					}
				}

				if (!errors.empty()) {
					wss << stringToWstring("Список содержит ошибки, которых быть не должно: \n");
					for (Error error : errors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 8. Дерево разбора выражения с составными операторами присваивания
		TEST_METHOD(CompoundOperators)
		{
			string tree = "a b c /= -= d f %= g *= +=";

			Node* expectedNode1 = new Node(NodeType::Variable, "b");
			Node* expectedNode2 = new Node(NodeType::Variable, "c");
			Node* expectedNode3 = new Node(NodeType::Variable, "d");
			Node* expectedNode4 = new Node(NodeType::Variable, "f");

			Node* expectedNode5 = new Node(NodeType::Variable, "a");
			Node* expectedNode6 = new Node(NodeType::DivideAssign, vector<Node*>{ expectedNode1, expectedNode2 });
			Node* expectedNode7 = new Node(NodeType::ModAssign, vector<Node*>{ expectedNode3, expectedNode4 });
			Node* expectedNode8 = new Node(NodeType::Variable, "g");

			Node* expectedNode9 = new Node(NodeType::SubtractAssign, vector<Node*>{ expectedNode5, expectedNode6 });
			Node* expectedNode10 = new Node(NodeType::MultiplyAssign, vector<Node*>{ expectedNode7, expectedNode8 });

			Node* expectedNode = new Node(NodeType::AddAssign, vector<Node*>{ expectedNode9, expectedNode10 });

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			string path = "/";
			set<string> differences;
			bool isOk = compareTrees(expectedNode, node, path, differences) && errors.empty();

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 8 провален: \n");

				if (!differences.empty()) {
					wss << stringToWstring("Деревья не совпадают: \n");
					for (string diff : differences) {
						wss << stringToWstring(diff) << "\n";
					}
				}

				if (!errors.empty()) {
					wss << stringToWstring("Список содержит ошибки, которых быть не должно: \n");
					for (Error error : errors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 9. Дерево разбора выражения с тригонометрическими функциями
		TEST_METHOD(TrigonometricFunctions)
		{
			string tree = "x atan tan acos cos asin sin";

			Node* expectedNode1 = new Node(NodeType::Variable, "x");
			Node* expectedNode2 = new Node(NodeType::ArcTan, vector<Node*>{ expectedNode1 });
			Node* expectedNode3 = new Node(NodeType::Tan, vector<Node*>{ expectedNode2 });
			Node* expectedNode4 = new Node(NodeType::ArcCos, vector<Node*>{ expectedNode3 });
			Node* expectedNode5 = new Node(NodeType::Cos, vector<Node*>{ expectedNode4 });
			Node* expectedNode6 = new Node(NodeType::ArcSin, vector<Node*>{ expectedNode5 });

			Node* expectedNode = new Node(NodeType::Sin, vector<Node*>{ expectedNode6 });

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			string path = "/";
			set<string> differences;
			bool isOk = compareTrees(expectedNode, node, path, differences) && errors.empty();

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 9 провален: \n");

				if (!differences.empty()) {
					wss << stringToWstring("Деревья не совпадают: \n");
					for (string diff : differences) {
						wss << stringToWstring(diff) << "\n";
					}
				}

				if (!errors.empty()) {
					wss << stringToWstring("Список содержит ошибки, которых быть не должно: \n");
					for (Error error : errors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 10. Дерево разбора выражения с математическими функциями
		TEST_METHOD(MathematicalFunctions)
		{
			string tree = "5.3f round floor ceil abs b log10 log2 log exp pow";

			Node* expectedNode1 = new Node(NodeType::Float, "5.3f");
			Node* expectedNode2 = new Node(NodeType::Variable, "b");

			Node* expectedNode3 = new Node(NodeType::Round, vector<Node*>{ expectedNode1 });
			Node* expectedNode4 = new Node(NodeType::Log10, vector<Node*>{ expectedNode2 });

			Node* expectedNode5 = new Node(NodeType::Floor, vector<Node*>{ expectedNode3 });
			Node* expectedNode6 = new Node(NodeType::Log2, vector<Node*>{ expectedNode4 });

			Node* expectedNode7 = new Node(NodeType::Ceil, vector<Node*>{ expectedNode5 });
			Node* expectedNode8 = new Node(NodeType::Log, vector<Node*>{ expectedNode6 });

			Node* expectedNode9 = new Node(NodeType::Abs, vector<Node*>{ expectedNode7 });
			Node* expectedNode10 = new Node(NodeType::Exp, vector<Node*>{ expectedNode8 });

			Node* expectedNode = new Node(NodeType::Pow, vector<Node*>{ expectedNode9, expectedNode10 });

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			string path = "/";
			set<string> differences;
			bool isOk = compareTrees(expectedNode, node, path, differences) && errors.empty();

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 10 провален: \n");

				if (!differences.empty()) {
					wss << stringToWstring("Деревья не совпадают: \n");
					for (string diff : differences) {
						wss << stringToWstring(diff) << "\n";
					}
				}

				if (!errors.empty()) {
					wss << stringToWstring("Список содержит ошибки, которых быть не должно: \n");
					for (Error error : errors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 11. Дерево разбора выражения с особыми операторами (тернарный, обращение к элементу массива, приведение типа)
		TEST_METHOD(SpecialOperators)
		{
			string tree = "a b i [] c (type) ?:";

			Node* expectedNode1 = new Node(NodeType::Variable, "b");
			Node* expectedNode2 = new Node(NodeType::Variable, "i");
			Node* expectedNode3 = new Node(NodeType::Variable, "c");

			Node* expectedNode4 = new Node(NodeType::Variable, "a");
			Node* expectedNode5 = new Node(NodeType::Indexing, vector<Node*>{ expectedNode1, expectedNode2 });
			Node* expectedNode6 = new Node(NodeType::TypeCast, vector<Node*>{ expectedNode3 });

			Node* expectedNode = new Node(NodeType::Ternary, vector<Node*>{ expectedNode4, expectedNode5, expectedNode6 });

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			string path = "/";
			set<string> differences;
			bool isOk = compareTrees(expectedNode, node, path, differences) && errors.empty();

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 11 провален: \n");

				if (!differences.empty()) {
					wss << stringToWstring("Деревья не совпадают: \n");
					for (string diff : differences) {
						wss << stringToWstring(diff) << "\n";
					}
				}

				if (!errors.empty()) {
					wss << stringToWstring("Список содержит ошибки, которых быть не должно: \n");
					for (Error error : errors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 12. Различный формат записи целочисленных операндов
		TEST_METHOD(IntegerValues)
		{
			string tree = "3 3L + 3U 0b11 + 03 0x3 + + +";

			Node* expectedNode1 = new Node(NodeType::Integer, "3U");
			Node* expectedNode2 = new Node(NodeType::Integer, "0b11");
			Node* expectedNode3 = new Node(NodeType::Integer, "03");
			Node* expectedNode4 = new Node(NodeType::Integer, "0x3");

			Node* expectedNode5 = new Node(NodeType::Integer, "3");
			Node* expectedNode6 = new Node(NodeType::Integer, "3L");
			Node* expectedNode7 = new Node(NodeType::Plus, vector<Node*>{ expectedNode1, expectedNode2 });
			Node* expectedNode8 = new Node(NodeType::Plus, vector<Node*>{ expectedNode3, expectedNode4 });

			Node* expectedNode9 = new Node(NodeType::Plus, vector<Node*>{ expectedNode5, expectedNode6 });
			Node* expectedNode10 = new Node(NodeType::Plus, vector<Node*>{ expectedNode7, expectedNode8 });

			Node* expectedNode = new Node(NodeType::Plus, vector<Node*>{ expectedNode9, expectedNode10 });

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			string path = "/";
			set<string> differences;
			bool isOk = compareTrees(expectedNode, node, path, differences) && errors.empty();

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 12 провален: \n");

				if (!differences.empty()) {
					wss << stringToWstring("Деревья не совпадают: \n");
					for (string diff : differences) {
						wss << stringToWstring(diff) << "\n";
					}
				}

				if (!errors.empty()) {
					wss << stringToWstring("Список содержит ошибки, которых быть не должно: \n");
					for (Error error : errors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}

		// 13. Различный формат записи вещественных операндов
		TEST_METHOD(FloatValues)
		{
			string tree = "5.3 .3 + 5.2e3 3.2f + +";

			Node* expectedNode1 = new Node(NodeType::Float, "5.3");
			Node* expectedNode2 = new Node(NodeType::Float, ".3");
			Node* expectedNode3 = new Node(NodeType::Float, "5.2e3");
			Node* expectedNode4 = new Node(NodeType::Float, "3.2f");

			Node* expectedNode5 = new Node(NodeType::Plus, vector<Node*>{ expectedNode1, expectedNode2 });
			Node* expectedNode6 = new Node(NodeType::Plus, vector<Node*>{ expectedNode3, expectedNode4 });

			Node* expectedNode = new Node(NodeType::Plus, vector<Node*>{ expectedNode5, expectedNode6 });

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			string path = "/";
			set<string> differences;
			bool isOk = compareTrees(expectedNode, node, path, differences) && errors.empty();

			std::wstringstream wss;
			if (!isOk) {
				wss << stringToWstring("Тест 13 провален: \n");

				if (!differences.empty()) {
					wss << stringToWstring("Деревья не совпадают: \n");
					for (string diff : differences) {
						wss << stringToWstring(diff) << "\n";
					}
				}

				if (!errors.empty()) {
					wss << stringToWstring("Список содержит ошибки, которых быть не должно: \n");
					for (Error error : errors) {
						wss << stringToWstring(error.generateErrorMessage()) << "\n";
					}
				}
			}

			Assert::IsTrue(isOk, wss.str().c_str());
		}
	};
}