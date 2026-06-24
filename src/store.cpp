/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 16:11:12 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/24 19:38:06 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Polynomial.hpp"

static void	store_new_variable(								\
		std::map<std::pair<std::string, std::string>, 		\
			const IType*> &stored, std::pair<std::string, 	\
		std::string> key)
{
	for (std::map<std::pair<std::string, std::string>, const IType*>	\
			::iterator it(stored.begin()); it != stored.end();)
	{
		if (to_lower(key.second) == to_lower(it->first.first))
			throw LogicError("Function parameter is already defined");
		if (to_lower(key.first) == to_lower(it->first.first))
		{
			delete it->second;
			it = stored.erase(it);
		}
		else
			it++;
	}
}

static void	store_function(t_data &data, 	\
		std::pair<std::string, std::string> key, Polynomial *polynomial)
{
	std::string	result;

	data.stored.erase(key);
	if (!polynomial->getName().empty() 										\
			&& to_lower(key.second) != to_lower(polynomial->getName()))
	{
		delete polynomial;
		delete_empty_function_stored(data.stored, 								\
			"Function parameter does not match the variable in the right side \
				of the equation", true);
	}
	key.first.erase(0, 1);
	try
	{
		store_new_variable(data.stored, key);
	}
	catch (const LogicError &e)
	{
		delete polynomial;
		throw;
	}
	polynomial->setName("χ");
	key.second = polynomial->getName();
	data.stored[key] = polynomial;
	result = key.first + "(" + key.second + ") = " + polynomial->to_string();
	std::cout << COLOR_BOLD << result << COLOR_RESET << std::endl;
	data.history_results.push_back(std::string(COLOR_GREEN) + result 	\
			+ std::string(COLOR_RESET));
}

bool		set_function_left(std::vector<Token> &tokens, 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::pair<std::string, std::string>	pair;

	if (tokens.size() == 4 && tokens[0].getType() == Token::E_TOKEN_POLYNOMIAL 	\
			&& tokens[1].getType() == Token::E_TOKEN_LEFT_PARENTHESIS 			\
			&& tokens[2].getType() == Token::E_TOKEN_POLYNOMIAL 					\
			&& tokens[3].getType() == Token::E_TOKEN_RIGHT_PARENTHESIS)
	{
		pair.first = "_" + tokens[0].getValue();
		pair.second = tokens[2].getValue();
		stored[pair] = nullptr;
		return (true);
	}
	return (false);
}

void		set_function_right(t_data &data, AST* ast)
{
	Polynomial*	polynomial;

	if (!ast->end_of_tree())
		delete_empty_function_stored(data.stored, 							\
				"The right side of the function definition must be a single \
					expression", true);
	polynomial = new Polynomial(*ast->getNode());
	for (std::map<std::pair<std::string, std::string>, const IType*>	\
			::iterator it(data.stored.begin()); it != data.stored.end(); it++)
		if (!it->second)
			return store_function(data, it->first, polynomial);
}

void		delete_empty_function_stored(				\
		std::map<std::pair<std::string, std::string>, 	\
			const IType*> &stored, 						\
		const std::string error_msg, const bool throw_error)
{
	std::map<std::pair<std::string, std::string>, const IType*>	\
			::iterator	it;

	for (it = stored.begin(); it != stored.end(); it++)
		if (!it->second)
		{
			it = stored.erase(it);
			break;
		}
	if (throw_error)
		throw LogicError(error_msg);
	std::cerr << error_msg << std::endl;
}
