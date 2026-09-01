/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 13:24:59 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/01 14:19:57 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static unsigned char	read_keywords(const std::string &str_line, t_data &data)
{
	std::smatch	match;

	if (std::regex_match(str_line, data.patterns.at(TOKEN_QUIT)))
		return (EXIT_SUCCESS);
	if (std::regex_match(str_line, data.patterns.at(TOKEN_WHITESPACE)))
		return (CONTINUE);
	if (std::regex_match(str_line, match, data.patterns.at(TOKEN_LIST)))
	{
		stored_variables(data);
		return (CONTINUE);
	}
	if (std::regex_match(str_line, match, data.patterns.at(TOKEN_HISTORY)))
	{
		history(match, data.history_results);
		return (HISTORY);
	}
	return (NOTHING);
}

static unsigned char	read_interactive(std::string &str_line, t_data &data)
{
	std::smatch		match;
	unsigned char	status;
	char*			line;

	line = readline("> ");
	if (!line)
		return (EXIT_SUCCESS);
	str_line = std::string(line);
	status = read_keywords(str_line, data);
	if (status == CONTINUE)
	{
		free(line);
		return (CONTINUE);
	}
	if (status != NOTHING && status != HISTORY)
	{
		free(line);
		rl_clear_history();
		return (status);
	}
	add_history(line);
	free(line);
	return (status);
}

static unsigned char	read_tty(std::string &str_line, t_data &data)
{
	if (!std::getline(std::cin, str_line))
		return (EXIT_SUCCESS);
	return (read_keywords(str_line, data));
}

unsigned char			read_line(bool is_interactive, 	\
		std::string& str_line, t_data& data)
{
	unsigned char	status;

	if (is_interactive)
		status = read_interactive(str_line, data);
	else
		status = read_tty(str_line, data);
	return (status);
}
