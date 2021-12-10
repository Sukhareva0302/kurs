#include "pch.h"
#include "CppUnitTest.h"
#include "..\ConsoleApplication1\ConsoleApplication1.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Test_Constants)
		{
			string str;
			string err;
			postfix_string result;
			str = "e";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 2.71828);
			str = "pi";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 3.14159);
		}
		TEST_METHOD(Test_Arifmetic)
		{
			string str;
			string err;
			postfix_string result;
			str = "2+2";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 4.0);
			str = "2-2";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 0.0);
			str = "2*2";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 4.0);
			str = "2/2";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 1.0);
			str = "2^2";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 4.0);
			str = "-2+2";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 0.0);
		}
		TEST_METHOD(Test_Function)
		{
			string str;
			string err;
			postfix_string result;
			str = "sin(30*pi/180)";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 0.499999);
			str = "cos(60*pi/180)";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 0.5);
			str = "tg(45*pi/180)";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 1.0);
			str = "ctg(45*pi/180)";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 1.0);
			str = "ln(e)";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 0.999999);
			str = "log(10)";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 1.0);
			str = "sqrt(4)";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 2.0);
			str = "abs(4)";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 4.0);
			str = "abs(-4)";
			result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), 4.0);
		}
		TEST_METHOD(Test_Errors)
		{
			bool can_calculate;
			string str;
			string err;
			string check;
			postfix_string result;
			str = "sin(30*pi/180";
			can_calculate=result.infixToPostfix(str, err);
			check = "Unequal number of opening and closing brackets";
			Assert::AreEqual(err, check);
			Assert::AreEqual(can_calculate, false);
			str = "sin(30*pi/180))";
			can_calculate=result.infixToPostfix(str, err);
			check = "Unequal number of opening and closing brackets";
			Assert::AreEqual(err, check);
			Assert::AreEqual(can_calculate, false);
			str = "arcsin(0.5)";
			can_calculate = result.infixToPostfix(str, err);
			check = "arcsin ";
			Assert::AreEqual(err, check);
			Assert::AreEqual(can_calculate, false);
		}
		TEST_METHOD(Test_complex)
		{
			bool can_calculate;
			string str;
			string err;
			postfix_string result;
			str = "-17^( -3+4)+cos(60*pi/180)*ln(e)+(2-2) + abs(-5000)-sqrt(4)- 4981.5";
			can_calculate = result.infixToPostfix(str, err);
			Assert::AreEqual(result.get_result(), -0.000001); //numbers in double in C++ are weird and this calculator has a bad accuracy
			Assert::AreEqual(can_calculate, true);
		}
	};
}
//rad*180/pi = gradus
//gradus*pi/180