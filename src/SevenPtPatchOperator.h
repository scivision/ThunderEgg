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

#ifndef SEVENPTPATCHOPERATOR_H
#define SEVENPTPATCHOPERATOR_H
#include <PatchOperator.h>
class SevenPtPatchOperator : public PatchOperator<3>
{
	public:
	virtual void apply(SchurDomain<3> &d, std::shared_ptr<const Vector<3>> u,
	                   std::shared_ptr<const Vector<2>> gamma, std::shared_ptr<Vector<3>> f)
	{
		int    n   = d.n;
		double h_x = d.domain.lengths[0] / n;
		double h_y = d.domain.lengths[1] / n;
		double h_z = d.domain.lengths[2] / n;

		LocalData<3>       f_data = f->getLocalData(d.domain.id_local);
		const LocalData<3> u_data = u->getLocalData(d.domain.id_local);

		double center, north, east, south, west, bottom, top;

		// derive in x direction
		// west
		if (d.hasNbr(Side<3>::west)) {
			const LocalData<2> boundary_data
			= gamma->getLocalData(d.getIfaceLocalIndex(Side<3>::west));
			for (int zi = 0; zi < n; zi++) {
				for (int yi = 0; yi < n; yi++) {
					west                = boundary_data[{yi, zi}];
					center              = u_data[{0, yi, zi}];
					east                = u_data[{1, yi, zi}];
					f_data[{0, yi, zi}] = (2 * west - 3 * center + east) / (h_x * h_x);
				}
			}
		} else if (d.isNeumann(Side<3>::west)) {
			for (int zi = 0; zi < n; zi++) {
				for (int yi = 0; yi < n; yi++) {
					center              = u_data[{0, yi, zi}];
					east                = u_data[{1, yi, zi}];
					f_data[{0, yi, zi}] = (-center + east) / (h_x * h_x);
				}
			}
		} else {
			for (int zi = 0; zi < n; zi++) {
				for (int yi = 0; yi < n; yi++) {
					center              = u_data[{0, yi, zi}];
					east                = u_data[{1, yi, zi}];
					f_data[{0, yi, zi}] = (-3 * center + east) / (h_x * h_x);
				}
			}
		}
		// middle
		for (int zi = 0; zi < n; zi++) {
			for (int yi = 0; yi < n; yi++) {
				for (int xi = 1; xi < n - 1; xi++) {
					west   = u_data[{xi - 1, yi, zi}];
					center = u_data[{xi, yi, zi}];
					east   = u_data[{xi + 1, yi, zi}];

					f_data[{xi, yi, zi}] = (west - 2 * center + east) / (h_x * h_x);
				}
			}
		}
		// east
		if (d.hasNbr(Side<3>::east)) {
			const LocalData<2> boundary_data
			= gamma->getLocalData(d.getIfaceLocalIndex(Side<3>::east));
			for (int zi = 0; zi < n; zi++) {
				for (int yi = 0; yi < n; yi++) {
					west                    = u_data[{n - 2, yi, zi}];
					center                  = u_data[{n - 1, yi, zi}];
					east                    = boundary_data[{yi, zi}];
					f_data[{n - 1, yi, zi}] = (west - 3 * center + 2 * east) / (h_x * h_x);
				}
			}
		} else if (d.isNeumann(Side<3>::east)) {
			for (int zi = 0; zi < n; zi++) {
				for (int yi = 0; yi < n; yi++) {
					west                    = u_data[{n - 2, yi, zi}];
					center                  = u_data[{n - 1, yi, zi}];
					f_data[{n - 1, yi, zi}] = (west - center) / (h_x * h_x);
				}
			}
		} else {
			for (int zi = 0; zi < n; zi++) {
				for (int yi = 0; yi < n; yi++) {
					west                    = u_data[{n - 2, yi, zi}];
					center                  = u_data[{n - 1, yi, zi}];
					f_data[{n - 1, yi, zi}] = (west - 3 * center) / (h_x * h_x);
				}
			}
		}

		// derive in y direction
		// south
		if (d.hasNbr(Side<3>::south)) {
			const LocalData<2> boundary_data
			= gamma->getLocalData(d.getIfaceLocalIndex(Side<3>::south));
			for (int zi = 0; zi < n; zi++) {
				for (int xi = 0; xi < n; xi++) {
					south  = boundary_data[{xi, zi}];
					center = u_data[{xi, 0, zi}];
					north  = u_data[{xi, 1, zi}];
					f_data[{xi, 0, zi}] += (2 * south - 3 * center + north) / (h_y * h_y);
				}
			}
		} else if (d.isNeumann(Side<3>::south)) {
			for (int zi = 0; zi < n; zi++) {
				for (int xi = 0; xi < n; xi++) {
					center = u_data[{xi, 0, zi}];
					north  = u_data[{xi, 1, zi}];
					f_data[{xi, 0, zi}] += (-center + north) / (h_y * h_y);
				}
			}
		} else {
			for (int zi = 0; zi < n; zi++) {
				for (int xi = 0; xi < n; xi++) {
					center = u_data[{xi, 0, zi}];
					north  = u_data[{xi, 1, zi}];
					f_data[{xi, 0, zi}] += (-3 * center + north) / (h_y * h_y);
				}
			}
		}
		// middle
		for (int zi = 0; zi < n; zi++) {
			for (int yi = 1; yi < n - 1; yi++) {
				for (int xi = 0; xi < n; xi++) {
					south  = u_data[{xi, yi - 1, zi}];
					center = u_data[{xi, yi, zi}];
					north  = u_data[{xi, yi + 1, zi}];

					f_data[{xi, yi, zi}] += (south - 2 * center + north) / (h_y * h_y);
				}
			}
		}
		// north
		if (d.hasNbr(Side<3>::north)) {
			const LocalData<2> boundary_data
			= gamma->getLocalData(d.getIfaceLocalIndex(Side<3>::north));
			for (int zi = 0; zi < n; zi++) {
				for (int xi = 0; xi < n; xi++) {
					south  = u_data[{xi, n - 2, zi}];
					center = u_data[{xi, n - 1, zi}];
					north  = boundary_data[{xi, zi}];
					f_data[{xi, n - 1, zi}] += (south - 3 * center + 2 * north) / (h_y * h_y);
				}
			}
		} else if (d.isNeumann(Side<3>::north)) {
			for (int zi = 0; zi < n; zi++) {
				for (int xi = 0; xi < n; xi++) {
					south  = u_data[{xi, n - 2, zi}];
					center = u_data[{xi, n - 1, zi}];
					f_data[{xi, n - 1, zi}] += (south - center) / (h_y * h_y);
				}
			}
		} else {
			for (int zi = 0; zi < n; zi++) {
				for (int xi = 0; xi < n; xi++) {
					south  = u_data[{xi, n - 2, zi}];
					center = u_data[{xi, n - 1, zi}];
					f_data[{xi, n - 1, zi}] += (south - 3 * center) / (h_y * h_y);
				}
			}
		}

		// derive in z direction
		// bottom
		if (d.hasNbr(Side<3>::bottom)) {
			const LocalData<2> boundary_data
			= gamma->getLocalData(d.getIfaceLocalIndex(Side<3>::bottom));
			for (int yi = 0; yi < n; yi++) {
				for (int xi = 0; xi < n; xi++) {
					bottom = boundary_data[{xi, yi}];
					center = u_data[{xi, yi, 0}];
					top    = u_data[{xi, yi, 1}];
					f_data[{xi, yi, 0}] += (2 * bottom - 3 * center + top) / (h_z * h_z);
				}
			}
		} else if (d.isNeumann(Side<3>::bottom)) {
			for (int yi = 0; yi < n; yi++) {
				for (int xi = 0; xi < n; xi++) {
					center = u_data[{xi, yi, 0}];
					top    = u_data[{xi, yi, 1}];
					f_data[{xi, yi, 0}] += (-center + top) / (h_z * h_z);
				}
			}
		} else {
			for (int yi = 0; yi < n; yi++) {
				for (int xi = 0; xi < n; xi++) {
					center = u_data[{xi, yi, 0}];
					top    = u_data[{xi, yi, 1}];
					f_data[{xi, yi, 0}] += (-3 * center + top) / (h_z * h_z);
				}
			}
		}
		// middle
		for (int zi = 1; zi < n - 1; zi++) {
			for (int yi = 0; yi < n; yi++) {
				for (int xi = 0; xi < n; xi++) {
					bottom = u_data[{xi, yi, zi - 1}];
					center = u_data[{xi, yi, zi}];
					top    = u_data[{xi, yi, zi + 1}];

					f_data[{xi, yi, zi}] += (bottom - 2 * center + top) / (h_z * h_z);
				}
			}
		}
		// top
		if (d.hasNbr(Side<3>::top)) {
			const LocalData<2> boundary_data
			= gamma->getLocalData(d.getIfaceLocalIndex(Side<3>::top));
			for (int yi = 0; yi < n; yi++) {
				for (int xi = 0; xi < n; xi++) {
					bottom = u_data[{xi, yi, n - 2}];
					center = u_data[{xi, yi, n - 1}];
					top    = boundary_data[{xi, yi}];
					f_data[{xi, yi, n - 1}] += (bottom - 3 * center + 2 * top) / (h_z * h_z);
				}
			}
		} else if (d.isNeumann(Side<3>::top)) {
			for (int yi = 0; yi < n; yi++) {
				for (int xi = 0; xi < n; xi++) {
					bottom = u_data[{xi, yi, n - 2}];
					center = u_data[{xi, yi, n - 1}];
					f_data[{xi, yi, n - 1}] += (bottom - center) / (h_z * h_z);
				}
			}
		} else {
			for (int yi = 0; yi < n; yi++) {
				for (int xi = 0; xi < n; xi++) {
					bottom = u_data[{xi, yi, n - 2}];
					center = u_data[{xi, yi, n - 1}];
					f_data[{xi, yi, n - 1}] += (bottom - 3 * center) / (h_z * h_z);
				}
			}
		}
	}
};
#endif