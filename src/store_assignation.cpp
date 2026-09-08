/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_assignation.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 18:02:13 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/08 18:06:49 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

void		assignation_store(std::map<std::pair				\
			<std::string, std::string>, const IType*> &stored, 	\
		const std::string& var, const IType* result)
{
	std::pair<std::string, std::string>	pair(var, std::string());
	std::string							var_lower(to_lower(var));

	for (std::map<std::pair<std::string, std::string>, const IType*>	\
			::iterator it(stored.begin()); it != stored.end();)
		if (to_lower(it->first.first) == var_lower)
		{
			delete it->second;
			it = stored.erase(it);
		}
		else
			it++;
	stored[pair] = result;
}

void		assignation(std::string var, 				\
		std::map<std::pair<std::string, std::string>, 	\
			const IType*> &stored, 						\
		IType *result, const bool assign_last)
{

	if (!assign_last)
	{
		stop_preset_terms(var);
		std::cout << COLOR_BOLD << var << " = " << *result << COLOR_RESET 	\
				<< std::endl;
	}
	if (!result->in_Q())
	{
		std::cout << COLOR_BOLD << COLOR_DIM 								\
				<< "Real numbers cannot be stored as variables" 			\
				<< COLOR_RESET << std::endl;
		return ;
	}
	if (!assign_last)
		result->print_rounded(var);
	assignation_store(stored, var, result);
	if (!assign_last)
		assignation("last", stored, result->clone(), true);
}

void			assign_last(t_data& data, const IType* result)
{
	assignation("last", data.stored, result->clone(), true);
}
