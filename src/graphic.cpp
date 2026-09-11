/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 16:22:54 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/11 18:05:19 by qpupier          ###   ########lyon.fr   */
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

static std::ostream&			print_graphic_result(std::ostream& os, 	\
		std::vector<std::string> reference, const bool trigo)
{
	for (std::vector<std::string>::iterator it(reference.begin()); 	\
			it != reference.end(); it++)
		if (trigo)
			os << *it << std::endl;
		else
			os << std::endl << *it;
	return (os);
}

std::ostream&					display_graphic(std::ostream& os, 	\
		std::vector<std::pair<Rational, Rational>> points, 			\
		const bool func, const bool trigo)
{
	std::vector<std::string>	reference	\
			(trigo ? get_reference_trigo() : get_reference());
	unsigned long int			x;
	unsigned long int			y;
	Rational*					relative_x;
	Rational*					relative_y;

	for (std::vector<std::pair<Rational, Rational>>::iterator it	\
			(points.begin()); it != points.end(); it++)
	{
		relative_x = it->first * (trigo ? 8 : 4);
		relative_y = it->second * (trigo ? 10 : 2);
		x = static_cast<unsigned long>(GRAPHIC_WIDTH / 2 + 1 	\
				+ relative_x->to_int());
		y = static_cast<unsigned long>(GRAPHIC_HEIGHT / 2 		\
				- relative_y->to_int());
		if (it->first.to_int() > 0 && y != 1)
			x += 2;
		delete relative_x;
		delete relative_y;
		if (it->first > GRAPHIC_X_MIN && it->first < GRAPHIC_X_MAX 	\
				&& y >= 0 && y < GRAPHIC_HEIGHT && reference[y][x] == ' ')
			reference[y][x] = '*';
	}
	return (print_graphic_result(os, reference, func));
}
