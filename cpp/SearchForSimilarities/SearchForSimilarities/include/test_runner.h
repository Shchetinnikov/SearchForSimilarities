//#pragma once
//
//#include <exception>
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <tuple>
//
//#include "Exception.h"
//#include "ConsoleColors.h"
//
//using namespace std;
//
//
//class TestRunner {
//public:
//	template <class TestFunc >
//	void RunTest(TestFunc func, const string& test_name)
//	{
//		stringstream os;
//		os << endl << test_name << " is running ...";
//		PrintSubheading(os.str());
//		os.str("");
//		try {
//			func();
//			os << "Conclusion: " << test_name << " OK";
//			PrintConclusion(os.str());
//		}
//		catch (runtime_error& e) {
//			++fail_count;
//			SetConsoleTextAttribute(HCONSOLE, (WORD)((Red << 4) | White));
//
//			cerr << test_name << " fail: " << e.what() << endl;
//
//			SetConsoleTextAttribute(HCONSOLE, (WORD)((Black << 4) | White));
//		}
//	}
//	~TestRunner()
//	{
//		if (fail_count > 0) {
//			SetConsoleTextAttribute(HCONSOLE, (WORD)((Red << 4) | White));
//
//			cerr << endl << fail_count << " unit tests failed. Program is completed";
//
//			SetConsoleTextAttribute(HCONSOLE, (WORD)((Black << 4) | White));
//			exit(1);
//		}
//	}
//private:
//	int fail_count = 0;
//};