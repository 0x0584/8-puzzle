// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   board.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid <archid-@1337.student.ma>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/04/21 00:31:40 by archid            #+#    #+#             //
//   Updated: 2023/04/21 17:52:45 by archid           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#pragma once

#include <string>
#include <vector>

namespace ft {
	struct board {
		typedef std::vector<int>					tile_t;
		typedef std::vector<board>				neighbours_t;

		board() {}

		board(const board &rhs) : tiles_(rhs.tiles_) {}

		// create a board from an n-by-n array of tiles,
		// where tiles[row][col] = tile at (row, col)
		board(const std::vector<tile_t> &tiles);

		// board dimension n
		int dimension() const;

		// number of tiles out of place
		int hamming() const;

		// sum of Manhattan distances between tiles and goal
		int manhattan() const;

		// is this board the goal board?
		bool goal() const;

		// all neighbouring boards
		neighbours_t neighbours() const;

		// a board that is obtained by exchanging any pair of tiles
		board twin() const;

		// sequence of boards in a shortest solution; null if unsolvable
		std::vector<board> solve() const;

		board &operator=(const board &rhs);

		friend std::ostream &operator<<(std::ostream &oss, const board &b);

	private:
		std::vector<tile_t> tiles_;
		std::pair<int, int> zero_;
	};


}	 // namespace ft
