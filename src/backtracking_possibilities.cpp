/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking_possibilities.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:53:13 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/25 14:13:28 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "backtracking_possibilities.hpp"
#include "computor-v2.hpp"

std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>>	backtracking_possibilities(const std::vector<Token>& tokens, t_parenthesis_data data = t_parenthesis_data({std::vector<std::pair<t_bracket, unsigned long int>>(), std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>()}), const unsigned long int pos = 0)
{
	if (pos == tokens.size())
	{
		if (!data.lasts.empty())
			throw LogicError("Mismatched parentheses");
		return (std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>>(1, data.pairs));
	}
	if (tokens[pos].getType() == Token::E_TOKEN_LEFT_PARENTHESIS)
	{
		data.lasts.push_back(std::make_pair(E_BRACKET_LEFT_PARENTHESIS, pos));
		return (backtracking_possibilities(tokens, data, pos + 1));
	}
	else if (tokens[pos].getType() == Token::E_TOKEN_RIGHT_PARENTHESIS)
	{
		if (data.lasts.empty() || data.lasts.back().first != E_BRACKET_LEFT_PARENTHESIS)
			throw LogicError("Mismatched parentheses");
		data.pairs.push_back(std::make_pair(E_BRACKET_PARENTHESIS, std::make_pair(data.lasts.back().second, pos)));
		data.lasts.pop_back();
		return (backtracking_possibilities(tokens, data, pos + 1));
	}
	else if (tokens[pos].getType() == Token::E_TOKEN_PIPE)
	{
		std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>> result;
		std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>> results;
		try
		{
			t_parenthesis_data	data_copy(data);

			data_copy.lasts.push_back(std::make_pair(E_BRACKET_LEFT_ABS, pos));
			result = backtracking_possibilities(tokens, data_copy, pos + 1);
			results.insert(results.end(), result.begin(), result.end());
		}
		catch (...)
		{
		}
		try
		{
			t_parenthesis_data	data_copy(data);

			if (data_copy.lasts.empty() || data_copy.lasts.back().first != E_BRACKET_LEFT_ABS)
				throw LogicError("Mismatched pipes (absolute values and/or norms)");
			data_copy.pairs.push_back(std::make_pair(E_BRACKET_ABS, std::make_pair(data_copy.lasts.back().second, pos)));
			data_copy.lasts.pop_back();
			result = backtracking_possibilities(tokens, data_copy, pos + 1);
			results.insert(results.end(), result.begin(), result.end());
		}
		catch (...)
		{
		}
		if (results.empty())
			throw LogicError("Mismatched pipes (absolute values and/or norms)");
		return (results);
	}
	return (backtracking_possibilities(tokens, data, pos + 1));
}

std::vector<t_possibility>	all_possibilities(const std::vector<Token>& initial_tokens)
{
	std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>>	possibilities;
	std::vector<t_possibility> results;

	possibilities = backtracking_possibilities(initial_tokens);
	for (const auto& pairs : possibilities)
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
						new_tokens.push_back(Token(token.getType() == Token::E_TOKEN_PIPE ? "[" : token.getValue(), token.getType() == Token::E_TOKEN_PIPE ? Token::E_TOKEN_LEFT_ABS : token.getType()));
						token_pushed = true;
						break;
					}
					else if (t.second.second == pos)
					{
						new_tokens.push_back(Token(token.getType() == Token::E_TOKEN_PIPE ? "]" : token.getValue(), token.getType() == Token::E_TOKEN_PIPE ? Token::E_TOKEN_RIGHT_ABS : token.getType()));
						token_pushed = true;
						break;
					}
				}
				if (!token_pushed)
					throw LogicError("Mismatched pipes (absolute values and/or norms)");//TODO: A adapter
			}
			else
				new_tokens.push_back(Token(token.getValue(), token.getType()));
			pos++;
		}
		results.push_back(t_possibility{new_tokens, pairs});
	}
	return (results);
}
