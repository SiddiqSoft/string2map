/*
	String to Map Parse Helpers

	Version 1.0.0
	https://github.com/siddiqsoftware/string2map/
	BSD 3-Clause License
	Copyright (c) 2003-2020, Abdelkareem Siddiq
	All rights reserved.
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	1. Redistributions of source code must retain the above copyright notice, this
	list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived from
	this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <string>
#include <map>
#include <unordered_map>
#include <exception>


namespace siddiqsoftware::string2map
{
	/// @brief Given a string which contains a key-value pair, extract them into a map of the same type.
	/// @tparam T Must be either std::string or std::wstring or std::u8string
	/// @tparam R Defaults to std::map but you can use std::multimap if you wish to tackle duplicates in the src string or std::unordered_map
	/// @param src Must be either std::string or std::wstring or std::u8string
	/// @param keyDelimiter Delimiter for the key portion. Example: ": " or ":" or "="
	/// @param valueDelimiter The "line terminator" delimiter which defines the value. Example: "\r\n".
	/// @return map of key-value elements of given type
	template <typename T, typename R> R parse(T& src, const T& keyDelimiter, const T& valueDelimiter) noexcept(false)
	{
		if constexpr ((std::is_same_v<T, std::string> ||
					   std::is_same_v<T, std::wstring>)&&((std::is_same_v<R, std::map<std::string, std::string>> ||
														   std::is_same_v<R, std::map<std::wstring, std::wstring>>) ||
														  (std::is_same_v<R, std::multimap<std::string, std::string>> ||
														   std::is_same_v<R, std::multimap<std::wstring, std::wstring>>) ||
														  (std::is_same_v<R, std::unordered_map<std::string, std::string>> ||
														   std::is_same_v<R, std::unordered_map<std::wstring, std::wstring>>)))
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