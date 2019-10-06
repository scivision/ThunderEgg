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

#ifndef QUADINTERPOLATOR_H
#define QUADINTERPOLATOR_H
#include <Thunderegg/IfaceInterp.h>
namespace Thunderegg
{
/**
 * @brief Quadradic interpolation for interfaces
 */
class QuadInterpolator : public IfaceInterp<2>
{
	public:
	/**
	 * @brief
	 *
	 * @param pinfo
	 * @param u the rhs vector
	 * @param interp
	 */
	void interpolate(PatchInfo<2> &pinfo, const Vec u, Vec interp);
	void interpolate(PatchInfo<2> &pinfo, Side<2> s, InterpCase icase, const Vec u, Vec interp);
};
} // namespace Thunderegg
#endif