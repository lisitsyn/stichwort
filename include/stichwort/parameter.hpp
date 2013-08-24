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

#ifndef STICHWORT_PARAMETER_H_
#define STICHWORT_PARAMETER_H_

#include <stichwort/exceptions.hpp>
#include <stichwort/value_keeper.hpp>

#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <list>

#ifdef USE_CXX11
#include <initializer_list>
#endif

namespace stichwort
{

class Parameters;

class Parameter
{
private:

	template <typename T>
	Parameter(const KeywordBase& kw, const T& value) : 
		valid(true), invalidity_reasons(),
		keyword_(kw), keeper(value)
	{
	}

public:

	template <typename T>
	static Parameter create(const KeywordBase& kw, const T& value) 
	{
		return Parameter(kw, value);
	}

	Parameter() : 
		valid(false), invalidity_reasons(),
		keyword_(invalid_keyword), keeper()
	{
	}

	Parameter(const Parameter& p) : 
		valid(p.valid), invalidity_reasons(p.invalidity_reasons),
		keyword_(p.keyword_), keeper(p.keeper)
	{
	}

	Parameter& operator=(const Parameter& p)
	{
		valid = p.valid;
		invalidity_reasons = p.invalidity_reasons;
		keyword_ = p.keyword_;
		keeper = p.keeper;
		return *this;
	}

	~Parameter()
	{
	}

	template <typename T>
	inline Parameter withDefault(T value)
	{
		if (!isInitialized())
		{
			keeper = stichwort_internal::ValueKeeper(value);
		}
		return *this;
	}

	template <typename T>
	inline operator T()
	{
		return getValue<T>();
	}

	operator Parameters();

	template <typename T>
	bool is(T v)
	{
		if (!isTypeCorrect<T>())
			return false;
		T kv = keeper.getValue<T>();
		if (v == kv)
			return true;
		return false;
	}

	template <typename T>
	bool operator==(T v) const
	{
		return is<T>(v);
	}

	bool isInitialized() const
	{
		return keeper.isInitialized();
	}
	
	template <template<class> class F, class Q>
	inline bool satisfies(F<Q> cond) const
	{
		return keeper.satisfies(cond);
	}

	KeywordBase keyword() const 
	{
		return keyword_;
	}

	std::string repr() const
	{
		return keeper.repr();
	}

	Parameters operator,(const Parameter& p);

private:

	template <typename T>
	inline T getValue() const
	{
		if (!valid)
			throw wrong_parameter_error(keyword_, invalidity_reasons);
		if (!isTypeCorrect<T>())
			throw wrong_parameter_type_error(keyword_, "wrong type");

		optional<T> opt = keeper.getValue<T>();
		if (!opt)
			throw missed_parameter_error(keyword_, "missed");

		return *opt;
	}
	
	template <typename T>
	inline bool isTypeCorrect() const
	{
		return keeper.isTypeCorrect<T>();
	}

	inline void invalidate(const std::string& reason)
	{
		if (valid) 
			valid = false;

		invalidity_reasons += reason;
	}

private:

	bool valid;
	std::string invalidity_reasons;
	KeywordBase keyword_;
	stichwort_internal::ValueKeeper keeper; 

};

class Parameters
{
public:

	typedef std::map<KeywordBase, Parameter> ParametersMap;
	typedef std::vector<KeywordBase> DuplicatesList;

#ifdef USE_CXX11
	Parameters(std::initializer_list<Parameter> pl) : pmap(), dups()
	{
		for (const auto& p : pl) 
			add(p);
	}
#endif
	Parameters() : pmap(), dups()
	{
	}
	Parameters(const Parameters& other) : pmap(other.pmap), dups(other.dups)
	{
	}
	Parameters& operator=(const Parameters& other)
	{
		this->pmap = other.pmap;
		this->dups = other.dups;
		return *this;
	}
	void check() 
	{
		if (!dups.empty())
			throw multiple_parameter_error(multiple_parameter_error::Keywords(dups), "multiple"); 
	}
	void add(const Parameter& p) 
	{
		if (pmap.count(p.keyword()))
			dups.push_back(p.keyword());

		pmap[p.keyword()] = p;
	}
	bool contains(const std::string& name) const
	{
		return pmap.count(name) > 0;
	}
	void merge(const Parameters& pg) 
	{
#ifdef USE_CXX11
		for (const auto& iter : pg.pmap)
		{
			if (!pmap.count(iter.first))
				pmap[iter.first] = iter.second;
		}
#else
		for (ParametersMap::const_iterator iter=pg.pmap.begin(); iter!=pg.pmap.end(); ++iter)
		{
			if (!pmap.count(iter->first))
				pmap[iter->first] = iter->second;
		}
#endif
	}
	Parameter operator[](const KeywordBase& kw) const
	{
		ParametersMap::const_iterator it = pmap.find(kw);
		if (it != pmap.end())
			return it->second;
		else
			throw missed_parameter_error(kw, "missed");
	}
	Parameters& operator,(const Parameter& p)
	{
		add(p);
		return *this;
	}

private:

	ParametersMap pmap;
	DuplicatesList dups;
};

Parameters Parameter::operator,(const Parameter& p)
{
	Parameters pg;
	pg.add(*this);
	pg.add(p);
	return pg;
}

Parameter::operator Parameters()
{
	Parameters pg;
	pg.add(*this);
	return pg;
}


}

#endif
