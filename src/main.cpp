/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/11 17:10:01 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static void	free_stored(const std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>::const_iterator	it(stored.begin());

	while (it != stored.end())
	{
		delete it->second;
		it++;
	}
}

static void	stored_varaiables(const std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>::const_iterator	it(stored.begin());

	std::cerr << "\033[33mListing stored variables and functions\033[0m" << std::endl;
	while (it != stored.end())
	{
		std::cerr << "\033[33m  ";
		if (!it->first.second.empty())
			std::cout << it->first.first << "(" << it->first.second << ")";
		else
			std::cout << it->first.first;
		std::cout << " = " << *it->second << "\033[0m" << std::endl;
		it++;
	}
}

static void	print_expression(const std::string &line, t_data &data)
{
	AST*	ast;

	ast = compute_expression(line, data, true);
	if (!ast)
		throw UnexpectedError("Unexpected error while computing the expression");
	if (!ast->end_of_tree())
	{
		delete ast;
		throw UnexpectedError("Unexpected error: the AST is not an expression");
	}
	std::cout << COLOR_BOLD << *ast->getNode() << COLOR_RESET << std::endl;
	ast->getNode()->print_variable("");
	delete ast;
}

static void	parse_line(const std::string &line, t_data &data)
{
	long int	nb_equal;

	nb_equal = std::count(line.begin(), line.end(), '=');
	if (nb_equal > 1)
		throw LogicError("Too many '=' in the expression");
	if (std::count(line.begin(), line.end(), '?') > 1)
		throw LogicError("Too many '?' in the expression");
	if (!std::regex_match(line, data.patterns.at(TOKEN_FULL)))
		throw ERROR_INVALID_EXPRESSION;
	if (nb_equal)
		compute_equation(line, data);
	else if (line.find('?') != std::string::npos)
		stored_varaiables(data.stored);
	else
		print_expression(line, data);
}

static void	compute_line(const std::string &line, t_data &data)
{
	if (line.empty())
		return;
	if (std::regex_match(line, data.patterns.at(TOKEN_LIST)))
		return stored_varaiables(data.stored);
	try
	{
		parse_line(line, data);
	}
	catch(const std::regex_error& e)
	{
		std::cerr << "\033[33mRegex error: " << e.what() << "\033[0m" << std::endl;
	}
	catch(const std::exception& e)
	{
		delete_empty_function_stored(data.stored, e.what());
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
	int			status;

	if (argc > 1)
		return (usage());
	define_patterns(data.patterns);
	define_token_types(data.tokens_types);
	status = loop(line, data, isatty(STDIN_FILENO));
	free_stored(data.stored);
	return (status);
	(void)argv;
}
