#include <gtest/gtest.h>
#include <stichwort/stichwort.hpp>
#include <string>
#include <list>

#include <iostream>

namespace test
{
	template<typename T>
	struct InRange
	{
		InRange(T l, T u) : lower(l), upper(u)
		{
		}
		bool operator()(T v) const
		{
			std::cout << "Value " << v << std::endl;
			return (v>=lower) && (v<upper);
		}
		std::string failureMessage(const stichwort::Parameter&) const
		{
			return "failed";
		}
		T lower;
		T upper;
	};

	stichwort::Keyword<double> x_in_range_kw("x in range", 0.0);
	typedef decltype(x_in_range_kw)::Type XInRangeType;
	stichwort::Keyword<double> x_not_in_range_kw("x not in range", 0.0);
	typedef decltype(x_not_in_range_kw)::Type XNotInRangeType;
}

TEST(Checkers,InRangeCondition)
{
	test::XInRangeType x_in_range_value = 1.0;
	test::XNotInRangeType x_not_in_range_value = 1.5;
	stichwort::Parameter x_in_range = stichwort::Parameter::create(test::x_in_range_kw, x_in_range_value);
	stichwort::Parameter x_not_in_range = stichwort::Parameter::create(test::x_not_in_range_kw, x_not_in_range_value);
	ASSERT_EQ(x_in_range.satisfies(test::InRange<test::XInRangeType>(0.5,1.25)), true);
	ASSERT_EQ(x_not_in_range.satisfies(test::InRange<test::XNotInRangeType>(0.5,1.25)), false);
}

