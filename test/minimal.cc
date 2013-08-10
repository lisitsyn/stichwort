#include <gtest/gtest.h>
#include <stichwort/keywords.hpp>
#include <string>
#include <list>

namespace test
{
	namespace 
	{
		const stichwort::ParametersForwarder kwargs;
		const stichwort::DefaultValue take_default;

		const stichwort::ParameterKeyword<float> a("A float", 1.0);
		const stichwort::ParameterKeyword<int> b("B int", 2);
		const stichwort::ParameterKeyword<double> c("C double", 3.0);
	}

	float select_a(stichwort::ParametersSet kw)
	{
		return kw[a];
	}
	int select_b(stichwort::ParametersSet kw)
	{
		return kw[b];
	}
	double select_c(stichwort::ParametersSet kw)
	{
		return kw[c];
	}
	struct result
	{
		result() : a(0.0f), b(0), c(0.0) {}
		result(float a_, int b_, double c_) : a(a_), b(b_), c(c_) {}
		float a;
		int b;
		double c;
	};
	result dispatch(stichwort::ParametersSet kw)
	{
		return result(kw[a], kw[b], kw[c]);
	}
}

using namespace test;

TEST(Minimal,Call)
{
	result r;
	float av = 89.0f;
	int bv = 56;
	double cv = 35.0;
	ASSERT_NO_THROW(r = dispatch(kwargs[a=av,b=bv,c=cv]));
	ASSERT_EQ(r.a, av);
	ASSERT_EQ(r.b, bv);
	ASSERT_EQ(r.c, cv);
}

TEST(Minimal,CallCombinations)
{
	std::list<stichwort::ParametersSet> combs;

	stichwort::ParametersSet abc = kwargs[a=take_default,b=take_default,c=take_default];
	combs.push_back(abc);
	stichwort::ParametersSet acb = kwargs[a=take_default,c=take_default,b=take_default];
	combs.push_back(acb);
	stichwort::ParametersSet bac = kwargs[b=take_default,a=take_default,c=take_default];
	combs.push_back(bac);
	stichwort::ParametersSet bca = kwargs[b=take_default,c=take_default,a=take_default];
	combs.push_back(bca);
	stichwort::ParametersSet cab = kwargs[c=take_default,a=take_default,b=take_default];
	combs.push_back(cab);
	stichwort::ParametersSet cba = kwargs[c=take_default,b=take_default,a=take_default];
	combs.push_back(cba);

	for (std::list<stichwort::ParametersSet>::const_iterator iter=combs.begin();
	     iter!=combs.end();
	     ++iter)
	{
		ASSERT_EQ(select_a(*iter), a.default_value);
		ASSERT_EQ(select_b(*iter), b.default_value);
		ASSERT_EQ(select_c(*iter), c.default_value);
	}
}
