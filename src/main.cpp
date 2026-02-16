/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/16 20:03:06 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

void	stored_variables(void)
{
	std::cerr << "\033[33mListing stored variables\033[0m" << std::endl;
}

void	compute_equation(const std::string &line)
{
	std::size_t	pos;

	pos = line.find('=');
	compute_expression(line.substr(0, pos));
	compute_expression(line.substr(pos + 1));
}

void	parse_line(const std::string &line)
{
	long int	nb_equal;

	nb_equal = std::count(line.begin(), line.end(), '=');
	if (nb_equal > 1)
		throw std::logic_error("Too many '=' in the expression");
	if (std::count(line.begin(), line.end(), '?') > 1)
		throw std::logic_error("Too many '?' in the expression");
	if (!std::regex_match(line, std::regex(TOKEN_FULL)))
		throw std::logic_error("Invalid expression format");
	if (nb_equal)
		compute_equation(line);
	else if (line.find('?') != std::string::npos)
		stored_variables();
	else
		compute_expression(line);
}

void	compute_line(const std::string &line)
{
	if (line.empty())
		return;
	if (std::regex_match(line, std::regex(TOKEN_LIST)))
		return stored_variables();
	try
	{
		parse_line(line);
		std::cout << "\033[32m  " << line << "\033[0m" << std::endl;
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
		compute_line(line);
	}
	(void)argv;
	return (0);
}
