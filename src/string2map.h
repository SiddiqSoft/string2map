#include <string>
#include <map>

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
	R parse(T& src, const T& keyDelimiter, const T& valueDelimiter) noexcept(false);
} // namespace siddiqsoftware::string2map