/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tokens.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 16:17:56 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/07 13:50:42 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static void	successive_operators_verification(std::vector<Token>& tokens)
{
	std::string		prev_token;
	std::string		current_token;

	for (size_t i = 1; i < tokens.size(); i++)
	{
		prev_token = i > 1 ? tokens[i - 1].getValue() : tokens[0].getValue();
		current_token = tokens[i].getValue();
		if ((current_token == "+" || current_token == "-") 	\
				&& (prev_token == "+" || prev_token == "-"))
			throw ERROR_SUCCESSIVE_OPERATORS;
	}
}

static void	semantic_verification(std::vector<Token>& tokens)
{
	Token::t_token	prev_type;
	Token::t_token	current_type;
	std::string		prev_token;
	std::string		current_token;

	for (size_t i = 1; i < tokens.size(); i++)
	{
		prev_type = i > 1 ? current_type : tokens[0].getType();
		current_type = tokens[i].getType();
		prev_token = i > 1 ? tokens[i - 1].getValue() : tokens[0].getValue();
		current_token = tokens[i].getValue();
		if (prev_type == Token::E_TOKEN_OPERATOR)
		{
			if (current_token == "+" || current_token == "-")
			{
				tokens[i].setValue(prev_token + 	\
						(current_token == "-" ? "-" : ""));
				tokens[i - 1].setType(Token::E_TOKEN_TO_DELETE);
			}
			else if (current_type == Token::E_TOKEN_OPERATOR)
				throw ERROR_SUCCESSIVE_OPERATORS;
		}
	}
}

void		clean_tokens(t_possibility& possibility)
{
	whitespaces_format_error(possibility.tokens);
	remove_whitespaces(possibility);
	successive_operators_verification(possibility.tokens);
	semantic_verification(possibility.tokens);
	remove_whitespaces(possibility);
	if (!possibility.tokens.empty() 										\
			&& possibility.tokens[possibility.tokens.size() - 1].getType() 	\
				== Token::E_TOKEN_QUESTION)
		possibility.tokens.pop_back();
}
