/*
	String to Vector Parse Helpers

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

#include <string>
#include <vector>

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

namespace siddiqsoft::string2vector
{
	/// @brief Splits a given string yielding a vector of substrings
	/// @tparam T std::string or std::wstring
	/// @param str The source string
	/// @param delimiters The delimiters
	/// @return A vector of type T
	template <class T> static std::vector<T> parse(const T& str, const T& delimiters)
	{
		std::vector<T> tokens;

		// Skip delimiters at beginning.
		auto lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		auto pos = str.find_first_of(delimiters, lastPos);

		while ((T::npos != pos) || (T::npos != lastPos))
		{
			// Found a token, add it to the std::vector.
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}

		return tokens;
	}
} // namespace siddiqsoft::string2vector
