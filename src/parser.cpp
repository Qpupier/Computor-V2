/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:51:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/15 13:42:53 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Polynomial.hpp"

static std::string	new_operator(Token::t_token prev_token, 	\
		Token::t_token current_token)
{
	if (prev_token == Token::E_MATRIX && current_token == Token::E_MATRIX)
		return ("**");
	if ((prev_token == Token::E_RIGHT_PARENTHESIS 			\
				|| prev_token == Token::E_POLYNOMIAL 		\
				|| prev_token == Token::E_MATRIX) 			\
			&& (current_token == Token::E_LEFT_PARENTHESIS 	\
				|| current_token == Token::E_POLYNOMIAL 	\
				|| current_token == Token::E_MATRIX))
		return ("***");
	return ("*");
}

static void			set_missing_operators(std::vector<Token> &tokens)
{
	for (unsigned long int i = 1; i < tokens.size(); i++)
	{
		Token::t_token	prev_token;
		Token::t_token	current_token;

		prev_token = tokens[i - 1].getType();
		current_token = tokens[i].getType();
		if (prev_token != Token::E_OPERATOR 								\
				&& current_token != Token::E_OPERATOR 						\
				&& prev_token != Token::E_LEFT_PARENTHESIS 					\
				&& current_token != Token::E_RIGHT_PARENTHESIS)
		{
			if (prev_token == Token::E_POLYNOMIAL 							\
					&& current_token == Token::E_LEFT_PARENTHESIS)
				tokens.insert(tokens.begin() + static_cast<long int>(i), 	\
						Token("<>", Token::E_OPERATOR));
			else
				tokens.insert(tokens.begin() + static_cast<long int>(i), 	\
						Token(new_operator(prev_token, current_token), 		\
						Token::E_OPERATOR));
		}
	}
}

static bool			waiting_function(const 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>	\
			::const_iterator	it(stored.begin());

	while (it != stored.end())
	{
		if (!it->second && !it->first.second.empty())
			return (true);
		it++;
	}
	return (false);
}

Token				create_token(std::string::const_iterator &start, 	\
		const std::string::const_iterator &end, const t_data &data)
{
	std::vector<int>			token_positions({1});
	std::sregex_token_iterator	token_null;
	std::sregex_token_iterator	it(start, end, 	\
			data.patterns.at(TOKEN_NEXT), token_positions);

	if (it == token_null)
		std::__throw_regex_error(std::regex_constants::error_complexity, 	\
				"Invalid format of TOKEN_NEXT regex");
	start = it->second;
	return Token(*it, get_token_type(*it, data.tokens_types));
}

AST*				compute_expression(const std::string &line, 	\
		t_data &data, bool is_right_side, const bool eval)
{
	std::vector<Token>	tokens;
	AST					*ast;

	if (!std::regex_match(line, data.patterns.at(TOKEN_FULL_EXPRESSION)))
		throw ERROR_INVALID_EXPRESSION;
	clean_tokens(tokens, line, data);
	if (!is_right_side && !eval && set_function_left(tokens, data.stored))
		return (nullptr);
	set_missing_operators(tokens);
	ast = build_ast(tokens, data);
	if (!ast)
		throw ERROR_INVALID_EXPRESSION;
	try
	{
		if (is_right_side || !ast->end_of_tree())
			ast->reduce_expression(data.stored);
		if (is_right_side && !eval && waiting_function(data.stored))
			set_function_right(data, ast);
	}
	catch (...)
	{
		delete ast;
		throw;
	}
	return (ast);
}
