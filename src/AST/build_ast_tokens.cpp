/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_tokens.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:02:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/11 18:22:45 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "backtracking_possibilities.hpp"

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
			brackets_pairs_increment(possibility.brackets_pairs, 0);
		}
		else
		{
			possibility.tokens.erase(possibility.tokens.begin());
			brackets_pairs_decrement(possibility.brackets_pairs, 0);
		}
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
	if ((!*pos || static_cast<unsigned long int>(*pos) 						\
				== possibility.tokens.size() - 1) 							\
			&& possibility.tokens[static_cast<unsigned long int>(*pos)]		\
				.getType() != Token::E_TOKEN_OPERATOR_INVERSE)
	{
		if (static_cast<unsigned long int>(*pos) 							\
					== possibility.tokens.size() - 1 						\
				&& possibility.tokens[static_cast<unsigned long int>(*pos)]	\
					.getType() == Token::E_TOKEN_OPERATOR_FACTORIAL)
			return (false);
		insert_token(possibility, pos);
		return (true);
	}
	return (false);
}
