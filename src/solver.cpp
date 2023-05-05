// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   solver.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid <archid-@1337.student.ma>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/05/04 15:31:54 by archid            #+#    #+#             //
//   Updated: 2023/05/04 15:36:30 by archid           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "board.hpp"

#include <iostream>
#include <numeric>
#include <queue>
#include <algorithm>
#include <map>

namespace ft
{
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
				lo = mid + 1;
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

	bool board::nil() const {
		if (zero_ == std::pair<int, int>(-1, -1))
			assert(depth_ == 0);
		return (zero_ == std::pair<int, int>(-1, -1));
	}


	void board::solve() const
	{
		std::vector<board> neighs = neighbours();
		std::priority_queue<board> pq;

		for (unsigned i = 0; i < neighs.size(); ++i)
			pq.push(neighs[i]);

		std::vector<board> visited;
		std::map<board, board> origin;
		std::size_t max_size = 0;
		board current = *this;
		origin[current] = board();
		visited.push_back(*this);
		while (not pq.empty()) {
			board b = pq.top();
			pq.pop();

			origin[b] = current;
			current = b;

			if (b.snail())
				break;
			else
				visited.push_back(b);

			//merge_sort(visited);
			std::sort(visited.begin(), visited.end());

			neighs = b.neighbours();
			for (unsigned i = 0; i < neighs.size(); ++i) {
				if (not std::binary_search(visited.begin(), visited.end(), neighs[i])) {
					pq.push(neighs[i]);
				}
			}
			std::cout << std::endl;

			max_size = std::max(max_size, pq.size());
		}

		std::vector<board> sol;
		sol.resize(current.depth() + 1);
		int depth = current.depth();
		for (board prev = origin[current]; not current.nil(); current = prev)
			sol[depth--] = prev;

		std::cout << "Number of branches: " << visited.size() << "\n";
		std::cout << "Maximum footprint: " << max_size << "\n";
		std::cout << "Number of moves: " << sol.size() << "\n";
		for (unsigned i = 0; i < sol.size(); ++i)
			std::cout << "step: " << i << "\n" << sol[i] << "\n\n";
	}

}
