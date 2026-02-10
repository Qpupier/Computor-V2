/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/10 15:51:07 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "computor-v2.hpp"

void	parse_line(const std::string &line)
{
	if (std::count(line.begin(), line.end(), '=') > 1)
		throw std::logic_error("Too many '=' in the expression");
	if (std::count(line.begin(), line.end(), '?') > 1)
		throw std::logic_error("Too many '?' in the expression");
	if (!std::regex_match(line, std::regex(TOKEN_FULL)))
		throw std::logic_error("Invalid expression format");
}

const std::string	compute_line(const std::string &line)
{
	parse_line(line);
	return (line);
}

int	main(int argc, const char **argv)
{
	if (argc > 1)
	{
		std::cerr << "Usage: ./computor-v2" << std::endl;
		return (1);
	}
	while (true)
	{
		std::string	line;

		std::cout << "> ";
		std::getline(std::cin, line);
		if (line == "quit")
			break;
		try
		{
			std::cout << "  " << compute_line(line) << std::endl;
		}
		catch(const std::regex_error& e)
		{
			std::cerr << "\033[33mRegex error: " << e.what() << "\033[0m" << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cerr << "\033[31m" << e.what() << "\033[0m" << std::endl;
		}
	}
	(void)argv;
	return (0);
}
