/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 16:22:54 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/11 12:04:38 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "graphic.hpp"

static std::vector<std::string>	get_reference(void)
{
	std::vector<std::string>	array;

	array.push_back("┌──────────────────────────────────────────────────────────────────────────────┐");
	array.push_back("│                                      ^                                       │");
	array.push_back("│                                    5 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                    4 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                    3 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                    2 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                    1 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│──┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──>│");
	array.push_back("│ -9  -8  -7  -6  -5  -4  -3  -2  -1  0│   1   2   3   4   5   6   7   8   9   │");
	array.push_back("│                                   -1 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                   -2 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                   -3 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                   -4 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                   -5 ┼                                       │");
	array.push_back("└──────────────────────────────────────────────────────────────────────────────┘");
	return (array);
}

static std::vector<std::string>	get_reference_trigo(void)
{
	std::vector<std::string>	array;

	array.push_back("┌──────────────────────────────────────────────────────────────────────────────┐");
	array.push_back("│                                      ^                                       │");
	array.push_back("│                                    1 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                  0.5 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│──────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼──────>│");
	array.push_back("│     -4      -3      -2      -1      0│       1       2       3       4       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                 -0.5 ┼                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                      │                                       │");
	array.push_back("│                                   -1 ┼                                       │");
	array.push_back("└──────────────────────────────────────────────────────────────────────────────┘");
	return (array);
}

static void						print_graphic_result(std::ostream& os, 	\
		std::vector<std::string> reference, const bool trigo)
{
	for (std::vector<std::string>::iterator it(reference.begin()); 	\
			it != reference.end(); it++)
		if (trigo)
			os << *it << std::endl;
		else
			os << std::endl << *it;
}

std::ostream&					display_graphic(std::ostream& os, 	\
		std::vector<std::pair<Rational, Rational>> points, const bool trigo)
{
	std::vector<std::string>	reference(trigo 	\
			? get_reference_trigo() : get_reference());
	Rational*	relative_x;
	Rational*	relative_y;
	unsigned long int	x;
	unsigned long int	y;

	for (std::vector<std::pair<Rational, Rational>>::iterator it	\
			(points.begin()); it != points.end(); it++)
	{
		relative_x = it->first * (trigo ? 8 : 4);
		relative_y = it->second * (trigo ? 10 : 2);
		x = GRAPHIC_WIDTH / 2 + 1 	\
				+ static_cast<unsigned long>(relative_x->to_int());
		y = GRAPHIC_HEIGHT / 2 		\
				- static_cast<unsigned long>(relative_y->to_int());
		if (*relative_x > 0 && it->second <= 5)
			x += 2;
		delete relative_x;
		delete relative_y;
		if (it->first > GRAPHIC_X_MIN && it->first < GRAPHIC_X_MAX 	\
				&& y >= 0 && y < 24 && reference[y][x] == ' ')
			reference[y][x] = '*';
	}
	print_graphic_result(os, reference, trigo);
	return (os);
}
