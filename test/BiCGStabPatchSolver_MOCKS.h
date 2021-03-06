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

#include <ThunderEgg/GhostFiller.h>
#include <ThunderEgg/PatchOperator.h>
#include <set>

#include "catch.hpp"

namespace ThunderEgg
{
namespace
{
template <int D> class MockGhostFiller : public GhostFiller<D>
{
	private:
	mutable int num_calls = 0;

	public:
	void fillGhost(std::shared_ptr<const Vector<D>> u) const override
	{
		num_calls++;
	}
	int numCalls()
	{
		return num_calls;
	}
};
template <int D> class MockPatchOperator : public PatchOperator<D>
{
	private:
	mutable bool rhs_was_modified   = false;
	mutable bool interior_dirichlet = false;

	public:
	MockPatchOperator(std::shared_ptr<const Domain<D>>      domain,
	                  std::shared_ptr<const GhostFiller<D>> ghost_filler)
	: PatchOperator<D>(domain, ghost_filler)
	{
	}
	void applySinglePatch(std::shared_ptr<const PatchInfo<D>> pinfo,
	                      const std::vector<LocalData<D>> &us, std::vector<LocalData<D>> &fs,
	                      bool treat_interior_boundary_as_dirichlet) const override
	{
		interior_dirichlet |= treat_interior_boundary_as_dirichlet;
		for (size_t c = 0; c < fs.size(); c++) {
			nested_loop<D>(fs[c].getStart(), fs[c].getEnd(), [&](const std::array<int, D> &coord) {
				fs[c][coord] = us[c][coord] / 2;
			});
		}
	}
	void addGhostToRHS(std::shared_ptr<const PatchInfo<D>> pinfo,
	                   const std::vector<LocalData<D>> &   us,
	                   std::vector<LocalData<D>> &         fs) const override
	{
		rhs_was_modified = true;
	}
	bool rhsWasModified()
	{
		return rhs_was_modified;
	}
	bool interiorDirichlet()
	{
		return interior_dirichlet;
	}
};
template <int D> class NonLinMockPatchOperator : public PatchOperator<D>
{
	private:
	mutable bool rhs_was_modified   = false;
	mutable bool interior_dirichlet = false;

	public:
	NonLinMockPatchOperator(std::shared_ptr<const Domain<D>>      domain,
	                        std::shared_ptr<const GhostFiller<D>> ghost_filler)
	: PatchOperator<D>(domain, ghost_filler)
	{
	}
	void applySinglePatch(std::shared_ptr<const PatchInfo<D>> pinfo,
	                      const std::vector<LocalData<D>> &us, std::vector<LocalData<D>> &fs,
	                      bool treat_interior_boundary_as_dirichlet) const override
	{
		interior_dirichlet |= treat_interior_boundary_as_dirichlet;
		for (size_t c = 0; c < fs.size(); c++) {
			nested_loop<D>(fs[c].getStart(), fs[c].getEnd(),
			               [&](const std::array<int, D> &coord) { fs[c][coord] += 1; });
		}
	}
	void addGhostToRHS(std::shared_ptr<const PatchInfo<D>> pinfo,
	                   const std::vector<LocalData<D>> &   us,
	                   std::vector<LocalData<D>> &         fs) const override
	{
		rhs_was_modified = true;
	}
	bool rhsWasModified()
	{
		return rhs_was_modified;
	}
	bool interiorDirichlet()
	{
		return interior_dirichlet;
	}
};
} // namespace
} // namespace ThunderEgg