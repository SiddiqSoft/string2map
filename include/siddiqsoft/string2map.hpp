/*
	String to Map and String to Vector Parse Helpers

	Version 1.0.0

	https://github.com/siddiqsoftware/string2map/
	
	BSD 3-Clause License
	
	Copyright (c) 2003-2020, Abdelkareem Siddiq, Siddiq Software LLC.
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

#include <stdexcept>
#include <string>
#include <map>
#include <unordered_map>
#include <exception>


namespace siddiqsoft::string2map
{
    /// @brief Helper function that converts from char <-> wchar_t
    /// @tparam T May be char or wchar_t
    /// @param ws May be std::string or std::wstring
    /// @return If source is std::string then the return is std::wstring and vice-versa.
    template <typename CT> auto yinyang(const std::basic_string<CT>& ws)
    {
        // The return will include the NUL-terminator. In order to properly initialize the string, we must then subtract this character.
        size_t convertedCount {};

        if constexpr (std::is_same_v<CT, char>)
        {
            if (ws.empty()) return std::wstring {};

            // FROM char TO wchar_t
            std::vector<wchar_t> ns(ws.length() + 1);
#if defined(WIN32)
            return 0 == mbstowcs_s(&convertedCount, ns.data(), ns.size(), ws.c_str(), ns.size())
                           ? std::wstring {ns.data(), convertedCount > 1 ? convertedCount - 1 : 0}
                           : std::wstring {};
#else
            convertedCount = std::mbstowcs(ns.data(), ws.c_str(), ns.size());
            return convertedCount == static_cast<std::size_t>(-1)
                           ? std::wstring {ns.data(), convertedCount > 1 ? convertedCount - 1 : 0}
                           : std::wstring {};
#endif
        }
        else if constexpr (std::is_same_v<CT, wchar_t>)
        {
            if (ws.empty()) return std::string {};

            // FROM wchar_t TO char
            std::vector<char> ns((ws.length() + 1) * 2); // overallocate for simplicity
#if defined(WIN32)
            return 0 == wcstombs_s(&convertedCount, ns.data(), ns.size(), ws.c_str(), ns.size())
                           ? std::string {ns.data(), convertedCount > 1 ? convertedCount - 1 : 0}
                           : std::string {};
#else
            convertedCount = std::wcstombs(ns.data(), ws.c_str(), ns.size());
            return convertedCount == static_cast<std::size_t>(-1)
                           ? std::string {ns.data(), convertedCount > 1 ? convertedCount - 1 : 0}
                           : std::string {};
#endif
        }
    };


    /// @brief Given a string which contains a key-value pair, extract them into a map of the same type.
    /// @tparam T Must be either std::string or std::wstring or std::u8string
    /// @tparam D Destination type: if T is std::string then this may be std::wstring and vice-versa. Defaults to T.
    /// @tparam R Defaults to std::map but you can use std::multimap if you wish to tackle duplicates in the src string or std::unordered_map
    /// @param src Must be either std::string or std::wstring or std::u8string
    /// @param keyDelimiter Delimiter for the key portion. Example: ": " or ":" or "="
    /// @param valueDelimiter The "line terminator" delimiter which defines the value. Example: "\r\n".
    /// @param terminalDelimiter The "end of frame" delimiter which defines the section. Stop processing if we encounter this value. Defaults to {}
    /// @return map of key-value elements of given type
    template <typename T, typename D = T, typename R = std::map<D, D>>
    static R parse(T& src, const T& keyDelimiter, const T& valueDelimiter, const T& terminalDelimiter = T {}) noexcept(false)
    {
        if constexpr ((std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>) &&
                      (std::is_same_v<D, std::string> || std::is_same_v<D, std::wstring>) &&
                      ((std::is_same_v<R, std::map<D, D>> || std::is_same_v<R, std::multimap<D, D>> ||
                        std::is_same_v<R, std::unordered_map<D, D>>)))
        {
            R resultMap {};

            // Limit to the position of the terminalDelimiter.
            size_t posTerminalDelimiter = !terminalDelimiter.empty() ? src.rfind(terminalDelimiter) : std::string::npos;

            for (size_t keyStart = 0; keyStart < src.length() && keyStart < posTerminalDelimiter;)
            {
                if (auto keyEnd = src.find(keyDelimiter, keyStart); keyEnd != std::string::npos)
                {
                    // Found a key
                    auto key      = src.substr(keyStart, keyEnd - keyStart);
                    auto valueEnd = src.find(valueDelimiter, keyEnd);

                    if (!key.empty())
                    {
                        // Found value (make sure we skip the key delimiter length)
                        auto value = src.substr(keyEnd + keyDelimiter.length(),
                                                valueEnd != std::string::npos ? valueEnd - (keyEnd + keyDelimiter.length())
                                                                              : std::string::npos);

                        // Check if we need transformation
                        if constexpr (std::is_same_v<T, std::string> && std::is_same_v<D, std::wstring>)
                        {
                            // Insert element.. from string to wstring
                            resultMap.insert({yinyang(key), yinyang(value)});
                        }
                        else if constexpr (std::is_same_v<T, std::wstring> && std::is_same_v<D, std::string>)
                        {
                            // Insert element.. from wstring to string
                            resultMap.insert({yinyang(key), yinyang(value)});
                        }
                        else if constexpr (std::is_same_v<T, D>)
                        {
                            // Transformation not needed; insert element as-is.
                            resultMap.insert({key, value});
                        }

                        // Check if we need to advance to next element
                        if (valueEnd != std::string::npos)
                        {
                            // Advance to the next potential element.
                            keyStart = valueEnd + valueDelimiter.length();
                        }
                        else
                        {
                            // Only the key was found and the final section is end of string
                            break;
                        }
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

        throw std::runtime_error("parse() src must be string or wstring");
    }
} // namespace siddiqsoft::string2map
