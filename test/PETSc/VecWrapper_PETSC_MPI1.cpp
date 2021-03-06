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
#include <ThunderEgg/PETSc/VecWrapper.h>
using namespace std;
using namespace ThunderEgg;
#define MESHES                                                                                     \
	"mesh_inputs/2d_uniform_2x2_mpi1.json", "mesh_inputs/2d_uniform_8x8_refined_cross_mpi1.json"
const string mesh_file = "mesh_inputs/2d_uniform_4x4_mpi1.json";
TEST_CASE("PETSc::VecWrapper<1> getNumGhostCells", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	array<int, 2> ns                = {nx, ny};
	int           num_local_patches = GENERATE(1, 13);
	int           size
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<2>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumGhostCells() == num_ghost_cells);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<1> getMPIComm", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	array<int, 1> ns                = {nx};
	int           num_local_patches = GENERATE(1, 13);
	int           size = (nx + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<1>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getMPIComm() == MPI_COMM_WORLD);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<1> getNumLocalPatches", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	array<int, 1> ns                = {nx};
	int           num_local_patches = GENERATE(1, 13);
	int           size = (nx + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<1>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumLocalPatches() == num_local_patches);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<1> getNumComponents", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	array<int, 1> ns                = {nx};
	int           num_local_patches = GENERATE(1, 13);
	int           size = (nx + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<1>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumComponents() == num_components);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<1> getVec", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	array<int, 1> ns                = {nx};
	int           num_local_patches = GENERATE(1, 13);
	int           size = (nx + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<1>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getVec() == vec);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<1> getNumLocalCells", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	array<int, 1> ns                = {nx};
	int           num_local_patches = GENERATE(1, 13);
	int           size = (nx + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<1>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumLocalCells() == nx * num_local_patches);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<2> getNumGhostCells", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	array<int, 2> ns                = {nx, ny};
	int           num_local_patches = GENERATE(1, 13);
	int           size
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<2>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumGhostCells() == num_ghost_cells);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<2> getMPIComm", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	array<int, 2> ns                = {nx, ny};
	int           num_local_patches = GENERATE(1, 13);
	int           size
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<2>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getMPIComm() == MPI_COMM_WORLD);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<2> getNumLocalPatches", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	array<int, 2> ns                = {nx, ny};
	int           num_local_patches = GENERATE(1, 13);
	int           size
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<2>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumLocalPatches() == num_local_patches);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<2> getNumComponents", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	array<int, 2> ns                = {nx, ny};
	int           num_local_patches = GENERATE(1, 13);
	int           size
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<2>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumComponents() == num_components);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<2> getVec", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	array<int, 2> ns                = {nx, ny};
	int           num_local_patches = GENERATE(1, 13);
	int           size
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<2>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getVec() == vec);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<2> getNumLocalCells", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	array<int, 2> ns                = {nx, ny};
	int           num_local_patches = GENERATE(1, 13);
	int           size
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<2>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumLocalCells() == nx * ny * num_local_patches);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<3> getNumGhostCells", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	int           nz                = GENERATE(1, 4, 5);
	array<int, 3> ns                = {nx, ny, nz};
	int           num_local_patches = GENERATE(1, 13);
	int size = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells)
	           * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("nz:                " << nz);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<3>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumGhostCells() == num_ghost_cells);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<3> getMPIComm", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	int           nz                = GENERATE(1, 4, 5);
	array<int, 3> ns                = {nx, ny, nz};
	int           num_local_patches = GENERATE(1, 13);
	int size = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells)
	           * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("nz:                " << nz);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<3>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getMPIComm() == MPI_COMM_WORLD);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<3> getNumLocalPatches", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	int           nz                = GENERATE(1, 4, 5);
	array<int, 3> ns                = {nx, ny, nz};
	int           num_local_patches = GENERATE(1, 13);
	int size = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells)
	           * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("nz:                " << nz);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<3>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumLocalPatches() == num_local_patches);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<3> getNumComponents", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	int           nz                = GENERATE(1, 4, 5);
	array<int, 3> ns                = {nx, ny, nz};
	int           num_local_patches = GENERATE(1, 13);
	int size = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells)
	           * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("nz:                " << nz);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<3>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumComponents() == num_components);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<3> getVec", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	int           nz                = GENERATE(1, 4, 5);
	array<int, 3> ns                = {nx, ny, nz};
	int           num_local_patches = GENERATE(1, 13);
	int size = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells)
	           * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("nz:                " << nz);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<3>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getVec() == vec);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<3> getNumLocalCells", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	int           nz                = GENERATE(1, 4, 5);
	array<int, 3> ns                = {nx, ny, nz};
	int           num_local_patches = GENERATE(1, 13);
	int size = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells)
	           * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("nz:                " << nz);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<3>>(vec, ns, num_components, num_ghost_cells, false);

	CHECK(vec_wrapper->getNumLocalCells() == nx * ny * nz * num_local_patches);

	VecDestroy(&vec);
}

