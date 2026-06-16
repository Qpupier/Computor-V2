/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tokens.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 16:17:56 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/16 16:17:45 by qpupier          ###   ########lyon.fr   */
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
		if (prev_type == Token::E_OPERATOR && current_type == Token::E_OPERATOR)
			throw LogicError("Two operators cannot be adjacent");
	}
}

static void	remove_whitespaces(std::vector<Token> &tokens)
{
	for (size_t i = 0; i < tokens.size();)
		if (tokens[i].getType() == Token::E_WHITESPACE)
			tokens.erase(tokens.begin() + static_cast<long>(i));
		else
			i++;
}

static bool	bad_sign_placement(const std::vector<Token> &tokens, size_t i)
{
	bool	prev_is_sign;
	bool	current_is_whitespace;

	prev_is_sign = tokens[i - 1].getValue() == "-" 							\
			|| tokens[i - 1].getValue() == "+";
	current_is_whitespace = tokens[i].getType() == Token::E_WHITESPACE;
	if ((i == 1 || tokens[i - 2].getType() == Token::E_LEFT_PARENTHESIS) 	\
			&& prev_is_sign && current_is_whitespace)
		return (true);
	return (i > 1 															\
			&& (i == 2 														\
				|| tokens[i - 3].getType() == Token::E_LEFT_PARENTHESIS) 	\
			&& tokens[i - 2].getType() == Token::E_WHITESPACE 				\
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
		if (i < size - 1 && current_type == Token::E_WHITESPACE)
		{
			next_type = tokens[i + 1].getType();
			if (prev_type != Token::E_OPERATOR 					\
					&& prev_type != Token::E_LEFT_PARENTHESIS 	\
					&& next_type != Token::E_OPERATOR 			\
					&& next_type != Token::E_RIGHT_PARENTHESIS 	\
					&& next_type != Token::E_QUESTION)
				throw LogicError("No space allowed without operator");
			if (prev_type == Token::E_OPERATOR_INVERSE)
				throw LogicError("No space allowed after inversed operator");
		}
		if (bad_sign_placement(tokens, i))
			throw LogicError("Invalid placement for sign operator");
	}
}

void		clean_tokens(std::vector<Token> &tokens, 	\
		const std::string line, const t_data &data)
{
	std::string::const_iterator	end(line.end());

	for (std::string::const_iterator start(line.begin()); start != end;)
		tokens.push_back(create_token(start, end, data));
	whitespaces_format_error(tokens);
	remove_whitespaces(tokens);
	semantic_verification(tokens);
	if (!tokens.empty() 	\
			&& tokens[tokens.size() - 1].getType() == Token::E_QUESTION)
		tokens.pop_back();
}
