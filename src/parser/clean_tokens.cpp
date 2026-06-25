/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tokens.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 16:17:56 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/25 14:20:42 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static void	semantic_verification(const std::vector<Token>& tokens)
{
	Token::t_token	prev_type;
	Token::t_token	current_type;
	std::string		prev_token;
	std::string		current_token;

	for (size_t i = 1; i < tokens.size(); i++)
	{
		prev_type = i > 1 ? current_type : tokens[0].getType();
		current_type = tokens[i].getType();
		prev_token = i > 1 ? current_token : tokens[0].getValue();
		current_token = tokens[i].getValue();
		if (prev_type == Token::E_TOKEN_OPERATOR 	\
				&& current_type == Token::E_TOKEN_OPERATOR)
			throw LogicError("Two operators cannot be adjacent");
	}
}

static void	remove_whitespaces(t_possibility& possibility)
{
	for (std::vector<Token>::size_type i = 0; i < possibility.tokens.size();)
		if (possibility.tokens[i].getType() == Token::E_TOKEN_WHITESPACE)
		{
			for (auto pair = possibility.brackets_pairs.begin(); 	\
					pair != possibility.brackets_pairs.end(); pair++)
			{
				if (pair->second.first > i)
					pair->second.first--;
				if (pair->second.second > i)
					pair->second.second--;
			}
			possibility.tokens.erase(possibility.tokens.begin() 	\
					+ static_cast<std::vector<Token>::difference_type>(i));
		}
		else
			i++;
}

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
	if ((i == 1 || token_type == Token::E_TOKEN_LEFT_PARENTHESIS 		\
				|| token_type == Token::E_TOKEN_LEFT_NORM 				\
				|| token_type == Token::E_TOKEN_LEFT_ABS) 				\
			&& prev_is_sign && current_is_whitespace)
		return (true);
	if (i > 3)
		token_type = tokens[i - 3].getType();
	return (i > 1 														\
			&& (i == 2 													\
				|| token_type == Token::E_TOKEN_LEFT_PARENTHESIS 		\
				|| token_type == Token::E_TOKEN_LEFT_NORM 				\
				|| token_type == Token::E_TOKEN_LEFT_ABS) 				\
			&& tokens[i - 2].getType() == Token::E_TOKEN_WHITESPACE 	\
			&& prev_is_sign && current_is_whitespace);
}

static void	whitespaces_format_error(const std::vector<Token> &tokens)
{
	unsigned long int	size;
	Token::t_token		prev_type;
	Token::t_token		current_type;
	Token::t_token		next_type;

	size = tokens.size();
	for (size_t i = 1; i < size; i++)
	{
		prev_type = tokens[i - 1].getType();
		current_type = tokens[i].getType();
		if (i < size - 1 && current_type == Token::E_TOKEN_WHITESPACE)
		{
			next_type = tokens[i + 1].getType();
			if (prev_type != Token::E_TOKEN_OPERATOR 					\
					&& prev_type != Token::E_TOKEN_LEFT_PARENTHESIS 	\
					&& next_type != Token::E_TOKEN_OPERATOR 			\
					&& next_type != Token::E_TOKEN_RIGHT_PARENTHESIS 	\
					&& next_type != Token::E_TOKEN_QUESTION)
				throw LogicError("No space allowed without operator");
			if (prev_type == Token::E_TOKEN_OPERATOR_INVERSE)
				throw LogicError("No space allowed after inversed operator");
		}
		if (bad_sign_placement(tokens, i))
			throw LogicError("Invalid placement for sign operator");
	}
}

void		clean_tokens(t_possibility& possibility)
{
	whitespaces_format_error(possibility.tokens);
	remove_whitespaces(possibility);
	semantic_verification(possibility.tokens);
	if (!possibility.tokens.empty() 	\
			&& possibility.tokens[possibility.tokens.size() - 1].getType() 	\
				== Token::E_TOKEN_QUESTION)
		possibility.tokens.pop_back();
}
