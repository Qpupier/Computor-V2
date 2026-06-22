/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking_possibilities.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:53:13 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/22 18:27:53 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "backtracking_possibilities.hpp"
#include "computor-v2.hpp"

std::vector<std::vector<std::pair<t_parenthesis, std::pair<unsigned long int, unsigned long int>>>>	backtracking_possibilities(const std::vector<Token>& tokens, t_parenthesis_data data = t_parenthesis_data({std::vector<std::pair<t_parenthesis, unsigned long int>>(), std::vector<std::pair<t_parenthesis, std::pair<unsigned long int, unsigned long int>>>()}), const unsigned long int pos = 0)
{
	std::cout << COLOR_YELLOW << "Backtracking at pos: " << pos << " / " << tokens.size() << COLOR_RESET << std::endl;
	std::cout << COLOR_BLUE << "Lasts: ";
	for (const auto& t : data.lasts)
		std::cout << t.first << " ";
	std::cout << COLOR_RESET << std::endl;
	// std::cout << COLOR_PINK << "All: ";
	// for (const auto& t : data.all)
	// 	std::cout << t << " ";
	// std::cout << COLOR_RESET << std::endl;
	// bool	norm_possible(false);

	// std::cout << COLOR_DIM << "-----------------------------------------------------" << COLOR_RESET << std::endl;
	// std::cout << COLOR_RED << "Backtracking operator at pos: " << pos << COLOR_RESET << std::endl;
	// std::cout << COLOR_YELLOW;
	// std::cout << "Last type: " << (data.lasts.empty() ? "None" : std::to_string(data.lasts.back())) << std::endl;
	// std::cout << COLOR_RESET;
	// std::cout << COLOR_DIM << "-----------------------------------------------------" << COLOR_RESET << std::endl;

	if (pos == tokens.size())
	{
		if (!data.lasts.empty())
		{
			for (const auto& t : data.lasts)
				std::cout << t.first << " ";
			std::cout << std::endl;
			std::cout << COLOR_RED << "Error 4: " << data.lasts.back().first << COLOR_RESET << std::endl;
			throw LogicError("Mismatched parentheses");
		}
		std::cout << COLOR_GREEN << "Backtracking completed successfully" << COLOR_RESET << std::endl;
		return (std::vector<std::vector<std::pair<t_parenthesis, std::pair<unsigned long int, unsigned long int>>>>(1, data.pairs));
	}
	if (tokens[pos].getType() == Token::E_LEFT_PARENTHESIS)
	{
		// data.all.push_back(E_LEFT_PARENTHESIS);
		data.lasts.push_back(std::make_pair(E_LEFT_PARENTHESIS, pos));
		return (backtracking_possibilities(tokens, data, pos + 1));
	}
	else if (tokens[pos].getType() == Token::E_RIGHT_PARENTHESIS)
	{
		if (data.lasts.empty() || data.lasts.back().first != E_LEFT_PARENTHESIS)
		{
			for (const auto& t : data.lasts)
				std::cout << t.first << " ";
			std::cout << std::endl;
			std::cout << COLOR_RED << "Error 5: " << data.lasts.back().first << COLOR_RESET << std::endl;
			throw LogicError("Mismatched parentheses");
		}
		// data.all.push_back(E_RIGHT_PARENTHESIS);
		data.lasts.pop_back();
		data.pairs.push_back(std::make_pair(E_PARENTHESIS, std::make_pair(data.lasts.back().second, pos)));
		return (backtracking_possibilities(tokens, data, pos + 1));
	}
	else if (tokens[pos].getType() == Token::E_PIPE)
	{
		std::vector<std::vector<std::pair<t_parenthesis, std::pair<unsigned long int, unsigned long int>>>> result;
		std::vector<std::vector<std::pair<t_parenthesis, std::pair<unsigned long int, unsigned long int>>>> results;
		// if (pos && tokens[pos - 1].getValue() == "|")
		// 	norm_possible = true;
		// try
		// {
		// 	// NORM entry
		// 	std::cout << COLOR_GREEN << "Test 1 (NORM entry)" << COLOR_RESET << std::endl;
		// 	if (!norm_possible)
		// 		throw LogicError("Mismatched pipes (absolute values and/or norms)");
		// 	data.lasts.push_back(E_NORM);
		// 	data.norm++;
		// 	results.push_back(pos > 1 ? backtracking_possibilities(tokens, data, pos - 2) : -1);
		// }
		// catch (...)
		// {
		// 	std::cout << COLOR_RED << "Not 1 (NORM entry)" << COLOR_RESET << std::endl;
		// }
		// try
		// {
		// 	// NORM exit
		// 	std::cout << COLOR_GREEN << "Test 2 (NORM exit)" << COLOR_RESET << std::endl;
		// 	if (!norm_possible || data.lasts.empty() || data.lasts.back() != E_NORM || !data.norm)
		// 		throw LogicError("Mismatched pipes (absolute values and/or norms)");
		// 	data.lasts.pop_back();
		// 	data.pairs.push_back(std::make_pair(E_NORM, std::make_pair(data.lasts.back().second, pos)));
		// 	data.norm--;
		// 	results.push_back(pos > 1 ? backtracking_possibilities(tokens, data, pos - 2) : -1);
		// }
		// catch (...)
		// {
		// 	std::cout << COLOR_RED << "Not 2 (NORM exit)" << COLOR_RESET << std::endl;
		// }
		try
		{
			// ABS entry
			t_parenthesis_data	data_copy(data);
			// data_copy.all.push_back(E_LEFT_ABS);
			data_copy.lasts.push_back(std::make_pair(E_LEFT_ABS, pos));
			result = backtracking_possibilities(tokens, data_copy, pos + 1);
			results.insert(results.end(), result.begin(), result.end());
		}
		catch (...)
		{
		}
		try
		{
			// ABS exit
			t_parenthesis_data	data_copy(data);
			if (data_copy.lasts.empty() || data_copy.lasts.back().first != E_LEFT_ABS)
			{
				std::cout << COLOR_RED << "Error 6" << COLOR_RESET << std::endl;
				throw LogicError("Mismatched pipes (absolute values and/or norms)");
			}
			// data_copy.all.push_back(E_RIGHT_ABS);
			data_copy.lasts.pop_back();
			data_copy.pairs.push_back(std::make_pair(E_ABS, std::make_pair(data_copy.lasts.back().second, pos)));
			std::cout << COLOR_GREEN << "Test 4 (ABS exit)" << COLOR_RESET << std::endl;
			result = backtracking_possibilities(tokens, data_copy, pos + 1);
			results.insert(results.end(), result.begin(), result.end());
		}
		catch (...)
		{
		}
		if (results.empty())
		{
			std::cout << COLOR_RED << "Error 3" << COLOR_RESET << std::endl;
			throw LogicError("Mismatched pipes (absolute values and/or norms)");
		}
		std::cout << COLOR_GREEN << "Backtracking for pipe at pos " << pos << " completed successfully with " << results.size() << " possibilities" << COLOR_RESET << std::endl;
		return (results);
	}
	return (backtracking_possibilities(tokens, data, pos + 1));
}

