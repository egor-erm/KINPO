#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestCompareNodes
{
	TEST_CLASS(TestCompareNodes)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			bool a = true;
			Assert::AreEqual(a, true);
		}
	};
}