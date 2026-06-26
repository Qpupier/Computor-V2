/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking_brackets.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 14:49:16 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/26 14:52:42 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "backtracking_possibilities.hpp"
#include "computor-v2.hpp"

static t_list_brackets_pairs	pipe_left_bracket(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos, t_bracket bracket_type)
{
	t_list_brackets_pairs	results;
	t_bracket				bracket;
	unsigned long int		increment;

	if (bracket_type == E_BRACKET_NORM)
	{
		bracket = E_BRACKET_LEFT_NORM;
		increment = 2;
	}
	else
	{
		bracket = E_BRACKET_LEFT_ABS;
		increment = 1;
	}
	data.lasts.push_back(std::make_pair(bracket, pos));
	add_new_possibility(results, tokens, data, pos + increment);
	return (results);
}

static t_list_brackets_pairs	pipe_right_bracket(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos, t_bracket bracket_type)
{
	t_list_brackets_pairs	results;
	t_bracket				bracket;
	unsigned long int		increment;

	if (bracket_type == E_BRACKET_NORM)
	{
		bracket = E_BRACKET_LEFT_NORM;
		increment = 2;
	}
	else
	{
		bracket = E_BRACKET_LEFT_ABS;
		increment = 1;
	}
	if (data.lasts.empty() || data.lasts.back().first != bracket)
		throw ERROR_BRACKETS;
	data.pairs.push_back(std::make_pair(bracket_type, 	\
			std::make_pair(data.lasts.back().second, pos)));
	data.lasts.pop_back();
	add_new_possibility(results, tokens, data, pos + increment);
	return (results);
}

static t_list_brackets_pairs	try_norm_absolute_brackets(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos, t_bracket bracket_type)
{
	t_list_brackets_pairs	result;
	t_list_brackets_pairs	results;

	try
	{
		result = pipe_left_bracket(tokens, data, pos, bracket_type);
		results.insert(results.end(), result.begin(), result.end());
	}
	catch (...) {}
	try
	{
		result = pipe_right_bracket(tokens, data, pos, bracket_type);
		results.insert(results.end(), result.begin(), result.end());
	}
	catch (...) {}
	return (results);
}

t_list_brackets_pairs	try_pipe_brackets(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos)
{
	t_list_brackets_pairs	results;

	try
	{
		if (pos + 1 >= tokens.size() 	\
				|| tokens[pos + 1].getType() != Token::E_TOKEN_PIPE)
			throw ERROR_BRACKETS;
		results = try_norm_absolute_brackets(tokens, data, pos, E_BRACKET_NORM);
		if (results.empty())
			throw ERROR_BRACKETS;
	}
	catch (...)
	{
		results = try_norm_absolute_brackets(tokens, data, pos, E_BRACKET_ABS);
		if (results.empty())
			throw ERROR_BRACKETS;
	}
	return (results);
}

t_list_brackets_pairs	parentheses_brackets(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos, Token::t_token token_type)
{
	if (token_type == Token::E_TOKEN_LEFT_PARENTHESIS)
	{
		data.lasts.push_back(std::make_pair(E_BRACKET_LEFT_PARENTHESIS, pos));
		return (backtracking_possibilities(tokens, data, pos + 1));
	}
	if (data.lasts.empty() 										\
			|| data.lasts.back().first != E_BRACKET_LEFT_PARENTHESIS)
		throw ERROR_BRACKETS;
	data.pairs.push_back(std::make_pair(E_BRACKET_PARENTHESIS, 	\
			std::make_pair(data.lasts.back().second, pos)));
	data.lasts.pop_back();
	return (backtracking_possibilities(tokens, data, pos + 1));
}
