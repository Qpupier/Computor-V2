/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:48:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/17 17:19:13 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ast.hpp"

void	free_ast(Node *ast)
{
	if (!ast)
		return;
	free_ast(ast->getLeft());
	free_ast(ast->getRight());
	delete ast;
}

std::vector<Token>	find_next_parenthesis_group(const std::vector<Token> &tokens)
{
	std::vector<Token>::const_iterator	start;
	unsigned int						depth;

	depth = 0;
	for (unsigned long int i = 0; i < tokens.size(); i++)
		if (tokens[i].get_type() == Token::E_LEFT_PARENTHESIS)
		{
			if (!depth)
				start = tokens.begin() + static_cast<long>(i) + 1;
			depth++;
		}
		else if (tokens[i].get_type() == Token::E_RIGHT_PARENTHESIS)
		{
			depth--;
			if (!depth)
				return (std::vector<Token>(start, tokens.begin() + static_cast<long>(i)));
		}
	if (depth)
		throw std::logic_error("Mismatched parentheses");
	return (std::vector<Token>());
}

static long int	select_operator(const std::vector<Token> &tokens, unsigned long int size, const std::vector<std::string> &operators)
{
	unsigned int	depth;

	depth = 0;
	for (size_t i = 0; i < size; i++)
	{
		unsigned long int	pos;

		pos = size - i - 1;
		if (!depth)
			for (const std::string &op: operators)
				if (tokens[pos].get_token() == op)
					return (static_cast<long int>(pos));
		if (tokens[pos].get_type() == Token::E_RIGHT_PARENTHESIS)
			depth++;
		else if (tokens[pos].get_type() == Token::E_LEFT_PARENTHESIS)
			depth--;
	}
	return (-1);
}

static unsigned long int	select_less_priority_operator(const std::vector<Token> &tokens)
{
	unsigned long int	size;
	long int			pos;

	size = tokens.size();
	pos = select_operator(tokens, size, {"-", "+"});
	if (pos != -1)
		return (static_cast<unsigned long int>(pos));
	pos = select_operator(tokens, size, {"*", "/", "%"});
	if (pos != -1)
		return (static_cast<unsigned long int>(pos));
	pos = select_operator(tokens, size, {"**", "^"});
	if (pos != -1)
		return (static_cast<unsigned long int>(pos));
	throw std::logic_error("No operator found in the expression");
	return (0);
}

static bool	can_remove_external_parenthesis(const std::vector<Token> &tokens)
{
	int	depth;

	depth = 0;
	for (const Token &token : tokens)
	{
		if (token.get_type() == Token::E_LEFT_PARENTHESIS)
			depth++;
		else if (token.get_type() == Token::E_RIGHT_PARENTHESIS)
		{
			depth--;
			if (depth < 0)
				return (false);
		}
	}
	return (!depth);
}

static std::vector<Token>	*adapt_tokens(std::vector<Token> &tokens, std::vector<Token> &sub_tokens, unsigned long int *pos)
{
	if (tokens[0].get_type() == Token::E_LEFT_PARENTHESIS 							\
			&& tokens[tokens.size() - 1].get_type() == Token::E_RIGHT_PARENTHESIS 	\
			&& can_remove_external_parenthesis(sub_tokens))
		return (&sub_tokens);
	*pos = select_less_priority_operator(tokens);
	if (!*pos || *pos == tokens.size() - 1)
	{
		if (!*pos && (tokens[0].get_token() == "-" || tokens[0].get_token() == "+"))
		{
			if (tokens[0].get_token() == "-")
			{
				tokens[0].set_token(tokens[0].get_token() + "1");
				tokens[0].set_type(Token::E_NUMBER);
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

Node	*make_ast(std::vector<Token> &tokens)
{
	std::vector<Token>::const_iterator	tokens_begin;
	std::vector<Token>::const_iterator	tokens_end;
	std::vector<Token>::const_iterator	tokens_operator;
	std::vector<Token>					sub_tokens;
	std::vector<Token>					*adapted_tokens;
	Node 								*node;
	unsigned long int					pos;

	std::cout << "Group: "; for (size_t i = 0; i < tokens.size(); i++) std::cout << "\033[30m[\033[32m" << tokens[i].get_token() << "\033[30m]\033[0m"; std::cout << std::endl;// Debug
	tokens_begin = tokens.begin();
	tokens_end = tokens.end();
	if (tokens.empty())
		return (nullptr);
	if (tokens.size() == 1)
		return (new Node(tokens[0]));
	sub_tokens = std::vector<Token>(tokens_begin + 1, tokens_end - 1);
	adapted_tokens = adapt_tokens(tokens, sub_tokens, &pos);
	if (adapted_tokens)
		return (make_ast(*adapted_tokens));
	std::cout << "Operator: " << tokens[pos].get_token() << std::endl;// Debug
	node = new Node(tokens[pos]);
	tokens_operator = tokens_begin + static_cast<long>(pos);
	std::vector<Token> left_tokens(tokens_begin, tokens_operator);
	std::vector<Token> right_tokens(tokens_operator + 1, tokens_end);
	node->setLeft(make_ast(left_tokens));
	node->setRight(make_ast(right_tokens));
	return (node);
}
