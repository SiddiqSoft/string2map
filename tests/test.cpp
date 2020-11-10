#include <string>
#include <map>
#include <unordered_map>

#include "../src/string2map.hpp"

#include "gtest/gtest.h"


namespace siddiqsoft::string2map
{
	TEST(string2map, string_string_map)
	{
		using namespace std;

		std::string sampleStr = "Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoft::string2map::parse<string, string, map<string, string>>(sampleStr, ": "s, "\r\n"s);
		EXPECT_EQ(3, kvmap.size());
	}

	TEST(string2map, wstring_wstring_map)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoft::string2map::parse<wstring, wstring, map<wstring, wstring>>(sampleStr, L": "s, L"\r\n"s);
		EXPECT_EQ(3, kvmap.size());
	}

	TEST(string2map, wstring_wstring_unorderedmap)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoft::string2map::parse<std::wstring, std::wstring, std::unordered_map<std::wstring, std::wstring>>(
				sampleStr, L": "s, L"\r\n"s);
		EXPECT_EQ(3, kvmap.size());
	}

	TEST(string2map, wstring_wstring_multimap)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoft::string2map::parse<std::wstring, std::wstring, std::multimap<std::wstring, std::wstring>>(
				sampleStr, L": "s, L"\r\n"s);
		// We should be able to extract the duplicates as-is.
		EXPECT_EQ(4, kvmap.size());
	}


	TEST(string2map, string_wstring_map)
	{
		using namespace std;

		std::string sampleStr = "Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoft::string2map::parse<string, wstring, map<wstring, wstring>>(sampleStr, ": "s, "\r\n"s);
		EXPECT_EQ(3, kvmap.size());
	}

	TEST(string2map, wstring_string_map)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoft::string2map::parse<wstring, string, map<string, string>>(sampleStr, L": "s, L"\r\n"s);
		EXPECT_EQ(3, kvmap.size());
	}

	TEST(string2map, wstring_string_unorderedmap)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoft::string2map::parse<std::wstring, std::string, std::unordered_map<std::string, std::string>>(
				sampleStr, L": "s, L"\r\n"s);
		EXPECT_EQ(3, kvmap.size());
	}

	TEST(string2map, wstring_string_multimap)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoft::string2map::parse<std::wstring, std::string, std::multimap<std::string, std::string>>(
				sampleStr, L": "s, L"\r\n"s);
		// We should be able to extract the duplicates as-is.
		EXPECT_EQ(4, kvmap.size());
	}

} // namespace siddiqsoft::string2map