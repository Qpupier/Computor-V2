/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:51:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/25 15:14:44 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Polynomial.hpp"

static std::string	new_operator(Token::t_token prev_token, 	\
		Token::t_token current_token)
{
	if (prev_token == Token::E_TOKEN_MATRIX && current_token == Token::E_TOKEN_MATRIX)
		return ("**");
	if ((prev_token == Token::E_TOKEN_RIGHT_PARENTHESIS 			\
				|| prev_token == Token::E_TOKEN_POLYNOMIAL 		\
				|| prev_token == Token::E_TOKEN_MATRIX 			\
				|| prev_token == Token::E_TOKEN_VECTOR) 			\
			&& (current_token == Token::E_TOKEN_LEFT_PARENTHESIS 	\
				|| current_token == Token::E_TOKEN_POLYNOMIAL 	\
				|| current_token == Token::E_TOKEN_MATRIX 		\
				|| current_token == Token::E_TOKEN_VECTOR))
		return ("***");
	return ("*");
}

static void			set_missing_operators(t_possibility &possibility)
{
	for (unsigned long int i = 1; i < possibility.tokens.size(); i++)
	{
		Token::t_token	prev_token;
		Token::t_token	current_token;

		prev_token = possibility.tokens[i - 1].getType();
		current_token = possibility.tokens[i].getType();
		if (prev_token != Token::E_TOKEN_OPERATOR 								\
				&& current_token != Token::E_TOKEN_OPERATOR 						\
				&& prev_token != Token::E_TOKEN_LEFT_PARENTHESIS 					\
				&& current_token != Token::E_TOKEN_RIGHT_PARENTHESIS 				\
				&& prev_token != Token::E_TOKEN_LEFT_ABS 							\
				&& current_token != Token::E_TOKEN_RIGHT_ABS 						\
				&& prev_token != Token::E_TOKEN_LEFT_NORM 						\
				&& current_token != Token::E_TOKEN_RIGHT_NORM 					\
				&& prev_token != Token::E_TOKEN_OPERATOR_INVERSE)
		{
			if (prev_token == Token::E_TOKEN_POLYNOMIAL 							\
					&& current_token == Token::E_TOKEN_LEFT_PARENTHESIS)
				possibility.tokens.insert(possibility.tokens.begin() + static_cast<long int>(i), 	\
						Token("<>", Token::E_TOKEN_OPERATOR));
			else
				possibility.tokens.insert(possibility.tokens.begin() + static_cast<long int>(i), 	\
						Token(new_operator(prev_token, current_token), 		\
						Token::E_TOKEN_OPERATOR));
			for (auto pair = possibility.brackets_pairs.begin(); pair != possibility.brackets_pairs.end(); pair++)
			{
				if (pair->second.first >= i)
					pair->second.first++;
				if (pair->second.second >= i)
					pair->second.second++;
			}
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
static std::vector<t_possibility>	get_tokens(const std::string &line, const t_data &data)
{
	std::vector<Token>	initial_tokens;
	std::string::const_iterator	end(line.end());

	for (std::string::const_iterator start(line.begin()); start != end;)
		initial_tokens.push_back(create_token(start, end, data));
	return (all_possibilities(initial_tokens));
}

AST*				compute_expression(const std::string &line, 	\
		t_data &data, bool is_right_side, const bool eval)
{
	std::vector<t_possibility>	possibilities;
	AST*				ast;
	AST*				tmp;
	unsigned long int	nb_possibilities(0);

	if (!std::regex_match(line, data.patterns.at(TOKEN_FULL_EXPRESSION)))
		throw ERROR_INVALID_EXPRESSION;
	possibilities = get_tokens(line, data);
	if (possibilities.size() > 1)
	{
		for (t_possibility &possibility : possibilities)
			try
			{
				clean_tokens(possibility);
				if (!is_right_side && !eval && set_function_left(possibility.tokens, data.stored))
					return (nullptr);
				set_missing_operators(possibility);
				tmp = build_ast(possibility, data);
				if (!tmp)
					throw ERROR_INVALID_EXPRESSION;
				ast = tmp;
				nb_possibilities++;
			}
			catch (...)
			{
			}
		if (nb_possibilities != 1)
			throw ERROR_NON_DETERMINISTIC_EXPRESSION;
	}
	else
	{
		clean_tokens(possibilities[0]);
		if (!is_right_side && !eval && set_function_left(possibilities[0].tokens, data.stored))
			return (nullptr);
		set_missing_operators(possibilities[0]);
		ast = build_ast(possibilities[0], data);
		if (!ast)
			throw ERROR_INVALID_EXPRESSION;
	}
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
