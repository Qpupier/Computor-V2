/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_operator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:22:06 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/03 15:24:10 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static long long int	is_operator(const std::vector<Token>& tokens, 		\
		const std::vector<std::string> &operators, unsigned long int pos)
{
	for (const std::string& op: operators)
		if (tokens[pos].getValue() == op)
			return (static_cast<long long int>(pos));
	return (-1);
}

static bool				is_operator_outside_brackets(						\
			const t_brackets_pairs& brackets_pairs, Token::t_token type, 	\
			unsigned long int pos)
{
	for (const auto& pair: brackets_pairs)
		if (pos >= pair.second.first && pos <= pair.second.second)
			return (false);
	return (type == Token::E_TOKEN_OPERATOR 			\
			|| type == Token::E_TOKEN_OPERATOR_INVERSE 	\
			|| type == Token::E_TOKEN_OPERATOR_FACTORIAL);
}

static long long int	select_operator(const t_possibility& possibility, 	\
		const std::vector<std::string> &operators, const bool is_power = false)
{
	unsigned long int	pos;
	long long int		last_found(-1);
	long long int		found;
	Token::t_token		type;

	for (size_t i = 0; i < possibility.tokens.size(); i++)
	{
		pos = possibility.tokens.size() - i - 1;
		type = possibility.tokens[pos].getType();
		if (!is_operator_outside_brackets(possibility.brackets_pairs, 	\
				type, pos))
			continue ;
		found = is_operator(possibility.tokens, operators, pos);
		if (found != -1 && !is_power)
			return (found);
		if (found != -1)
			last_found = found;
		else if (last_found != -1)
			return (last_found);
	}
	return (last_found);
}

long long int			select_less_priority_operator(						\
		const t_possibility& possibility)
{
	long long int	pos;

	pos = select_operator(possibility, {"-", "+"});
	if (pos != -1)
		return (pos);
	pos = select_operator(possibility, {"*", "*-", "/", "/-", "%", "%-"});
	if (pos != -1)
		return (pos);
	pos = select_operator(possibility, {"**", "**-", "***"});
	if (pos != -1)
		return (pos);
	pos = select_operator(possibility, {"^", "^-"}, true);
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
