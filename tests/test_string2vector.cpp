
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "gtest/gtest.h"
#include "../src/string2vector.hpp"


namespace siddiqsoft::string2vector
{
	TEST(string2vector, parse1a)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kv = siddiqsoft::string2vector::parse<std::wstring>(sampleStr, L"\r\n"s);
		EXPECT_EQ(5, kv.size());
	}

	TEST(string2vector, parse1b)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kv = siddiqsoft::string2vector::parse<std::wstring>(sampleStr, L": "s);
		EXPECT_EQ(6, kv.size());
	}

	TEST(string2vector, parse2a)
	{
		using namespace std;

		std::string sampleStr = "Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kv = siddiqsoft::string2vector::parse<std::string>(sampleStr, "\r\n"s);
		EXPECT_EQ(5, kv.size());
	}

	TEST(string2vector, parse2b)
	{
		using namespace std;

		std::string sampleStr = "Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kv = siddiqsoft::string2vector::parse<std::string>(sampleStr, ": "s);
		EXPECT_EQ(6, kv.size());
	}


	// "http://<ServerName>/_vti_bin/ExcelRest.aspx/Docs/Documents/sampleWorkbook.xlsx/model/Charts('Chart%201')?Ranges('Sheet1!A1')=5.5"
	TEST(string2vector, parse3)
	{
		using namespace std;

		std::string sampleStr = "/_vti_bin/ExcelRest.aspx/Docs/Documents/sampleWorkbook.xlsx/model/Charts('Chart%201')";

		auto kv = siddiqsoft::string2vector::parse<std::string>(sampleStr, "/"s);
		EXPECT_EQ(7, kv.size());
	}
} // namespace siddiqsoft::string2vector