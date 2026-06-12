/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/12 15:50:08 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static void	free_stored(const 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>	\
			::const_iterator	it(stored.begin());

	while (it != stored.end())
	{
		delete it->second;
		it++;
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
		if (std::regex_match(line, data.patterns.at(TOKEN_QUIT)))
			return (EXIT_SUCCESS);
		compute_line(line, data);
		if (std::cin.eof())
			return (EXIT_SUCCESS);
	}
	
}

void		print_expression(const std::string &line, t_data &data)
{
	AST*	ast;

	ast = compute_expression(line, data, true);
	if (!ast)
		throw UnexpectedError	\
				("Unexpected error while computing the expression");
	if (!ast->end_of_tree())
	{
		delete ast;
		throw UnexpectedError("Unexpected error: the AST is not an expression");
	}
	std::cout << COLOR_BOLD << *ast->getNode() << COLOR_RESET << std::endl;
	ast->getNode()->print_rounded();
	delete ast;
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
