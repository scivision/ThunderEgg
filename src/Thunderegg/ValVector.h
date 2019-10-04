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

#ifndef VALVECTOR_H
#define VALVECTOR_H
#include <Thunderegg/Vector.h>
#include <valarray>
namespace Thunderegg
{
template <size_t D> class ValVector : public Vector<D>

{
	private:
	int                patch_stride;
	std::array<int, D> lengths;
	std::array<int, D> strides;
	int                num_ghost_cells;
	int                first_offset;

	public:
	std::valarray<double> vec;
	std::valarray<double> ghost_data;
	ValVector() = default;
	ValVector(const std::array<int, D> &lengths, int num_ghost_cells, int num_patches)
	{
		this->num_local_patches = num_patches;
		this->num_ghost_cells   = num_ghost_cells;
		int size                = 1;
		this->lengths           = lengths;
		first_offset            = 0;
		for (size_t i = 0; i < D; i++) {
			this->lengths[i] += 2 * num_ghost_cells;
			strides[i] = size;
			size *= lengths[i];
			first_offset += strides[i] * num_ghost_cells;
		}
		patch_stride = size;
		size *= num_patches;
		vec.resize(size);
	}
	~ValVector() = default;
	LocalData<D> getLocalData(int local_patch_id)
	{
		double *data = &vec[patch_stride * local_patch_id + first_offset];
		return LocalData<D>(data, strides, lengths, num_ghost_cells, nullptr);
	}
	const LocalData<D> getLocalData(int local_patch_id) const
	{
		double *data = const_cast<double *>(&vec[patch_stride * local_patch_id + first_offset]);
		return LocalData<D>(data, strides, lengths, num_ghost_cells, nullptr);
	}
	void setNumGhostPatches(int num_ghost_patches)
	{
		ghost_data.resize(patch_stride * num_ghost_patches);
	}
};
} // namespace Thunderegg
#endif