/** Stichwort
 * A simple library for named parameters in C++
 *
 * Copyright (c) 2013, Sergey Lisitsyn <lisitsyn.s.o@gmail.com>
 * All rights reserved.
 *
 * Distributed under the BSD 2-clause license:
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef STICHWORT_BASE_H_
#define STICHWORT_BASE_H_

#include <stdexcept>

namespace stichwort
{
	struct KeywordBase
	{
		typedef std::string Identifier;
		Identifier identifier__;

		KeywordBase(const Identifier& id) : identifier__(id) { }
		virtual ~KeywordBase() { }

		bool operator==(const KeywordBase& other) const
		{ return (identifier__ == other.identifier__); }
		bool operator!=(const KeywordBase& other) const
		{ return (identifier__ != other.identifier__); }
		bool operator<(const KeywordBase& other) const
		{ return identifier__.compare(other.identifier__) < 0; }
		Identifier identifier() const
		{ return identifier__; }
		operator Identifier () const
		{ return identifier__; }
	};

	struct ParameterBase
	{
		ParameterBase(KeywordBase& kw) : keyword__(kw)
		{
		}
		ParameterBase(const ParameterBase& other) : keyword__(other.keyword__)
		{
		}
		ParameterBase& operator=(const ParameterBase& other)
		{
			keyword__ = other.keyword__;
			return *this;
		}
		virtual ~ParameterBase()
		{
		}

		virtual ParameterBase* clone() const = 0;
		KeywordBase& keyword() const
		{
			return keyword__;
		}
		KeywordBase& keyword__;
	};

	namespace 
	{
		KeywordBase invalid_keyword("Invalid keyword");
	}

	class stichwort_keyword_error : public std::logic_error
	{
	public:
		explicit stichwort_keyword_error(const KeywordBase& kw, const std::string& what_msg) : 
			std::logic_error(what_msg), keyword(kw) {}

		const KeywordBase& keyword;
	};

}

#endif
