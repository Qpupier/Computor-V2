/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:48:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/12 14:06:37 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static AST*	build_ast_recur(std::vector<Token> &tokens, long int& pos, 	\
		t_data& data, bool& must_return)
{
	std::vector<Token>	sub_tokens;
	std::vector<Token>*	adapted_tokens;

	sub_tokens = std::vector<Token>(tokens.begin() + 1, tokens.end() - 1);
	adapted_tokens = adapt_tokens(tokens, sub_tokens, &pos);
	if (adapted_tokens)
	{
		must_return = true;
		return (build_ast(*adapted_tokens, data));
	}
	if (pos == -1)
	{
		if (tokens.size() != 2)
			throw LogicError("Invalid expression format: operator expected");
		must_return = true;
		return (new AST(Token(tokens[0].getValue() + tokens[1].getValue(), 	\
				Token::E_FUNCTION), data));
	}
	return (nullptr);
}

static AST*	build_node(std::vector<Token>& left_tokens, 	\
		std::vector<Token>& right_tokens, Token& token, t_data &data)
{
	AST*	left_child;
	AST*	right_child;

	left_child = build_ast(left_tokens, data);
	right_child = build_ast(right_tokens, data);
	if (!left_child || !right_child)
	{
		if (left_child)
			delete left_child;
		if (right_child)
			delete right_child;
		throw LogicError("Invalid expression: empty parenthesis");
	}
	return (new AST(token, left_child, right_child, data));
}

AST*		build_ast(std::vector<Token> &tokens, t_data &data)
{
	std::vector<Token>	left_tokens;
	std::vector<Token>	right_tokens;
	long int			pos;
	bool				must_return(false);
	AST*				recur;

	if (tokens.empty())
		return (nullptr);
	if (tokens.size() == 1)
		return (new AST(tokens[0], data));
	recur = build_ast_recur(tokens, pos, data, must_return);
	if (must_return)
		return (recur);
	left_tokens = std::vector<Token>(tokens.begin(), tokens.begin() + pos);
	right_tokens = std::vector<Token>(tokens.begin() + pos + 1, tokens.end());
	return (build_node(left_tokens, right_tokens, 	\
			tokens[static_cast<unsigned long int>(pos)], data));
}
