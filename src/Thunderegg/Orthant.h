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

#ifndef THUNDEREGG_ORTHANT_H
#define THUNDEREGG_ORTHANT_H
#include <Thunderegg/Side.h>
#include <array>
#include <iostream>
#include <numeric>
#include <vector>
namespace Thunderegg
{
/**
 * @brief An enum-style class that represents the octants of a cube.
 *
 * The octants are named in the following way:
 *
 * bse means Bottom-South-West, which is in the corner of where the bottom, south, and west sides
 * meet.
 */
template <size_t D> class Orthant
{
	private:
	/**
	 * @brief the value of the enum.
	 */
	unsigned int val = ~0x0;

	public:
	/**
	 * @brief null value
	 */
	static Orthant<D> null()
	{
		return Orthant<D>(~0x0);
	}
	/**
	 * @brief Lower half of line.
	 */
	static constexpr unsigned int lower = 0b000;
	/**
	 * @brief Upper half of line.
	 */
	static constexpr unsigned int upper = 0b001;
	/**
	 * @brief South-West quadrant of square.
	 */
	static constexpr unsigned int sw = 0b000;
	/**
	 * @brief South-East quadrant of square.
	 */
	static constexpr unsigned int se = 0b001;
	/**
	 * @brief North-West quadrant of square.
	 */
	static constexpr unsigned int nw = 0b010;
	/**
	 * @brief North-East quadrant of square.
	 */
	static constexpr unsigned int ne = 0b011;
	/**
	 * @brief Bottom-South-West octant of cube.
	 */
	static constexpr unsigned int bsw = 0b000;
	/**
	 * @brief Bottom-South-East octant of cube.
	 */
	static constexpr unsigned int bse = 0b001;
	/**
	 * @brief Bottom-North-West octant of cube.
	 */
	static constexpr unsigned int bnw = 0b010;
	/**
	 * @brief Bottom-North-East octant of cube.
	 */
	static constexpr unsigned int bne = 0b011;
	/**
	 * @brief Top-South-West octant of cube.
	 */
	static constexpr unsigned int tsw = 0b100;
	/**
	 * @brief Top-South-East octant of cube.
	 */
	static constexpr unsigned int tse = 0b101;
	/**
	 * @brief Top-North-West octant of cube.
	 */
	static constexpr unsigned int tnw = 0b110;
	/**
	 * @brief Top-North-East octant of cube.
	 */
	static constexpr unsigned int tne = 0b111;

	static constexpr unsigned int num_orthants = 1 << D;
	/**
	 * @brief Default constructor that initializes the value to -1.
	 */
	Orthant() = default;
	/**
	 * @brief Create new Orthant<D> with given value.
	 *
	 * @param val the value
	 */
	Orthant(const unsigned int val)
	{
		this->val = val;
	}
	/**
	 * @brief Get the integer value of the octant.
	 *
	 * @return The integer value.
	 */
	int toInt() const
	{
		return val;
	}
	/**
	 * @brief Return the octant that neighbors this octant on a particular side.
	 *
	 * @param s the side of the octant that you want the neighbor of.
	 *
	 * @return  The octant that neighbors on that side.
	 */
	Orthant<D> getInteriorNbrOnSide(Side<D> s) const;
	/**
	 * @brief Return the octant that neighbors this octant on a particular side.
	 *
	 * @param s the side of the octant that you want the neighbor of.
	 *
	 * @return  The octant that neighbors on that side.
	 */
	Orthant<D> getExteriorNbrOnSide(Side<D> s) const;
	/**
	 * @brief Get the sides of the octant that are on the interior of the cube.
	 *
	 * @return The sides of the octant that are on the interior of the cube.
	 */
	inline std::array<Side<D>, D> getInteriorSides() const
	{
		std::array<Side<D>, D> retval;
		for (size_t i = 0; i < D; i++) {
			int side = 2 * i;
			if (!((1 << i) & val)) {
				side |= 1;
			}
			retval[i] = side;
		}
		return retval;
	}
	/**
	 * @brief Get the sides of the octant that are on the exterior of the cube.
	 *
	 * @return The sides of the octant that are on the exterior of the cube.
	 */
	inline std::array<Side<D>, D> getExteriorSides() const
	{
		std::array<Side<D>, D> retval;
		for (size_t i = 0; i < D; i++) {
			int side = 2 * i;
			if ((1 << i) & val) {
				side |= 1;
			}
			retval[i] = side;
		}
		return retval;
	}
	/**
	 * @brief Return whether or not the octant lies on a particular side of a cube.
	 *
	 * @param s the side of the cube.j
	 *
	 * @return Whether or not it lies on that side.
	 */
	inline bool isOnSide(Side<D> s) const
	{
		int  idx        = s.toInt() / 2;
		int  remainder  = s.toInt() % 2;
		bool is_bit_set = val & (0x1 << idx);
		return is_bit_set == remainder;
	}
	/**
	 * @brief From the point of view of an axis, get orthant that this orthant lies on in the D-1
	 * dimension
	 *
	 * @param axis the axis
	 * @return Orthant<D - 1> the resulting orthant
	 */
	inline Orthant<D - 1> collapseOnAxis(int axis) const
	{
		unsigned int upper_mask = (~0x0) << axis;
		return Orthant<D - 1>(((val >> 1) & upper_mask) | (val & ~upper_mask));
	}
	/**
	 * @brief Get an array of all Orthant<D> values, in increasing order.
	 *
	 * @return The array.
	 */
	static std::array<Orthant, num_orthants> getValues();
	/**
	 * @brief Get an array of all Orthant<D> values that lie on a particular side of the cube.
	 *
	 * When the two axis that the side lies on are arranged in the following way, the octants are
	 * returned in the following order:
	 *
	 *   ^
	 *   |
	 *   |  2  |  3
	 *   |-----+-----
	 *   |  0  |  1
	 *   +----------->
	 *
	 * @return The array.
	 */
	static std::array<Orthant, num_orthants / 2> getValuesOnSide(Side<D> s);
	/**
	 * @brief Equals operator.
	 *
	 * @param other The other octant.
	 *
	 * @return Whether or not the value of this octant equals the value other octant.
	 */
	bool operator==(const Orthant<D> &other) const
	{
		return val == other.val;
	}
	/**
	 * @brief Less Tan operator.
	 *
	 * @param other The other octant.
	 *
	 * @return Whether or not the value of this octant is less than the value other octant.
	 */
	bool operator<(const Orthant<D> &other) const
	{
		return val < other.val;
	}
};

// function definitions
template <size_t D> inline Orthant<D> Orthant<D>::getInteriorNbrOnSide(Side<D> s) const
{
	Orthant<D> retval = *this;
	// flip the bit for that side
	retval.val ^= (0x1 << (s.toInt() / 2));
	return retval;
}
template <size_t D> inline Orthant<D> Orthant<D>::getExteriorNbrOnSide(Side<D> s) const
{
	Orthant<D> retval = *this;
	// flip the bit for that side
	retval.val ^= (0x1 << (s.toInt() / 2));
	return retval;
}
template <size_t D>
inline std::array<Orthant<D>, Orthant<D>::num_orthants / 2> Orthant<D>::getValuesOnSide(Side<D> s)
{
	unsigned int bit_to_insert = s.toInt() / 2;
	unsigned int set_bit       = s.isLowerOnAxis() ? 0 : 1;
	unsigned int lower_mask    = ~((~0x0) << bit_to_insert);
	unsigned int upper_mask    = (~0x0) << (bit_to_insert + 1);

	std::array<Orthant<D>, Orthant<D>::num_orthants / 2> retval;
	for (size_t i = 0; i < Orthant<D>::num_orthants / 2; i++) {
		size_t value = (i << 1) & upper_mask;
		value |= i & lower_mask;
		value |= set_bit << bit_to_insert;
		retval[i] = value;
	}
	return retval;
}
template <size_t D> inline std::array<Orthant<D>, Orthant<D>::num_orthants> Orthant<D>::getValues()
{
	std::array<Orthant<D>, Orthant<D>::num_orthants> retval;
	std::iota(retval.begin(), retval.end(), 0);
	return retval;
}
template <size_t D> constexpr unsigned int Orthant<D>::lower;
template <size_t D> constexpr unsigned int Orthant<D>::upper;
template <size_t D> constexpr unsigned int Orthant<D>::sw;
template <size_t D> constexpr unsigned int Orthant<D>::se;
template <size_t D> constexpr unsigned int Orthant<D>::nw;
template <size_t D> constexpr unsigned int Orthant<D>::ne;
template <size_t D> constexpr unsigned int Orthant<D>::bsw;
template <size_t D> constexpr unsigned int Orthant<D>::bse;
template <size_t D> constexpr unsigned int Orthant<D>::bnw;
template <size_t D> constexpr unsigned int Orthant<D>::bne;
template <size_t D> constexpr unsigned int Orthant<D>::tsw;
template <size_t D> constexpr unsigned int Orthant<D>::tse;
template <size_t D> constexpr unsigned int Orthant<D>::tnw;
template <size_t D> constexpr unsigned int Orthant<D>::tne;
template <size_t D> constexpr unsigned int Orthant<D>::num_orthants;
/**
 * @brief ostream operator that prints a string representation of orthant enum.
 *
 * For example, Orthant::bsw will print out "Orthant::bsw".
 *
 * @param os the ostream
 * @param s the side to print out.
 *
 * @return  the ostream
 */
inline std::ostream &operator<<(std::ostream &os, const Orthant<3> &o)
{
	switch (o.toInt()) {
		case Orthant<3>::bsw:
			os << "Orthant::bsw";
			break;
		case Orthant<3>::bse:
			os << "Orthant::bse";
			break;
		case Orthant<3>::bnw:
			os << "Orthant::bnw";
			break;
		case Orthant<3>::bne:
			os << "Orthant::bne";
			break;
		case Orthant<3>::tsw:
			os << "Orthant::tsw";
			break;
		case Orthant<3>::tse:
			os << "Orthant::tse";
			break;
		case Orthant<3>::tnw:
			os << "Orthant::tnw";
			break;
		case Orthant<3>::tne:
			os << "Orthant::tne";
			break;
	}
	return os;
}
/**
 * @brief ostream operator that prints a string representation of quadrant enum.
 *
 * For example, Orthant::sw will print out "Orthant::sw".
 *
 * @param os the ostream
 * @param s the side to print out.
 *
 * @return  the ostream
 */
inline std::ostream &operator<<(std::ostream &os, const Orthant<2> &o)
{
	switch (o.toInt()) {
		case ~0x0:
			os << "Orthant::null";
			break;
		case Orthant<2>::sw:
			os << "Orthant::sw";
			break;
		case Orthant<2>::se:
			os << "Orthant::se";
			break;
		case Orthant<2>::nw:
			os << "Orthant::nw";
			break;
		case Orthant<2>::ne:
			os << "Orthant::ne";
			break;
		default:
			os << "(Invalid Value)";
	}
	return os;
}
} // namespace Thunderegg
#endif