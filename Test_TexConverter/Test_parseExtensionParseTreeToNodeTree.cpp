#include "pch.h"
#include "CppUnitTest.h"

#include "../TexConverter/Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestParseExtensionParseTreeToNodeTree
{
	TEST_CLASS(TestParseExtensionParseTreeToNodeTree)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			string tree = "";

			set<Error> errors;
			Node* node = parseExtensionParseTreeToNodeTree(tree, errors);

			Assert::AreEqual(a, true);
		}
	};
}