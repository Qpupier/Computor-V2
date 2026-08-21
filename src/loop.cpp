/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 14:30:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/21 19:57:07 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static void				history(std::smatch match, 	\
		const std::vector<std::string> &history_results)
{
	unsigned long int	first;
	unsigned long int	last(0);
	unsigned long int	n;
	bool				max(false);
	HIST_ENTRY**		hist(history_list());

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

static void				stored_variables(const 		\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::cout << COLOR_YELLOW											\
			<< "Listing stored variables and functions" << COLOR_RESET	\
			<< std::endl;//TODO: Print already defined functions and variables
	print_variables(stored, std::vector<std::string>({"pi", "e"}));
	print_variables(stored, std::vector<std::string>({"pi", "e"}), false);
}

static unsigned char	read_interactive(std::string &str_line, t_data &data)
{
	std::smatch	match;
	char*		line;

	line = readline("> ");
	if (!line)
		return (EXIT_SUCCESS);
	str_line = std::string(line);
	if (std::regex_match(str_line, data.patterns.at(TOKEN_QUIT)))
	{
		free(line);
		return (EXIT_SUCCESS);
	}
	if (std::regex_match(str_line, data.patterns.at(TOKEN_WHITESPACE)))
	{
		free(line);
		return (CONTINUE);
	}
	if (std::regex_match(str_line, match, data.patterns.at(TOKEN_LIST)))
	{
		stored_variables(data.stored);
		free(line);
		return (CONTINUE);
	}
	if (std::regex_match(str_line, match, data.patterns.at(TOKEN_HISTORY)))
	{
		history(match, data.history_results);
		free(line);
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

int						loop(t_data &data, bool is_interactive)
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
