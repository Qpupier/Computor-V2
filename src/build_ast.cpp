/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:48:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/06 19:49:23 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static long int				select_operator(const std::vector<Token> &tokens, 	\
		unsigned long int size, const std::vector<std::string> &operators)
{
	unsigned long int	pos;
	unsigned int		depth;

	depth = 0;
	for (size_t i = 0; i < size; i++)
	{
		pos = size - i - 1;
		if (!depth)
			for (const std::string& op: operators)
				if (tokens[pos].getValue() == op)
					return (static_cast<long int>(pos));
		if (tokens[pos].getType() == Token::E_RIGHT_PARENTHESIS)
			depth++;
		else if (tokens[pos].getType() == Token::E_LEFT_PARENTHESIS)
			depth--;
	}
	return (-1);
}

static long int				select_less_priority_operator(const std::vector<Token> &tokens)
{
	unsigned long int	size;
	long int			pos;

	size = tokens.size();
	pos = select_operator(tokens, size, {"-", "+"});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {"*", "/", "%"});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {"**", "***"});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {"^"});
	if (pos != -1)
		return (pos);
	return (-1);
}

static bool					can_remove_external_parenthesis(const std::vector<Token> &tokens)
{
	Token::t_token	type;
	int				depth;

	depth = 0;
	for (const Token& token: tokens)
	{
		type = token.getType();
		if (type == Token::E_LEFT_PARENTHESIS)
			depth++;
		else if (type == Token::E_RIGHT_PARENTHESIS)
		{
			depth--;
			if (depth < 0)
				return (false);
		}
	}
	return (!depth);
}

static std::vector<Token>	*adapt_tokens(std::vector<Token> &tokens, 	\
		std::vector<Token> &sub_tokens, long int *pos)
{
	std::string	first_value;

	if (tokens[0].getType() == Token::E_LEFT_PARENTHESIS 							\
			&& tokens[tokens.size() - 1].getType() == Token::E_RIGHT_PARENTHESIS 	\
			&& can_remove_external_parenthesis(sub_tokens))
		return (&sub_tokens);
	*pos = select_less_priority_operator(tokens);
	if (*pos < 0)
		return (nullptr);
	if (!*pos || static_cast<unsigned long int>(*pos) == tokens.size() - 1)
	{
		first_value = tokens[0].getValue();
		if (!*pos && (first_value == "-" || first_value == "+"))
		{
			if (first_value == "-")
			{
				tokens[0].setValue(first_value + "1");
				tokens[0].setType(Token::E_NUMBER);
				tokens.insert(tokens.begin() + 1, Token("*", Token::E_OPERATOR));
			}
			else
				tokens.erase(tokens.begin());
			return (&tokens);
		}
		throw std::logic_error("Operator cannot be at the beginning or end of an expression");
	}
	return (nullptr);
}

static AST					*build_node(std::vector<Token> &tokens, 	\
		std::vector<Token>::const_iterator tokens_begin, 				\
		std::vector<Token>::const_iterator tokens_end, long int pos, 	\
		t_data &data)
{
	AST									*left_child;
	AST									*right_child;
	std::vector<Token>::const_iterator	tokens_operator;
	std::vector<Token>					left_tokens;
	std::vector<Token>					right_tokens;

	tokens_operator = tokens_begin + pos;
	left_tokens = std::vector<Token>(tokens_begin, tokens_operator);
	right_tokens = std::vector<Token>(tokens_operator + 1, tokens_end);
	left_child = build_ast(left_tokens, data);
	right_child = build_ast(right_tokens, data);
	if (!left_child || !right_child)
	{
		if (!right_child)
			delete left_child;
		throw std::logic_error("Invalid expression: empty parenthesis");
	}
	return (new AST(tokens[static_cast<unsigned long int>(pos)], left_child, right_child, data));
}

AST							*build_ast(std::vector<Token> &tokens, t_data &data)
{
	std::vector<Token>::const_iterator	tokens_begin;
	std::vector<Token>::const_iterator	tokens_end;
	std::vector<Token>					*adapted_tokens;
	std::vector<Token>					sub_tokens;
	long int							pos;

	tokens_begin = tokens.begin();
	tokens_end = tokens.end();
	if (tokens.empty())
		return (nullptr);
	if (tokens.size() == 1)
		return (new AST(tokens[0], data));
	sub_tokens = std::vector<Token>(tokens_begin + 1, tokens_end - 1);
	adapted_tokens = adapt_tokens(tokens, sub_tokens, &pos);
	if (adapted_tokens)
		return (build_ast(*adapted_tokens, data));
	if (pos == -1)
	{
		if (tokens.size() != 2)
			throw std::logic_error("Invalid expression format: operator expected");
		return (new AST(Token(tokens[0].getValue() + tokens[1].getValue(), Token::E_FUNCTION), data));
	}
	return (build_node(tokens, tokens_begin, tokens_end, pos, data));
}
