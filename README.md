# string2map

[![CodeQL](https://github.com/SiddiqSoft/string2map/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/SiddiqSoft/string2map/actions/workflows/codeql-analysis.yml)

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
namespace string2map
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

## Usage

Get it from [nuget](https://www.nuget.org/packages/string2map/)

```cpp
#include <string>
#include <map>

#include "siddiqsoft/string2map.hpp"

void Test()
{
    auto sampleStr{ "Host: duplicate\r\n"
                    "Host: hostname.com\r\n"
                    "Content-Type: text\r\n"
                    "Content-Length: 99\r\n\r\n"s };

    auto kvmap= siddiqsoft::string2map::parse<string,  // input string
                                              wstring, // transform to wstring
                                              map<wstring,wstring> // destination container
                                             >(sampleStr);

    // We expect only 3 items even though there is a duplicate key in the source string.
    // The std::map container ensures unique keys.
    EXPECT_EQ(3, kvmap.size());
}
```

<small align="right">

&copy; 2020 Siddiq Software LLC. All rights reserved. Refer to [LICENSE](LICENSE).

</small>
