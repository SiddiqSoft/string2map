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

#include <cwchar>
#include <iostream>
#include <stdexcept>
#include <string>
#include <map>
#include <unordered_map>
#include <exception>


namespace siddiqsoft::string2map
{
    namespace internal_helpers
    {
        static auto n2w(const std::string& srcStr) -> std::wstring
        {
            std::mbstate_t       state = std::mbstate_t();
            const char*          mbstr = srcStr.c_str();
            std::size_t          len   = 1 + std::mbsrtowcs(nullptr, &mbstr, 0, &state);
            std::vector<wchar_t> wstr(len);

            if (auto resultLen = std::mbsrtowcs(&wstr[0], &mbstr, wstr.size(), &state); resultLen != -1)
                return {wstr.data(), resultLen};
            // Failure
            return {};
        }

        static auto w2n(const std::wstring& srcStr) -> std::string
        {
            std::mbstate_t    state = std::mbstate_t();
            const wchar_t*    wstr  = srcStr.c_str();
            std::size_t       len   = 1 + std::wcsrtombs(nullptr, &wstr, 0, &state);
            std::vector<char> mbstr(len);

            if (auto resultLen = std::wcsrtombs(&mbstr[0], &wstr, mbstr.size(), &state); resultLen != -1)
                return {mbstr.data(), resultLen};
            // Failure
            return {};
        }
    } // namespace internal_helpers

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
                    T    key      = src.substr(keyStart, keyEnd - keyStart);
                    auto valueEnd = src.find(valueDelimiter, keyEnd);

                    if (!key.empty())
                    {
                        // Found value (make sure we skip the key delimiter length)
                        T value = src.substr(keyEnd + keyDelimiter.length(),
                                             valueEnd != std::string::npos ? valueEnd - (keyEnd + keyDelimiter.length())
                                                                           : std::string::npos);

                        // Check if we need transformation
                        if constexpr (std::is_same_v<T, std::string> && std::is_same_v<D, std::wstring>)
                        {
                            // Insert element.. from string to wstring
                            resultMap[internal_helpers::n2w(key)] = internal_helpers::n2w(value);
                        }
                        else if constexpr (std::is_same_v<T, std::wstring> && std::is_same_v<D, std::string>)
                        {
                            // Insert element.. from wstring to string
                            resultMap.insert(std::pair {internal_helpers::w2n(key), internal_helpers::w2n(value)});
                        }
                        else if constexpr (std::is_same_v<T, D>)
                        {
                            // Transformation not needed; insert element as-is.
                            resultMap.insert(std::pair {key, value});
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
