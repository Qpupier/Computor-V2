/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:48:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/25 12:37:14 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

static t_bracket	test_external_brackets(t_possibility& possibility)
{
	std::vector<Token>	sub_tokens(possibility.tokens.begin() + 1, possibility.tokens.end() - 1);

	for (auto pair = possibility.brackets_pairs.begin(); pair != possibility.brackets_pairs.end(); pair++)
	{
		if (pair->second.first == 0 && pair->second.second == possibility.tokens.size() - 1)
		{
			for (auto sub_pair = possibility.brackets_pairs.begin(); sub_pair != possibility.brackets_pairs.end(); sub_pair++)
			{
				sub_pair->second.first--;
				sub_pair->second.second--;
			}
			if (possibility.tokens.front().getType() == Token::E_TOKEN_LEFT_PARENTHESIS && possibility.tokens.back().getType() == Token::E_TOKEN_RIGHT_PARENTHESIS)
			{
				possibility.tokens = sub_tokens;
				possibility.brackets_pairs.erase(pair);
				return (E_BRACKET_PARENTHESIS);
			}
			if (possibility.tokens.front().getType() == Token::E_TOKEN_LEFT_NORM && possibility.tokens.back().getType() == Token::E_TOKEN_RIGHT_NORM)
			{
				possibility.tokens = sub_tokens;
				possibility.brackets_pairs.erase(pair);
				return (E_BRACKET_NORM);
			}
			if (possibility.tokens.front().getType() == Token::E_TOKEN_LEFT_ABS && possibility.tokens.back().getType() == Token::E_TOKEN_RIGHT_ABS)
			{
				possibility.tokens = sub_tokens;
				possibility.brackets_pairs.erase(pair);
				return (E_BRACKET_ABS);
			}
		}
	}
	return (E_BRACKET_NOTHING);
}

static AST*	build_ast_recur(t_possibility& possibility, long int& pos, 	\
		t_data& data, bool& must_return)
{
	std::vector<Token>*	first_token_completed;
	t_bracket	external_brackets;

	external_brackets = test_external_brackets(possibility);
	if (external_brackets == E_BRACKET_PARENTHESIS)
	{
		must_return = true;
		return (build_ast(possibility, data));
	}
	if (external_brackets == E_BRACKET_NORM)
	{
		must_return = true;
		return (build_ast(possibility, data));
		// return (new AST(new Function(NORM), build_ast(possibility, data), nullptr));//TODO
	}
	if (external_brackets == E_BRACKET_ABS)
	{
		must_return = true;
		return (build_ast(possibility, data));
		// return (new AST(new Function(ABSOLUTE), build_ast(possibility, data), nullptr));//TODO
	}
	first_token_completed = begin_by_operator(possibility.tokens, &pos);
	if (first_token_completed)
	{
		must_return = true;
		t_possibility	new_possibility = (t_possibility){*first_token_completed, possibility.brackets_pairs};
		return (build_ast(new_possibility, data));
	}
	if (pos == -1)
	{
		if (possibility.tokens.size() != 2)
			throw ERROR_OPERATOR_EXPECTED;
		must_return = true;
		return (new AST(Token(possibility.tokens[0].getValue() + possibility.tokens[1].getValue(), 	\
				Token::E_TOKEN_FUNCTION), data));
	}
	return (nullptr);
}

static AST*	build_node(t_possibility& left_tokens, 	\
		t_possibility& right_tokens, Token& token, t_data &data)
{
	AST*	left_child;
	AST*	right_child;

	left_child = build_ast(left_tokens, data);
	right_child = build_ast(right_tokens, data);
	if ((token.getType() != Token::E_TOKEN_OPERATOR_INVERSE && !left_child) 	\
			|| !right_child)
	{
		delete left_child;
		delete right_child;
		throw LogicError("Invalid expression: empty parenthesis");
	}
	std::cout << COLOR_YELLOW << "Building node with token: " << token.getValue() 	\
			<< COLOR_RESET << std::endl;
	return (new AST(token, left_child, right_child, data));
}

static t_possibility	separate_tokens(t_possibility& possibility, 	\
		unsigned long int start, unsigned long int pos)
{
	t_possibility	result;

	std::cout << COLOR_RED << "Separating tokens from " << start << " to " << pos - 1 << COLOR_RESET << std::endl;
	if (start >= pos || pos > possibility.tokens.size())
		throw LogicError("Invalid range for separate_tokens");
	result.tokens = std::vector<Token>(possibility.tokens.begin() + static_cast<long int>(start), possibility.tokens.begin() + static_cast<long int>(pos));
	for (auto pair = possibility.brackets_pairs.begin(); pair != possibility.brackets_pairs.end(); pair++)
	{
		std::cout << COLOR_CYAN << "Checking pair: " << pair->first << "|" << pair->second.first << "," << pair->second.second << " for range: " << start << "," << pos - 1 << COLOR_RESET << std::endl;
		if (pair->second.first >= start && pair->second.second < pos)
			result.brackets_pairs.push_back(std::make_pair(pair->first, std::make_pair(pair->second.first - start, pair->second.second - start)));
		else if ((pair->second.first >= start && pair->second.first < pos) || (pair->second.second >= start && pair->second.second < pos))
			throw LogicError("Mismatched brackets");
	}
	return (result);
}

AST*		build_ast(t_possibility& possibility, t_data& data)
{
	t_possibility	left_tokens;
	t_possibility	right_tokens;
	long int			pos;
	bool				must_return(false);
	AST*				recur;

	std::cout << COLOR_DIM << "Building AST with tokens: ";
	for (const auto& token : possibility.tokens)
		std::cout << token.getValue() << " ";
	std::cout << COLOR_RESET << std::endl;
	if (possibility.tokens.empty())
		return (nullptr);
	if (possibility.tokens.size() == 1)
	{
		std::cout << COLOR_YELLOW << "Building node with token: " << possibility.tokens[0].getValue() 	\
				<< COLOR_RESET << std::endl;
		return (new AST(possibility.tokens[0], data));
	}
	recur = build_ast_recur(possibility, pos, data, must_return);
	if (must_return)
		return (recur);
	if (pos < 0)
		throw ERROR_OPERATOR_EXPECTED;
	std::cout << COLOR_GREEN << "Pos: " << pos << COLOR_RESET << std::endl;
	left_tokens = separate_tokens(possibility, 0, static_cast<unsigned long int>(pos));
	right_tokens = separate_tokens(possibility, static_cast<unsigned long int>(pos) + 1, possibility.tokens.size());
	return (build_node(left_tokens, right_tokens, 	\
			possibility.tokens[static_cast<unsigned long int>(pos)], data));
}
