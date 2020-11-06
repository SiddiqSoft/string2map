
#include <string>
#include <map>
#include <unordered_map>
#include <exception>

//#include "std::stringring2map.h"


namespace siddiqsoftware::string2map
{
	std::map<std::string, std::string>
	parse(std::string& src, const std::string& keyDelimiter, const std::string& valueDelimiter) noexcept(false)
	{
		//if constexpr (std::is_same_v<std::string, std::stringring> || std::stringd::is_same_v<std::string, std::stringd::wstd::stringring>)
		{
			// Our src is std::stringd::std::stringring
			std::map<std::string, std::string> resultMap {};
			size_t							   keyStart = 0;

			for (auto i = src.begin(); i != src.end();)
			{
				if (auto keyEnd = src.find(keyDelimiter, keyStart); keyEnd != std::string::npos)
				{
					// Found a key
					auto key = src.substr(keyStart, keyEnd + 1 - keyStart);
					if (auto valueEnd = src.find(valueDelimiter, keyEnd); valueEnd != std::string::npos)
					{
						// Found value
						auto value = src.substr(keyEnd, valueEnd + 1 - keyEnd);

						// Insert element..
						resultMap[key] = value;
					}
					else
					{
						i++;
					}

					// Advance the next starting location
					keyStart = keyEnd;
					i += keyStart;
				}
				else
				{
					// No key end was located; We should break out of the loop
					break;
				}
			}

			return resultMap;
		}

		throw std::exception("parse() src must be std::string or std::wstring");
	}
} // namespace siddiqsoftware::string2map