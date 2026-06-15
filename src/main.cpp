/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/15 14:59:46 by qpupier          ###   ########lyon.fr   */
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

static void	history(std::smatch match, 	\
		const std::vector<std::string> &history_results)
{
	HIST_ENTRY**		hist(history_list());
	bool				max(false);
	unsigned long int	n;
	unsigned long int	last;
	unsigned long int	first;

	if (match.length() > 1 && !match[1].str().empty())
	{
		n = std::stoul(match[1].str());
		max = true;
	}
	if (hist)
	{
		for (unsigned long int i = 0; hist[i] != NULL; ++i)
			last = i;
		if (!max)
			n = last + 1;
		first = last + 1 >= n ? last - n + 1 : 0;
		for (unsigned long int i(first); hist[i] != NULL; i++)
			std::cout << hist[i]->line << COLOR_DIM << " => " << COLOR_RESET << COLOR_ITALIC << history_results[i] 	\
					<< std::endl;
	}
}

static int	loop(t_data &data, bool is_interactive)
{
	std::smatch	match;
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
		if (std::regex_match(str_line, match, data.patterns.at(TOKEN_HISTORY)))
			history(match, data.history_results);
		else if (!str_line.empty())
		{
			add_history(line);
			compute_line(str_line, data);
		}
		free(line);
	}
	return (EXIT_FAILURE);
}

void		print_expression(const std::string &line, t_data &data)
{
	std::string	result;
	AST*		ast;

	ast = compute_expression(line, data, true);
	if (!ast)
		throw UnexpectedError											\
				("Unexpected error while computing the expression");
	if (!ast->end_of_tree())
	{
		delete ast;
		throw UnexpectedError("Unexpected error: the AST is not an expression");
	}
	result = ast->getNode()->to_string();
	std::cout << COLOR_BOLD << result << COLOR_RESET << std::endl;
	data.history_results.push_back(std::string(COLOR_GREEN) + result 	\
			+ std::string(COLOR_RESET));
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
