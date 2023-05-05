// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   board.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid <archid-@1337.student.ma>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/04/21 00:31:40 by archid            #+#    #+#             //
//   Updated: 2023/05/04 02:18:38 by archid           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <assert.h>

namespace ft
{
	struct board
	{
		board() : zero_(std::pair<int, int>(-1, -1)), depth_(0) {}
		board(const board &rhs) : tiles_(rhs.tiles_), zero_(rhs.zero_), depth_(rhs.depth_) {}
		board(const std::vector< std::vector<int> > &tiles, int depth);

		int dimension() const;
		bool snail() const;
		std::vector<board> neighbours() const;
		bool solvable() const;
		void solve() const;
		bool nil() const;
		int depth() const { return depth_; }


		int hamming() const; // number of tiles out of place
		int manhattan() const; // sum of Manhattan distances between tiles and goal

		board &operator=(const board &rhs);
		bool operator==(const board &rhs) const;
		bool operator!=(const board &rhs) const;
		bool operator<(const board &rhs) const;
		bool operator>(const board &rhs) const;
		friend std::ostream &operator<<(std::ostream &oss, const board &b);

	private:

		int calc_hamming() const; // number of tiles out of place
		int calc_manhattan() const; // sum of Manhattan distances between tiles and goal

		board get_neighbour(int i, int j) const;

		std::vector< std::vector<int> > tiles_;
		std::pair<int, int> zero_;
		int depth_;
		int manh_, ham_;
	};

	bool binary_search(const std::vector<board> &, board);
	std::vector<board> merge_sort(const std::vector<board> &);

}	 // namespace ft