TEST_CASE("PETSc::VecWrapper<1> getLocalData", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	array<int, 1> ns                = {nx};
	int           num_local_patches = GENERATE(1, 13);
	int           component_stride  = (nx + 2 * num_ghost_cells);
	int           patch_stride      = component_stride * num_components;
	int           size = (nx + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<1>>(vec, ns, num_components, num_ghost_cells, false);

	double *view;
	VecGetArray(vec, &view);
	for (int i = 0; i < num_local_patches; i++) {
		INFO("i:                 " << i);
		for (int c = 0; c < num_components; c++) {
			INFO("c:                 " << c);
			LocalData<1> ld = vec_wrapper->getLocalData(c, i);
			CHECK(&ld[ld.getGhostStart()] == view + patch_stride * i + c * component_stride);
			CHECK(&ld[ld.getGhostEnd()]
			      == view + patch_stride * i + (c + 1) * component_stride - 1);
		}
	}
	VecRestoreArray(vec, &view);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<1> getLocalData const", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	array<int, 1> ns                = {nx};
	int           num_local_patches = GENERATE(1, 13);
	int           component_stride  = (nx + 2 * num_ghost_cells);
	int           patch_stride      = component_stride * num_components;
	int           size = (nx + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<1>>(vec, ns, num_components, num_ghost_cells, false);
	auto const_vec_wrapper = std::const_pointer_cast<const PETSc::VecWrapper<1>>(vec_wrapper);

	double *view;
	VecGetArray(vec, &view);
	for (int i = 0; i < num_local_patches; i++) {
		INFO("i:                 " << i);
		for (int c = 0; c < num_components; c++) {
			INFO("c:                 " << c);
			const LocalData<1> ld = const_vec_wrapper->getLocalData(c, i);
			CHECK(&ld[ld.getGhostStart()] == view + patch_stride * i + c * component_stride);
			CHECK(&ld[ld.getGhostEnd()]
			      == view + patch_stride * i + (c + 1) * component_stride - 1);
		}
	}
	VecRestoreArray(vec, &view);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<2> getLocalData", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	array<int, 2> ns                = {nx, ny};
	int           num_local_patches = GENERATE(1, 13);
	int           component_stride  = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells);
	int           patch_stride      = component_stride * num_components;
	int           size
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<2>>(vec, ns, num_components, num_ghost_cells, false);

	double *view;
	VecGetArray(vec, &view);
	for (int i = 0; i < num_local_patches; i++) {
		INFO("i:                 " << i);
		for (int c = 0; c < num_components; c++) {
			INFO("c:                 " << c);
			LocalData<2> ld = vec_wrapper->getLocalData(c, i);
			CHECK(&ld[ld.getGhostStart()] == view + patch_stride * i + c * component_stride);
			CHECK(&ld[ld.getGhostEnd()]
			      == view + patch_stride * i + (c + 1) * component_stride - 1);
		}
	}
	VecRestoreArray(vec, &view);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<2> getLocalData const", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	array<int, 2> ns                = {nx, ny};
	int           num_local_patches = GENERATE(1, 13);
	int           component_stride  = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells);
	int           patch_stride      = component_stride * num_components;
	int           size
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<2>>(vec, ns, num_components, num_ghost_cells, false);
	auto const_vec_wrapper = std::const_pointer_cast<const PETSc::VecWrapper<2>>(vec_wrapper);

	double *view;
	VecGetArray(vec, &view);
	for (int i = 0; i < num_local_patches; i++) {
		INFO("i:                 " << i);
		for (int c = 0; c < num_components; c++) {
			INFO("c:                 " << c);
			const LocalData<2> ld = const_vec_wrapper->getLocalData(c, i);
			CHECK(&ld[ld.getGhostStart()] == view + patch_stride * i + c * component_stride);
			CHECK(&ld[ld.getGhostEnd()]
			      == view + patch_stride * i + (c + 1) * component_stride - 1);
		}
	}
	VecRestoreArray(vec, &view);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<3> getLocalData", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	int           nz                = GENERATE(1, 4, 5);
	array<int, 3> ns                = {nx, ny, nz};
	int           num_local_patches = GENERATE(1, 13);
	int           component_stride
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells);
	int patch_stride = component_stride * num_components;
	int size = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells)
	           * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("nz:                " << nz);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<3>>(vec, ns, num_components, num_ghost_cells, false);

	double *view;
	VecGetArray(vec, &view);
	for (int i = 0; i < num_local_patches; i++) {
		INFO("i:                 " << i);
		for (int c = 0; c < num_components; c++) {
			INFO("c:                 " << c);
			LocalData<3> ld = vec_wrapper->getLocalData(c, i);
			CHECK(&ld[ld.getGhostStart()] == view + patch_stride * i + c * component_stride);
			CHECK(&ld[ld.getGhostEnd()]
			      == view + patch_stride * i + (c + 1) * component_stride - 1);
		}
	}
	VecRestoreArray(vec, &view);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper<3> getLocalData const", "[PETSc::VecWrapper]")
{
	int           num_components    = GENERATE(1, 2, 3);
	auto          num_ghost_cells   = GENERATE(0, 1, 5);
	int           nx                = GENERATE(1, 4, 5);
	int           ny                = GENERATE(1, 4, 5);
	int           nz                = GENERATE(1, 4, 5);
	array<int, 3> ns                = {nx, ny, nz};
	int           num_local_patches = GENERATE(1, 13);
	int           component_stride
	= (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells);
	int patch_stride = component_stride * num_components;
	int size = (nx + 2 * num_ghost_cells) * (ny + 2 * num_ghost_cells) * (nz + 2 * num_ghost_cells)
	           * num_local_patches * num_components;

	INFO("num_ghost_cells:   " << num_ghost_cells);
	INFO("nx:                " << nx);
	INFO("ny:                " << ny);
	INFO("nz:                " << nz);
	INFO("num_local_patches: " << num_local_patches);

	Vec vec;
	VecCreateMPI(MPI_COMM_WORLD, size, PETSC_DETERMINE, &vec);

	auto vec_wrapper
	= make_shared<PETSc::VecWrapper<3>>(vec, ns, num_components, num_ghost_cells, false);
	auto const_vec_wrapper = std::const_pointer_cast<const PETSc::VecWrapper<3>>(vec_wrapper);

	double *view;
	VecGetArray(vec, &view);
	for (int i = 0; i < num_local_patches; i++) {
		INFO("i:                 " << i);
		for (int c = 0; c < num_components; c++) {
			INFO("c:                 " << c);
			const LocalData<3> ld = const_vec_wrapper->getLocalData(c, i);
			CHECK(&ld[ld.getGhostStart()] == view + patch_stride * i + c * component_stride);
			CHECK(&ld[ld.getGhostEnd()]
			      == view + patch_stride * i + (c + 1) * component_stride - 1);
		}
	}
	VecRestoreArray(vec, &view);

	VecDestroy(&vec);
}
TEST_CASE("PETSc::VecWrapper getNewVector works", "[PETSc::VecWrapper]")
{
	int  num_components = GENERATE(1, 2, 3);
	auto mesh_file      = GENERATE(as<std::string>{}, MESHES);
	INFO("MESH FILE " << mesh_file);
	int nx = GENERATE(1, 4, 5);
	int ny = GENERATE(1, 4, 5);
	INFO("nx:       " << nx);
	INFO("ny:       " << ny);
	int                   num_ghost = GENERATE(0, 1, 2);
	DomainReader<2>       domain_reader(mesh_file, {nx, ny}, num_ghost);
	shared_ptr<Domain<2>> d_fine = domain_reader.getFinerDomain();

	auto vec_wrapper = PETSc::VecWrapper<2>::GetNewVector(d_fine, num_components);

	CHECK(vec_wrapper->getNumComponents() == num_components);
	CHECK(vec_wrapper->getNumGhostCells() == num_ghost);
	CHECK(vec_wrapper->getNumLocalCells() == d_fine->getNumLocalCells());
	CHECK(vec_wrapper->getNumLocalPatches() == d_fine->getNumLocalPatches());
	CHECK(vec_wrapper->getMPIComm() == MPI_COMM_WORLD);
	CHECK(vec_wrapper->getLocalData(0, 0).getLengths()[0] == nx);
	CHECK(vec_wrapper->getLocalData(0, 0).getLengths()[1] == ny);
	int size
	= (nx + 2 * num_ghost) * (ny + 2 * num_ghost) * d_fine->getNumLocalPatches() * num_components;
	int vec_size;
	VecGetSize(vec_wrapper->getVec(), &vec_size);
	CHECK(vec_size == size);
}