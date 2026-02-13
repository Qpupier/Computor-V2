/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:51:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/13 17:59:26 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// # include "computor-v2.hpp"
#include "AST.hpp"

static t_token	get_token_type(const std::string &token)
{
	if (std::regex_match(token, std::regex("^" TOKEN_VARIABLE "$")))
		return (E_VARIABLE);
	if (std::regex_match(token, std::regex("^" TOKEN_NUMBER "$")))
		return (E_NUMBER);
	if (std::regex_match(token, std::regex("^" TOKEN_IMAGINARY "$")))
		return (E_IMAGINARY);
	if (std::regex_match(token, std::regex("^" TOKEN_OPERATOR "$")))
		return (E_OPERATOR);
	if (std::regex_match(token, std::regex("^" TOKEN_MATRIX "$")))
		return (E_MATRIX);
	if (std::regex_match(token, std::regex("^" TOKEN_WHITESPACE "$")))
		return (E_WHITESPACE);
	if (token == "(")
		return (E_LEFT_PARENTHESIS);
	if (token == ")")
		return (E_RIGHT_PARENTHESIS);
	if (token == "?")
		return (E_QUESTION);
	return (E_ERROR);
}

static void	semantic_verification(const std::vector<std::string> &tokens)
{
	t_token	prev;
	t_token	current;

	for (size_t i = 1; i < tokens.size(); i++)
	{
		prev = i > 1 ? current : get_token_type(tokens[0]);
		current = get_token_type(tokens[i]);
		if (prev == E_OPERATOR && current == E_OPERATOR)
			throw std::logic_error("Two operators cannot be adjacent");
		if ((prev == E_MATRIX && current == E_IMAGINARY) || (prev == E_IMAGINARY && current == E_MATRIX))
			throw std::logic_error("Matrix and imaginary unit cannot multiply each other");
		if (tokens[i - 1] == "^" && (current == E_IMAGINARY || current == E_MATRIX))
			throw std::logic_error("Imaginary unit and matrix cannot be a power");
		if ((prev == E_IMAGINARY && tokens[i] == "%") || (tokens[i - 1] == "%" && current == E_IMAGINARY))
			throw std::logic_error("Imaginary unit cannot be used with modulus operator");
		if ((prev == E_MATRIX && tokens[i] == "%") || (tokens[i - 1] == "%" && current == E_MATRIX))
			throw std::logic_error("Matrix cannot be used with modulus operator");
		if ((tokens[i - 1] == "**" && (current == E_IMAGINARY || current == E_NUMBER))  \
				|| ((prev == E_VARIABLE || prev == E_IMAGINARY || prev == E_NUMBER) && tokens[i] == "**"))
			throw std::logic_error("Exponentiation operator cannot be used with imaginary unit, rational numbers or variables");
	}
}

static void	test_bounds(std::vector<std::string> &tokens)
{
	if (tokens[tokens.size() - 1] == "?")
		tokens.pop_back();
	if ((get_token_type(tokens[0]) == E_OPERATOR) 	\
			|| get_token_type(tokens[tokens.size() - 1]) == E_OPERATOR)
		throw std::logic_error("Expression cannot start or end with an operator");
}

static void	remove_whitespaces(std::vector<std::string> &tokens)
{
	for (size_t i = 0; i < tokens.size();)
		if (get_token_type(tokens[i]) == E_WHITESPACE)
			tokens.erase(tokens.begin() + static_cast<long>(i));
		else
			i++;
}

static void	test_whitespaces(const std::vector<std::string> &tokens)
{
	t_token	prev_token;
	t_token	next_token;

	for (size_t i = 1; i < tokens.size() - 1; i++)
		if (get_token_type(tokens[i]) == E_WHITESPACE)
		{
			prev_token = get_token_type(tokens[i - 1]);
			next_token = get_token_type(tokens[i + 1]);
			if (!((prev_token == E_OPERATOR || prev_token == E_LEFT_PARENTHESIS || prev_token == E_QUESTION) 	\
					^ (next_token == E_OPERATOR || next_token == E_RIGHT_PARENTHESIS || next_token == E_QUESTION)))
				throw std::logic_error("Invalid expression format2");
		}
}

void	compute_expression(const std::string &line)
{
	std::string::const_iterator	start(line.begin());
	std::regex					re(TOKEN_NEXT);
	std::vector<std::string>	tokens;

	if (!std::regex_match(line, std::regex(TOKEN_FULL_EXPRESSION)))
		throw std::logic_error("Invalid expression format1");
	std::cerr << "\033[33mComputing expression: " << line << "\033[0m" << std::endl;
	for (; start != line.end();)
	{
		std::sregex_token_iterator	it(start, line.end(), re, std::vector<int>{1});

		if (it == std::sregex_token_iterator())
			std::__throw_regex_error(std::regex_constants::error_complexity, "Invalid format of TOKEN_NEXT regex");
		start = it->second;
		tokens.push_back(*it);
	}
	test_whitespaces(tokens);
	remove_whitespaces(tokens);
	test_bounds(tokens);
	semantic_verification(tokens);
	make_ast(tokens);
}
