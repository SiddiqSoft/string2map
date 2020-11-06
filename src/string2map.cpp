
#include <string>
#include <map>
#include <unordered_map>
#include <exception>

#include "string2map.h"


namespace siddiqsoftware::string2map
{
	/// @brief Given a string which contains a key-value pair, extract them into a map of the same type.
	/// @tparam T Must be either std::string or std::wstring or std::u8string
	/// @tparam R Defaults to std::map but you can use std::multimap if you wish to tackle duplicates in the src string or std::unordered_map
	/// @param src Must be either std::string or std::wstring or std::u8string
	/// @param keyDelimiter Delimiter for the key portion. Example: ": " or ":" or "="
	/// @param valueDelimiter The "line terminator" delimiter which defines the value. Example: "\r\n".
	/// @return map of key-value elements of given type
	template <typename T, typename R = std::map<T, T>>
	R parse(T& src, const T& keyDelimiter, const T& valueDelimiter) noexcept(false)
	{
		if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>)
		{
			R resultMap {};

			for (size_t keyStart = 0; keyStart < src.length();)
			{
				if (auto keyEnd = src.find(keyDelimiter, keyStart); keyEnd != std::string::npos)
				{
					// Found a key
					auto key = src.substr(keyStart, keyEnd - keyStart);
					if (auto valueEnd = src.find(valueDelimiter, keyEnd); valueEnd != std::string::npos)
					{
						// Found value (make sure we skip the key delimiter length)
						auto value = src.substr(keyEnd + keyDelimiter.length(), valueEnd - (keyEnd + keyDelimiter.length()));

						// Insert element..
						resultMap.insert({key, value});
						// Advance to the next potential element.
						keyStart = valueEnd + valueDelimiter.length();
					}
					else
					{
						// No value end was located! We must break out of the loop
						break;
					}
				}
				else
				{
					// No key end was located; We must break out of the loop
					break;
				}
			}

			return resultMap;
		}

		throw std::exception("parse() src must be std::string or std::wstring");
	}
} // namespace siddiqsoftware::string2map