/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_possibilities.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 17:04:19 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/05 12:23:13 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "backtracking_possibilities.hpp"
#include "AST.hpp"

static std::string	new_operator(Token::t_token prev_token, 			\
		Token::t_token current_token)
{
	if (prev_token == Token::E_TOKEN_MATRIX && current_token == Token::E_TOKEN_MATRIX)
		return ("**");
	if ((prev_token == Token::E_TOKEN_RIGHT_PARENTHESES 			\
				|| prev_token == Token::E_TOKEN_POLYNOMIAL 			\
				|| prev_token == Token::E_TOKEN_MATRIX 				\
				|| prev_token == Token::E_TOKEN_VECTOR) 			\
			&& (current_token == Token::E_TOKEN_LEFT_PARENTHESES 	\
				|| current_token == Token::E_TOKEN_POLYNOMIAL 		\
				|| current_token == Token::E_TOKEN_MATRIX 			\
				|| current_token == Token::E_TOKEN_VECTOR))
		return ("***");
	return ("*");
}

static inline bool	is_operator_missing(Token::t_token prev_token, 		\
		Token::t_token current_token)
{
	return (prev_token != Token::E_TOKEN_OPERATOR 					\
			&& current_token != Token::E_TOKEN_OPERATOR 			\
			&& prev_token != Token::E_TOKEN_LEFT_PARENTHESES 		\
			&& current_token != Token::E_TOKEN_RIGHT_PARENTHESES 	\
			&& prev_token != Token::E_TOKEN_LEFT_ABS 				\
			&& current_token != Token::E_TOKEN_RIGHT_ABS 			\
			&& prev_token != Token::E_TOKEN_LEFT_NORM 				\
			&& current_token != Token::E_TOKEN_RIGHT_NORM 			\
			&& prev_token != Token::E_TOKEN_OPERATOR_INVERSE);
}

static void			set_missing_operators(t_possibility &possibility)
{
	for (unsigned long int i = 1; i < possibility.tokens.size(); i++)
	{
		Token::t_token	prev_token(possibility.tokens[i - 1].getType());
		Token::t_token	current_token(possibility.tokens[i].getType());

		if (is_operator_missing(prev_token, current_token))
		{
			bool	is_function(prev_token == Token::E_TOKEN_POLYNOMIAL 	\
					&& current_token == Token::E_TOKEN_LEFT_PARENTHESES);

			possibility.tokens.insert(possibility.tokens.begin() 			\
					+ static_cast<long int>(i), Token(is_function 			\
						? "<>" 												\
						: new_operator(prev_token, current_token), 			\
					Token::E_TOKEN_OPERATOR));
			for (auto pair = possibility.brackets_pairs.begin(); 			\
					pair != possibility.brackets_pairs.end(); pair++)
			{
				if (pair->second.first >= i)
					pair->second.first++;
				if (pair->second.second >= i)
					pair->second.second++;
			}
		}
	}
}

static AST*			compute_possibility(t_possibility& possibility, 	\
		t_data &data, bool is_right_side, const bool eval)
{
	AST*	ast;

	clean_tokens(possibility);
	if (!is_right_side && !eval 	\
			&& set_function_left(possibility.tokens, data.stored))
		return (nullptr);
	set_missing_operators(possibility);
	ast = build_ast(possibility, data);
	if (!ast)
		throw ERROR_INVALID_EXPRESSION;
	return (ast);
}

AST*				get_the_only_possibility(							\
		std::vector<t_possibility>& possibilities, t_data &data, 		\
		bool is_right_side, const bool eval)
{
	unsigned long int	nb_possibilities(0);
	AST*				ast(nullptr);
	AST*				tmp;

	if (possibilities.empty())
		throw LogicError("Brackets (parentheses, absolute values or norms) \
				cannot be empty");
	if (possibilities.size() == 1)
		return (compute_possibility(possibilities[0], data, is_right_side, 	\
				eval));
	for (t_possibility &possibility : possibilities)
		try
		{
			tmp = compute_possibility(possibility, data, is_right_side, eval);
			if (tmp)
			{
				ast = tmp;
				nb_possibilities++;
			}
		}
		catch (...) {}
	if (nb_possibilities != 1)
		throw ERROR_NON_DETERMINISTIC_EXPRESSION;
	return (ast);
}
