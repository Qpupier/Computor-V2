/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:51:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/26 14:04:01 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
// #include "Variable.hpp"

static void	semantic_verification(const std::vector<Token> &tokens)
{
	Token::t_token	prev_type;
	Token::t_token	current_type;
	std::string		prev_token;
	std::string		current_token;

	for (size_t i = 1; i < tokens.size(); i++)
	{
		prev_type = i > 1 ? current_type : tokens[0].getType();
		current_type = tokens[i].getType();
		prev_token = i > 1 ? current_token : tokens[0].getValue();
		current_token = tokens[i].getValue();
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
	}
}

static void	remove_whitespaces(std::vector<Token> &tokens)
{
	for (size_t i = 0; i < tokens.size();)
		if (tokens[i].getType() == Token::E_WHITESPACE)
			tokens.erase(tokens.begin() + static_cast<long>(i));
		else
			i++;
}

static bool	bad_sign_placement(const std::vector<Token> &tokens, size_t i)
{
	bool	prev_is_sign;
	bool	current_is_whitespace;

	prev_is_sign = tokens[i - 1].getValue() == "-" || tokens[i - 1].getValue() == "+";
	current_is_whitespace = tokens[i].getType() == Token::E_WHITESPACE;
	if ((i == 1 || tokens[i - 2].getType() == Token::E_LEFT_PARENTHESIS) 	\
			&& prev_is_sign && current_is_whitespace)
		return (true);
	if (i > 1 && (i == 2 || tokens[i - 3].getType() == Token::E_LEFT_PARENTHESIS) 	\
			&& tokens[i - 2].getType() == Token::E_WHITESPACE && prev_is_sign && current_is_whitespace)
		return (true);
	return (false);
}

static void	whitespaces_format_error(const std::vector<Token> &tokens)
{
	unsigned long int	size;
	Token::t_token		prev_type;
	Token::t_token		current_type;
	Token::t_token		next_type;

	size = tokens.size();
	for (size_t i = 1; i < size; i++)
	{
		prev_type = tokens[i - 1].getType();
		current_type = tokens[i].getType();
		if (i < size - 1 && current_type == Token::E_WHITESPACE)
		{
			next_type = tokens[i + 1].getType();
			if ((prev_type != Token::E_OPERATOR && next_type == Token::E_LEFT_PARENTHESIS) 	\
					|| (prev_type == Token::E_RIGHT_PARENTHESIS && next_type != Token::E_OPERATOR && next_type != Token::E_QUESTION))
				throw std::logic_error("No space allowed without operator");
		}
		if (bad_sign_placement(tokens, i))
			throw std::logic_error("Invalid placement for sign operator");
	}
}

static std::string	new_operator(Token::t_token prev_token, Token::t_token current_token)
{
	if (prev_token == Token::E_MATRIX && current_token == Token::E_MATRIX)
		return ("**");
	if ((prev_token == Token::E_RIGHT_PARENTHESIS || prev_token == Token::E_VARIABLE || prev_token == Token::E_MATRIX) 	\
			&& (current_token == Token::E_LEFT_PARENTHESIS || current_token == Token::E_VARIABLE || current_token == Token::E_MATRIX))
		return ("***");
	return ("*");
}

static bool	test_function(std::vector<Token> &tokens, size_t pos, 	\
		const std::map<std::string, const IType*> &stored)
{
	// if (stored.find(tokens[pos - 1].getValue()) != stored.end() 	\
	// 		&& !dynamic_cast<const Variable*>(stored.at(tokens[pos - 1].getValue())))
	// {
	// 	tokens[pos - 1].setType(Token::E_FUNCTION);
	// 	return (true);
	// }
	(void)tokens;
	(void)pos;
	(void)stored;
	return (false);
}

void	set_missing_operators(std::vector<Token> &tokens, 	\
		const std::map<std::string, const IType*> &stored)
{
	for (unsigned long int i = 1; i < tokens.size(); i++)
	{
		Token::t_token	prev_token;
		Token::t_token	current_token;

		prev_token = tokens[i - 1].getType();
		current_token = tokens[i].getType();
		if (prev_token != Token::E_OPERATOR && current_token != Token::E_OPERATOR && prev_token != Token::E_LEFT_PARENTHESIS && current_token != Token::E_RIGHT_PARENTHESIS)
		{
			if (prev_token == Token::E_VARIABLE && current_token == Token::E_LEFT_PARENTHESIS && test_function(tokens, i, stored))
				continue;
			tokens.insert(tokens.begin() + static_cast<long int>(i), 	\
					Token(new_operator(prev_token, current_token), 		\
					Token::E_OPERATOR));
		}
	}
}

AST	*compute_expression(const std::string &line, 							\
		const std::map<std::string, std::regex> &patterns, 					\
		const std::map<const Token::t_token, std::regex> &tokens_types, 	\
		const std::map<std::string, const IType*> &stored)
{
	std::string::const_iterator	end(line.end());
	std::vector<Token>			tokens;
	std::sregex_token_iterator	token_null;
	std::vector<int>			token_positions({1});
	AST							*ast;

	if (!std::regex_match(line, patterns.at(TOKEN_FULL_EXPRESSION)))
		throw std::logic_error(ERROR_INVALID_EXPRESSION);
	// std::cerr << "\033[33mComputing expression: " << line << "\033[0m" << std::endl;// Debug
	for (std::string::const_iterator start(line.begin()); start != end;)
	{
		std::sregex_token_iterator	it(start, end, patterns.at(TOKEN_NEXT), token_positions);

		if (it == token_null)
			std::__throw_regex_error(std::regex_constants::error_complexity, "Invalid format of TOKEN_NEXT regex");
		start = it->second;
		tokens.push_back(Token(*it, get_token_type(*it, tokens_types)));
	}
	whitespaces_format_error(tokens);
	remove_whitespaces(tokens);
	semantic_verification(tokens);
	if (tokens[tokens.size() - 1].getType() == Token::E_QUESTION)
		tokens.pop_back();
	set_missing_operators(tokens, stored);
	ast = build_ast(tokens);
	ast->reduce_expression();
	return (ast);
}
