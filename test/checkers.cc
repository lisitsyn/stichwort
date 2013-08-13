#include <gtest/gtest.h>
#include <stichwort/keywords.hpp>
#include <string>
#include <list>

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
			return (v>=lower) && (v<upper);
		}
		std::string failureMessage(const stichwort::Parameter&) const
		{
			return "failed";
		}
		T lower;
		T upper;
	};
}

TEST(Checkers,InRangeCondition)
{
	typedef double XType;
	XType x_in_range_value = 1.0;
	XType x_not_in_range_value = 1.5;
	stichwort::Parameter x_in_range = stichwort::Parameter::create("x in range", x_in_range_value);
	stichwort::Parameter x_not_in_range = stichwort::Parameter::create("x not in range", x_not_in_range_value);
	ASSERT_EQ(x_in_range.isCondition(test::InRange<XType>(0.5,1.25)), true);
	ASSERT_EQ(x_not_in_range.isCondition(test::InRange<XType>(0.5,1.25)), false);
}

TEST(Checkers,InRangeException)
{
	typedef double XType;
	XType x_in_range_value = 1.0;
	XType x_not_in_range_value = 1.5;
	stichwort::Parameter x_in_range = stichwort::Parameter::create("x in range", x_in_range_value);
	stichwort::Parameter x_not_in_range = stichwort::Parameter::create("x not in range", x_not_in_range_value);
	ASSERT_NO_THROW(x_in_range.checked().satisfies(test::InRange<XType>(0.5,1.25)));
	ASSERT_NO_THROW(x_not_in_range.checked().satisfies(test::InRange<XType>(0.5,1.25)));
	XType tmp;
	ASSERT_THROW(tmp = x_not_in_range, stichwort::wrong_parameter_error);
	ASSERT_NO_THROW(tmp = x_in_range);
	ASSERT_EQ(tmp, x_in_range_value);
}
