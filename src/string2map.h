#include <string>
#include <map>

namespace siddiqsoftware::string2map
{
	template <typename ST> static std::map<ST, ST> parse(ST& src, const ST& keyDelimiter, const ST& valueDelimiter) noexcept(false);
}