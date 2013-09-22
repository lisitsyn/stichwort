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

#ifndef STICHWORT_EXCEPTIONS_H_
#define STICHWORT_EXCEPTIONS_H_

#include <stdexcept>
#include <string>
#include <vector>

#include <stichwort/base.hpp>

namespace stichwort
{

class wrong_parameter_error : public stichwort_keyword_error
{
	public:
		explicit wrong_parameter_error(const KeywordBase& kw, const std::string& what_msg) : 
			stichwort_keyword_error(kw, what_msg) {};
};

class wrong_parameter_type_error : public stichwort_keyword_error
{
	public:
		explicit wrong_parameter_type_error(const KeywordBase& kw, const std::string& what_msg) : 
			stichwort_keyword_error(kw, what_msg) {};
};

class multiple_parameter_error : public std::logic_error
{
	public:

		typedef std::vector<KeywordBase> Keywords;

		explicit multiple_parameter_error(const Keywords& kws, const std::string& what_msg) : 
			std::logic_error(what_msg), keywords(kws) {};
		virtual ~multiple_parameter_error() throw() {}

		Keywords keywords;
};

}
#endif

