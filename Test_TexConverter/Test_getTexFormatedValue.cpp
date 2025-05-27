#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Node.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestGetTexFormatedValue
{
	TEST_CLASS(TestGetTexFormatedValue)
	{
	public:

		TEST_METHOD(IntegerInDecimalSystem)
		{
			Node* node = new Node(NodeType::Integer, "3");
			string expectedValue = "3";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(IntegerInDecimalSystemWithLiteral)
		{
			Node* node = new Node(NodeType::Integer, "5L");
			string expectedValue = "5";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(IntegerInBinarySystem)
		{
			Node* node = new Node(NodeType::Integer, "0b11");
			string expectedValue = "11_2";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(IntegerInOctalSystem)
		{
			Node* node = new Node(NodeType::Integer, "03");
			string expectedValue = "3_8";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(IntegerInHexadecimalSystem)
		{
			Node* node = new Node(NodeType::Integer, "0x3");
			string expectedValue = "3_{16}";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(IntegerInHexadecimalSystemWithLetters)
		{
			Node* node = new Node(NodeType::Integer, "0x5ABC3DEF");
			string expectedValue = "5ABC3DEF_{16}";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(RealNumber)
		{
			Node* node = new Node(NodeType::Float, "3.5");
			string expectedValue = "3.5";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(RealNumberWithLiteral)
		{
			Node* node = new Node(NodeType::Float, "3.5f");
			string expectedValue = "3.5";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(IntegerWithRealLiteral)
		{
			Node* node = new Node(NodeType::Float, "3f");
			string expectedValue = "3.0";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(RealNumberWithoutIntegerPart)
		{
			Node* node = new Node(NodeType::Float, ".5");
			string expectedValue = "0.5";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(ExponentiallyRealNumber)
		{
			Node* node = new Node(NodeType::Float, "5.2e3");
			string expectedValue = "5.2 \\bullet 10^{3}";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(Variable)
		{
			Node* node = new Node(NodeType::Variable, "result");
			string expectedValue = "result";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}

		TEST_METHOD(VariableWithUnderscore)
		{
			Node* node = new Node(NodeType::Variable, "count_");
			string expectedValue = "count\\_";

			Assert::AreEqual(expectedValue, node->getTexFormatedValue());
		}
	};
}