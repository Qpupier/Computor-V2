/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_tokens.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:02:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/31 17:27:44 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "computor-v2.hpp"
#include "backtracking_possibilities.hpp"

static long long int	select_operator(const t_possibility& possibility, 	\
		const std::vector<std::string> &operators)
{
	unsigned long int	pos;
	bool				surface;

	for (size_t i = 0; i < possibility.tokens.size(); i++)
	{
		pos = possibility.tokens.size() - i - 1;
		surface = true;
		for (const auto& pair : possibility.brackets_pairs)
			if (pos >= pair.second.first && pos <= pair.second.second)
				surface = false;
		if (surface)
			for (const std::string& op: operators)
				if (possibility.tokens[pos].getValue() == op)
					return (static_cast<long int>(pos));
	}
	return (-1);
}

static long long int	select_less_priority_operator(	\
		const t_possibility& possibility)// [ ] Attention, l'ordre est inverse dans 2^3^4
{
	long long int	pos;

	pos = select_operator(possibility, {"-", "+"});
	if (pos != -1)
		return (pos);
	pos = select_operator(possibility, {"*", "/", "%"});
	if (pos != -1)
		return (pos);
	pos = select_operator(possibility, {"**", "***"});
	if (pos != -1)
		return (pos);
	pos = select_operator(possibility, {"^"});
	if (pos != -1)
		return (pos);
	pos = select_operator(possibility, {TOKEN_OPERATOR_INVERSE});
	if (pos != -1)
		return (pos);
	pos = select_operator(possibility, {TOKEN_OPERATOR_FACTORIAL});
	if (pos != -1)
		return (pos);
	pos = select_operator(possibility, {"<>"});
	if (pos != -1)
		return (pos);
	return (-1);
}

static void		insert_token(t_possibility &possibility, long long int* pos)
{
	std::string	first_value;

	first_value = possibility.tokens[0].getValue();
	if (!*pos && (first_value == "-" || first_value == "+"))
	{
		if (first_value == "-")
		{
			possibility.tokens[0].setValue(first_value + "1");
			possibility.tokens[0].setType(Token::E_TOKEN_NUMBER);
			possibility.tokens.insert(possibility.tokens.begin() + 1, 	\
					Token("*", Token::E_TOKEN_OPERATOR));
			for (auto pair = possibility.brackets_pairs.begin(); 		\
					pair != possibility.brackets_pairs.end(); pair++)
			{
				pair->second.first++;
				pair->second.second++;
			}
		}
		else
			possibility.tokens.erase(possibility.tokens.begin());
	}
	else
		throw LogicError	\
				("Operator cannot be at the beginning or end of an expression");
}

bool			handle_operators(t_possibility& possibility, long long int *pos)
{
	*pos = select_less_priority_operator(possibility);
	if (*pos < 0)
		return (false);
	if ((!*pos || static_cast<unsigned long int>(*pos) 					\
				== possibility.tokens.size() - 1) 						\
			&& possibility.tokens[static_cast<unsigned long int>(*pos)]	\
				.getType() != Token::E_TOKEN_OPERATOR_INVERSE 			\
			&& possibility.tokens[static_cast<unsigned long int>(*pos)]	\
				.getType() != Token::E_TOKEN_OPERATOR_FACTORIAL)
	{
		insert_token(possibility, pos);
		return (true);
	}
	return (false);
}
