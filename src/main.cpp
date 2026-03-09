/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/09 13:50:47 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static void	stored_variables(const std::map<std::string, const IType*> &stored)
{
	std::map<std::string, const IType*>::const_iterator	it(stored.begin());

	std::cerr << "\033[33mListing stored variables\033[0m" << std::endl;
	while (it != stored.end())
	{
		std::cerr << "\033[33m  " << it->first << "\033[0m" << std::endl;
		it++;
	}
}

static void	compute_equation(const std::string &line, t_data &data)
{
	AST			*left_ast;
	AST			*right_ast;
	std::size_t	pos;

	pos = line.find('=');
	left_ast = compute_expression(line.substr(0, pos), data);
	right_ast = compute_expression(line.substr(pos + 1), data);
	delete left_ast;
	delete right_ast;
	// stored["test"] = new Variable("test", nullptr);// Debug
}

static void	parse_line(const std::string &line, t_data &data)
{
	AST			*ast;
	long int	nb_equal;

	nb_equal = std::count(line.begin(), line.end(), '=');
	if (nb_equal > 1)
		throw std::logic_error("Too many '=' in the expression");
	if (std::count(line.begin(), line.end(), '?') > 1)
		throw std::logic_error("Too many '?' in the expression");
	if (!std::regex_match(line, data.patterns.at(TOKEN_FULL)))
		throw ERROR_INVALID_EXPRESSION;
	if (nb_equal)
		compute_equation(line, data);
	else if (line.find('?') != std::string::npos)
		stored_variables(data.stored);
	else
	{
		ast = compute_expression(line, data);
		std::cout << *ast << std::endl;
		delete ast;
	}
}

static void	compute_line(const std::string &line, t_data &data)
{
	if (line.empty())
		return;
	if (std::regex_match(line, data.patterns.at(TOKEN_LIST)))
		return stored_variables(data.stored);
	try
	{
		parse_line(line, data);
	}
	catch(const std::regex_error& e)
	{
		std::cerr << "\033[33mRegex error: " << e.what() << "\033[0m" << std::endl;
	}
	catch(const std::logic_error& e)
	{
		std::cerr << "\033[31m" << e.what() << "\033[0m" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[31mUnexpected error: " << e.what() << "\033[0m" << std::endl;
	}
}

static int	loop(std::string &line, t_data &data, bool is_interactive)
{
	while (true)
	{
		if (is_interactive)
			std::cout << "> ";
		std::getline(std::cin, line);
		if (std::cin.bad())
		{
			std::cerr << "\033[31mError reading input\033[0m" << std::endl;
			return (EXIT_FAILURE);
		}
		if (line == "quit")
			return (EXIT_SUCCESS);
		compute_line(line, data);
		if (std::cin.eof())
			return (EXIT_SUCCESS);
	}
	
}

int			main(int argc, const char **argv)
{
	t_data		data;
	std::string	line;

	if (argc > 1)
		return (usage());
	define_patterns(data.patterns);
	define_token_types(data.tokens_types);
	return (loop(line, data, isatty(STDIN_FILENO)));
	(void)argv;
}
