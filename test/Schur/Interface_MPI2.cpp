/***************************************************************************
 *  ThunderEgg, a library for solving Poisson's equation on adaptively
 *  refined block-structured Cartesian grids
 *
 *  Copyright (C) 2019  ThunderEgg Developers. See AUTHORS.md file at the
 *  top-level directory.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "../utils/DomainReader.h"
#include "catch.hpp"
#include <ThunderEgg/Schur/Interface.h>
#include <algorithm>
using namespace std;
using namespace ThunderEgg;
TEST_CASE(
"Schur::Interface enumerateIfacesFromPiinfoVector refined interface on processor boundary",
"[Schur::Interface]")
{
	DomainReader<2> domain_reader("mesh_inputs/2d_refined_east_1x2_east_on_1_mpi2.json", {10, 10},
	                              0);
	auto            domain = domain_reader.getFinerDomain();
	vector<shared_ptr<const Schur::PatchIfaceInfo<2>>> piinfos;
	for (auto &patch : domain->getPatchInfoVector()) {
		piinfos.push_back(make_shared<Schur::PatchIfaceInfo<2>>(patch));
	}

	map<int, std::shared_ptr<Schur::Interface<2>>>    ifaces;
	vector<std::shared_ptr<Schur::PatchIfaceInfo<2>>> off_proc_piinfos;
	Schur::Interface<2>::EnumerateIfacesFromPiinfoVector(piinfos, ifaces, off_proc_piinfos);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(ifaces.size() == 1);
		auto coarse_piinfo = piinfos[0];

		// check coarse interface
		int  id    = coarse_piinfo->getFineIfaceInfo(Side<2>::east())->id;
		auto iface = ifaces.at(id);
		CHECK(iface->id == id);
		CHECK(iface->patches.size() == 3);

		Schur::Interface<2>::SideTypePiinfo *coarse_patch = nullptr;
		for (auto &patch : iface->patches) {
			if (patch.piinfo->pinfo->id == coarse_piinfo->pinfo->id) {
				coarse_patch = &patch;
				break;
			}
		}

		REQUIRE(coarse_patch != nullptr);
		CHECK(coarse_patch->type.isCoarseToCoarse());
		CHECK(coarse_patch->side == Side<2>::east());

		Schur::Interface<2>::SideTypePiinfo *ref_se_patch = nullptr;
		for (auto &patch : iface->patches) {
			if (patch.piinfo->pinfo->id
			    == coarse_piinfo->pinfo->getFineNbrInfo(Side<2>::east()).ids[0]) {
				ref_se_patch = &patch;
				break;
			}
		}

		REQUIRE(ref_se_patch != nullptr);
		CHECK(ref_se_patch->type.isFineToCoarse());
		CHECK(ref_se_patch->side == Side<2>::west());

		Schur::Interface<2>::SideTypePiinfo *ref_ne_patch = nullptr;
		for (auto &patch : iface->patches) {
			if (patch.piinfo->pinfo->id
			    == coarse_piinfo->pinfo->getFineNbrInfo(Side<2>::east()).ids[1]) {
				ref_ne_patch = &patch;
				break;
			}
		}

		REQUIRE(ref_ne_patch != nullptr);
		CHECK(ref_ne_patch->type.isFineToCoarse());
		CHECK(ref_ne_patch->side == Side<2>::west());

		REQUIRE(off_proc_piinfos.size() == 2);

		for (auto piinfo : off_proc_piinfos) {
			CHECK((piinfo->pinfo->id == ref_ne_patch->piinfo->pinfo->id
			       || piinfo->pinfo->id == ref_se_patch->piinfo->pinfo->id));
		}
	} else {
		CHECK(ifaces.size() == 6);
		std::shared_ptr<const Schur::PatchIfaceInfo<2>> ref_sw_piinfo;
		for (auto piinfo : piinfos) {
			double x = piinfo->pinfo->starts[0];
			double y = piinfo->pinfo->starts[1];
			if (x == Approx(1) && y == Approx(0)) {
				ref_sw_piinfo = piinfo;
				break;
			}
		}
		REQUIRE(ref_sw_piinfo != nullptr);
		std::shared_ptr<const Schur::PatchIfaceInfo<2>> ref_se_piinfo;
		for (auto piinfo : piinfos) {
			double x = piinfo->pinfo->starts[0];
			double y = piinfo->pinfo->starts[1];
			if (x == Approx(1.5) && y == Approx(0)) {
				ref_se_piinfo = piinfo;
				break;
			}
		}
		REQUIRE(ref_se_piinfo != nullptr);
		std::shared_ptr<const Schur::PatchIfaceInfo<2>> ref_nw_piinfo;
		for (auto piinfo : piinfos) {
			double x = piinfo->pinfo->starts[0];
			double y = piinfo->pinfo->starts[1];
			if (x == Approx(1) && y == Approx(.5)) {
				ref_nw_piinfo = piinfo;
				break;
			}
		}
		REQUIRE(ref_nw_piinfo != nullptr);
		std::shared_ptr<const Schur::PatchIfaceInfo<2>> ref_ne_piinfo;
		for (auto piinfo : piinfos) {
			double x = piinfo->pinfo->starts[0];
			double y = piinfo->pinfo->starts[1];
			if (x == Approx(1.5) && y == Approx(.5)) {
				ref_ne_piinfo = piinfo;
				break;
			}
		}
		REQUIRE(ref_ne_piinfo != nullptr);

		// check sw fine interface
		{
			int  id    = ref_sw_piinfo->getCoarseIfaceInfo(Side<2>::west())->id;
			auto iface = ifaces.at(id);
			CHECK(iface->id == id);
			CHECK(iface->patches.size() == 2);

			Schur::Interface<2>::SideTypePiinfo *coarse_patch = nullptr;
			for (auto &patch : iface->patches) {
				if (patch.piinfo->pinfo->id
				    == ref_sw_piinfo->pinfo->getCoarseNbrInfo(Side<2>::west()).id) {
					coarse_patch = &patch;
					break;
				}
			}

			REQUIRE(coarse_patch != nullptr);
			CHECK(coarse_patch->type.isCoarseToFine());
			CHECK(coarse_patch->side == Side<2>::east());

			Schur::Interface<2>::SideTypePiinfo *ref_se_patch = nullptr;
			for (auto &patch : iface->patches) {
				if (patch.piinfo->pinfo->id == ref_sw_piinfo->pinfo->id) {
					ref_se_patch = &patch;
					break;
				}
			}

			REQUIRE(ref_se_patch != nullptr);
			CHECK(ref_se_patch->type.isFineToFine());
			CHECK(ref_se_patch->side == Side<2>::west());
		}
		// check ne fine interface
		{
			int  id    = ref_nw_piinfo->getCoarseIfaceInfo(Side<2>::west())->id;
			auto iface = ifaces.at(id);
			CHECK(iface->id == id);
			CHECK(iface->patches.size() == 2);

			Schur::Interface<2>::SideTypePiinfo *coarse_patch = nullptr;
			for (auto &patch : iface->patches) {
				if (patch.piinfo->pinfo->id
				    == ref_nw_piinfo->pinfo->getCoarseNbrInfo(Side<2>::west()).id) {
					coarse_patch = &patch;
					break;
				}
			}

			REQUIRE(coarse_patch != nullptr);
			CHECK(coarse_patch->type.isCoarseToFine());
			CHECK(coarse_patch->side == Side<2>::east());

			Schur::Interface<2>::SideTypePiinfo *ref_ne_patch = nullptr;
			for (auto &patch : iface->patches) {
				if (patch.piinfo->pinfo->id == ref_nw_piinfo->pinfo->id) {
					ref_ne_patch = &patch;
					break;
				}
			}

			REQUIRE(ref_ne_patch != nullptr);
			CHECK(ref_ne_patch->type.isFineToFine());
			CHECK(ref_ne_patch->side == Side<2>::west());
		}
		// check interface between se and ne
		{
			int  id    = ref_se_piinfo->getNormalIfaceInfo(Side<2>::north())->id;
			auto iface = ifaces.at(id);
			CHECK(iface->id == id);
			CHECK(iface->patches.size() == 2);

			Schur::Interface<2>::SideTypePiinfo *ref_se_patch = nullptr;
			for (auto &patch : iface->patches) {
				if (patch.piinfo->pinfo->id == ref_se_piinfo->pinfo->id) {
					ref_se_patch = &patch;
					break;
				}
			}

			REQUIRE(ref_se_patch != nullptr);
			CHECK(ref_se_patch->type.isNormal());
			CHECK(ref_se_patch->side == Side<2>::north());

			Schur::Interface<2>::SideTypePiinfo *ref_ne_patch = nullptr;
			for (auto &patch : iface->patches) {
				if (patch.piinfo->pinfo->id == ref_ne_piinfo->pinfo->id) {
					ref_ne_patch = &patch;
					break;
				}
			}

			REQUIRE(ref_ne_patch != nullptr);
			CHECK(ref_ne_patch->type.isNormal());
			CHECK(ref_ne_patch->side == Side<2>::south());
		}

		REQUIRE(off_proc_piinfos.size() == 1);

		for (auto piinfo : off_proc_piinfos) {
			CHECK(piinfo->pinfo->id == ref_nw_piinfo->pinfo->getCoarseNbrInfo(Side<2>::west()).id);
		}
	}
}
TEST_CASE("Schur::Interface enumerateIfacesFromPiinfoVector normal interface on processor boundary",
          "[Schur::Interface]")
{
	DomainReader<2> domain_reader("mesh_inputs/2d_uniform_1x2_east_on_1_mpi2.json", {10, 10}, 0);
	auto            domain = domain_reader.getFinerDomain();
	vector<shared_ptr<const Schur::PatchIfaceInfo<2>>> piinfos;
	for (auto &patch : domain->getPatchInfoVector()) {
		piinfos.push_back(make_shared<Schur::PatchIfaceInfo<2>>(patch));
	}

	map<int, std::shared_ptr<Schur::Interface<2>>>    ifaces;
	vector<std::shared_ptr<Schur::PatchIfaceInfo<2>>> off_proc_piinfos;
	Schur::Interface<2>::EnumerateIfacesFromPiinfoVector(piinfos, ifaces, off_proc_piinfos);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		CHECK(ifaces.size() == 1);
		auto west_piinfo = piinfos[0];

		// check coarse interface
		int  id    = west_piinfo->getNormalIfaceInfo(Side<2>::east())->id;
		auto iface = ifaces.at(id);
		CHECK(iface->id == id);
		CHECK(iface->patches.size() == 2);

		Schur::Interface<2>::SideTypePiinfo *coarse_patch = nullptr;
		for (auto &patch : iface->patches) {
			if (patch.piinfo->pinfo->id == west_piinfo->pinfo->id) {
				coarse_patch = &patch;
				break;
			}
		}

		REQUIRE(coarse_patch != nullptr);
		CHECK(coarse_patch->type.isNormal());
		CHECK(coarse_patch->side == Side<2>::east());

		Schur::Interface<2>::SideTypePiinfo *east_patch = nullptr;
		for (auto &patch : iface->patches) {
			if (patch.piinfo->pinfo->id
			    == west_piinfo->pinfo->getNormalNbrInfo(Side<2>::east()).id) {
				east_patch = &patch;
				break;
			}
		}

		REQUIRE(east_patch != nullptr);
		CHECK(east_patch->type.isNormal());
		CHECK(east_patch->side == Side<2>::west());
		REQUIRE(off_proc_piinfos.size() == 1);
		CHECK(off_proc_piinfos[0]->pinfo->id
		      == west_piinfo->pinfo->getNormalNbrInfo(Side<2>::east()).id);
	} else {
		CHECK(ifaces.size() == 0);
		CHECK(off_proc_piinfos.size() == 0);
	}
}