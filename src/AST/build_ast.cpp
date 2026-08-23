/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:48:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/23 17:52:01 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static AST*				build_ast_recur(t_possibility& possibility, 	\
		long long int& pos, t_data& data, bool& must_return)
{
	t_bracket	external_brackets;

	must_return = true;
	external_brackets = test_external_brackets(possibility);
	if (external_brackets == E_BRACKET_PARENTHESES 				\
			|| external_brackets == E_BRACKET_NORM 				\
			|| external_brackets == E_BRACKET_ABS)
		return (handle_external_brackets(possibility, external_brackets, data));
	if (handle_operators(possibility, &pos))
		return (build_ast(possibility, data));
	if (pos == -1)
	{
		if (possibility.tokens.size() != 2)
			throw ERROR_OPERATOR_EXPECTED;
		return (new AST(Token(possibility.tokens[0].getValue() 	\
					+ possibility.tokens[1].getValue(), 		\
				Token::E_TOKEN_FUNCTION), data));
	}
	must_return = false;
	return (nullptr);
}

static AST*				build_node(t_possibility& left_tokens, 			\
		t_possibility& right_tokens, Token& token, t_data &data)
{
	AST*	left_child;
	AST*	right_child;

	left_child = build_ast(left_tokens, data);
	right_child = build_ast(right_tokens, data);
	if ((token.getType() != Token::E_TOKEN_OPERATOR_INVERSE 			\
				&& !left_child) 										\
			|| (token.getType() != Token::E_TOKEN_OPERATOR_FACTORIAL 	\
				&& !right_child))
	{
		delete left_child;
		delete right_child;
		throw LogicError("Invalid expression: empty parentheses");
	}
	return (new AST(token, left_child, right_child, data));
}

static t_possibility	separate_tokens(t_possibility& possibility, 	\
		unsigned long int start, unsigned long int end)
{
	t_possibility	result;

	if (start > end || end > possibility.tokens.size())
		throw UnexpectedError("Invalid range for separate_tokens");
	result.tokens = std::vector<Token>(possibility.tokens.begin() 			\
				+ static_cast<std::vector<Token>::difference_type>(start), 	\
			possibility.tokens.begin() 										\
				+ static_cast<std::vector<Token>::difference_type>(end));
	for (auto pair = possibility.brackets_pairs.begin(); 					\
			pair != possibility.brackets_pairs.end(); pair++)
	{
		if (pair->second.first >= start && pair->second.second < end)
			result.brackets_pairs.push_back(std::make_pair(pair->first, 	\
					std::make_pair(pair->second.first - start, 				\
						pair->second.second - start)));
		else if ((pair->second.first >= start && pair->second.first < end) 	\
				|| (pair->second.second >= start && pair->second.second < end))
			throw LogicError("Mismatched brackets");
	}
	return (result);
}

AST*					build_ast(t_possibility possibility, t_data& data)
{
	t_possibility	left_tokens;
	t_possibility	right_tokens;
	long long int	pos;
	bool			must_return(false);
	AST*			recur;

	if (possibility.tokens.empty())
		return (nullptr);
	if (possibility.tokens.size() == 1)
		return (new AST(possibility.tokens[0], data));
	recur = build_ast_recur(possibility, pos, data, must_return);
	if (must_return)
		return (recur);
	if (pos < 0)
		throw ERROR_OPERATOR_EXPECTED;
	left_tokens = separate_tokens(possibility, 		\
			0, static_cast<unsigned long int>(pos));
	right_tokens = separate_tokens(possibility, 	\
			static_cast<unsigned long int>(pos) + 1, possibility.tokens.size());
	return (build_node(left_tokens, right_tokens, 	\
			possibility.tokens[static_cast<unsigned long int>(pos)], data));
}
