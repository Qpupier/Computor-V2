/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:48:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/25 15:32:54 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static t_bracket	remove_external_brackets(t_possibility& possibility, 	\
		std::vector<Token>& sub_tokens, 									\
		std::vector<std::pair<t_bracket, std::pair							\
			<unsigned long int, unsigned long int>>>::iterator 				\
			brackets_info, 													\
		t_bracket brackets_type)
{
	for (auto sub_pair = possibility.brackets_pairs.begin(); 	\
			sub_pair != possibility.brackets_pairs.end(); sub_pair++)
	{
		sub_pair->second.first--;
		sub_pair->second.second--;
	}
	possibility.tokens = sub_tokens;
	possibility.brackets_pairs.erase(brackets_info);
	return (brackets_type);
}

static t_bracket	test_external_brackets(t_possibility& p)
{
	std::vector<Token>	sub_tokens(p.tokens.begin() + 1, p.tokens.end() - 1);
	Token::t_token		first_token_type;
	Token::t_token		last_token_type;

	for (auto pair = p.brackets_pairs.begin(); 							\
			pair != p.brackets_pairs.end(); pair++)
		if (pair->second.first == 0 									\
				&& pair->second.second == p.tokens.size() - 1)
		{
			first_token_type = p.tokens.front().getType();
			last_token_type = p.tokens.back().getType();
			if (first_token_type == Token::E_TOKEN_LEFT_PARENTHESIS 	\
					&& last_token_type == Token::E_TOKEN_RIGHT_PARENTHESIS)
				return (remove_external_brackets(p, sub_tokens, 		\
						pair, E_BRACKET_PARENTHESIS));
			if (first_token_type == Token::E_TOKEN_LEFT_NORM 			\
					&& last_token_type == Token::E_TOKEN_RIGHT_NORM)
				return (remove_external_brackets(p, sub_tokens, 		\
						pair, E_BRACKET_NORM));
			if (first_token_type == Token::E_TOKEN_LEFT_ABS 			\
					&& last_token_type == Token::E_TOKEN_RIGHT_ABS)
				return (remove_external_brackets(p, sub_tokens, 		\
						pair, E_BRACKET_ABS));
		}
	return (E_BRACKET_NOTHING);
}

static AST*	handle_external_brackets(t_possibility& possibility, 	\
		t_bracket brackets_type, t_data& data)
{
	return (build_ast(possibility, data));
	// if (brackets_type == E_BRACKET_PARENTHESIS)
	// 	return (build_ast(possibility, data));
	// if (brackets_type == E_BRACKET_NORM)
	// 	return (new AST(new Function(NORM), build_ast(possibility, data), nullptr));
	// if (brackets_type == E_BRACKET_ABS)
	// 	return (new AST(new Function(ABSOLUTE), build_ast(possibility, data), nullptr));
	(void)brackets_type;
	throw UnexpectedError("Invalid bracket type");
}

static AST*	build_ast_recur(t_possibility& possibility, long long int& pos, 	\
		t_data& data, bool& must_return)
{
	t_bracket	external_brackets;

	must_return = true;
	external_brackets = test_external_brackets(possibility);
	if (external_brackets == E_BRACKET_PARENTHESIS 				\
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

static AST*	build_node(t_possibility& left_tokens, 	\
		t_possibility& right_tokens, Token& token, t_data &data)
{
	AST*	left_child;
	AST*	right_child;

	left_child = build_ast(left_tokens, data);
	right_child = build_ast(right_tokens, data);
	if ((token.getType() != Token::E_TOKEN_OPERATOR_INVERSE 	\
				&& !left_child) 								\
			|| !right_child)
	{
		delete left_child;
		delete right_child;
		throw LogicError("Invalid expression: empty parenthesis");
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

AST*		build_ast(t_possibility possibility, t_data& data)
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
