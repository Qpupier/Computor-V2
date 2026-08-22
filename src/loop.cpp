/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 14:30:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/22 13:29:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

void	history(std::smatch match, 	\
		const std::vector<std::string>& history_results)
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

void	stored_variables(const 		\
		std::map<std::pair<std::string, std::string>, const IType*>& stored)
{
	std::cout << COLOR_YELLOW											\
			<< "Listing stored variables and functions" << COLOR_RESET	\
			<< std::endl;//TODO: Print already defined functions and variables
	print_variables(stored, std::vector<std::string>({"pi", "e"}));
	print_variables(stored, std::vector<std::string>({"pi", "e"}), false);
}

int		loop(t_data &data, bool is_interactive)
{
	std::string		str_line;
	unsigned char	status;

	while (true)
	{
		status = read_line(is_interactive, str_line, data);
		if (status == CONTINUE)
			continue;
		if (status != NOTHING)
			return (status);
		compute_line(str_line, data);
	}
	if (is_interactive)
		rl_clear_history();
	return (EXIT_FAILURE);
}
