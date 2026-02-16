/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:51:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/16 19:56:59 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Token.hpp"

static void	semantic_verification(const std::vector<Token> &tokens)
{
	Token::t_token	prev_type;
	Token::t_token	current_type;
	std::string		prev_token;
	std::string		current_token;

	for (size_t i = 1; i < tokens.size(); i++)
	{
		prev_type = i > 1 ? current_type : tokens[0].get_type();
		current_type = tokens[i].get_type();
		prev_token = i > 1 ? current_token : tokens[0].get_token();
		current_token = tokens[i].get_token();
		if (prev_type == Token::E_OPERATOR && current_type == Token::E_OPERATOR)
			throw std::logic_error("Two operators cannot be adjacent");
		if ((prev_type == Token::E_MATRIX && current_type == Token::E_IMAGINARY) || (prev_type == Token::E_IMAGINARY && current_type == Token::E_MATRIX))
			throw std::logic_error("Matrix and imaginary unit cannot multiply each other");
		if (prev_token == "^" && (current_type == Token::E_IMAGINARY || current_type == Token::E_MATRIX))
			throw std::logic_error("Imaginary unit and matrix cannot be a power");
		if ((prev_type == Token::E_IMAGINARY && current_token == "%") || (prev_token == "%" && current_type == Token::E_IMAGINARY))
			throw std::logic_error("Imaginary unit cannot be used with modulus operator");
		if ((prev_type == Token::E_MATRIX && current_token == "%") || (prev_token == "%" && current_type == Token::E_MATRIX))
			throw std::logic_error("Matrix cannot be used with modulus operator");
		if ((prev_token == "**" && (current_type == Token::E_IMAGINARY || current_type == Token::E_NUMBER))  \
				|| ((prev_type == Token::E_VARIABLE || prev_type == Token::E_IMAGINARY || prev_type == Token::E_NUMBER) && current_token == "**"))
			throw std::logic_error("Exponentiation operator cannot be used with imaginary unit, rational numbers or variables");
	}
}

static void	remove_whitespaces(std::vector<Token> &tokens)
{
	for (size_t i = 0; i < tokens.size();)
		if (tokens[i].get_type() == Token::E_WHITESPACE)
			tokens.erase(tokens.begin() + static_cast<long>(i));
		else
			i++;
}

static void	whitespaces_format_error(const std::vector<Token> &tokens)
{
	Token::t_token	prev_token;
	Token::t_token	next_token;

	for (size_t i = 1; i < tokens.size(); i++)
	{
		if (i < tokens.size() - 1 && tokens[i].get_type() == Token::E_WHITESPACE)
		{
			prev_token = tokens[i - 1].get_type();
			next_token = tokens[i + 1].get_type();
			if ((prev_token != Token::E_OPERATOR && next_token == Token::E_LEFT_PARENTHESIS) 	\
					|| (prev_token == Token::E_RIGHT_PARENTHESIS && next_token != Token::E_OPERATOR && next_token != Token::E_QUESTION))
				throw std::logic_error("No space allowed without operator");
		}
		if ((i > 1 && (i == 2 || tokens[i - 2].get_type() == Token::E_LEFT_PARENTHESIS) 									\
					&& (tokens[i - 1].get_token() == "-" || tokens[i - 1].get_token() == "+") && tokens[i].get_type() == Token::E_WHITESPACE) 	\
				|| (i > 2 && (i == 3 || tokens[i - 3].get_type() == Token::E_LEFT_PARENTHESIS) 							\
					&& tokens[i - 2].get_type() == Token::E_WHITESPACE && (tokens[i - 1].get_token() == "-" || tokens[i - 1].get_token() == "+") && tokens[i].get_type() == Token::E_WHITESPACE))
			throw std::logic_error("Espace sign");
	}
}

void	free_ast(Node *ast)
{
	if (!ast)
		return;
	free_ast(ast->getLeft());
	free_ast(ast->getRight());
	delete ast;
}

void	set_missing_operators(std::vector<Token> &tokens)
{
	for (unsigned long int i = 1; i < tokens.size(); i++)
	{
		Token::t_token	prev_token;
		Token::t_token	current_token;

		prev_token = tokens[i - 1].get_type();
		current_token = tokens[i].get_type();
		if (prev_token != Token::E_OPERATOR && current_token != Token::E_OPERATOR && prev_token != Token::E_LEFT_PARENTHESIS && current_token != Token::E_RIGHT_PARENTHESIS)
		{
			if (prev_token == Token::E_VARIABLE && current_token == Token::E_LEFT_PARENTHESIS)
				continue; // Functions?
			tokens.insert(tokens.begin() + static_cast<long int>(i), 											\
					Token((prev_token == Token::E_RIGHT_PARENTHESIS || prev_token == Token::E_VARIABLE || prev_token == Token::E_MATRIX) 	\
						&& (current_token == Token::E_LEFT_PARENTHESIS || current_token == Token::E_VARIABLE || current_token == Token::E_MATRIX) 	\
					? "**" : "*", Token::E_OPERATOR));
		}
	}
}

static void	get_token_types(std::map<const Token::t_token, std::regex> &tokens_types)
{
	tokens_types[Token::E_LEFT_PARENTHESIS] = std::regex("^\\($");
	tokens_types[Token::E_RIGHT_PARENTHESIS] = std::regex("^\\)$");
	tokens_types[Token::E_NUMBER] = std::regex("^" TOKEN_NUMBER "$");
	tokens_types[Token::E_IMAGINARY] = std::regex("^" TOKEN_IMAGINARY "$");
	tokens_types[Token::E_VARIABLE] = std::regex("^" TOKEN_VARIABLE "$");
	tokens_types[Token::E_OPERATOR] = std::regex("^" TOKEN_OPERATOR "$");
	tokens_types[Token::E_MATRIX] = std::regex("^" TOKEN_MATRIX "$");
	tokens_types[Token::E_WHITESPACE] = std::regex("^" TOKEN_WHITESPACE "$");
	tokens_types[Token::E_QUESTION] = std::regex("^" TOKEN_QUESTION "$");
}

void	compute_expression(const std::string &line)
{
	std::string::const_iterator	start(line.begin());
	std::regex					re(TOKEN_NEXT);
	std::vector<Token>	tokens;
	Node						*ast;
	std::map<const Token::t_token, std::regex>	tokens_types;

	get_token_types(tokens_types);
	if (!std::regex_match(line, std::regex(TOKEN_FULL_EXPRESSION)))
		throw std::logic_error("Invalid expression format1");
	std::cerr << "\033[33mComputing expression: " << line << "\033[0m" << std::endl;
	for (; start != line.end();)
	{
		std::sregex_token_iterator	it(start, line.end(), re, std::vector<int>{1});

		if (it == std::sregex_token_iterator())
			std::__throw_regex_error(std::regex_constants::error_complexity, "Invalid format of TOKEN_NEXT regex");
		start = it->second;
		tokens.push_back(Token(*it, get_token_type(*it, tokens_types)));
	}
	whitespaces_format_error(tokens);
	remove_whitespaces(tokens);
	semantic_verification(tokens);
	if (tokens[tokens.size() - 1].get_type() == Token::E_QUESTION)
		tokens.pop_back();
	set_missing_operators(tokens);
	ast = make_ast(tokens);
	free_ast(ast);
}
