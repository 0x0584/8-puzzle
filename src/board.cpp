/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid <archid-@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 13:18:14 by archid            #+#    #+#             */
//   Updated: 2023/05/04 15:36:30 by archid           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <set>
#include <vector>

#include "board.hpp"
#include <assert.h>

namespace ft {

	board::board(const std::vector< std::vector<int> > &tiles, int depth)
		: tiles_(tiles), zero_(std::pair<int, int>(-1, -1)), depth_(depth)
	{
		// for (unsigned i = 0; i < tiles.size(); ++i) {
		// 	for (unsigned j = 0; j < tiles.size(); ++j) {
		// 		std::cout << tiles[i][j] << " ";
		// 	}
		// 	std::cout << "\n";
		// }
		// std::cout << "\n";

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

		ham_ = calc_hamming();
		manh_ = calc_manhattan();
	}

	int board::dimension() const
	{
		return tiles_.size();
	}

	int board::manhattan() const // to take place
	{
		return manh_;
	}

	int board::hamming() const // count fo misplaced tiles
	{
		return ham_;
	}

	board board::get_neighbour(int i, int j) const
	{
		std::vector< std::vector<int> > tiles = tiles_;
		std::swap(tiles[zero_.first][zero_.second],
							tiles[zero_.first + i][zero_.second + j]);
		return board(tiles, depth_ + 1);
	}

	std::vector<board> board::neighbours() const
	{
		std::vector<board> neighs;
		neighs.reserve(4);

		if (zero_.first < dimension() && zero_.first + 1 < dimension())
			neighs.push_back(get_neighbour(1, 0));
		if (zero_.second < dimension() && zero_.second + 1 < dimension())
			neighs.push_back(get_neighbour(0, 1));
		if (zero_.second > 0 && zero_.second - 1 < dimension())
			neighs.push_back(get_neighbour(0, -1));
		if (zero_.first > 0 && zero_.first - 1 < dimension())
			neighs.push_back(get_neighbour(-1, 0));

		std::cout << "neighs of \n" << *this << "\n";
		for (int i = 0; i < neighs.size(); ++i)
			std::cout << neighs[i] << "\n";

		return neighs;
	}

	board &board::operator=(const board &rhs)
	{
		if (this != &rhs) {
			tiles_ = rhs.tiles_;
			depth_ = rhs.depth_;
			zero_ = rhs.zero_;
		}
		return *this;
	}

	bool board::operator==(const board &rhs) const
	{
		return tiles_ == rhs.tiles_;
	}

	bool board::operator!=(const board &rhs) const
	{
		return not operator==(rhs);
	}

	bool board::operator<(const board &rhs) const
	{
		//		std::cerr << "cmp:\n" << *this << "\n" << rhs << "\n";
		return manh_ < rhs.manh_;
	}

	bool board::operator>(const board &rhs) const
	{
		//		std::cerr << "cmp:\n" << *this << "\n" << rhs << "\n";
		return manh_ > rhs.manh_;
	}

	std::ostream &operator<<(std::ostream &oss, const board &b)
	{
		for (int i = 0; i < b.dimension(); ++i) {
			for (int j = 0; j < b.dimension(); ++j) {
				oss << b.tiles_[i][j] << " ";
			}
			oss << '\n';
		}
		return oss;
	}

}
