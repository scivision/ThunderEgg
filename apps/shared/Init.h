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

#include <ThunderEgg/Domain.h>
#include <functional>
#include <petscvec.h>
class Init
{
	public:
	static void initNeumann(ThunderEgg::Domain<3> &domain, Vec f, Vec exact,
	                        std::function<double(double, double, double)> ffun,
	                        std::function<double(double, double, double)> efun,
	                        std::function<double(double, double, double)> nfunx,
	                        std::function<double(double, double, double)> nfuny,
	                        std::function<double(double, double, double)> nfunz);
	static void initDirichlet(ThunderEgg::Domain<3> &domain, Vec f, Vec exact,
	                          std::function<double(double, double, double)> ffun,
	                          std::function<double(double, double, double)> efun);
	static void initNeumann2d(ThunderEgg::Domain<2> &domain, Vec f, Vec exact,
	                          std::function<double(double, double)> ffun,
	                          std::function<double(double, double)> efun,
	                          std::function<double(double, double)> nfunx,
	                          std::function<double(double, double)> nfuny);
	static void initDirichlet2d(ThunderEgg::Domain<2> &domain, Vec f, Vec exact,
	                            std::function<double(double, double)> ffun,
	                            std::function<double(double, double)> efun);
	static void fillSolution2d(ThunderEgg::Domain<2> &domain, Vec u,
	                           std::function<double(double, double, double)> fun, double time);
};
