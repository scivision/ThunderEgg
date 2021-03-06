#include "catch.hpp"
#include <ThunderEgg/Side.h>
#include <sstream>
using namespace std;
using namespace ThunderEgg;
TEST_CASE("Test num_sides for Side<1>", "[Side]")
{
	size_t sides = Side<1>::num_sides;
	CHECK(sides == 2);
}
TEST_CASE("Test num_sides for Side<2>", "[Side]")
{
	size_t sides = Side<2>::num_sides;
	CHECK(sides == 4);
}
TEST_CASE("Test num_sides for Side<3>", "[Side]")
{
	size_t sides = Side<3>::num_sides;
	CHECK(sides == 6);
}
TEST_CASE("Test that named constructors for Side<1> give expected index values", "[Side]")
{
	CHECK(Side<1>::west().getIndex() == 0);
	CHECK(Side<1>::east().getIndex() == 1);
	CHECK(Side<1>::null().getIndex() == 2);
}
TEST_CASE("Test that named constructors for Side<2> give expected index values", "[Side]")
{
	CHECK(Side<2>::west().getIndex() == 0);
	CHECK(Side<2>::east().getIndex() == 1);
	CHECK(Side<2>::south().getIndex() == 2);
	CHECK(Side<2>::north().getIndex() == 3);
	CHECK(Side<2>::null().getIndex() == 4);
}
TEST_CASE("Test that named constructors for Side<3> give expected index values", "[Side]")
{
	CHECK(Side<3>::west().getIndex() == 0);
	CHECK(Side<3>::east().getIndex() == 1);
	CHECK(Side<3>::south().getIndex() == 2);
	CHECK(Side<3>::north().getIndex() == 3);
	CHECK(Side<3>::bottom().getIndex() == 4);
	CHECK(Side<3>::top().getIndex() == 5);
	CHECK(Side<3>::null().getIndex() == 6);
}
TEST_CASE("Test that named constructors for Side<1> give expected axis index values", "[Side]")
{
	CHECK(Side<1>::west().getAxisIndex() == 0);
	CHECK(Side<1>::east().getAxisIndex() == 0);
	CHECK(Side<1>::null().getAxisIndex() == 1);
}
TEST_CASE("Test that named constructors for Side<2> give expected axis index values", "[Side]")
{
	CHECK(Side<2>::west().getAxisIndex() == 0);
	CHECK(Side<2>::east().getAxisIndex() == 0);
	CHECK(Side<2>::south().getAxisIndex() == 1);
	CHECK(Side<2>::north().getAxisIndex() == 1);
	CHECK(Side<2>::null().getAxisIndex() == 2);
}
TEST_CASE("Test that named constructors for Side<3> give expected axis index values", "[Side]")
{
	CHECK(Side<3>::west().getAxisIndex() == 0);
	CHECK(Side<3>::east().getAxisIndex() == 0);
	CHECK(Side<3>::south().getAxisIndex() == 1);
	CHECK(Side<3>::north().getAxisIndex() == 1);
	CHECK(Side<3>::bottom().getAxisIndex() == 2);
	CHECK(Side<3>::top().getAxisIndex() == 2);
	CHECK(Side<3>::null().getAxisIndex() == 3);
}
TEST_CASE("Test opposite for Side<1>", "[Side]")
{
	CHECK(Side<1>::west().opposite() == Side<1>::east());
	CHECK(Side<1>::east().opposite() == Side<1>::west());
}
TEST_CASE("Test opposite for Side<2>", "[Side]")
{
	CHECK(Side<2>::west().opposite() == Side<2>::east());
	CHECK(Side<2>::east().opposite() == Side<2>::west());
	CHECK(Side<2>::south().opposite() == Side<2>::north());
	CHECK(Side<2>::north().opposite() == Side<2>::south());
}
TEST_CASE("Test opposite for Side<3>", "[Side]")
{
	CHECK(Side<3>::west().opposite() == Side<3>::east());
	CHECK(Side<3>::east().opposite() == Side<3>::west());
	CHECK(Side<3>::south().opposite() == Side<3>::north());
	CHECK(Side<3>::north().opposite() == Side<3>::south());
	CHECK(Side<3>::bottom().opposite() == Side<3>::top());
	CHECK(Side<3>::top().opposite() == Side<3>::bottom());
}
TEST_CASE("Test isLowerOnAxis for Side<1>", "[Side]")
{
	CHECK(Side<1>::west().isLowerOnAxis());
	CHECK_FALSE(Side<1>::east().isLowerOnAxis());
}
TEST_CASE("Test isLowerOnAxis for Side<2>", "[Side]")
{
	CHECK(Side<2>::west().isLowerOnAxis());
	CHECK_FALSE(Side<2>::east().isLowerOnAxis());
	CHECK(Side<2>::south().isLowerOnAxis());
	CHECK_FALSE(Side<2>::north().isLowerOnAxis());
}
TEST_CASE("Test isLowerOnAxis for Side<3>", "[Side]")
{
	CHECK(Side<3>::west().isLowerOnAxis());
	CHECK_FALSE(Side<3>::east().isLowerOnAxis());
	CHECK(Side<3>::south().isLowerOnAxis());
	CHECK_FALSE(Side<3>::north().isLowerOnAxis());
	CHECK(Side<3>::bottom().isLowerOnAxis());
	CHECK_FALSE(Side<3>::top().isLowerOnAxis());
}
TEST_CASE("Test isHigherOnAxis for Side<1>", "[Side]")
{
	CHECK_FALSE(Side<1>::west().isHigherOnAxis());
	CHECK(Side<1>::east().isHigherOnAxis());
}
TEST_CASE("Test isHigherOnAxis for Side<2>", "[Side]")
{
	CHECK_FALSE(Side<2>::west().isHigherOnAxis());
	CHECK(Side<2>::east().isHigherOnAxis());
	CHECK_FALSE(Side<2>::south().isHigherOnAxis());
	CHECK(Side<2>::north().isHigherOnAxis());
}
TEST_CASE("Test isHigherOnAxis for Side<3>", "[Side]")
{
	CHECK_FALSE(Side<3>::west().isHigherOnAxis());
	CHECK(Side<3>::east().isHigherOnAxis());
	CHECK_FALSE(Side<3>::south().isHigherOnAxis());
	CHECK(Side<3>::north().isHigherOnAxis());
	CHECK_FALSE(Side<3>::bottom().isHigherOnAxis());
	CHECK(Side<3>::top().isHigherOnAxis());
}
TEST_CASE("Test iterator for Side<1>", "[Side]")
{
	auto iter = Side<1>::getValues().begin();
	CHECK(iter == Side<1>::getValues().begin());
	CHECK(iter != Side<1>::getValues().end());
	CHECK(*iter == Side<1>::west());
	++iter;
	CHECK(iter->isHigherOnAxis());
	CHECK(*iter == Side<1>::east());
	++iter;
	CHECK(*iter == Side<1>::null());
	CHECK(iter == Side<1>::getValues().end());
}
TEST_CASE("Test iterator for Side<2>", "[Side]")
{
	auto iter = Side<2>::getValues().begin();
	CHECK(iter == Side<2>::getValues().begin());
	CHECK(iter != Side<2>::getValues().end());
	CHECK(*iter == Side<2>::west());
	++iter;
	CHECK(iter->isHigherOnAxis());
	CHECK(*iter == Side<2>::east());
	++iter;
	CHECK(*iter == Side<2>::south());
	++iter;
	CHECK(*iter == Side<2>::north());
	++iter;
	CHECK(*iter == Side<2>::null());
	CHECK(iter == Side<2>::getValues().end());
}
TEST_CASE("Test iterator for Side<3>", "[Side]")
{
	auto iter = Side<3>::getValues().begin();
	CHECK(iter == Side<3>::getValues().begin());
	CHECK(iter != Side<3>::getValues().end());
	CHECK(*iter == Side<3>::west());
	++iter;
	CHECK(iter->isHigherOnAxis());
	CHECK(*iter == Side<3>::east());
	++iter;
	CHECK(*iter == Side<3>::south());
	++iter;
	CHECK(*iter == Side<3>::north());
	++iter;
	CHECK(*iter == Side<3>::bottom());
	++iter;
	CHECK(*iter == Side<3>::top());
	++iter;
	CHECK(*iter == Side<3>::null());
	CHECK(iter == Side<3>::getValues().end());
}
TEST_CASE("Test ostream for Side<1>", "[Side]")
{
	stringstream ss;
	ss << Side<1>::west();
	CHECK(ss.str() == "Side<1>::west()");
	ss.str("");
	ss << Side<1>::east();
	CHECK(ss.str() == "Side<1>::east()");
	ss.str("");
	ss << Side<1>::null();
	CHECK(ss.str() == "Side<1>::null()");
	ss.str("");
	ss << Side<1>(13);
	CHECK(ss.str() == "Side<1> undefined value: 13");
}
TEST_CASE("Test ostream for Side<2>", "[Side]")
{
	stringstream ss;
	ss << Side<2>::east();
	CHECK(ss.str() == "Side<2>::east()");
	ss.str("");
	ss << Side<2>::west();
	CHECK(ss.str() == "Side<2>::west()");
	ss.str("");
	ss << Side<2>::south();
	CHECK(ss.str() == "Side<2>::south()");
	ss.str("");
	ss << Side<2>::north();
	CHECK(ss.str() == "Side<2>::north()");
	ss.str("");
	ss << Side<2>::null();
	CHECK(ss.str() == "Side<2>::null()");
	ss.str("");
	ss << Side<2>(13);
	CHECK(ss.str() == "Side<2> undefined value: 13");
}
TEST_CASE("Test ostream for Side<3>", "[Side]")
{
	stringstream ss;
	ss << Side<3>::east();
	CHECK(ss.str() == "Side<3>::east()");
	ss.str("");
	ss << Side<3>::west();
	CHECK(ss.str() == "Side<3>::west()");
	ss.str("");
	ss << Side<3>::south();
	CHECK(ss.str() == "Side<3>::south()");
	ss.str("");
	ss << Side<3>::north();
	CHECK(ss.str() == "Side<3>::north()");
	ss.str("");
	ss << Side<3>::bottom();
	CHECK(ss.str() == "Side<3>::bottom()");
	ss.str("");
	ss << Side<3>::top();
	CHECK(ss.str() == "Side<3>::top()");
	ss.str("");
	ss << Side<3>::null();
	CHECK(ss.str() == "Side<3>::null()");
	ss.str("");
	ss << Side<3>(13);
	CHECK(ss.str() == "Side<3> undefined value: 13");
}
TEST_CASE("== operator works for Side<1>", "[Side]")
{
	CHECK_FALSE(Side<1>::east() == Side<1>::west());
	CHECK(Side<1>::east() == Side<1>::east());
	CHECK_FALSE(Side<1>::east() == Side<1>::null());

	CHECK(Side<1>::west() == Side<1>::west());
	CHECK_FALSE(Side<1>::west() == Side<1>::east());
	CHECK_FALSE(Side<1>::east() == Side<1>::null());

	CHECK_FALSE(Side<1>::null() == Side<1>::west());
	CHECK_FALSE(Side<1>::null() == Side<1>::east());
	CHECK(Side<1>::null() == Side<1>::null());
}
TEST_CASE("== operator works for Side<2>", "[Side]")
{
	CHECK(Side<2>::west() == Side<2>::west());
	CHECK_FALSE(Side<2>::west() == Side<2>::east());
	CHECK_FALSE(Side<2>::west() == Side<2>::south());
	CHECK_FALSE(Side<2>::west() == Side<2>::north());
	CHECK_FALSE(Side<2>::west() == Side<2>::null());

	CHECK_FALSE(Side<2>::east() == Side<2>::west());
	CHECK(Side<2>::east() == Side<2>::east());
	CHECK_FALSE(Side<2>::east() == Side<2>::south());
	CHECK_FALSE(Side<2>::east() == Side<2>::north());
	CHECK_FALSE(Side<2>::east() == Side<2>::null());

	CHECK_FALSE(Side<2>::south() == Side<2>::west());
	CHECK_FALSE(Side<2>::south() == Side<2>::east());
	CHECK(Side<2>::south() == Side<2>::south());
	CHECK_FALSE(Side<2>::south() == Side<2>::north());
	CHECK_FALSE(Side<2>::south() == Side<2>::null());

	CHECK_FALSE(Side<2>::north() == Side<2>::west());
	CHECK_FALSE(Side<2>::north() == Side<2>::east());
	CHECK_FALSE(Side<2>::north() == Side<2>::south());
	CHECK(Side<2>::north() == Side<2>::north());
	CHECK_FALSE(Side<2>::north() == Side<2>::null());

	CHECK_FALSE(Side<2>::null() == Side<2>::west());
	CHECK_FALSE(Side<2>::null() == Side<2>::east());
	CHECK_FALSE(Side<2>::null() == Side<2>::south());
	CHECK_FALSE(Side<2>::null() == Side<2>::north());
	CHECK(Side<2>::null() == Side<2>::null());
}
TEST_CASE("== operator works for Side<3>", "[Side]")
{
	CHECK(Side<3>::west() == Side<3>::west());
	CHECK_FALSE(Side<3>::west() == Side<3>::east());
	CHECK_FALSE(Side<3>::west() == Side<3>::south());
	CHECK_FALSE(Side<3>::west() == Side<3>::north());
	CHECK_FALSE(Side<3>::west() == Side<3>::bottom());
	CHECK_FALSE(Side<3>::west() == Side<3>::top());
	CHECK_FALSE(Side<3>::west() == Side<3>::null());

	CHECK_FALSE(Side<3>::east() == Side<3>::west());
	CHECK(Side<3>::east() == Side<3>::east());
	CHECK_FALSE(Side<3>::east() == Side<3>::south());
	CHECK_FALSE(Side<3>::east() == Side<3>::north());
	CHECK_FALSE(Side<3>::east() == Side<3>::bottom());
	CHECK_FALSE(Side<3>::east() == Side<3>::top());
	CHECK_FALSE(Side<3>::east() == Side<3>::null());

	CHECK_FALSE(Side<3>::south() == Side<3>::west());
	CHECK_FALSE(Side<3>::south() == Side<3>::east());
	CHECK(Side<3>::south() == Side<3>::south());
	CHECK_FALSE(Side<3>::south() == Side<3>::north());
	CHECK_FALSE(Side<3>::south() == Side<3>::bottom());
	CHECK_FALSE(Side<3>::south() == Side<3>::top());
	CHECK_FALSE(Side<3>::south() == Side<3>::null());

	CHECK_FALSE(Side<3>::north() == Side<3>::west());
	CHECK_FALSE(Side<3>::north() == Side<3>::east());
	CHECK_FALSE(Side<3>::north() == Side<3>::south());
	CHECK(Side<3>::north() == Side<3>::north());
	CHECK_FALSE(Side<3>::north() == Side<3>::bottom());
	CHECK_FALSE(Side<3>::north() == Side<3>::top());
	CHECK_FALSE(Side<3>::north() == Side<3>::null());

	CHECK_FALSE(Side<3>::bottom() == Side<3>::west());
	CHECK_FALSE(Side<3>::bottom() == Side<3>::east());
	CHECK_FALSE(Side<3>::bottom() == Side<3>::south());
	CHECK_FALSE(Side<3>::bottom() == Side<3>::north());
	CHECK(Side<3>::bottom() == Side<3>::bottom());
	CHECK_FALSE(Side<3>::bottom() == Side<3>::top());
	CHECK_FALSE(Side<3>::bottom() == Side<3>::null());

	CHECK_FALSE(Side<3>::top() == Side<3>::west());
	CHECK_FALSE(Side<3>::top() == Side<3>::east());
	CHECK_FALSE(Side<3>::top() == Side<3>::south());
	CHECK_FALSE(Side<3>::top() == Side<3>::north());
	CHECK_FALSE(Side<3>::top() == Side<3>::bottom());
	CHECK(Side<3>::top() == Side<3>::top());
	CHECK_FALSE(Side<3>::top() == Side<3>::null());

	CHECK_FALSE(Side<3>::null() == Side<3>::west());
	CHECK_FALSE(Side<3>::null() == Side<3>::east());
	CHECK_FALSE(Side<3>::null() == Side<3>::south());
	CHECK_FALSE(Side<3>::null() == Side<3>::north());
	CHECK_FALSE(Side<3>::null() == Side<3>::bottom());
	CHECK_FALSE(Side<3>::null() == Side<3>::top());
	CHECK(Side<3>::null() == Side<3>::null());
}
TEST_CASE("!= operator works for Side<1>", "[Side]")
{
	CHECK_FALSE(Side<1>::west() != Side<1>::west());
	CHECK(Side<1>::west() != Side<1>::east());
	CHECK(Side<1>::west() != Side<1>::null());

	CHECK(Side<1>::east() != Side<1>::west());
	CHECK_FALSE(Side<1>::east() != Side<1>::east());
	CHECK(Side<1>::east() != Side<1>::null());

	CHECK(Side<1>::null() != Side<1>::west());
	CHECK(Side<1>::null() != Side<1>::east());
	CHECK_FALSE(Side<1>::null() != Side<1>::null());
}
TEST_CASE("!= operator works for Side<2>", "[Side]")
{
	CHECK_FALSE(Side<2>::west() != Side<2>::west());
	CHECK(Side<2>::west() != Side<2>::east());
	CHECK(Side<2>::west() != Side<2>::south());
	CHECK(Side<2>::west() != Side<2>::north());
	CHECK(Side<2>::west() != Side<2>::null());

	CHECK(Side<2>::east() != Side<2>::west());
	CHECK_FALSE(Side<2>::east() != Side<2>::east());
	CHECK(Side<2>::east() != Side<2>::south());
	CHECK(Side<2>::east() != Side<2>::north());
	CHECK(Side<2>::east() != Side<2>::null());

	CHECK(Side<2>::south() != Side<2>::west());
	CHECK(Side<2>::south() != Side<2>::east());
	CHECK_FALSE(Side<2>::south() != Side<2>::south());
	CHECK(Side<2>::south() != Side<2>::north());
	CHECK(Side<2>::south() != Side<2>::null());

	CHECK(Side<2>::north() != Side<2>::west());
	CHECK(Side<2>::north() != Side<2>::east());
	CHECK(Side<2>::north() != Side<2>::south());
	CHECK_FALSE(Side<2>::north() != Side<2>::north());
	CHECK(Side<2>::north() != Side<2>::null());

	CHECK(Side<2>::null() != Side<2>::west());
	CHECK(Side<2>::null() != Side<2>::east());
	CHECK(Side<2>::null() != Side<2>::south());
	CHECK(Side<2>::null() != Side<2>::north());
	CHECK_FALSE(Side<2>::null() != Side<2>::null());
}
TEST_CASE("!= operator works for Side<3>", "[Side]")
{
	CHECK_FALSE(Side<3>::west() != Side<3>::west());
	CHECK(Side<3>::west() != Side<3>::east());
	CHECK(Side<3>::west() != Side<3>::south());
	CHECK(Side<3>::west() != Side<3>::north());
	CHECK(Side<3>::west() != Side<3>::bottom());
	CHECK(Side<3>::west() != Side<3>::top());
	CHECK(Side<3>::west() != Side<3>::null());

	CHECK(Side<3>::east() != Side<3>::west());
	CHECK_FALSE(Side<3>::east() != Side<3>::east());
	CHECK(Side<3>::east() != Side<3>::south());
	CHECK(Side<3>::east() != Side<3>::north());
	CHECK(Side<3>::east() != Side<3>::bottom());
	CHECK(Side<3>::east() != Side<3>::top());
	CHECK(Side<3>::east() != Side<3>::null());

	CHECK(Side<3>::south() != Side<3>::west());
	CHECK(Side<3>::south() != Side<3>::east());
	CHECK_FALSE(Side<3>::south() != Side<3>::south());
	CHECK(Side<3>::south() != Side<3>::north());
	CHECK(Side<3>::south() != Side<3>::bottom());
	CHECK(Side<3>::south() != Side<3>::top());
	CHECK(Side<3>::south() != Side<3>::null());

	CHECK(Side<3>::north() != Side<3>::west());
	CHECK(Side<3>::north() != Side<3>::east());
	CHECK(Side<3>::north() != Side<3>::south());
	CHECK_FALSE(Side<3>::north() != Side<3>::north());
	CHECK(Side<3>::north() != Side<3>::bottom());
	CHECK(Side<3>::north() != Side<3>::top());
	CHECK(Side<3>::north() != Side<3>::null());

	CHECK(Side<3>::bottom() != Side<3>::west());
	CHECK(Side<3>::bottom() != Side<3>::east());
	CHECK(Side<3>::bottom() != Side<3>::south());
	CHECK(Side<3>::bottom() != Side<3>::north());
	CHECK_FALSE(Side<3>::bottom() != Side<3>::bottom());
	CHECK(Side<3>::bottom() != Side<3>::top());
	CHECK(Side<3>::bottom() != Side<3>::null());

	CHECK(Side<3>::top() != Side<3>::west());
	CHECK(Side<3>::top() != Side<3>::east());
	CHECK(Side<3>::top() != Side<3>::south());
	CHECK(Side<3>::top() != Side<3>::north());
	CHECK(Side<3>::top() != Side<3>::bottom());
	CHECK_FALSE(Side<3>::top() != Side<3>::top());
	CHECK(Side<3>::top() != Side<3>::null());

	CHECK(Side<3>::null() != Side<3>::west());
	CHECK(Side<3>::null() != Side<3>::east());
	CHECK(Side<3>::null() != Side<3>::south());
	CHECK(Side<3>::null() != Side<3>::north());
	CHECK(Side<3>::null() != Side<3>::bottom());
	CHECK(Side<3>::null() != Side<3>::top());
	CHECK_FALSE(Side<3>::null() != Side<3>::null());
}
TEST_CASE("< operator works for Side<1>", "[Side]")
{
	CHECK_FALSE(Side<1>::west() < Side<1>::west());
	CHECK(Side<1>::west() < Side<1>::east());
	CHECK(Side<1>::west() < Side<1>::null());

	CHECK_FALSE(Side<1>::east() < Side<1>::west());
	CHECK_FALSE(Side<1>::east() < Side<1>::east());
	CHECK(Side<1>::east() < Side<1>::null());

	CHECK_FALSE(Side<1>::null() < Side<1>::west());
	CHECK_FALSE(Side<1>::null() < Side<1>::east());
	CHECK_FALSE(Side<1>::null() < Side<1>::null());
}
TEST_CASE("< operator works for Side<2>", "[Side]")
{
	CHECK_FALSE(Side<2>::west() < Side<2>::west());
	CHECK(Side<2>::west() < Side<2>::east());
	CHECK(Side<2>::west() < Side<2>::south());
	CHECK(Side<2>::west() < Side<2>::north());
	CHECK(Side<2>::west() < Side<2>::null());

	CHECK_FALSE(Side<2>::east() < Side<2>::west());
	CHECK_FALSE(Side<2>::east() < Side<2>::east());
	CHECK(Side<2>::east() < Side<2>::south());
	CHECK(Side<2>::east() < Side<2>::north());
	CHECK(Side<2>::east() < Side<2>::null());

	CHECK_FALSE(Side<2>::south() < Side<2>::west());
	CHECK_FALSE(Side<2>::south() < Side<2>::east());
	CHECK_FALSE(Side<2>::south() < Side<2>::south());
	CHECK(Side<2>::south() < Side<2>::north());
	CHECK(Side<2>::south() < Side<2>::null());

	CHECK_FALSE(Side<2>::north() < Side<2>::west());
	CHECK_FALSE(Side<2>::north() < Side<2>::east());
	CHECK_FALSE(Side<2>::north() < Side<2>::south());
	CHECK_FALSE(Side<2>::north() < Side<2>::north());
	CHECK(Side<2>::north() < Side<2>::null());

	CHECK_FALSE(Side<2>::null() < Side<2>::west());
	CHECK_FALSE(Side<2>::null() < Side<2>::east());
	CHECK_FALSE(Side<2>::null() < Side<2>::south());
	CHECK_FALSE(Side<2>::null() < Side<2>::north());
	CHECK_FALSE(Side<2>::null() < Side<2>::null());
}
TEST_CASE("< operator works for Side<3>", "[Side]")
{
	CHECK_FALSE(Side<3>::west() < Side<3>::west());
	CHECK(Side<3>::west() < Side<3>::east());
	CHECK(Side<3>::west() < Side<3>::south());
	CHECK(Side<3>::west() < Side<3>::north());
	CHECK(Side<3>::west() < Side<3>::bottom());
	CHECK(Side<3>::west() < Side<3>::top());
	CHECK(Side<3>::west() < Side<3>::null());

	CHECK_FALSE(Side<3>::east() < Side<3>::west());
	CHECK_FALSE(Side<3>::east() < Side<3>::east());
	CHECK(Side<3>::east() < Side<3>::south());
	CHECK(Side<3>::east() < Side<3>::north());
	CHECK(Side<3>::east() < Side<3>::bottom());
	CHECK(Side<3>::east() < Side<3>::top());
	CHECK(Side<3>::east() < Side<3>::null());

	CHECK_FALSE(Side<3>::south() < Side<3>::west());
	CHECK_FALSE(Side<3>::south() < Side<3>::east());
	CHECK_FALSE(Side<3>::south() < Side<3>::south());
	CHECK(Side<3>::south() < Side<3>::north());
	CHECK(Side<3>::south() < Side<3>::bottom());
	CHECK(Side<3>::south() < Side<3>::top());
	CHECK(Side<3>::south() < Side<3>::null());

	CHECK_FALSE(Side<3>::north() < Side<3>::west());
	CHECK_FALSE(Side<3>::north() < Side<3>::east());
	CHECK_FALSE(Side<3>::north() < Side<3>::south());
	CHECK_FALSE(Side<3>::north() < Side<3>::north());
	CHECK(Side<3>::north() < Side<3>::bottom());
	CHECK(Side<3>::north() < Side<3>::top());
	CHECK(Side<3>::north() < Side<3>::null());

	CHECK_FALSE(Side<3>::bottom() < Side<3>::west());
	CHECK_FALSE(Side<3>::bottom() < Side<3>::east());
	CHECK_FALSE(Side<3>::bottom() < Side<3>::south());
	CHECK_FALSE(Side<3>::bottom() < Side<3>::north());
	CHECK_FALSE(Side<3>::bottom() < Side<3>::bottom());
	CHECK(Side<3>::bottom() < Side<3>::top());
	CHECK(Side<3>::bottom() < Side<3>::null());

	CHECK_FALSE(Side<3>::top() < Side<3>::west());
	CHECK_FALSE(Side<3>::top() < Side<3>::east());
	CHECK_FALSE(Side<3>::top() < Side<3>::south());
	CHECK_FALSE(Side<3>::top() < Side<3>::north());
	CHECK_FALSE(Side<3>::top() < Side<3>::bottom());
	CHECK_FALSE(Side<3>::top() < Side<3>::top());
	CHECK(Side<3>::top() < Side<3>::null());

	CHECK_FALSE(Side<3>::null() < Side<3>::west());
	CHECK_FALSE(Side<3>::null() < Side<3>::east());
	CHECK_FALSE(Side<3>::null() < Side<3>::south());
	CHECK_FALSE(Side<3>::null() < Side<3>::north());
	CHECK_FALSE(Side<3>::null() < Side<3>::bottom());
	CHECK_FALSE(Side<3>::null() < Side<3>::top());
	CHECK_FALSE(Side<3>::null() < Side<3>::null());
}
TEST_CASE("LowerSideOnAxis works for Side<3>", "[Side]")
{
	CHECK(Side<3>::LowerSideOnAxis(0) == Side<3>::west());
	CHECK(Side<3>::LowerSideOnAxis(1) == Side<3>::south());
	CHECK(Side<3>::LowerSideOnAxis(2) == Side<3>::bottom());
}
TEST_CASE("LowerSideOnAxis works for Side<2>", "[Side]")
{
	CHECK(Side<2>::LowerSideOnAxis(0) == Side<2>::west());
	CHECK(Side<2>::LowerSideOnAxis(1) == Side<2>::south());
}
TEST_CASE("LowerSideOnAxis works for Side<1>", "[Side]")
{
	CHECK(Side<1>::LowerSideOnAxis(0) == Side<1>::west());
}
TEST_CASE("HigherSideOnAxis works for Side<3>", "[Side]")
{
	CHECK(Side<3>::HigherSideOnAxis(0) == Side<3>::east());
	CHECK(Side<3>::HigherSideOnAxis(1) == Side<3>::north());
	CHECK(Side<3>::HigherSideOnAxis(2) == Side<3>::top());
}
TEST_CASE("HigherSideOnAxis works for Side<2>", "[Side]")
{
	CHECK(Side<2>::HigherSideOnAxis(0) == Side<2>::east());
	CHECK(Side<2>::HigherSideOnAxis(1) == Side<2>::north());
}
TEST_CASE("HigherSideOnAxis works for Side<1>", "[Side]")
{
	CHECK(Side<1>::HigherSideOnAxis(0) == Side<1>::east());
}
TEST_CASE("Test to_json for Side<1>", "[Side]")
{
	nlohmann::json j;
	j["null"] = Side<1>::null();
	j["west"] = Side<1>::west();
	j["east"] = Side<1>::east();
	CHECK(j["null"] == nullptr);
	CHECK(j["west"] == "WEST");
	CHECK(j["east"] == "EAST");
}
TEST_CASE("Test to_json for Side<2>", "[Side]")
{
	nlohmann::json j;
	j["null"]  = Side<2>::null();
	j["west"]  = Side<2>::west();
	j["east"]  = Side<2>::east();
	j["south"] = Side<2>::south();
	j["north"] = Side<2>::north();
	CHECK(j["null"] == nullptr);
	CHECK(j["west"] == "WEST");
	CHECK(j["east"] == "EAST");
	CHECK(j["south"] == "SOUTH");
	CHECK(j["north"] == "NORTH");
}
TEST_CASE("Test to_json for Side<3>", "[Side]")
{
	nlohmann::json j;
	j["null"]   = Side<3>::null();
	j["west"]   = Side<3>::west();
	j["east"]   = Side<3>::east();
	j["south"]  = Side<3>::south();
	j["north"]  = Side<3>::north();
	j["bottom"] = Side<3>::bottom();
	j["top"]    = Side<3>::top();
	CHECK(j["null"] == nullptr);
	CHECK(j["west"] == "WEST");
	CHECK(j["east"] == "EAST");
	CHECK(j["south"] == "SOUTH");
	CHECK(j["north"] == "NORTH");
	CHECK(j["bottom"] == "BOTTOM");
	CHECK(j["top"] == "TOP");
}
TEST_CASE("Test from_json for Side<1>", "[Side]")
{
	nlohmann::json j;
	j["null"] = nullptr;
	j["west"] = "WEST";
	j["east"] = "EAST";
	CHECK(j["null"].get<Side<1>>() == Side<1>::null());
	CHECK(j["west"].get<Side<1>>() == Side<1>::west());
	CHECK(j["east"].get<Side<1>>() == Side<1>::east());
}
TEST_CASE("Test from_json for Side<2>", "[Side]")
{
	nlohmann::json j;
	j["null"]  = nullptr;
	j["west"]  = "WEST";
	j["east"]  = "EAST";
	j["south"] = "SOUTH";
	j["north"] = "NORTH";
	CHECK(j["null"].get<Side<2>>() == Side<2>::null());
	CHECK(j["west"].get<Side<2>>() == Side<2>::west());
	CHECK(j["east"].get<Side<3>>() == Side<3>::east());
	CHECK(j["south"].get<Side<2>>() == Side<2>::south());
	CHECK(j["north"].get<Side<2>>() == Side<2>::north());
}
TEST_CASE("Test from_json for Side<3>", "[Side]")
{
	nlohmann::json j;
	j["null"]   = nullptr;
	j["west"]   = "WEST";
	j["east"]   = "EAST";
	j["south"]  = "SOUTH";
	j["north"]  = "NORTH";
	j["bottom"] = "BOTTOM";
	j["top"]    = "TOP";
	CHECK(j["null"].get<Side<3>>() == Side<3>::null());
	CHECK(j["west"].get<Side<3>>() == Side<3>::west());
	CHECK(j["east"].get<Side<3>>() == Side<3>::east());
	CHECK(j["south"].get<Side<3>>() == Side<3>::south());
	CHECK(j["north"].get<Side<3>>() == Side<3>::north());
	CHECK(j["bottom"].get<Side<3>>() == Side<3>::bottom());
	CHECK(j["top"].get<Side<3>>() == Side<3>::top());
}