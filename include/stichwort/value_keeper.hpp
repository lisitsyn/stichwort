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

#ifndef STICHWORT_KEEPER_H_
#define STICHWORD_KEEPER_H_

#include <stichwort/policy.hpp>
#include <stichwort/exceptions.hpp>

namespace stichwort
{
namespace stichwort_internal
{

struct EmptyType
{
};

class ValueKeeper
{
public:
	template <typename T>
	explicit ValueKeeper(const T& value) : 
		policy_(getPolicy<T>()), value_ptr_(NULL) 
	{
		policy_->copyFromValue(&value, &value_ptr_);
	}

	ValueKeeper() :
		policy_(getPolicy<EmptyType>()), value_ptr_(NULL) 
	{
	}

	~ValueKeeper()
	{
		policy_->free(&value_ptr_);
	}

	ValueKeeper(const ValueKeeper& v) : policy_(v.policy_), value_ptr_(NULL)
	{
		policy_->clone(&(v.value_ptr_), &value_ptr_);
	}

	ValueKeeper& operator=(const ValueKeeper& v)
	{
		policy_->free(&value_ptr_);
		policy_ = v.policy_;
		policy_->clone(&(v.value_ptr_), &value_ptr_);
		return *this;
	}

	template <typename T>
	optional<T> getValue() const
	{
		T* v;
		if (!isInitialized())
			return optional<T>();

		if (isTypeCorrect<T>())
		{
			void* vv = policy_->getValue(&value_ptr_);
			v = reinterpret_cast<T*>(vv);
		}
		else
			return optional<T>();

		return optional<T>(*v);
	}

	template <typename T>
	inline bool isTypeCorrect() const
	{ return getPolicy<T>() == policy_; }

	inline bool isInitialized() const
	{ return getPolicy<EmptyType>() != policy_; }

	template <template<class> class F, class Q>
	inline bool satisfies(F<Q> cond) const
	{
		optional<Q> opt = getValue<Q>();
		if (opt)
			return cond(*opt);
		return false;
	}

	inline std::string repr() const 
	{ return policy_->repr(const_cast<void**>(&value_ptr_)); }

private:
	TypePolicyBase* policy_;
	void* value_ptr_;
};

}
}
#endif
