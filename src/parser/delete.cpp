/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:45:33 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/11 16:54:12 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Polynomial.hpp"
#include "Operator.hpp"

static std::map<std::pair<std::string, std::string>, const IType*>		\
		::iterator	delete_specific_variable(							\
			std::map<std::pair<std::string, std::string>, const IType*>	\
			::iterator it, t_data &data)
{
	std::cout << COLOR_BOLD << "Deleted " 								\
			<< (it->first.second.empty() ? "variable" : "function") 	\
			<< ": " << it->first.first << COLOR_RESET << std::endl;
		delete it->second;
	return (data.stored.erase(it));
}

static void	delete_all_variables(const std::string &str_line, t_data &data)
{
	std::map<std::pair<std::string, std::string>, const IType*>::iterator	\
						it(data.stored.begin());
	unsigned long int	count(0);

	add_history(str_line.c_str());
	while (it != data.stored.end())
	{
		if (std::regex_match(it->first.first, std::regex(TOKEN_NO_VARIABLE)))
		{
			it++;
			continue ;
		}
		it = delete_specific_variable(it, data);
		count++;
	}
	add_history_result(data.history_results, std::to_string(count) 	\
			+ " variables or functions deleted");
}

static void	delete_variable(const std::string &str_line, t_data &data)
{
	std::string				variable;
	std::regex				regex(TOKEN_VARIABLE);
	std::sregex_iterator	re_it(str_line.begin(), str_line.end(), regex);
	std::map<std::pair<std::string, std::string>, const IType*>::iterator	\
							it(data.stored.begin());

	re_it++;
	variable = re_it->str();
	while (it != data.stored.end())
	{
		if (it->first.first != variable)
		{
			it++;
			continue ;
		}
		it = delete_specific_variable(it, data);
		add_history(str_line.c_str());
		add_history_result(data.history_results, 							\
				"Variable or function deleted");
	}
}

unsigned char	read_keyword_delete(const std::string &str_line, t_data &data)
{
	std::smatch	match;

	if (std::regex_match(str_line, data.patterns.at(TOKEN_DELETE_ALL)))
	{
		delete_all_variables(str_line, data);
		return (CONTINUE);
	}
	if (std::regex_match(str_line, data.patterns.at(TOKEN_DELETE)))
	{
		delete_variable(str_line, data);
		return (CONTINUE);
	}
	return (NOTHING);
}

void			verif_functions(AST* ast, t_data &data, const bool is_function)
{
	bool		is_next_function(false);
	Polynomial*	polynomial;
	Operator*	op;

	if (ast->getNode()->getType() == IType::E_TYPE_OPERATOR)
	{
		op = dynamic_cast<Operator*>(ast->getNode());
		if (op->getOperator() == Operator::E_FUNCTION)
			is_next_function = true;
	}
	if (ast->getLeft())
		verif_functions(ast->getLeft(), data, is_next_function);
	if (ast->getRight())
		verif_functions(ast->getRight(), data, is_next_function);
	if (!is_function && ast->getNode()->getType() == IType::E_TYPE_POLYNOMIAL)
	{
		polynomial = dynamic_cast<Polynomial*>(ast->getNode());
		for (std::map<std::pair<std::string, std::string>, const IType*>	\
				::iterator it(data.stored.begin()); 						\
				it != data.stored.end(); it++)
			if (to_lower(it->first.first) == to_lower(polynomial->getName()))
				throw LogicError("Variable "+ it->first.first 				\
						+ " is defined as a function,"						\
						" but is used as a variable");
	}
}
