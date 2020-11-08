#include <string>
#include <map>
#include <unordered_map>

#include "../src/string2map.hpp"

#include "gtest/gtest.h"


namespace siddiqsoftware::string2map
{
	TEST(string2map, parseHeadersFromString)
	{
		using namespace std;

		std::string sampleStr = "Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoftware::string2map::parse<string, map<string, string>>(sampleStr, ": "s, "\r\n"s);
		EXPECT_EQ(3, kvmap.size());
	}

	TEST(string2map, parseHeadersFromWString)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoftware::string2map::parse<wstring, map<wstring, wstring>>(sampleStr, L": "s, L"\r\n"s);
		EXPECT_EQ(3, kvmap.size());
	}

	TEST(string2map, parseHeadersFromWStringToUnordered)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoftware::string2map::parse<std::wstring, std::unordered_map<std::wstring, std::wstring>>(
				sampleStr, L": "s, L"\r\n"s);
		EXPECT_EQ(3, kvmap.size());
	}

	TEST(string2map, parseHeadersFromWStringToMulti)
	{
		using namespace std;

		std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

		auto kvmap = siddiqsoftware::string2map::parse<std::wstring, std::multimap<std::wstring, std::wstring>>(
				sampleStr, L": "s, L"\r\n"s);
		// We should be able to extract the duplicates as-is.
		EXPECT_EQ(4, kvmap.size());
	}
} // namespace siddiqsoftware::string2map