/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid <archid-@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 00:36:27 by archid            #+#    #+#             */
//   Updated: 2023/05/04 13:45:27 by archid           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

// ./n-puzzle file

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include "board.hpp"

#define BUFF_SIZE 128

std::vector<std::vector<int > > parse_puzzle(int argc, const char *argv[], int index=1) {
	if (argc != 2)
		exit(1);

	int fd = open(argv[index], O_RDONLY);
	if (fd < 0)
		exit(1);
	std::vector<std::string> lines;
	char buff[BUFF_SIZE + 1];
	int ret;
	while (true) {
		ret = read(fd, buff, sizeof(buff));
		buff[ret] = '\0';
		if (ret < 0)
			exit(1);
		else if (ret == 0)
			break;
		char *buff_ptr = buff;
		while (true) {
			char *ptr = std::strchr(buff_ptr, '\n');
			if (!ptr)
				break;
			lines.push_back(std::string(static_cast<char *>(buff_ptr), ptr));
			buff_ptr = ptr + 1;
		}
	}
	close(fd);
	if (*buff)
		lines.push_back(std::string(buff));

	int n = std::atoi(lines.front().c_str());
	std::vector<std::vector<int> > tiles;
	tiles.reserve(n);
	for (int i = 0; i < n; ++i) {
		std::vector<int> tile(n);
		std::vector<int>::iterator it = tile.begin();
		while (it != tile.end()) {
			*it++ = std::atoi(lines[i + 1].c_str());
			std::size_t pos = lines[i + 1].find(' ');
			if (pos != std::string::npos)
				lines[i + 1] = lines[i + 1].substr(pos + 1);
		}
		tiles.push_back(tile);
	}

	return tiles;
}

int main(int argc, const char *argv[])
{
	ft::board boards[64];
	for (int i = 1; i < argc; ++i) {
			std::vector<std::vector<int > > tile = parse_puzzle(argc, argv, i);
			boards[i-1] = ft::board(tile, 1);
	}

	for (int i = 1; i < argc; ++i) {
		std::cout << boards[i] << "\n";
		std::cout << boards[i].hamming() << '\n';
		std::cout << boards[i].manhattan() << '\n';
	}

	exit(1);
	/*
	try {
		ft::board board(tiles, 1);
		assert(board == board);
		if (board.solvable())
			board.solve();
		else
			std::cerr << "Non solution to board:\n" << board;
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << '\n';
	}
	return 0;*/
}
