# string2map
Parse std::string to std::map

## Objective

Input
- std::[w]string
- Key delimiter (default `=`)
- Value delimiter (default `CRLF`)

Output
- std::map<[w]string,[w]string> or std::unordered_map<[w]string,[w]string>

Converts the input string with the specified delimiters into a map of key-value pairs.

## Usage

```cpp
namespace string2map
{
    template<typename St, typename Tout>
    Tout parse(St& src);
}
```