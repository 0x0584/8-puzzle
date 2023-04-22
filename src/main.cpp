// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid <archid-@1337.student.ma>           +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/04/21 00:36:27 by archid            #+#    #+#             //
//   Updated: 2023/04/22 00:43:11 by archid           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

// ./n-puzzle file

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include "board.hpp"

#define BUFF_SIZE 128

int main(int argc, const char *argv[])
{
	if (argc != 2)
		exit(1);

	int fd = open(argv[1], O_RDONLY);
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
	std::vector<ft::board::tile_t> tiles;
	tiles.reserve(n);
	for (int i = 0; i < n; ++i) {
		ft::board::tile_t tile(n);
		ft::board::tile_t::iterator it = tile.begin();
		while (it != tile.end()) {
			*it++ = std::atoi(lines[i + 1].c_str());
			std::size_t pos = lines[i + 1].find(' ');
			if (pos != std::string::npos)
				lines[i + 1] = lines[i + 1].substr(pos + 1);
		}
		tiles.push_back(tile);
	}

	ft::board board(tiles, 0);
	std::cout << board.solvable() << "\n";
	if (board.solvable())
		board.solve();
	else
		std::cerr << "Non solution to board:\n" << board<< '\n';
	return 0;
}
