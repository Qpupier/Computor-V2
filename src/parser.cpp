/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:51:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/05 12:08:03 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Polynomial.hpp"

static void			semantic_verification(const std::vector<Token> &tokens)
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
			throw LogicError("Two operators cannot be adjacent");
	}
}

static void			remove_whitespaces(std::vector<Token> &tokens)
{
	for (size_t i = 0; i < tokens.size();)
		if (tokens[i].getType() == Token::E_WHITESPACE)
			tokens.erase(tokens.begin() + static_cast<long>(i));
		else
			i++;
}

static bool			bad_sign_placement(const std::vector<Token> &tokens, size_t i)
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

static void			whitespaces_format_error(const std::vector<Token> &tokens)
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
			if (prev_type != Token::E_OPERATOR 					\
					&& prev_type != Token::E_LEFT_PARENTHESIS 	\
					&& next_type != Token::E_OPERATOR 			\
					&& next_type != Token::E_RIGHT_PARENTHESIS 	\
					&& next_type != Token::E_QUESTION)
				throw LogicError("No space allowed without operator");
		}
		if (bad_sign_placement(tokens, i))
			throw LogicError("Invalid placement for sign operator");
	}
}

static std::string	new_operator(Token::t_token prev_token, Token::t_token current_token)
{
	if (prev_token == Token::E_MATRIX && current_token == Token::E_MATRIX)
		return ("**");
	if ((prev_token == Token::E_RIGHT_PARENTHESIS || prev_token == Token::E_POLYNOMIAL || prev_token == Token::E_MATRIX) 	\
			&& (current_token == Token::E_LEFT_PARENTHESIS || current_token == Token::E_POLYNOMIAL || current_token == Token::E_MATRIX))
		return ("***");
	return ("*");
}

static bool			set_function_left(std::vector<Token> &tokens, std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>::iterator	it;
	std::pair<std::string, std::string>										pair;

	if (tokens.size() == 4 && tokens[0].getType() == Token::E_POLYNOMIAL 	\
			&& tokens[1].getType() == Token::E_LEFT_PARENTHESIS 			\
			&& tokens[2].getType() == Token::E_POLYNOMIAL 					\
			&& tokens[3].getType() == Token::E_RIGHT_PARENTHESIS)
	{
		pair.first = tokens[0].getValue();
		pair.second = tokens[2].getValue();
		for (it = stored.begin(); it != stored.end();)
		{
			if (to_lower(it->first.first) == to_lower(pair.first))
			{
				delete it->second;
				it = stored.erase(it);
			}
			else
				it++;
		}
		stored[pair] = nullptr;
		return (true);
	}
	return (false);
}

static void			set_function_right(std::map<std::pair<std::string, std::string>, const IType*> &stored, AST *ast)
{
	Polynomial*																polynomial;
	std::map<std::pair<std::string, std::string>, const IType*>::iterator	it;
	std::pair<std::string, std::string>										key;

	if (!ast->end_of_tree())
		throw LogicError("The right side of the function definition must be a single expression");
	polynomial = dynamic_cast<Polynomial*>(ast->getNode());
	if (!polynomial)
		throw LogicError("The right side of the equation must contain the variable of the function");
	for (it = stored.begin(); it != stored.end(); it++)
	{
		if (!it->second)
		{
			key = it->first;
			if (to_lower(key.second) != to_lower(polynomial->getName()))
			{
				stored.erase(key);
				throw LogicError("Function parameter does not match the variable in the right side of the equation");
			}
			it->second = polynomial->clone();
			std::cout << key.first << "(" << key.second << ") = " << *it->second << std::endl;
			break;
		}
	}
}

static void			set_missing_operators(std::vector<Token> &tokens)
{
	for (unsigned long int i = 1; i < tokens.size(); i++)
	{
		Token::t_token	prev_token;
		Token::t_token	current_token;

		prev_token = tokens[i - 1].getType();
		current_token = tokens[i].getType();
		if (prev_token != Token::E_OPERATOR && current_token != Token::E_OPERATOR && prev_token != Token::E_LEFT_PARENTHESIS && current_token != Token::E_RIGHT_PARENTHESIS)
		{
			if (prev_token == Token::E_POLYNOMIAL && current_token == Token::E_LEFT_PARENTHESIS)
				tokens.insert(tokens.begin() + static_cast<long int>(i), 	\
						Token("<>", Token::E_OPERATOR));
			else
				tokens.insert(tokens.begin() + static_cast<long int>(i), 	\
						Token(new_operator(prev_token, current_token), 		\
						Token::E_OPERATOR));
		}
	}
}

static Token		create_token(std::string::const_iterator &start, 	\
		const std::string::const_iterator &end, const t_data &data)
{
	std::vector<int>			token_positions({1});
	std::sregex_token_iterator	token_null;
	std::sregex_token_iterator	it(start, end, 	\
			data.patterns.at(TOKEN_NEXT), token_positions);

	if (it == token_null)
		std::__throw_regex_error(std::regex_constants::error_complexity, "Invalid format of TOKEN_NEXT regex");
	start = it->second;
	return Token(*it, get_token_type(*it, data.tokens_types));
}

static bool			waiting_function(const std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>::const_iterator	it(stored.begin());

	while (it != stored.end())
	{
		if (!it->second && !it->first.second.empty())
			return (true);
		it++;
	}
	return (false);
}

AST*				compute_expression(const std::string &line, 		\
		t_data &data, bool is_right_side)
{
	std::string::const_iterator	end(line.end());
	std::vector<Token>			tokens;
	AST							*ast;

	if (!std::regex_match(line, data.patterns.at(TOKEN_FULL_EXPRESSION)))
		throw ERROR_INVALID_EXPRESSION;
	for (std::string::const_iterator start(line.begin()); start != end;)
		tokens.push_back(create_token(start, end, data));
	whitespaces_format_error(tokens);
	remove_whitespaces(tokens);
	semantic_verification(tokens);
	if (tokens.size() && tokens[tokens.size() - 1].getType() == Token::E_QUESTION)
		tokens.pop_back();
	if (!is_right_side && set_function_left(tokens, data.stored))
		return (nullptr);
	set_missing_operators(tokens);
	ast = build_ast(tokens, data);
	if (!ast)
		throw ERROR_INVALID_EXPRESSION;
	if (is_right_side || !ast->end_of_tree())
		ast->reduce_expression(data.stored);
	if (is_right_side && waiting_function(data.stored))
		set_function_right(data.stored, ast);
	return (ast);
}
