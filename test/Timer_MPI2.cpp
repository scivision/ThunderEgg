#include "catch.hpp"
#include <ThunderEgg/Domain.h>
#include <ThunderEgg/Timer.h>
#include <fstream>
#include <sstream>
using namespace std;
using namespace ThunderEgg;
static Domain<2> GetDomain()
{
	map<int, shared_ptr<PatchInfo<2>>> pinfo_map;

	int    n         = 10;
	double spacing   = 0.01;
	int    num_ghost = 1;

	pinfo_map[0].reset(new PatchInfo<2>());
	pinfo_map[0]->id = 0;
	pinfo_map[0]->ns.fill(n);
	pinfo_map[0]->spacings.fill(spacing);
	pinfo_map[0]->num_ghost_cells = num_ghost;

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	pinfo_map[0]->rank = rank;

	Domain<2> d(pinfo_map, {n, n}, num_ghost);
	return d;
}
static int occurrences(const std::string &s, const std::string &target)
{
	int                    occurrences = 0;
	std::string::size_type pos         = 0;
	while ((pos = s.find(target, pos)) != std::string::npos) {
		++occurrences;
		pos += target.length();
	}
	return occurrences;
}
TEST_CASE("Timer to_json empty timer", "[Timer]")
{
	Timer          timer(MPI_COMM_WORLD);
	nlohmann::json j = timer;
	INFO(j.dump(4));
	REQUIRE(j == nullptr);
}
TEST_CASE("Timer to_json unassociated timing", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);

	timer.start("A");
	timer.stop("A");

	const nlohmann::json j = timer;
	INFO(j.dump(4));

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		REQUIRE(j != nullptr);
		CHECK(j.size() == 2);
		CHECK(j["comm_size"] == 2);

		CHECK(j["timings"].is_array());
		CHECK(j["timings"].size() == 2);
		CHECK(j["timings"][0]["rank"] == 0);
		CHECK(j["timings"][0]["min"].is_number());
		CHECK(j["timings"][0]["max"].is_number());
		CHECK(j["timings"][0]["sum"].is_number());
		CHECK(j["timings"][0]["num_calls"].is_number());
		CHECK(j["timings"][0]["name"] == "A");
		CHECK(j["timings"][1]["rank"] == 1);
		CHECK(j["timings"][1]["min"].is_number());
		CHECK(j["timings"][1]["max"].is_number());
		CHECK(j["timings"][1]["sum"].is_number());
		CHECK(j["timings"][1]["num_calls"].is_number());
		CHECK(j["timings"][1]["name"] == "A");
	} else {
		CHECK(j == nullptr);
	}
}
TEST_CASE("Timer to_json unassociated timing with int info", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	int   rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	timer.start("A");
	if (rank == 0) {
		timer.addIntInfo("Example", 0);
	} else {
		timer.addIntInfo("Example", 1);
	}
	timer.stop("A");

	const nlohmann::json j = timer;
	INFO(j.dump(4));

	if (rank == 0) {
		REQUIRE(j != nullptr);
		CHECK(j.size() == 2);
		CHECK(j["comm_size"] == 2);

		CHECK(j["timings"].is_array());
		CHECK(j["timings"].size() == 2);
		CHECK(j["timings"][0]["rank"] == 0);
		CHECK(j["timings"][0]["min"].is_number());
		CHECK(j["timings"][0]["max"].is_number());
		CHECK(j["timings"][0]["sum"].is_number());
		CHECK(j["timings"][0]["num_calls"].is_number());
		CHECK(j["timings"][0]["name"] == "A");

		CHECK(j["timings"][0]["infos"].is_array());
		CHECK(j["timings"][0]["infos"].size() == 1);
		CHECK(j["timings"][0]["infos"][0]["name"] == "Example");
		CHECK(j["timings"][0]["infos"][0]["min"] == 0);
		CHECK(j["timings"][0]["infos"][0]["max"] == 0);
		CHECK(j["timings"][0]["infos"][0]["num_calls"] == 1);
		CHECK(j["timings"][0]["infos"][0]["sum"] == 0);

		CHECK(j["timings"][1]["rank"] == 1);
		CHECK(j["timings"][1]["min"].is_number());
		CHECK(j["timings"][1]["max"].is_number());
		CHECK(j["timings"][1]["sum"].is_number());
		CHECK(j["timings"][1]["num_calls"].is_number());
		CHECK(j["timings"][1]["name"] == "A");

		CHECK(j["timings"][1]["infos"].is_array());
		CHECK(j["timings"][1]["infos"].size() == 1);
		CHECK(j["timings"][1]["infos"][0]["name"] == "Example");
		CHECK(j["timings"][1]["infos"][0]["min"] == 1);
		CHECK(j["timings"][1]["infos"][0]["max"] == 1);
		CHECK(j["timings"][1]["infos"][0]["num_calls"] == 1);
		CHECK(j["timings"][1]["infos"][0]["sum"] == 1);
	} else {
		CHECK(j == nullptr);
	}
}
TEST_CASE("Timer to_json unassociated timing with double info", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	int   rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	timer.start("A");
	if (rank == 0) {
		timer.addDoubleInfo("Example", 0);
	} else {
		timer.addDoubleInfo("Example", 1);
	}
	timer.stop("A");

	const nlohmann::json j = timer;
	INFO(j.dump(4));

	if (rank == 0) {
		REQUIRE(j != nullptr);
		CHECK(j.size() == 2);
		CHECK(j["comm_size"] == 2);

		CHECK(j["timings"].is_array());
		CHECK(j["timings"].size() == 2);
		CHECK(j["timings"][0]["rank"] == 0);
		CHECK(j["timings"][0]["min"].is_number());
		CHECK(j["timings"][0]["max"].is_number());
		CHECK(j["timings"][0]["sum"].is_number());
		CHECK(j["timings"][0]["num_calls"].is_number());
		CHECK(j["timings"][0]["name"] == "A");

		CHECK(j["timings"][0]["infos"].is_array());
		CHECK(j["timings"][0]["infos"].size() == 1);
		CHECK(j["timings"][0]["infos"][0]["name"] == "Example");
		CHECK(j["timings"][0]["infos"][0]["min"] == 0);
		CHECK(j["timings"][0]["infos"][0]["max"] == 0);
		CHECK(j["timings"][0]["infos"][0]["num_calls"] == 1);
		CHECK(j["timings"][0]["infos"][0]["sum"] == 0);

		CHECK(j["timings"][1]["rank"] == 1);
		CHECK(j["timings"][1]["min"].is_number());
		CHECK(j["timings"][1]["max"].is_number());
		CHECK(j["timings"][1]["sum"].is_number());
		CHECK(j["timings"][1]["num_calls"].is_number());
		CHECK(j["timings"][1]["name"] == "A");

		CHECK(j["timings"][1]["infos"].is_array());
		CHECK(j["timings"][1]["infos"].size() == 1);
		CHECK(j["timings"][1]["infos"][0]["name"] == "Example");
		CHECK(j["timings"][1]["infos"][0]["min"] == 1);
		CHECK(j["timings"][1]["infos"][0]["max"] == 1);
		CHECK(j["timings"][1]["infos"][0]["num_calls"] == 1);
		CHECK(j["timings"][1]["infos"][0]["sum"] == 1);
	} else {
		CHECK(j == nullptr);
	}
}
TEST_CASE("Timer to_json two unassociated timings sequential", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.start("A");
	timer.stop("A");
	timer.start("B");
	timer.stop("B");
	const nlohmann::json j = timer;
	INFO(j.dump(4));

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		REQUIRE(j != nullptr);
		CHECK(j.size() == 2);
		CHECK(j["comm_size"] == 2);

		CHECK(j["timings"].is_array());
		CHECK(j["timings"].size() == 4);

		CHECK(j["timings"][0]["rank"] == 0);
		CHECK(j["timings"][0]["min"].is_number());
		CHECK(j["timings"][0]["max"].is_number());
		CHECK(j["timings"][0]["sum"].is_number());
		CHECK(j["timings"][0]["num_calls"].is_number());
		CHECK(j["timings"][0]["name"] == "A");

		CHECK(j["timings"][0]["rank"] == 0);
		CHECK(j["timings"][1]["min"].is_number());
		CHECK(j["timings"][1]["max"].is_number());
		CHECK(j["timings"][1]["sum"].is_number());
		CHECK(j["timings"][1]["num_calls"].is_number());
		CHECK(j["timings"][1]["name"] == "B");

		CHECK(j["timings"][2]["rank"] == 1);
		CHECK(j["timings"][2]["min"].is_number());
		CHECK(j["timings"][2]["max"].is_number());
		CHECK(j["timings"][2]["sum"].is_number());
		CHECK(j["timings"][2]["num_calls"].is_number());
		CHECK(j["timings"][2]["name"] == "A");

		CHECK(j["timings"][3]["rank"] == 1);
		CHECK(j["timings"][3]["min"].is_number());
		CHECK(j["timings"][3]["max"].is_number());
		CHECK(j["timings"][3]["sum"].is_number());
		CHECK(j["timings"][3]["num_calls"].is_number());
		CHECK(j["timings"][3]["name"] == "B");
	} else {
		CHECK(j == nullptr);
	}
}
TEST_CASE("Timer to_json nested timing", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.start("A");
	timer.start("B");
	timer.stop("B");
	timer.stop("A");
	const nlohmann::json j = timer;
	INFO(j.dump(4));
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		REQUIRE(j != nullptr);
		CHECK(j.size() == 2);
		CHECK(j["comm_size"] == 2);

		CHECK(j["timings"].is_array());
		CHECK(j["timings"].size() == 2);

		CHECK(j["timings"][0]["rank"] == 0);
		CHECK(j["timings"][0]["min"].is_number());
		CHECK(j["timings"][0]["max"].is_number());
		CHECK(j["timings"][0]["sum"].is_number());
		CHECK(j["timings"][0]["num_calls"].is_number());
		CHECK(j["timings"][0]["name"] == "A");
		CHECK(j["timings"][0]["timings"].is_array());
		CHECK(j["timings"][0]["timings"].size() == 1);

		CHECK(j["timings"][1]["rank"] == 1);
		CHECK(j["timings"][1]["min"].is_number());
		CHECK(j["timings"][1]["max"].is_number());
		CHECK(j["timings"][1]["sum"].is_number());
		CHECK(j["timings"][1]["num_calls"].is_number());
		CHECK(j["timings"][1]["name"] == "A");
		CHECK(j["timings"][1]["timings"].is_array());
		CHECK(j["timings"][1]["timings"].size() == 1);
	} else {
		CHECK(j == nullptr);
	}
}
TEST_CASE("Timer to_json domain timing", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.stopDomainTiming(0, "A");
	const nlohmann::json j = timer;
	INFO(j.dump(4));
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		REQUIRE(j != nullptr);
		CHECK(j.size() == 3);
		CHECK(j["comm_size"] == 2);

		CHECK(j["domains"].is_array());
		CHECK(j["domains"].size() == 1);
		CHECK(j["domains"][0].is_array());
		CHECK(j["domains"][0].size() == 2);
		CHECK(j["domains"][0][1].size() == 2);
		CHECK(j["domains"][0][1].is_array());
		CHECK(j["domains"][0][1].size() == 2);
		CHECK(j["domains"][0][1][0]["rank"] == 0);
		CHECK(j["domains"][0][1][1]["rank"] == 1);

		CHECK(j["timings"].is_array());
		CHECK(j["timings"].size() == 2);

		CHECK(j["timings"][0]["rank"] == 0);
		CHECK(j["timings"][0]["min"].is_number());
		CHECK(j["timings"][0]["max"].is_number());
		CHECK(j["timings"][0]["sum"].is_number());
		CHECK(j["timings"][0]["num_calls"].is_number());
		CHECK(j["timings"][0]["name"] == "A");
		CHECK(j["timings"][0]["domain_id"] == 0);

		CHECK(j["timings"][1]["rank"] == 1);
		CHECK(j["timings"][1]["min"].is_number());
		CHECK(j["timings"][1]["max"].is_number());
		CHECK(j["timings"][1]["sum"].is_number());
		CHECK(j["timings"][1]["num_calls"].is_number());
		CHECK(j["timings"][1]["name"] == "A");
		CHECK(j["timings"][1]["domain_id"] == 0);
	} else {
		REQUIRE(j == nullptr);
	}
}
TEST_CASE("Timer to_json domain timing only on rank 0", "[Timer]")
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	if (rank == 0) {
		timer.startDomainTiming(0, "A");
		timer.stopDomainTiming(0, "A");
	}
	const nlohmann::json j = timer;
	INFO(j.dump(4));
	if (rank == 0) {
		REQUIRE(j != nullptr);
		CHECK(j.size() == 3);
		CHECK(j["comm_size"] == 2);

		CHECK(j["domains"].is_array());
		CHECK(j["domains"].size() == 1);
		CHECK(j["domains"][0].is_array());
		CHECK(j["domains"][0].size() == 2);
		CHECK(j["domains"][0][1].size() == 2);
		CHECK(j["domains"][0][1].is_array());
		CHECK(j["domains"][0][1].size() == 2);
		CHECK(j["domains"][0][1][0]["rank"] == 0);
		CHECK(j["domains"][0][1][1]["rank"] == 1);

		CHECK(j["timings"].is_array());
		CHECK(j["timings"].size() == 1);

		CHECK(j["timings"][0]["rank"] == 0);
		CHECK(j["timings"][0]["min"].is_number());
		CHECK(j["timings"][0]["max"].is_number());
		CHECK(j["timings"][0]["sum"].is_number());
		CHECK(j["timings"][0]["num_calls"].is_number());
		CHECK(j["timings"][0]["name"] == "A");
		CHECK(j["timings"][0]["domain_id"] == 0);
	} else {
		REQUIRE(j == nullptr);
	}
}
TEST_CASE("Timer to_json domain timing only on rank 1", "[Timer]")
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	if (rank == 1) {
		timer.startDomainTiming(0, "A");
		timer.stopDomainTiming(0, "A");
	}
	const nlohmann::json j = timer;
	INFO(j.dump(4));
	if (rank == 0) {
		REQUIRE(j != nullptr);
		CHECK(j.size() == 3);
		CHECK(j["comm_size"] == 2);

		CHECK(j["domains"].is_array());
		CHECK(j["domains"].size() == 1);
		CHECK(j["domains"][0].is_array());
		CHECK(j["domains"][0].size() == 2);
		CHECK(j["domains"][0][1].size() == 2);
		CHECK(j["domains"][0][1].is_array());
		CHECK(j["domains"][0][1].size() == 2);
		CHECK(j["domains"][0][1][0]["rank"] == 0);
		CHECK(j["domains"][0][1][1]["rank"] == 1);

		CHECK(j["timings"].is_array());
		CHECK(j["timings"].size() == 1);

		CHECK(j["timings"][0]["rank"] == 1);
		CHECK(j["timings"][0]["min"].is_number());
		CHECK(j["timings"][0]["max"].is_number());
		CHECK(j["timings"][0]["sum"].is_number());
		CHECK(j["timings"][0]["num_calls"].is_number());
		CHECK(j["timings"][0]["name"] == "A");
		CHECK(j["timings"][0]["domain_id"] == 0);
	} else {
		REQUIRE(j == nullptr);
	}
}
TEST_CASE("Timer ostream empty timing", "[Timer]")
{
	Timer        timer(MPI_COMM_WORLD);
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(occurrences(s, "No timings to report") == 1);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE("Timer ostream unassociated timing", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.start("A");
	timer.stop("A");
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(occurrences(s, "A") == 1);
		CHECK(occurrences(s, "time (sec)") == 0);
		CHECK(occurrences(s, "average (sec)") == 1);
		CHECK(occurrences(s, "min (sec)") == 1);
		CHECK(occurrences(s, "max (sec)") == 1);
		CHECK(occurrences(s, "average calls per rank") == 1);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE("Timer ostream nested unassociated timing", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.start("A");
	timer.start("B");
	timer.stop("B");
	timer.stop("A");
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(occurrences(s, "A") == 2);
		CHECK(occurrences(s, "B") == 1);
		CHECK(occurrences(s, "time (sec)") == 0);
		CHECK(occurrences(s, "average (sec)") == 2);
		CHECK(occurrences(s, "min (sec)") == 2);
		CHECK(occurrences(s, "max (sec)") == 2);
		CHECK(occurrences(s, "average calls per rank") == 2);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE("Timer ostream sequential unassociated timing", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.start("A");
	timer.stop("A");
	timer.start("A");
	timer.stop("A");
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(occurrences(s, "A") == 1);
		CHECK(occurrences(s, "time (sec)") == 0);
		CHECK(occurrences(s, "average (sec)") == 1);
		CHECK(occurrences(s, "min (sec)") == 1);
		CHECK(occurrences(s, "max (sec)") == 1);
		CHECK(occurrences(s, "average calls per rank") == 1);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE("Timer ostream domain timing", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.stopDomainTiming(0, "A");
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(occurrences(s, "A") == 1);
		CHECK(occurrences(s, "time (sec)") == 0);
		CHECK(occurrences(s, "average (sec)") == 1);
		CHECK(occurrences(s, "min (sec)") == 1);
		CHECK(occurrences(s, "max (sec)") == 1);
		CHECK(occurrences(s, "average calls per rank") == 1);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE("Timer ostream domain timing two different domains sequential", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.addDomain(1, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.stopDomainTiming(0, "A");
	timer.startDomainTiming(1, "A");
	timer.stopDomainTiming(1, "A");
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(occurrences(s, "A") == 1);
		CHECK(occurrences(s, "time (sec)") == 0);
		CHECK(occurrences(s, "average (sec)") == 1);
		CHECK(occurrences(s, "min (sec)") == 1);
		CHECK(occurrences(s, "max (sec)") == 1);
		CHECK(occurrences(s, "average calls per rank") == 1);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE(
"Timer ostream domain timing two different domains sequential rank 0 has unrelated timing",
"[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.addDomain(1, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.stopDomainTiming(0, "A");
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		timer.startDomainTiming(0, "B");
		timer.stopDomainTiming(0, "B");
	}
	timer.startDomainTiming(1, "A");
	timer.stopDomainTiming(1, "A");
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	if (rank == 0) {
		CHECK(occurrences(s, "A") == 1);
		CHECK(occurrences(s, "B") == 1);
		CHECK(occurrences(s, "time (sec)") == 1);
		CHECK(occurrences(s, "average (sec)") == 1);
		CHECK(occurrences(s, "min (sec)") == 1);
		CHECK(occurrences(s, "max (sec)") == 1);
		CHECK(occurrences(s, "average calls per rank") == 1);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE("Timer ostream domain timing two different domains sequential nested", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.addDomain(1, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.startDomainTiming(0, "B");
	timer.stopDomainTiming(0, "B");
	timer.stopDomainTiming(0, "A");
	timer.startDomainTiming(1, "A");
	timer.startDomainTiming(1, "B");
	timer.stopDomainTiming(1, "B");
	timer.stopDomainTiming(1, "A");
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(occurrences(s, "A") == 2);
		CHECK(occurrences(s, "B") == 1);
		CHECK(occurrences(s, "time (sec)") == 0);
		CHECK(occurrences(s, "average (sec)") == 2);
		CHECK(occurrences(s, "min (sec)") == 2);
		CHECK(occurrences(s, "max (sec)") == 2);
		CHECK(occurrences(s, "average calls per rank") == 2);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE(
"Timer ostream domain timing two different domains sequential nested first domain has extra timing",
"[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.addDomain(1, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.startDomainTiming(0, "C");
	timer.stopDomainTiming(0, "C");
	timer.startDomainTiming(0, "B");
	timer.stopDomainTiming(0, "B");
	timer.stopDomainTiming(0, "A");
	timer.startDomainTiming(1, "A");
	timer.startDomainTiming(1, "B");
	timer.stopDomainTiming(1, "B");
	timer.stopDomainTiming(1, "A");
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(occurrences(s, "A") == 3);
		CHECK(occurrences(s, "B") == 1);
		CHECK(occurrences(s, "C") == 1);
		CHECK(occurrences(s, "time (sec)") == 0);
		CHECK(occurrences(s, "average (sec)") == 3);
		CHECK(occurrences(s, "min (sec)") == 3);
		CHECK(occurrences(s, "max (sec)") == 3);
		CHECK(occurrences(s, "average calls per rank") == 3);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE(
"Timer ostream domain timing two different domains sequential nested second domain has extra timing",
"[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.addDomain(1, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.startDomainTiming(0, "B");
	timer.stopDomainTiming(0, "B");
	timer.stopDomainTiming(0, "A");
	timer.startDomainTiming(1, "A");
	timer.startDomainTiming(0, "C");
	timer.stopDomainTiming(0, "C");
	timer.startDomainTiming(1, "B");
	timer.stopDomainTiming(1, "B");
	timer.stopDomainTiming(1, "A");
	stringstream ss;
	ss << timer;
	std::string s = ss.str();
	INFO(s);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(occurrences(s, "A") == 3);
		CHECK(occurrences(s, "B") == 1);
		CHECK(occurrences(s, "C") == 1);
		CHECK(occurrences(s, "time (sec)") == 0);
		CHECK(occurrences(s, "average (sec)") == 3);
		CHECK(occurrences(s, "min (sec)") == 3);
		CHECK(occurrences(s, "max (sec)") == 3);
		CHECK(occurrences(s, "average calls per rank") == 3);
	} else {
		CHECK(s.size() == 0);
	}
}
TEST_CASE("Timer saveToFile new empty file", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.addDomain(1, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.startDomainTiming(0, "B");
	timer.stopDomainTiming(0, "B");
	timer.stopDomainTiming(0, "A");
	timer.startDomainTiming(1, "A");
	timer.startDomainTiming(0, "C");
	timer.stopDomainTiming(0, "C");
	timer.startDomainTiming(1, "B");
	timer.stopDomainTiming(1, "B");
	timer.stopDomainTiming(1, "A");

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		std::remove("timerne2.json");
	}
	timer.saveToFile("timerne2.json");
	nlohmann::json j = timer;

	if (rank == 0) {
		ifstream       input("timerne2.json");
		nlohmann::json file_j;
		input >> file_j;
		nlohmann::json extra_j;
		CHECK_THROWS(input >> extra_j);
		CHECK(file_j.dump() == j.dump());
		input.close();
		std::remove("timerne2.json");
	}
}
TEST_CASE("Timer saveToFile overwrites file", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.addDomain(1, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.startDomainTiming(0, "B");
	timer.stopDomainTiming(0, "B");
	timer.stopDomainTiming(0, "A");
	timer.startDomainTiming(1, "A");
	timer.startDomainTiming(0, "C");
	timer.stopDomainTiming(0, "C");
	timer.startDomainTiming(1, "B");
	timer.stopDomainTiming(1, "B");
	timer.stopDomainTiming(1, "A");

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		std::remove("timerow2.json");
	}
	timer.saveToFile("timerow2.json");
	timer.saveToFile("timerow2.json");
	nlohmann::json j = timer;

	if (rank == 0) {
		ifstream       input("timerow2.json");
		nlohmann::json file_j;
		input >> file_j;
		nlohmann::json extra_j;
		CHECK_THROWS(input >> extra_j);
		CHECK(file_j.dump() == j.dump());
		input.close();
		std::remove("timerow2.json");
	}
}
TEST_CASE("Timer saveToFile throws with nonexistant directory", "[Timer]")
{
	Timer timer(MPI_COMM_WORLD);
	timer.addDomain(0, GetDomain());
	timer.addDomain(1, GetDomain());
	timer.startDomainTiming(0, "A");
	timer.startDomainTiming(0, "B");
	timer.stopDomainTiming(0, "B");
	timer.stopDomainTiming(0, "A");
	timer.startDomainTiming(1, "A");
	timer.startDomainTiming(0, "C");
	timer.stopDomainTiming(0, "C");
	timer.startDomainTiming(1, "B");
	timer.stopDomainTiming(1, "B");
	timer.stopDomainTiming(1, "A");

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK_THROWS_AS(timer.saveToFile("surely/this/directory/does/not/exist/timer.json"),
		                RuntimeError);
	} else {
		timer.saveToFile("surely/this/directory/does/not/exist/timer.json");
	}
}