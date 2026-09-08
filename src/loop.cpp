/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 14:30:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/08 16:20:15 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static void	print_preset_functions(const 					\
		std::vector<std::pair<std::string, std::string>>& preset_functions)
{
	std::vector<std::pair<std::string, std::string>>::const_iterator	it(preset_functions.begin());

	while (it != preset_functions.end())
	{
		std::cout << "    " << it->first << " => " << it->second << std::endl;
		it++;
	}
}

static void	print_history_line(const std::string& line, 	\
		const std::string& result)
{
	std::cout << line;
	if (line != "history")
		std::cout << COLOR_DIM << " => " << COLOR_RESET 	\
				<< COLOR_ITALIC << result;
	std::cout << COLOR_RESET << std::endl;
}

void	history(std::smatch match, 							\
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
		for (unsigned long int i = 0; hist[i]; ++i)
			last = i;
		if (!max)
			n = last + 1;
		first = last + 1 >= n ? last - n + 1 : 0;
		for (unsigned long int i(first); hist[i]; i++)
			print_history_line(hist[i]->line, history_results[i]);
	}
	std::cout << "history" << std::endl;
}

void	stored_variables(const t_data& data)
{
	std::cout << COLOR_YELLOW << COLOR_BOLD 								\
			<< "Listing stored variables and functions" << COLOR_RESET 		\
			<< COLOR_YELLOW << std::endl 									\
			<< std::endl << "  Pre-set keywords:" 							\
			<< std::endl << "    delete <name> => Delete a user-defined"	\
				" variable or function" 									\
			<< std::endl << "    delete all => Delete all user-defined"		\
				" variables and functions"									\
			<< std::endl << "    history => Display the history of"			\
				" commands and results" 									\
			<< std::endl << "    quit => Exit the program" << std::endl 	\
			<< std::endl << "  Pre-set constants:" << std::endl;
	print_variables(data, true, true);
	std::cout << std::endl << "  Pre-set functions:" << std::endl;
	print_preset_functions(data.preset_functions);
	std::cout << std::endl << "  User-defined variables:" << std::endl;
	print_variables(data, false, true);
	std::cout << std::endl << "  User-defined functions:" << std::endl;
	print_variables(data, false, false);
	std::cout << COLOR_RESET << std::endl;
}

int		loop(t_data &data, bool is_interactive)
{
	std::string		str_line;
	unsigned char	status;

	while (true)
	{
		status = read_line(is_interactive, str_line, data);
		if (status == CONTINUE || status == HISTORY)
		{
			if (status == HISTORY)
				add_history_result(data.history_results, "");
			continue;
		}
		if (status != NOTHING)
			return (status);
		compute_line(str_line, data);
	}
	if (is_interactive)
		rl_clear_history();
	return (EXIT_FAILURE);
}
