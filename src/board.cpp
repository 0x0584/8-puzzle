// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   board.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid <archid-@1337.student.ma>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/04/21 13:18:14 by archid            #+#    #+#             //
//   Updated: 2023/04/21 18:56:22 by archid           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <iostream>
#include <set>
#include <vector>
#include <numeric>
#include <deque>

#include "board.hpp"

namespace ft {

	board::board(const std::vector<tile_t> &tiles) : tiles_(tiles) {
		std::set<int> numbers;
		for (int i = 0; i < dimension(); ++i) {
			for (int j = 0; j < dimension(); ++j) {
				if (!numbers.insert(tiles[i][j]).second)
					throw std::runtime_error("duplicated value");
				if (tiles[i][j] == 0)
					zero_ = std::make_pair<int, int>(i, j);
			}
		}
		if (zero_ == std::pair<int, int>(-1, -1))
			throw std::runtime_error("invalid board");
		for (unsigned i = 0; i < numbers.size(); ++i) {
			if (*numbers.begin() != static_cast<int>(i))
				throw std::runtime_error("invalid board");
			numbers.erase(i);
		}
	}

	std::ostream &operator<<(std::ostream &oss, const board &b) {
		for (int i = 0; i < b.dimension(); ++i) {
			for (int j = 0; j < b.dimension(); ++j) {
				oss << b.tiles_[i][j] << " ";
			}
			oss << '\n';
		}
		return oss << '\n';
	}

	// board dimension n
	int board::dimension() const { return tiles_.size(); }

	// number of tiles out of place
	int board::hamming() const {

	}

	// sum of Manhattan distances between tiles and goal
	int board::manhattan() const {
	}

	// is this board the goal board?
	bool board::goal() const {
		return false;
	}

	// all neighbouring boards
	board::neighbours_t board::neighbours() const {

	}

	// a board that is obtained by exchanging any pair of tiles
	board board::twin() const {

	}

	// sequence of boards in a shortest solution; null if unsolvable
	std::vector<board> board::solve() const {
		std::deque<board> q;
		std::vector<board> sol;



		return sol;
	}

	board &board::operator=(const board &rhs) {
		tiles_ = rhs.tiles_;
		return *this;
	}

}
