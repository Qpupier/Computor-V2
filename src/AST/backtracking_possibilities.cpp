/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking_possibilities.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:53:13 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/25 19:01:50 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "backtracking_possibilities.hpp"
#include "computor-v2.hpp"

static void	add_new_possibility(std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>>& results, const std::vector<Token>& tokens, t_parenthesis_data& data, const unsigned long int pos)
{
	std::vector<std::vector<std::pair<t_bracket, 	\
			std::pair<unsigned long int, unsigned long int>>>> result;

	result = backtracking_possibilities(tokens, data, pos);
	results.insert(results.end(), result.begin(), result.end());
}

static std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>>	try_absolute_brackets(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos)
{
	std::vector<std::vector<std::pair<t_bracket, 	\
			std::pair<unsigned long int, unsigned long int>>>> results;

	try
	{
		t_parenthesis_data	data_copy(data);

		data_copy.lasts.push_back(std::make_pair(E_BRACKET_LEFT_ABS, pos));
		add_new_possibility(results, tokens, data_copy, pos + 1);
	}
	catch (...) {}
	try
	{
		t_parenthesis_data	data_copy(data);

		if (data_copy.lasts.empty() 								\
				|| data_copy.lasts.back().first != E_BRACKET_LEFT_ABS)
			throw ERROR_BRACKETS;
		data_copy.pairs.push_back(std::make_pair(E_BRACKET_ABS, 	\
				std::make_pair(data_copy.lasts.back().second, pos)));
		data_copy.lasts.pop_back();
		add_new_possibility(results, tokens, data_copy, pos + 1);
	}
	catch (...) {}
	return (results);
}

static std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>>	parentheses_brackets(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos, Token::t_token token_type)
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

std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>>	backtracking_possibilities(const std::vector<Token>& tokens, t_parenthesis_data data = t_parenthesis_data({std::vector<std::pair<t_bracket, unsigned long int>>(), std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>()}), const unsigned long int pos = 0)
{
	std::vector<std::vector<std::pair<t_bracket, 						\
			std::pair<unsigned long int, unsigned long int>>>> results;
	Token::t_token	token_type;

	if (pos == tokens.size())
	{
		if (!data.lasts.empty())
			throw ERROR_BRACKETS;
		return (std::vector<std::vector<std::pair<t_bracket, std::pair	\
				<unsigned long int, unsigned long int>>>>(1, data.pairs));
	}
	token_type = tokens[pos].getType();
	if (token_type == Token::E_TOKEN_LEFT_PARENTHESIS 					\
			|| token_type == Token::E_TOKEN_RIGHT_PARENTHESIS)
		return (parentheses_brackets(tokens, data, pos, token_type));
	if (token_type == Token::E_TOKEN_PIPE)
	{
		results = try_absolute_brackets(tokens, data, pos);
		if (results.empty())
			throw ERROR_BRACKETS;
		return (results);
	}
	return (backtracking_possibilities(tokens, data, pos + 1));
}

t_possibility	get_possibility(const std::vector<Token>& initial_tokens, const std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>& pairs)
{
	std::vector<Token>		new_tokens;
	unsigned long long int	pos(0);

	for (const auto& token : initial_tokens)
	{
		if (token.getType() == Token::E_TOKEN_LEFT_PARENTHESIS || token.getType() == Token::E_TOKEN_RIGHT_PARENTHESIS || token.getType() == Token::E_TOKEN_PIPE)
		{
			bool	token_pushed(false);

			for (const auto& t : pairs)
			{
				if (t.second.first == pos)
				{
					new_tokens.push_back(Token(token.getValue(), token.getType() == Token::E_TOKEN_PIPE ? Token::E_TOKEN_LEFT_ABS : token.getType()));
					token_pushed = true;
					break;
				}
				if (t.second.second == pos)
				{
					new_tokens.push_back(Token(token.getValue(), token.getType() == Token::E_TOKEN_PIPE ? Token::E_TOKEN_RIGHT_ABS : token.getType()));
					token_pushed = true;
					break;
				}
			}
			if (!token_pushed)
				throw ERROR_BRACKETS;
		}
		else
			new_tokens.push_back(Token(token.getValue(), token.getType()));
		pos++;
	}
	return (t_possibility{new_tokens, pairs});
}

std::vector<t_possibility>	all_possibilities(const std::vector<Token>& initial_tokens)
{
	std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>>	possibilities;
	std::vector<t_possibility> results;

	possibilities = backtracking_possibilities(initial_tokens);
	for (const auto& pairs : possibilities)
		results.push_back(get_possibility(initial_tokens, pairs));
	return (results);
}
