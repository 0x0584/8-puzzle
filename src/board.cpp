// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   board.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid <archid-@1337.student.ma>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/04/21 13:18:14 by archid            #+#    #+#             //
//   Updated: 2023/04/22 13:57:29 by archid           ###   ########.fr       //
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
		: tiles_(tiles), zero_(std::pair<int, int>(-1, -1)), depth_(depth)
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


	board board::get_neighbour(int i, int j) const
	{
		std::vector<tile_t> tiles = tiles_;
		std::swap(tiles[zero_.first][zero_.second],
							tiles[zero_.first + i][zero_.second + j]);
		return board(tiles, depth_ + 1);
	}

	board::neighbours_t board::neighbours() const
	{
		neighbours_t neighs;
		neighs.reserve(4);

		if (zero_.first + 1 < dimension())
			neighs.push_back(get_neighbour(1, 0));
		if (zero_.second + 1 < dimension())
			neighs.push_back(get_neighbour(0, 1));
		if (zero_.second - 1 < dimension())
			neighs.push_back(get_neighbour(0, -1));
		if (zero_.first - 1 < dimension())
			neighs.push_back(get_neighbour(-1, 0));

		return neighs;
	}

	bool board::solvable() const
	{
		std::vector<int> vec;
		for (unsigned i = 0; i < tiles_.size(); ++i)
			vec.insert(vec.end(), tiles_[i].begin(), tiles_[i].end());

		int inversions = 0;
		for (unsigned i = 0; i < vec.size(); ++i)
			for (unsigned j = i + 1; j < vec.size(); ++j)
				if (vec[i] != 0 && vec[j] != 0)
					inversions += (vec[i] > vec[j]);

		return inversions % 2 != dimension() % 2;
	}

	void board::solve() const
	{
		exit(1);
		std::vector<board> neighs = neighbours();
		std::priority_queue<board> pq;
		std::set<board> visited;

		visited.insert(*this);
		for (unsigned i = 0; i < neighs.size(); ++i) {
			pq.push(neighs[i]);
			visited.insert(neighs[i]);
		}

		exit(1);

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
		return oss;
	}

}
