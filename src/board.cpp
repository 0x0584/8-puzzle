// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   board.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid <archid-@1337.student.ma>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/04/21 13:18:14 by archid            #+#    #+#             //
//   Updated: 2023/04/26 14:43:16 by archid           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <iostream>
#include <set>
#include <vector>
#include <numeric>
#include <queue>
#include <algorithm>

#include "board.hpp"
#include <assert.h>

namespace ft {

	board::board(const std::vector< std::vector<int> > &tiles, int depth)
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
		std::vector< std::vector<int> > tiles = tiles_;
		std::swap(tiles[zero_.first][zero_.second],
							tiles[zero_.first + i][zero_.second + j]);
		return board(tiles, depth_ + 1);
	}

	std::vector<board> board::neighbours() const
	{
		std::vector<board> neighs;
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

	bool binary_search(const std::vector<board> &v, board b)
	{
		int lo = 0, hi = v.size();
		while (lo < hi) {
			int mid = lo + (hi - lo) / 2;
			if (v[mid] == b) {
				return true;
			}
			else if (v[mid] < b)
				lo = mid;
			else
				hi = mid;
		}
		return false;
	}

	void swap(board &a, board &b) {
		board c = a;
		a = b;
		b = c;
	}

	std::vector<board> merge_impl(const std::vector<board> &v, int lo, int mid, int hi) {
		std::vector<board> merged;

		merged.reserve(hi - lo);
		int i = lo, j = mid;
		while (i < mid || j < hi) {
			if (j >= hi || (i < mid && v[i] < v[j]))
				merged.push_back(v[i++]);
			if (i >= mid && (j < hi && v[j] < v[i]))
				merged.push_back(v[j++]);
		}
		return merged;
	}

	std::vector<board> sort_impl(const std::vector<board> &v, int lo, int hi) {
		if (hi - lo > 2) {
			int mid = lo + (hi - lo) / 2;
			std::vector<board> left = sort_impl(v, lo, mid),
				right = sort_impl(v, mid, hi);
			return merge_impl(v, lo, mid, hi);
		}
		std::vector<board> merged = std::vector<board>(v.begin() + lo, v.begin() + hi);
		if (hi - lo > 1 && merged[lo] > merged[lo + 1]) {
			ft::swap(merged[0], merged[1]);
		}
		return merged;
	}

	std::vector<board> merge_sort(const std::vector<board> &v) {
		return sort_impl(v, 0, v.size());
	}

	void board::solve() const
	{
		std::vector<board> neighs = neighbours();
		std::priority_queue<board> pq;
		std::vector<board> visited;

		for (unsigned i = 0; i < neighs.size(); ++i)
			pq.push(neighs[i]);

		std::size_t max_size = 0;
		std::vector<board> sol;
		visited.push_back(*this);
		while (not pq.empty()) {
			board b = pq.top();
			pq.pop();

			std::cout << "current: \n" << b << "\n";
			sol.push_back(b);
			if (b.snail())
				break;
			else
				visited.push_back(b);


			//merge_sort(visited);
			std::sort(visited.begin(), visited.end());

			neighs = b.neighbours();
			std::cout << "branch:\n";
			for (unsigned i = 0; i < neighs.size(); ++i) {
				if (not binary_search(visited, neighs[i])) {
					std::cout << "considering\n" << neighs[i];
					pq.push(neighs[i]);
				}
				// std::cout << "\n";
			}
			std::cout << std::endl;

			max_size = std::max(max_size, pq.size());
		}

		assert(sol.back().snail());
		std::cout << "Number of branches: " << visited.size() << "\n";
		std::cout << "Maximum footprint: " << max_size << "\n";
		std::cout << "Number of moves: " << sol.size() << "\n";
		for (unsigned i = 0; i < sol.size(); ++i)
			std::cout << "step: " << i << "\n" << sol[i] << "\n\n";
	}

	board &board::operator=(board &rhs)
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
		//		std::cerr << "cmp:\n" << *this << "\n" << rhs << "\n";
		return manhattan() < rhs.manhattan();
	}

	bool board::operator>(const board &rhs) const
	{
		//		std::cerr << "cmp:\n" << *this << "\n" << rhs << "\n";
		return manhattan() > rhs.manhattan();
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
