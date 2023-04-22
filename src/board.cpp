// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   board.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid <archid-@1337.student.ma>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/04/21 13:18:14 by archid            #+#    #+#             //
//   Updated: 2023/04/22 01:22:50 by archid           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <iostream>
#include <set>
#include <vector>
#include <numeric>
#include <queue>

#include "board.hpp"
#include <assert.h>

namespace ft {

	board::board(const std::vector<tile_t> &tiles, int depth)
		: tiles_(tiles), depth_(depth)
	{
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

	int board::dimension() const
	{
		return tiles_.size();
	}

	int board::hamming() const
	{
		int ham = 0;
		for (int i = 0; i < dimension(); ++i)
			for (int j = 0; j < dimension(); ++j)
				ham += (tiles_[i][j] != 0 && tiles_[i][j] != 1 + i + j);
		return ham;
	}

	int board::manhattan() const
	{
		int manh = 0;
		for (int i = 0; i < dimension(); ++i)
			for (int j = 0; j < dimension(); ++j)
				if (tiles_[i][j] != 0 && tiles_[i][j] != 1 + i + j)
					manh += (tiles_[i][j] / dimension()) + (tiles_[i][j] % dimension());
		return manh;
	}

	bool board::snail() const
	{
		const int index = dimension() - 1;
		const int &tile = tiles_[index][index];

		if (tile != 0)
			return false;
		for (int i = 0; i < dimension(); ++i)
			for (int j = 0; j < dimension(); ++j)
				if (tiles_[i][j] != i + j + 1 && i == index && j == index && tile != 0)
					return false;
		return true;
	}


	board board::get_neighbour(bool updown, bool leftright) const
	{
		std::vector<tile_t> tiles = tiles_;
		std::swap(tiles[zero_.first][zero_.second],
							tiles[zero_.first + updown][zero_.second + leftright]);
		return board(tiles, depth_ + 1);
	}

	board::neighbours_t board::neighbours() const
	{
		neighbours_t neighs;
		neighs.reserve(4);

		if (zero_.first + 1 < dimension()) {
			if (zero_.second + 1 < dimension())
				neighs.push_back(get_neighbour(true, true));
			if (zero_.second - 1 < dimension())
				neighs.push_back(get_neighbour(true, false));
		}

		if (zero_.first - 1 < dimension()) {
			if (zero_.second + 1 < dimension())
				neighs.push_back(get_neighbour(false, true));
			if (zero_.second - 1 < dimension())
				neighs.push_back(get_neighbour(false, false));
		}

		return neighs;
	}

	bool board::solvable() const
	{
		// int inversions = 0;
		// for (int i = 0; i < dimension(); ++i)
		// 	for (int j = 0; j < dimension(); ++j)
		// 		if (tiles_[i][j] != 0)
		// 			for (int k = 0; k < )
		// 			inversions += 1;
		// std::cout << inversions << "\n";
		// exit(1);
		// return inversions % 2 == tiles_.size() % 2;
		return false;
	}

	void board::solve() const
	{
		std::vector<board> neighs = neighbours();
		std::priority_queue<board> pq;
		std::set<board> visited;

		for (unsigned i = 0; i < neighs.size(); ++i) {
			pq.push(neighs[i]);
			visited.insert(neighs[i]);
		}

		std::size_t max_size = 0;
		std::vector<board> sol;
		while (not pq.empty()) {
			board b = pq.top();
			pq.pop();

			sol.push_back(b);
			if (b.snail())
				break;

			neighs = b.neighbours();
			for (unsigned i = 0; i < neighs.size(); ++i) {
				if (visited.insert(neighs[i]).second)
					pq.push(neighs[i]);
			}

			max_size = std::max(max_size, pq.size());
		}

		assert(sol.back().snail());
		std::cout << "Number of branches: " << visited.size() << "\n";
		std::cout << "Maximum footprint: " << max_size << "\n";
		std::cout << "Number of moves: " << sol.size() << "\n";
		for (unsigned i = 0; i < sol.size(); ++i)
			std::cout << "step: " << i << "\n" << sol[i] << "\n\n";
	}

	board &board::operator=(const board &rhs)
	{
		tiles_ = rhs.tiles_;
		return *this;
	}

	bool board::operator==(const board &rhs) const
	{
		return rhs.tiles_ == tiles_;
	}

	bool board::operator!=(const board &rhs) const
	{
		return not operator==(rhs);
	}

	bool board::operator<(const board &rhs) const
	{
		return manhattan() < rhs.manhattan();
	}

	std::ostream &operator<<(std::ostream &oss, const board &b)
	{
		for (int i = 0; i < b.dimension(); ++i) {
			for (int j = 0; j < b.dimension(); ++j) {
				oss << b.tiles_[i][j] << " ";
			}
			oss << '\n';
		}
		return oss << '\n';
	}

}
