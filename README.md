# string2map and string2vector

[![CodeQL](https://github.com/SiddiqSoft/string2map/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/SiddiqSoft/string2map/actions/workflows/codeql-analysis.yml)
[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.string2map?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=3&branchName=main)
![](https://img.shields.io/nuget/v/SiddiqSoft.string2map)
![](https://img.shields.io/github/v/tag/SiddiqSoft/string2map)
![](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/2)
![](https://img.shields.io/azure-devops/coverage/siddiqsoft/siddiqsoft/2)

Simple C++17 library to aid in the parsing of HTTP headers into a STL map-type container.


## Objective

Convert the input string with the specified delimiters into a map of key-value pairs with the given input:
- std::[w]string
- Key delimiter (example: `: `, `:`, `=`, `= `)
- Value delimiter (end of the key-value pair element, example: CRLF)
- Destination output type for key/value: `string` or `wstring`.
- Destination container type: `map`, `multimap`, `unordered_map`.


## API

```cpp
namespace siddiqsoft::string2map
{
    template <typename T, typename D = T, typename R = std::map<D, D>>
    R parse(T& src, const T& keyDelimiter, const T& valueDelimiter) noexcept(false)
}
```

typename | Type      | Comment
---------|-----------|--------------
`T`      | `string` or `wstring`  | Type of the source string
`D`      | `string` or `wstring`  | Type of the destination string (used in the container)
`R`      | `map`, `unordered_map`, `multimap` | Generally type is container<D,D>


```cpp
namespace siddiqsoft::string2vector
{
    template <typename T>
    std::vector<T> parse(const T& src, const T& keyDelimiter)
}
```

typename | Type      | Comment
---------|-----------|--------------
`T`      | `string` or `wstring`  | Type of the source string


## Usage

Get it from [nuget](https://www.nuget.org/packages/string2map/) or you can submodule it.

```cpp
#include <string>
#include <map>

#include "siddiqsoft/string2map.hpp"

TEST(parse, Test_string2map)
{
    std::string sampleStr{ "Host: duplicate\r\n"
                    "Host: hostname.com\r\n"
                    "Content-Type: text\r\n"
                    "Content-Length: 99\r\n\r\n" };

    auto kvmap= siddiqsoft::string2map::parse<string,  // input string
                                              wstring, // transform to wstring
                                              map<wstring,wstring> // destination container
                                             >(sampleStr);

    // We expect only 3 items even though there is a duplicate key in the source string.
    // The std::map container ensures unique keys.
    EXPECT_EQ(3, kvmap.size());
}


TEST(parse, Test_string2vector)
{
    std::string sampleStr{ "/a/b/c/d" };

    auto kv= siddiqsoft::string2vector::parse<string>(sampleStr);

    EXPECT_EQ(4, kv.size());
}

```

<small align="right">

&copy; 2020 Siddiq Software LLC. All rights reserved. Refer to [LICENSE](LICENSE).

</small>
