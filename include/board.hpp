// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   board.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid <archid-@1337.student.ma>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/04/21 00:31:40 by archid            #+#    #+#             //
//   Updated: 2023/04/22 02:42:57 by archid           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#pragma once

#include <string>
#include <vector>

namespace ft
{
	struct board
	{
		typedef std::vector<int>					tile_t;
		typedef std::vector<board>				neighbours_t;

		board() : zero_(std::pair<int, int>(-1, -1)), depth_(0) {}
		board(const board &rhs) : tiles_(rhs.tiles_), zero_(rhs.zero_), depth_(rhs.depth_) {}
		board(const std::vector<tile_t> &tiles, int depth);

		int dimension() const;
		bool snail() const;
		neighbours_t neighbours() const;
		bool solvable() const;
		void solve() const;
		board twin();

		int hamming() const; // number of tiles out of place
		int manhattan() const; // sum of Manhattan distances between tiles and goal

		board &operator=(const board &rhs);
		bool operator==(const board &rhs) const;
		bool operator!=(const board &rhs) const;
		bool operator<(const board &rhs) const;
		friend std::ostream &operator<<(std::ostream &oss, const board &b);

	private:

		board get_neighbour(int i, int j) const;

		std::vector<tile_t> tiles_;
		std::pair<int, int> zero_;
		int depth_;
	};

}	 // namespace ft
