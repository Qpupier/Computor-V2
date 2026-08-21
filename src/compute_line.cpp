/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_line.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 15:48:19 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/21 17:20:29 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static bool	is_eval(std::string &line, t_data &data, long int *nb_equal)
{
	if (std::regex_match(line, data.patterns.at(TOKEN_EXPRESSION_EVAL)))
	{
		*nb_equal = 0;
		line.erase(line.find('='), line.size());
		return (true);
	}
	if (std::regex_match(line, data.patterns.at(TOKEN_EQUATION_EVAL)))
	{
		line.erase(line.find('?'), line.size());
		return (true);
	}
	return (false);
}

static void	parse_line(std::string &line, t_data &data)
{
	long int	nb_equal;
	bool		eval(false);

	nb_equal = std::count(line.begin(), line.end(), '=');
	if (nb_equal > 1)
		throw LogicError("Too many '=' in the expression");
	if (std::count(line.begin(), line.end(), '?') > 1)
		throw LogicError("Too many '?' in the expression");
	if (!std::regex_match(line, data.patterns.at(TOKEN_FULL)))
		throw ERROR_INVALID_EXPRESSION;
	if (nb_equal && is_eval(line, data, &nb_equal))
		eval = true;
	if (nb_equal)
		compute_equation(line, data, eval);
	else
		print_expression(line, data);
}

void		compute_line(std::string line, t_data &data)
{
	std::string	error;

	if (line.empty())
		return;
	try
	{
		parse_line(line, data);
	}
	catch(const std::regex_error& e)
	{
		error = std::string(COLOR_RED) + "Regex error: " 					\
				+ std::string(e.what()) + COLOR_RESET;
		std::cerr << error << std::endl;
		data.history_results.push_back(error);
	}
	catch(const std::exception& e)
	{
		delete_empty_function_stored(data.stored, e.what());
		data.history_results.push_back(std::string(COLOR_RED) + e.what() 	\
				+ COLOR_RESET);
	}
}
