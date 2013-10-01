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

#include <stichwort/base.hpp>
#include <stichwort/utils.hpp>

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

struct DefaultValue
{
	DefaultValue() { }
};

template <typename T> class Parameter;

template <typename ValueType> 
class Keyword : public KeywordBase
{
public:
	typedef ValueType Type;
	Keyword(const Identifier& id, const ValueType& default_value) : 
		KeywordBase(id), default_value__(default_value) { }
	Keyword(const Keyword& pk);
	Keyword& operator=(const Keyword& pk); 

	Parameter<ValueType> operator=(const ValueType& value) const;
	Parameter<ValueType> operator=(const DefaultValue&) const;

	inline ValueType default_value() const
	{
		return default_value__;
	}
private:
	const ValueType default_value__;
};

template <typename T>
class Parameter : public ParameterBase
{
private:

	Parameter(KeywordBase& kw, const T& v) : 
		ParameterBase(kw), value__(v)
	{
	}

public:

	static Parameter create(KeywordBase& kw, const T& value) 
	{
		return Parameter(kw, value);
	}

	Parameter() : 
		ParameterBase(invalid_keyword), value__()
	{
	}

	Parameter(const Parameter& p) : 
		ParameterBase(p.keyword()), value__(p.value__)
	{
	}

	Parameter& operator=(const Parameter& p)
	{
		this->keyword__ = p.keyword__;
		this->value__ = p.value__;
		return *this;
	}

	ParameterBase* clone() const
	{
		return new Parameter(*this);
	}

	~Parameter()
	{
	}

	operator Parameters();

	template <template<class> class F, class Q>
	inline bool satisfies(F<Q> cond) const
	{
		return cond(value__);
	}

	template <typename R>
	Parameters operator,(const Parameter<R>& p);
	
	inline T get() const
	{
		return value__;
	}

	T value__;

};

class Parameters
{
public:

	typedef std::map<KeywordBase, unique_ptr<ParameterBase> > ParametersMap;
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
	Parameters(const Parameters& other) : pmap(), dups(other.dups)
	{
		for (ParametersMap::const_iterator iter=other.pmap.begin(); iter!=other.pmap.end(); ++iter)
		{
			pmap[iter->first] = unique_ptr<ParameterBase>(iter->second->clone());
		}
	}
	Parameters& operator=(const Parameters& other)
	{
		this->pmap = other.pmap;
		this->dups = other.dups;
		return *this;
	}
	template <typename T>
	void add(const Parameter<T>& p) 
	{
		if (pmap.count(p.keyword()))
			dups.push_back(p.keyword());

		pmap[p.keyword()] = unique_ptr<ParameterBase>(p.clone());
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
	template <typename R>
	R operator[](const Keyword<R>& kw) const
	{
		ParametersMap::const_iterator it = pmap.find(kw);
		if (it != pmap.end())
			return (dynamic_cast< const Parameter<R>* >(*(it->second)))->get();
		else
			return kw.default_value();
	}
	template <typename R>
	Parameters& operator,(const Parameter<R>& p)
	{
		add(p);
		return *this;
	}

private:

	ParametersMap pmap;
	DuplicatesList dups;
};

template<typename T>
template<typename R>
Parameters Parameter<T>::operator,(const Parameter<R>& p)
{
	Parameters pg;
	pg.add(*this);
	pg.add(p);
	return pg;
}

template<typename T>
Parameter<T>::operator Parameters()
{
	Parameters pg;
	pg.add(*this);
	return pg;
}

template<typename ValueType>
Parameter<ValueType> Keyword<ValueType>::operator=(const ValueType& value) const
{
	return Parameter<ValueType>::create(*const_cast<Keyword<ValueType>* >(this),value);
}
template<typename ValueType>
Parameter<ValueType> Keyword<ValueType>::operator=(const DefaultValue&) const
{
	return Parameter<ValueType>::create(*const_cast<Keyword<ValueType>* >(this),default_value());
}

struct Forwarder
{
	Forwarder() { }
	Forwarder(const Forwarder&);
	Forwarder& operator=(const Forwarder&);
	Parameters operator[](Parameters parameters) const
	{ return parameters; }
};

namespace
{
	const DefaultValue take_default;
	const Forwarder kwargs;
}

}

#endif
