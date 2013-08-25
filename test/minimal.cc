#include <gtest/gtest.h>
#include <stichwort/stichwort.hpp>
#include <string>
#include <list>

namespace test
{
	namespace 
	{
		const stichwort::Keyword<float> a("A float", 1.0);
		const stichwort::Keyword<int> b("B int", 2);
		const stichwort::Keyword<double> c("C double", 3.0);
	}

	float select_a(stichwort::Parameters kw)
	{
		return kw[a];
	}
	int select_b(stichwort::Parameters kw)
	{
		return kw[b];
	}
	double select_c(stichwort::Parameters kw)
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
	result dispatch(stichwort::Parameters kw)
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
	ASSERT_NO_THROW(r = dispatch(stichwort::kwargs[a=av,b=bv,c=cv]));
	ASSERT_EQ(r.a, av);
	ASSERT_EQ(r.b, bv);
	ASSERT_EQ(r.c, cv);
}

TEST(Minimal,CallCombinations)
{
	std::list<stichwort::Parameters> combs;

	stichwort::Parameters abc = 
		stichwort::kwargs[a=stichwort::take_default,b=stichwort::take_default,c=stichwort::take_default];
	combs.push_back(abc);
	stichwort::Parameters acb = 
		stichwort::kwargs[a=stichwort::take_default,c=stichwort::take_default,b=stichwort::take_default];
	combs.push_back(acb);
	stichwort::Parameters bac = 
		stichwort::kwargs[b=stichwort::take_default,a=stichwort::take_default,c=stichwort::take_default];
	combs.push_back(bac);
	stichwort::Parameters bca = 
		stichwort::kwargs[b=stichwort::take_default,c=stichwort::take_default,a=stichwort::take_default];
	combs.push_back(bca);
	stichwort::Parameters cab = 
		stichwort::kwargs[c=stichwort::take_default,a=stichwort::take_default,b=stichwort::take_default];
	combs.push_back(cab);
	stichwort::Parameters cba = 
		stichwort::kwargs[c=stichwort::take_default,b=stichwort::take_default,a=stichwort::take_default];
	combs.push_back(cba);

	for (std::list<stichwort::Parameters>::const_iterator iter=combs.begin();
	     iter!=combs.end();
	     ++iter)
	{
		ASSERT_EQ(select_a(*iter), a.default_value());
		ASSERT_EQ(select_b(*iter), b.default_value());
		ASSERT_EQ(select_c(*iter), c.default_value());
	}
}
