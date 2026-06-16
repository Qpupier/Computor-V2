/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_tokens.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:02:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/16 17:43:18 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"
#include "computor-v2.hpp"

static long int				select_operator(						\
		const std::vector<Token> &tokens, unsigned long int size, 	\
		const std::vector<std::string> &operators)
{
	unsigned long int	pos;
	unsigned int		depth;

	depth = 0;
	for (size_t i = 0; i < size; i++)
	{
		pos = size - i - 1;
		if (!depth)
			for (const std::string& op: operators)
				if (tokens[pos].getValue() == op)
					return (static_cast<long int>(pos));
		if (tokens[pos].getType() == Token::E_RIGHT_PARENTHESIS)
			depth++;
		else if (tokens[pos].getType() == Token::E_LEFT_PARENTHESIS)
			depth--;
	}
	return (-1);
}

static long int				select_less_priority_operator(	\
		const std::vector<Token> &tokens)
{
	unsigned long int	size;
	long int			pos;

	size = tokens.size();
	pos = select_operator(tokens, size, {"-", "+"});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {"*", "/", "%"});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {"**", "***"});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {"^"});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {TOKEN_OPERATOR_INVERSE});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {"<>"});
	if (pos != -1)
		return (pos);
	return (-1);
}

static bool					can_remove_external_parenthesis(	\
		const std::vector<Token> &tokens)
{
	Token::t_token	type;
	int				depth;

	depth = 0;
	for (const Token& token: tokens)
	{
		type = token.getType();
		if (type == Token::E_LEFT_PARENTHESIS)
			depth++;
		else if (type == Token::E_RIGHT_PARENTHESIS)
		{
			depth--;
			if (depth < 0)
				return (false);
		}
	}
	return (!depth);
}

static std::vector<Token>*	insert_token(std::vector<Token>* tokens, 	\
		long int* pos)
{
	std::string	first_value;

	first_value = (*tokens)[0].getValue();
	if (!*pos && (first_value == "-" || first_value == "+"))
	{
		if (first_value == "-")
		{
			(*tokens)[0].setValue(first_value + "1");
			(*tokens)[0].setType(Token::E_NUMBER);
			(*tokens).insert((*tokens).begin() + 1, 	\
					Token("*", Token::E_OPERATOR));
		}
		else
			(*tokens).erase((*tokens).begin());
		return (tokens);
	}
	throw LogicError	\
			("Operator cannot be at the beginning or end of an expression");
}

std::vector<Token>*			adapt_tokens(std::vector<Token> &tokens, 	\
		std::vector<Token> &sub_tokens, long int *pos)
{
	if (tokens[0].getType() == Token::E_LEFT_PARENTHESIS 				\
			&& tokens[tokens.size() - 1].getType() 						\
				== Token::E_RIGHT_PARENTHESIS 							\
			&& can_remove_external_parenthesis(sub_tokens))
		return (&sub_tokens);
	*pos = select_less_priority_operator(tokens);
	if (*pos < 0)
		return (nullptr);
	if ((!*pos || static_cast<unsigned long int>(*pos) 					\
				== tokens.size() - 1) 									\
			&& tokens[static_cast<unsigned long int>(*pos)].getType() 	\
				!= Token::E_OPERATOR_INVERSE)
		return (insert_token(&tokens, pos));
	return (nullptr);
}
