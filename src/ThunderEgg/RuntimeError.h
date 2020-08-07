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

#ifndef THUNDEREGG_RUNTIMEERROR_H
#define THUNDEREGG_RUNTIMEERROR_H

#include <stdexcept>

namespace ThunderEgg
{
/**
 * @brief ThunderEgg runtime exception
 */
struct RuntimeError : std::runtime_error {
	/**
	 * @brief Construct a new RuntimeError object
	 *
	 * @param message the message to print
	 */
	RuntimeError(std::string message) : std::runtime_error(message){};
};
} // namespace ThunderEgg
#endif