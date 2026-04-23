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
        for (const auto& items : kvmap)
        {
            std::cerr << items.first << ": " << items.second << std::endl;
        }
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
        for (const auto& items : kvmap)
        {
            std::wcerr << items.first << L": " << items.second << std::endl;
        }
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


    // ---- Edge case tests ----

    TEST(string2map, edge_empty_source)
    {
        using namespace std;
        // Empty source string should return an empty map.
        std::string src;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, "&"s);
        EXPECT_TRUE(kvmap.empty());
    }

    TEST(string2map, edge_empty_key_delimiter)
    {
        using namespace std;
        // Empty key delimiter should return an empty map (no infinite loop).
        std::string src = "key=value&foo=bar"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, ""s, "&"s);
        EXPECT_TRUE(kvmap.empty());
    }

    TEST(string2map, edge_empty_value_delimiter)
    {
        using namespace std;
        // Empty value delimiter should return an empty map (no infinite loop).
        std::string src = "key=value&foo=bar"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, ""s);
        EXPECT_TRUE(kvmap.empty());
    }

    TEST(string2map, edge_single_key_value)
    {
        using namespace std;
        // A single key-value pair with no trailing value delimiter.
        std::string src = "key=value"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, "&"s);
        EXPECT_EQ(1, kvmap.size());
        EXPECT_EQ("value", kvmap["key"]);
    }

    TEST(string2map, edge_value_with_empty_content)
    {
        using namespace std;
        // Key with an empty value: "key=&foo=bar"
        std::string src = "key=&foo=bar"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, "&"s);
        EXPECT_EQ(2, kvmap.size());
        EXPECT_EQ("", kvmap["key"]);
        EXPECT_EQ("bar", kvmap["foo"]);
    }

    TEST(string2map, edge_trailing_value_delimiter)
    {
        using namespace std;
        // Trailing value delimiter: "key=value&"
        std::string src = "key=value&"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, "&"s);
        EXPECT_EQ(1, kvmap.size());
        EXPECT_EQ("value", kvmap["key"]);
    }

    TEST(string2map, edge_no_key_delimiter_found)
    {
        using namespace std;
        // Source has no key delimiter at all.
        std::string src = "nokeydelimiterhere"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, "&"s);
        EXPECT_TRUE(kvmap.empty());
    }

    TEST(string2map, edge_terminal_delimiter_not_found)
    {
        using namespace std;
        // Terminal delimiter is specified but not present in the source — parse entire string.
        std::string src = "key=value&foo=bar"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, "&"s, "|||"s);
        EXPECT_EQ(2, kvmap.size());
        EXPECT_EQ("value", kvmap["key"]);
        EXPECT_EQ("bar", kvmap["foo"]);
    }

    TEST(string2map, edge_terminal_at_start)
    {
        using namespace std;
        // Terminal delimiter at the very start — nothing should be parsed.
        std::string src = "\r\n\r\nkey=value"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, "&"s, "\r\n\r\n"s);
        EXPECT_TRUE(kvmap.empty());
    }

    TEST(string2map, edge_key_delimiter_beyond_terminal)
    {
        using namespace std;
        // Key delimiter exists only after the terminal delimiter — should not be parsed.
        std::string src = "nodelim\r\n\r\nkey=value"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, "&"s, "\r\n\r\n"s);
        EXPECT_TRUE(kvmap.empty());
    }

    TEST(string2map, edge_value_clamped_at_terminal)
    {
        using namespace std;
        // The last value before the terminal should not bleed past the terminal delimiter.
        // "a: 1\r\nb: 2\r\n\r\nBODY" — with terminal "\r\n\r\n", value of "b" should be "2", not "2\r\n\r\nBODY".
        std::string src = "a: 1\r\nb: 2\r\n\r\nBODY"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, ": "s, "\r\n"s, "\r\n\r\n"s);
        EXPECT_EQ(2, kvmap.size());
        EXPECT_EQ("1", kvmap["a"]);
        EXPECT_EQ("2", kvmap["b"]);
    }

    TEST(string2map, edge_source_is_just_delimiters)
    {
        using namespace std;
        // Source is only delimiters — should produce no entries (empty key).
        std::string src = "=&=&"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, "="s, "&"s);
        EXPECT_TRUE(kvmap.empty());
    }

    TEST(string2map, edge_wstring_empty_source)
    {
        using namespace std;
        // Wide string empty source.
        std::wstring src;
        auto         kvmap = siddiqsoft::string2map::parse<wstring>(src, L"="s, L"&"s);
        EXPECT_TRUE(kvmap.empty());
    }

    TEST(string2map, edge_wstring_single_pair)
    {
        using namespace std;
        // Wide string single key-value pair.
        std::wstring src = L"key=value"s;
        auto         kvmap = siddiqsoft::string2map::parse<wstring>(src, L"="s, L"&"s);
        EXPECT_EQ(1, kvmap.size());
        EXPECT_EQ(L"value", kvmap[L"key"]);
    }

    TEST(string2map, edge_cross_conversion_empty)
    {
        using namespace std;
        // Cross-type conversion with empty source.
        std::string src;
        auto        kvmap = siddiqsoft::string2map::parse<string, wstring, map<wstring, wstring>>(src, "="s, "&"s);
        EXPECT_TRUE(kvmap.empty());
    }

    TEST(string2map, edge_multiple_terminal_delimiters)
    {
        using namespace std;
        // Multiple terminal delimiters in source — should stop at the FIRST one.
        std::string src = "a: 1\r\n\r\nb: 2\r\n\r\nc: 3"s;
        auto        kvmap = siddiqsoft::string2map::parse<string>(src, ": "s, "\r\n"s, "\r\n\r\n"s);
        EXPECT_EQ(1, kvmap.size());
        EXPECT_EQ("1", kvmap["a"]);
    }

} // namespace siddiqsoft::string2map
