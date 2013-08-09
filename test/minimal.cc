#include <gtest/gtest.h>
#include <stichwort/keywords.hpp>
#include <string>

namespace test
{
	const stichwort::ParametersForwarder kwargs;
	const stichwort::DefaultValue take_default;

	const stichwort::ParameterKeyword<float> a("A float", 1.0);
	const stichwort::ParameterKeyword<int> b("B int", 2);
	const stichwort::ParameterKeyword<double> c("C double", 3.0);

	double sum(double bias, stichwort::ParametersSet kw)
	{
		float av = kw[a];
		int bv = kw[b];
		double cv = kw[c];
		return bias + av + bv + cv;
	}
}

TEST(Minimal,Call)
{
	double sum;
	ASSERT_NO_THROW(sum = test::sum(4.0,test::kwargs[test::a=test::take_default,test::b=2,test::c=3.0]));
	ASSERT_NEAR(sum,10.0,1e-9);
}
