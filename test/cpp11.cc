#include <gtest/gtest.h>
#include <stichwort/stichwort.hpp>
#include <string>
#include <list>

namespace test
{
	namespace
	{
		const stichwort::Keyword<int> a("A", 1);
		const stichwort::Keyword<int> b("B", 2);
		const stichwort::Keyword<int> c("C", 3);
	}

	int sum(int s, stichwort::Parameters ps)
	{
		int av = ps[a];
		int bv = ps[b];
		int cv = ps[c];
		return s + av + bv + cv;
	}
	int identity(stichwort::Parameters ps)
	{
		return ps[a];
	}
}

TEST(Cpp11,InitializerListMultiple)
{
	int s = 700;
	int a_value = 6;
	int b_value = 99;
	int c_value = 76;
	int expected_sum = a_value + b_value + c_value + s;
	int sum;
	ASSERT_NO_THROW(sum = test::sum(s, {test::a=a_value, test::b=b_value, test::c=c_value}));
	ASSERT_EQ(sum, expected_sum);
}

TEST(Cpp11,InitializerListSingle)
{
	int a_value = 49;
	decltype(test::a)::Type id;
	ASSERT_NO_THROW(id = test::identity({test::a=a_value}));
	ASSERT_EQ(id, a_value);
}