std::vector<std::vector<Token>>	all_possibilities(const std::vector<Token>& initial_tokens)
{
	std::vector<std::vector<std::pair<t_parenthesis, std::pair<unsigned long int, unsigned long int>>>>	possibilities;
	std::vector<std::vector<Token>> results;

	possibilities = backtracking_possibilities(initial_tokens);
	for (const auto& possibility : possibilities)
	{
		std::vector<Token> new_tokens;
		unsigned long int	pos = 0;
		
		for (const auto& token : initial_tokens)
		{
			bool	token_pushed(false);
			for (const auto& t : possibility)
				std::cout << COLOR_PINK << t.first << "|" << t.second.first << "," << t.second.second << " ";
			std::cout << COLOR_RESET << std::endl;
			for (const auto& t : possibility)
				if (t.second.first == pos)
				{
					new_tokens.push_back(Token(token.getValue(), token.getType() == Token::E_PIPE ? Token::E_LEFT_ABS : token.getType()));
					token_pushed = true;
				}
				else if (t.second.second == pos)
				{
					new_tokens.push_back(Token(token.getValue(), token.getType() == Token::E_PIPE ? Token::E_RIGHT_ABS : token.getType()));
					token_pushed = true;
				}
			if (!token_pushed)
			{
				if (token.getType() == Token::E_PIPE)
					std::cout << COLOR_RED << "Error 7" << COLOR_RESET << std::endl;
				new_tokens.push_back(Token(token.getValue(), token.getType()));
			}
			// if (token.getValue() == "|")
			// {
			// 	if (pos >= possibility.size())
			// 	{
			// 		std::cout << COLOR_RED << "Error 1" << COLOR_RESET << std::endl;
			// 		throw LogicError("Mismatched pipes (absolute values and/or norms)");
			// 	}
			// 	if (possibility[pos].first == E_LEFT_ABS)
			// 		new_tokens.push_back(Token("|", Token::E_LEFT_ABS));
			// 	else if (possibility[pos].first == E_RIGHT_ABS)
			// 		new_tokens.push_back(Token("|", Token::E_RIGHT_ABS));
			// 	else
			// 	{
			// 		for (const auto& t : possibility)
			// 			std::cout << t.first << " ";
			// 		std::cout << std::endl;
			// 		std::cout << COLOR_RED << "Error 2: " << possibility[pos].first << COLOR_RESET << std::endl;
			// 		throw LogicError("Mismatched pipes (absolute values and/or norms)");
			// 	}
			// }
			// else
			// 	new_tokens.push_back(token);
			if (token.getValue() == "(" || token.getValue() == ")" || token.getValue() == "|")
				pos++;
			if (new_tokens.back().getType() == Token::E_LEFT_NORM || new_tokens.back().getType() == Token::E_RIGHT_NORM)
				pos++;
		}
		results.push_back(new_tokens);
	}
	return (results);
}
