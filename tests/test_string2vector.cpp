#include "gtest/gtest.h"
#include "../include/siddiqsoft/string2vector.hpp"


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


    // ---- Edge case tests ----

    TEST(string2vector, edge_empty_string)
    {
        using namespace std;
        // Empty source string should return an empty vector.
        std::string src;
        auto        kv = siddiqsoft::string2vector::parse<std::string>(src, ","s);
        EXPECT_TRUE(kv.empty());
    }

    TEST(string2vector, edge_empty_wstring)
    {
        using namespace std;
        // Empty wide source string should return an empty vector.
        std::wstring src;
        auto         kv = siddiqsoft::string2vector::parse<std::wstring>(src, L","s);
        EXPECT_TRUE(kv.empty());
    }

    TEST(string2vector, edge_no_delimiters_found)
    {
        using namespace std;
        // Source has no delimiters — should return the entire string as a single token.
        std::string src = "nodelimiters"s;
        auto        kv = siddiqsoft::string2vector::parse<std::string>(src, ","s);
        EXPECT_EQ(1, kv.size());
        EXPECT_EQ("nodelimiters", kv[0]);
    }

    TEST(string2vector, edge_only_delimiters)
    {
        using namespace std;
        // Source is only delimiters — should return an empty vector (all chars are delimiters).
        std::string src = ",,,"s;
        auto        kv = siddiqsoft::string2vector::parse<std::string>(src, ","s);
        EXPECT_TRUE(kv.empty());
    }

    TEST(string2vector, edge_single_char_tokens)
    {
        using namespace std;
        // Single character tokens separated by delimiters.
        std::string src = "a,b,c"s;
        auto        kv = siddiqsoft::string2vector::parse<std::string>(src, ","s);
        EXPECT_EQ(3, kv.size());
        EXPECT_EQ("a", kv[0]);
        EXPECT_EQ("b", kv[1]);
        EXPECT_EQ("c", kv[2]);
    }

    TEST(string2vector, edge_leading_trailing_delimiters)
    {
        using namespace std;
        // Leading and trailing delimiters should be skipped.
        std::string src = ",hello,world,"s;
        auto        kv = siddiqsoft::string2vector::parse<std::string>(src, ","s);
        EXPECT_EQ(2, kv.size());
        EXPECT_EQ("hello", kv[0]);
        EXPECT_EQ("world", kv[1]);
    }

    TEST(string2vector, edge_consecutive_delimiters)
    {
        using namespace std;
        // Consecutive delimiters should be treated as a single separator (find_first_not_of behavior).
        std::string src = "hello,,,world"s;
        auto        kv = siddiqsoft::string2vector::parse<std::string>(src, ","s);
        EXPECT_EQ(2, kv.size());
        EXPECT_EQ("hello", kv[0]);
        EXPECT_EQ("world", kv[1]);
    }

} // namespace siddiqsoft::string2vector