/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_line.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 15:48:19 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/12 15:49:55 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static void	stored_variables(const 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>			\
			::const_iterator	it(stored.begin());

	std::cout 															\
			<< "\033[33mListing stored variables and functions\033[0m" 	\
			<< std::endl;
	while (it != stored.end())
	{
		std::cout << "\033[33m  ";
		if (!it->first.second.empty())
			std::cout << it->first.first << "(" << it->first.second << ")";
		else
			std::cout << it->first.first;
		std::cout << " = " << *it->second << "\033[0m" << std::endl;
		it++;
	}
}

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
	else if (line.find('?') != std::string::npos)
		stored_variables(data.stored);
	else
		print_expression(line, data);
}

void		compute_line(std::string &line, t_data &data)
{
	if (line.empty())
		return;
	if (std::regex_match(line, data.patterns.at(TOKEN_LIST)))
		return stored_variables(data.stored);
	try
	{
		parse_line(line, data);
	}
	catch(const std::regex_error& e)
	{
		std::cerr << "\033[33mRegex error: " << e.what() << "\033[0m" 	\
				<< std::endl;
	}
	catch(const std::exception& e)
	{
		delete_empty_function_stored(data.stored, e.what());
	}
}
