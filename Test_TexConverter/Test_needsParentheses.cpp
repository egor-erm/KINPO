#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Node.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestNeedsParentheses
{
	TEST_CLASS(TestNeedsParentheses)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			bool a = true;
			Assert::AreEqual(a, true);
		}
	};
}