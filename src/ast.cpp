/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 13:42:09 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/16 18:17:20 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

std::vector<std::string>	find_next_parenthesis_group(const std::vector<std::string> &tokens)
{
	std::vector<std::string>::const_iterator	start;
	unsigned int	depth;

	depth = 0;
	for (unsigned long int i = 0; i < tokens.size(); i++)
		if (tokens[i] == "(")
		{
			if (!depth)
				start = tokens.begin() + static_cast<long>(i) + 1;
			depth++;
		}
		else if (tokens[i] == ")")
		{
			depth--;
			if (!depth)
				return (std::vector<std::string>(start, tokens.begin() + static_cast<long>(i)));
		}
	if (depth)
		throw std::logic_error("Mismatched parentheses");
	return (std::vector<std::string>());
}

static long int	select_operator(const std::vector<std::string> &tokens, unsigned long int size, const std::vector<std::string> &operators)
{
	unsigned int	depth;

	depth = 0;
	for (size_t i = 0; i < size; i++)
	{
		unsigned long int	pos;

		pos = size - i - 1;
		if (!depth)
			for (const std::string &op: operators)
				if (tokens[pos] == op)
					return (static_cast<long int>(pos));
		if (tokens[pos] == ")")
			depth++;
		else if (tokens[pos] == "(")
			depth--;
	}
	return (-1);
}

static unsigned long int	select_less_priority_operator(const std::vector<std::string> &tokens)
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

static bool	can_remove_external_parenthesis(const std::vector<std::string> &tokens)
{
	int	depth;

	depth = 0;
	for (const std::string &token : tokens)
	{
		if (token == "(")
			depth++;
		else if (token == ")")
		{
			depth--;
			if (depth < 0)
				return (false);
		}
	}
	return (!depth);
}

Node	*make_ast(std::vector<std::string> &tokens)
{
	std::vector<std::string>::const_iterator	tokens_begin;
	std::vector<std::string>::const_iterator	tokens_end;
	std::vector<std::string>::const_iterator	tokens_operator;
	std::vector<std::string>					sub_tokens;
	Node 										*node;
	unsigned long int							pos;

	std::cout << "Group: "; for (size_t i = 0; i < tokens.size(); i++) std::cout << "\033[30m[\033[32m" << tokens[i] << "\033[30m]\033[0m"; std::cout << std::endl;
	tokens_begin = tokens.begin();
	tokens_end = tokens.end();
	if (tokens.empty())
		return (nullptr);
	if (tokens.size() == 1)
		return (new Node(tokens[0]));
	sub_tokens = std::vector<std::string>(tokens_begin + 1, tokens_end - 1);
	if (tokens[0] == "(" && tokens[tokens.size() - 1] == ")" && can_remove_external_parenthesis(sub_tokens))
		return (make_ast(sub_tokens));
	pos = select_less_priority_operator(tokens);
	if (!pos || pos == tokens.size() - 1)
	{
		if (!pos && (tokens[0] == "-" || tokens[0] == "+"))
		{
			if (tokens[0] == "-")
			{
				tokens[0] += "1";
				tokens.insert(tokens.begin() + 1, "*");
			}
			else
				tokens.erase(tokens.begin());
			return (make_ast(tokens));
		}
		throw std::logic_error("Operator cannot be at the beginning or end of an expression");
	}
	std::cout << "Operator: " << tokens[pos] << std::endl;
	node = new Node(tokens[pos]);
	tokens_operator = tokens_begin + static_cast<long>(pos);
	std::vector<std::string> left_tokens(tokens_begin, tokens_operator);
	std::vector<std::string> right_tokens(tokens_operator + 1, tokens_end);
	node->setLeft(make_ast(left_tokens));
	node->setRight(make_ast(right_tokens));
	return (node);
}
