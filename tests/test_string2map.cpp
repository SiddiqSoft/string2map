#include <string>
#include <map>
#include <unordered_map>

#include "gtest/gtest.h"

#include "../include/siddiqsoft/string2map.hpp"


namespace siddiqsoft::string2map
{
    TEST(string2map, string_string_map)
    {
        using namespace std;

        std::string sampleStr = "Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

        // Note that we will stop at the \r\n\r\n which means the last section my: body is *NOT* going to be part of the decode!
        // Furthermore, the use of the map means we will not count duplicates.
        auto kvmap = siddiqsoft::string2map::parse<string, string, map<string, string>>(sampleStr, ": "s, "\r\n"s, "\r\n\r\n"s);
        EXPECT_EQ(3, kvmap.size());
    }

    TEST(string2map, string_string_unorderedmap)
    {
        using namespace std;

        std::string sampleStr = "Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

        // Note that we will stop at the \r\n\r\n which means the last section my: body is *NOT* going to be part of the decode!
        // Furthermore, the use of the map means we will not count duplicates.
        auto kvmap = siddiqsoft::string2map::parse<string, string, unordered_map<string, string>>(
                sampleStr, ": "s, "\r\n"s, "\r\n\r\n"s);
        EXPECT_EQ(3, kvmap.size());
    }

    TEST(string2map, string_string_map_2)
    {
        using namespace std;

        std::string sampleStr = "tag=networking&order=newest&final=section"s;

        auto kvmap = siddiqsoft::string2map::parse<string, string, map<string, string>>(sampleStr, "="s, "&"s);
        EXPECT_EQ(3, kvmap.size());
    }


    TEST(string2map, wstring_wstring_map)
    {
        using namespace std;

        std::wstring sampleStr = L"Host: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

        // Note that we will stop at the \r\n\r\n which means the last section my: body is *NOT* going to be part of the decode!
        auto kvmap = siddiqsoft::string2map::parse<wstring>(sampleStr, L": "s, L"\r\n"s, L"\r\n\r\n"s);
        EXPECT_EQ(3, kvmap.size());
    }

    TEST(string2map, wstring_wstring_unorderedmap)
    {
        using namespace std;

        std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

        // Note that we will stop at the \r\n\r\n which means the last section my: body is *NOT* going to be part of the decode!
        auto kvmap = siddiqsoft::string2map::parse<std::wstring, std::wstring, std::unordered_map<std::wstring, std::wstring>>(
                sampleStr, L": "s, L"\r\n"s, L"\r\n\r\n"s);
        // In an unordered_map the keys are unique so the two "Host: " will fold into a single (last one wins) element.
        EXPECT_EQ(3, kvmap.size());
    }

    TEST(string2map, wstring_wstring_multimap)
    {
        using namespace std;

        std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

        // Note that we will stop at the \r\n\r\n which means the last section my: body is *NOT* going to be part of the decode!
        auto kvmap = siddiqsoft::string2map::parse<std::wstring, std::wstring, std::multimap<std::wstring, std::wstring>>(
                sampleStr, L": "s, L"\r\n"s, L"\r\n\r\n"s);
        // We should be able to extract the duplicates as-is.
        EXPECT_EQ(4, kvmap.size());
    }


    TEST(string2map, string_wstring_map)
    {
        using namespace std;

        std::string sampleStr {"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"};

        // Note that we will stop at the \r\n\r\n which means the last section my: body is *NOT* going to be part of the decode!
        auto kvmap = siddiqsoft::string2map::parse<string, wstring, map<wstring, wstring>>(sampleStr, ": "s, "\r\n"s, "\r\n\r\n"s);
        EXPECT_EQ(3, kvmap.size());
    }

    TEST(string2map, wstring_string_map)
    {
        using namespace std;

        std::wstring sampleStr = L"Host: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

        // Note that we will stop at the \r\n\r\n which means the last section my: body is *NOT* going to be part of the decode!
        auto kvmap = siddiqsoft::string2map::parse<wstring, string, map<string, string>>(sampleStr, L": "s, L"\r\n"s, L"\r\n\r\n"s);
        EXPECT_EQ(3, kvmap.size());
    }

    TEST(string2map, wstring_string_unorderedmap)
    {
        using namespace std;

        std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

        // Note that we will stop at the \r\n\r\n which means the last section my: body is *NOT* going to be part of the decode!
        auto kvmap = siddiqsoft::string2map::parse<std::wstring, std::string, std::unordered_map<std::string, std::string>>(
                sampleStr, L": "s, L"\r\n"s, L"\r\n\r\n"s);
        // In an unordered_map the keys are unique so the two "Host: " will fold into a single (last one wins) element.
        EXPECT_EQ(3, kvmap.size());
    }

    TEST(string2map, wstring_string_multimap)
    {
        using namespace std;

        std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

        // Note that we will stop at the \r\n\r\n which means the last section my: body is *NOT* going to be part of the decode!
        auto kvmap = siddiqsoft::string2map::parse<std::wstring, std::string, std::multimap<std::string, std::string>>(
                sampleStr, L": "s, L"\r\n"s, L"\r\n\r\n"s);
        // We should be able to extract the duplicates as-is.
        EXPECT_EQ(4, kvmap.size());
    }

    TEST(string2map, wstring_string_multimap_2)
    {
        using namespace std;

        std::wstring sampleStr = L"Host: Duplicate\r\nHost: Hi\r\nAccept: Something\r\nContent-Length: 8\r\n\r\nmy: body"s;

        // Note that in this case, we will have an odd key: `\r\nmy` will be the final key since we did not specify
        // the terminal case of `\r\n\r\n` therefore causing the parser to continue until it reaches end of string.
        auto kvmap = siddiqsoft::string2map::parse<std::wstring, std::string, std::multimap<std::string, std::string>>(
                sampleStr, L": "s, L"\r\n"s);
        // We should be able to extract the duplicates as-is.
        EXPECT_EQ(5, kvmap.size());
    }

} // namespace siddiqsoft::string2map
