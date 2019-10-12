/***************************************************************************
 *  Thunderegg, a library for solving Poisson's equation on adaptively
 *  refined block-structured Cartesian grids
 *
 *  Copyright (C) 2019  Thunderegg Developers. See AUTHORS.md file at the
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

#ifndef THUNDEREGG_PATCHOPERATOR_H
#define THUNDEREGG_PATCHOPERATOR_H
#include <Thunderegg/GMG/CycleFactoryCtx.h>
#include <Thunderegg/Operator.h>
#include <Thunderegg/Vector.h>
namespace Thunderegg
{
template <size_t D> class PatchOperator : public Operator<D>
{
	public:
	virtual ~PatchOperator() {}

	virtual void applySinglePatch(std::shared_ptr<const PatchInfo<D>> pinfo, const LocalData<D> u,
	                              LocalData<D> f) const                                        = 0;
	virtual std::shared_ptr<PatchOperator<D>> getNewPatchOperator(GMG::CycleFactoryCtx<D> ctx) = 0;
};
} // namespace Thunderegg
#endif