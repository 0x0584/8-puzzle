/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cost.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid <archid-@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:32:10 by archid            #+#    #+#             */
/*   Updated: 2023/05/05 13:55:34 by archid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "board.hpp"
#include <iostream>

namespace ft
{
	int board::calc_hamming() const // count fo misplaced tiles
	{
		int ham = 0;
		for (int i = 0; i < dimension(); ++i)
			for (int j = 0; j < dimension(); ++j) {
				int tile = tiles_[i][j];
				if (tile == 0) 
					continue;
				--tile;
				int y = tile / dimension(), x = tile % dimension();
				if (i != y || x != j)
					++ham;
			}
		return ham;
	}

	int board::calc_manhattan() const // to take place
	{
		int manh = 0;
		for (int i = 0; i < dimension(); ++i)
			for (int j = 0; j < dimension(); ++j) {
				int tile = tiles_[i][j];
				if (tile == 0)
					continue;
				--tile;
				int y = tile / dimension(), x = tile % dimension();
				if (y != i || x != j)
					manh += std::abs(j - (tile % dimension())) + std::abs(i - (tile / dimension()));
			}
		return manh;
	}
}
