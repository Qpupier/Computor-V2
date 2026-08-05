/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking_possibilities.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:53:13 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/05 12:21:14 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "backtracking_possibilities.hpp"
#include "computor-v2.hpp"

static void					push_bracket_token(						\
		std::vector<Token>& new_tokens, t_bracket bracket_type, 	\
		bool is_left, unsigned long int& increment)
{
	if (bracket_type == E_BRACKET_PARENTHESES)
		new_tokens.push_back(Token("(", is_left 	\
				? Token::E_TOKEN_LEFT_PARENTHESES 	\
				: Token::E_TOKEN_RIGHT_PARENTHESES));
	else if (bracket_type == E_BRACKET_NORM)
	{
		new_tokens.push_back(Token("||", is_left 	\
				? Token::E_TOKEN_LEFT_NORM 			\
				: Token::E_TOKEN_RIGHT_NORM));
		new_tokens.push_back(Token("", Token::E_TOKEN_TO_DELETE));
		increment++;
	}
	else if (bracket_type == E_BRACKET_ABS)
		new_tokens.push_back(Token("|", is_left 	\
				? Token::E_TOKEN_LEFT_ABS 			\
				: Token::E_TOKEN_RIGHT_ABS));
	else
		throw ERROR_UNKNOWN_BRACKET;
}

static t_possibility		get_possibility(	\
		const std::vector<Token>& tokens, const t_brackets_pairs& pairs)
{
	std::vector<Token>	result;

	for (std::vector<Token>::size_type i = 0; i < tokens.size(); i++)
		if (tokens[i].getType() == Token::E_TOKEN_LEFT_PARENTHESES 			\
				|| tokens[i].getType() == Token::E_TOKEN_RIGHT_PARENTHESES 	\
				|| tokens[i].getType() == Token::E_TOKEN_PIPE)
		{
			bool	token_pushed(false);

			for (const auto& pair : pairs)
				if (pair.second.first == i || pair.second.second == i)
				{
					push_bracket_token(result, pair.first, 					\
							pair.second.first == i, i);
					token_pushed = true;
					break;
				}
			if (!token_pushed)
				throw ERROR_BRACKETS;
		}
		else
			result.push_back(Token(tokens[i].getValue(), tokens[i].getType()));
	return (t_possibility{result, pairs});
}

void						add_new_possibility(							\
		t_list_brackets_pairs& results, const std::vector<Token>& tokens, 	\
		t_parentheses_data& data, const unsigned long int pos)
{
	t_list_brackets_pairs	result;

	result = backtracking_possibilities(tokens, data, pos);
	results.insert(results.end(), result.begin(), result.end());
}

t_list_brackets_pairs		backtracking_possibilities(						\
		const std::vector<Token>& tokens, 									\
		t_parentheses_data data 											\
			= t_parentheses_data({											\
				std::vector<std::pair<t_bracket, unsigned long int>>(), 	\
				t_brackets_pairs()}), 										\
		const unsigned long int pos = 0)
{
	Token::t_token	token_type;

	if (pos == tokens.size())
	{
		if (!data.lasts.empty())
			throw ERROR_BRACKETS;
		return (t_list_brackets_pairs(1, data.pairs));
	}
	token_type = tokens[pos].getType();
	if (token_type == Token::E_TOKEN_LEFT_PARENTHESES 	\
			|| token_type == Token::E_TOKEN_RIGHT_PARENTHESES)
		return (parentheses_brackets(tokens, data, pos, token_type));
	if (token_type == Token::E_TOKEN_PIPE)
		return (try_pipe_brackets(tokens, data, pos));
	return (backtracking_possibilities(tokens, data, pos + 1));
}

std::vector<t_possibility>	all_possibilities(	\
		const std::vector<Token>& initial_tokens)
{
	std::vector<t_possibility>	results;
	t_list_brackets_pairs		possibilities;
	bool						empty_brackets(false);

	possibilities = backtracking_possibilities(initial_tokens);
	for (const auto& pairs : possibilities)
	{
		for (const auto& pair : pairs)
			if (pair.second.first == pair.second.second - 1)
				empty_brackets = true;
		if (!empty_brackets)
			results.push_back(get_possibility(initial_tokens, pairs));
	}
	return (results);
}
