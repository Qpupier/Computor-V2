/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/15 11:16:51 by qpupier          ###   ########lyon.fr   */
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

static int	loop(t_data &data, bool is_interactive)
{
	std::string	str_line;
	char*		line;

	while (true)
	{
		line = readline(is_interactive ? "> " : "");
		if (!line || std::regex_match(std::string(line), 	\
				data.patterns.at(TOKEN_QUIT)))
		{
			free(line);
			rl_clear_history();
			return (EXIT_SUCCESS);
		}
		str_line = std::string(line);
		if (!str_line.empty())
			add_history(line);
		free(line);
		compute_line(str_line, data);
	}
	return (EXIT_FAILURE);
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
	t_data	data;
	int		status;

	if (argc > 1)
		return (usage());
	define_patterns(data.patterns);
	define_token_types(data.tokens_types);
	status = loop(data, isatty(STDIN_FILENO));
	free_stored(data.stored);
	return (status);
	(void)argv;
}
