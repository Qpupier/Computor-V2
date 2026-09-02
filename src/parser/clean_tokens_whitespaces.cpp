/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tokens_whitespaces.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:51:42 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/02 17:19:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static bool	bad_sign_placement(const std::vector<Token> &tokens, size_t i)
{
	Token::t_token	token_type(Token::E_TOKEN_ERROR);
	bool			prev_is_sign;
	bool			current_is_whitespace;

	prev_is_sign = tokens[i - 1].getValue() == "-" 						\
			|| tokens[i - 1].getValue() == "+";
	current_is_whitespace = tokens[i].getType() == Token::E_TOKEN_WHITESPACE;
	if (i > 2)
		token_type = tokens[i - 2].getType();
	if ((i == 1 || token_type == Token::E_TOKEN_LEFT_PARENTHESES 		\
				|| token_type == Token::E_TOKEN_LEFT_NORM 				\
				|| token_type == Token::E_TOKEN_LEFT_ABS) 				\
			&& prev_is_sign && current_is_whitespace)
		return (true);
	if (i > 3)
		token_type = tokens[i - 3].getType();
	return (i > 1 														\
			&& (i == 2 													\
				|| token_type == Token::E_TOKEN_LEFT_PARENTHESES 		\
				|| token_type == Token::E_TOKEN_LEFT_NORM 				\
				|| token_type == Token::E_TOKEN_LEFT_ABS) 				\
			&& tokens[i - 2].getType() == Token::E_TOKEN_WHITESPACE 	\
			&& prev_is_sign && current_is_whitespace);
}

static void	sides_verifications(Token::t_token prev_type, 	\
		Token::t_token next_type)
{
	if (prev_type != Token::E_TOKEN_OPERATOR 					\
			&& prev_type != Token::E_TOKEN_LEFT_PARENTHESES 	\
			&& next_type != Token::E_TOKEN_OPERATOR 			\
			&& next_type != Token::E_TOKEN_RIGHT_PARENTHESES 	\
			&& next_type != Token::E_TOKEN_QUESTION)
		throw LogicError("No space allowed without operator");
	if (prev_type == Token::E_TOKEN_OPERATOR_INVERSE)
		throw LogicError("No space allowed after inversed operator");
	if (next_type == Token::E_TOKEN_OPERATOR_FACTORIAL)
		throw LogicError("No space allowed before factorial operator");
}

void		remove_whitespaces(t_possibility& possibility)
{
	for (std::vector<Token>::size_type i = 0; i < possibility.tokens.size();)
		if (possibility.tokens[i].getType() == Token::E_TOKEN_WHITESPACE 	\
				|| possibility.tokens[i].getType() == Token::E_TOKEN_TO_DELETE)
		{
			brackets_pairs_decrement(possibility.brackets_pairs, i + 1);
			possibility.tokens.erase(possibility.tokens.begin() 			\
					+ static_cast<std::vector<Token>::difference_type>(i));
		}
		else
			i++;
}

void		whitespaces_format_error(const std::vector<Token> &tokens)
{
	unsigned long int	size;
	Token::t_token		current_type;

	size = tokens.size();
	for (size_t i = 1; i < size; i++)
	{
		current_type = tokens[i].getType();
		if (i < size - 1 && current_type == Token::E_TOKEN_WHITESPACE)
			sides_verifications(tokens[i - 1].getType(), 	\
					tokens[i + 1].getType());
		if (bad_sign_placement(tokens, i))
			throw LogicError("Invalid placement for sign operator");
	}
}
