/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/15 18:35:41 by qpupier          ###   ########lyon.fr   */
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
	unsigned long int	n;
	unsigned long int	last;
	unsigned long int	first;
	bool				max(false);

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
			std::cout << hist[i]->line << COLOR_DIM << " => " 				\
					<< COLOR_RESET << COLOR_ITALIC << history_results[i] 	\
					<< std::endl;
	}
}

static unsigned char	read_interactive(std::string &str_line, t_data &data)
{
	std::smatch	match;
	char*		line;

	line = readline("> ");
	if (!line)
		return (EXIT_SUCCESS);
	str_line = std::string(line);
	if (std::regex_match(str_line, data.patterns.at(TOKEN_WHITESPACE)))
	{
		free(line);
		return (CONTINUE);
	}
	if (std::regex_match(str_line, data.patterns.at(TOKEN_QUIT)))
	{
		free(line);
		return (EXIT_SUCCESS);
	}
	if (std::regex_match(str_line, match, data.patterns.at(TOKEN_HISTORY)))
	{
		history(match, data.history_results);
		return (CONTINUE);
	}
	add_history(line);
	free(line);
	return (NOTHING);
}

static unsigned char	read_tty(std::string &str_line, t_data &data)
{
	if (!std::getline(std::cin, str_line))
		return (EXIT_SUCCESS);
	if (std::regex_match(str_line, data.patterns.at(TOKEN_WHITESPACE)))
		return (CONTINUE);
	if (std::regex_match(str_line, data.patterns.at(TOKEN_QUIT)))
		return (EXIT_SUCCESS);
	if (std::regex_match(str_line, data.patterns.at(TOKEN_HISTORY)))
		return (CONTINUE);
	return (NOTHING);
}

static int	loop(t_data &data, bool is_interactive)
{
	std::string		str_line;
	unsigned char	status;

	while (true)
	{
		if (is_interactive)
			status = read_interactive(str_line, data);
		else
			status = read_tty(str_line, data);
		if (status == CONTINUE)
			continue;
		if (status != NOTHING)
		{
			rl_clear_history();
			return (status);
		}
		compute_line(str_line, data);
	}
	if (is_interactive)
		rl_clear_history();
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
