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

#ifndef STICHWORT_POLICY_H_
#define STICHWORT_POLICY_H_

#include <string>
#include <sstream>

#include <stichwort/utils.hpp>

namespace stichwort
{
namespace stichwort_internal
{

struct TypePolicyBase
{
	virtual ~TypePolicyBase() {}
	virtual void copyFromValue(void const*, void**) const = 0;
	virtual void* getValue(void* const*) const = 0;
	virtual void free(void**) const = 0;
	virtual void clone(void* const*, void**) const = 0;
	virtual void move(void* const*, void**) const = 0;
	virtual std::string repr(void* const*) const = 0;
};

template <typename T, bool>
struct repr_impl_if_streaming_supported
{
	std::string operator()(const TypePolicyBase* const impl, void** src) const;
};

template <typename T>
struct PointerTypePolicyImpl : public TypePolicyBase
{
	inline virtual void copyFromValue(void const* src, void** dest) const
	{
		*dest = new T(*reinterpret_cast<T const*>(src));
	}
	inline virtual void* getValue(void* const* src) const
	{
		return *src;
	}
	inline virtual void free(void** src) const
	{
		if (*src) 
			delete (*reinterpret_cast<T**>(src));
		*src = NULL;
	}
	virtual void clone(void* const* src, void** dest) const
	{
		if (*dest) 
			(*reinterpret_cast<T**>(dest))->~T();
		*dest = new T(**reinterpret_cast<T* const*>(src));
	}
	inline virtual void move(void* const* src, void** dest) const
	{
		(*reinterpret_cast<T**>(dest))->~T();
		**reinterpret_cast<T**>(dest) = **reinterpret_cast<T* const*>(src); 
	}
	inline virtual std::string repr(void* const* src) const
	{
		return repr_impl_if_streaming_supported<T,sfinae::supports_saving<std::stringstream,T>::value>()(this,src);
	}
};

template <typename T>
struct repr_impl_if_streaming_supported<T,true>
{
	std::string operator()(const TypePolicyBase* const impl, void* const* src) const
	{
		void* vv = impl->getValue(src);
		T* vp = reinterpret_cast<T*>(vv);
		T v = *vp;
		std::stringstream ss;
		ss << v;
		return ss.str();
	}
};

template <typename T>
struct repr_impl_if_streaming_supported<T,false>
{
	std::string operator()(const TypePolicyBase* const, void* const*) const
	{
		return "(can't obtain value)";
	}
};

namespace 
{
	template <typename T>
	TypePolicyBase* getPolicy()
	{
		static PointerTypePolicyImpl<T> policy;
		return &policy;
	}
}

}
}
#endif
