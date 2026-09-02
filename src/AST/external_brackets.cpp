/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_brackets.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 17:12:46 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/02 17:17:01 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "DefinedFunction.hpp"

static t_bracket	remove_external_brackets(t_possibility& possibility, 	\
		std::vector<Token>& sub_tokens, 									\
		std::vector<std::pair<t_bracket, std::pair							\
			<unsigned long int, unsigned long int>>>::iterator 				\
			brackets_info, 													\
		t_bracket brackets_type)
{
	for (auto sub_pair = possibility.brackets_pairs.begin(); 	\
			sub_pair != possibility.brackets_pairs.end(); sub_pair++)
	{
		sub_pair->second.first--;
		sub_pair->second.second--;
	}
	possibility.tokens = sub_tokens;
	possibility.brackets_pairs.erase(brackets_info);
	return (brackets_type);
}

t_bracket			test_external_brackets(t_possibility& p)
{
	std::vector<Token>	sub_tokens(p.tokens.begin() + 1, p.tokens.end() - 1);
	Token::t_token		first_token_type;
	Token::t_token		last_token_type;

	for (auto pair = p.brackets_pairs.begin(); 							\
			pair != p.brackets_pairs.end(); pair++)
		if (pair->second.first == 0 									\
				&& pair->second.second == p.tokens.size() - 1)
		{
			first_token_type = p.tokens.front().getType();
			last_token_type = p.tokens.back().getType();
			if (first_token_type == Token::E_TOKEN_LEFT_PARENTHESES 	\
					&& last_token_type == Token::E_TOKEN_RIGHT_PARENTHESES)
				return (remove_external_brackets(p, sub_tokens, 		\
						pair, E_BRACKET_PARENTHESES));
			if (first_token_type == Token::E_TOKEN_LEFT_NORM 			\
					&& last_token_type == Token::E_TOKEN_RIGHT_NORM)
				return (remove_external_brackets(p, sub_tokens, 		\
						pair, E_BRACKET_NORM));
			if (first_token_type == Token::E_TOKEN_LEFT_ABS 			\
					&& last_token_type == Token::E_TOKEN_RIGHT_ABS)
				return (remove_external_brackets(p, sub_tokens, 		\
						pair, E_BRACKET_ABS));
		}
	return (E_BRACKET_NOTHING);
}

AST*				handle_external_brackets(t_possibility& possibility, 	\
		t_bracket brackets_type, t_data& data)
{
	if (brackets_type == E_BRACKET_PARENTHESES)
		return (build_ast(possibility, data));
	if (brackets_type == E_BRACKET_NORM)
		return (new AST(													\
				new DefinedFunction(DefinedFunction::E_FUNCTION_NORM), 		\
				build_ast(possibility, data), nullptr));
	if (brackets_type == E_BRACKET_ABS)
		return (new AST(													\
				new DefinedFunction(DefinedFunction::E_FUNCTION_ABSOLUTE), 	\
				build_ast(possibility, data), nullptr));
	throw ERROR_UNKNOWN_BRACKET;
}

void				brackets_pairs_decrement(std::vector<std::pair			\
			<t_bracket, std::pair<unsigned long int, unsigned long int>>>& 	\
			brackets_pairs, 												\
		unsigned long int start)
{
	for (auto pair = brackets_pairs.begin(); pair != brackets_pairs.end(); 	\
			pair++)
	{
		if (pair->second.first >= start)
			pair->second.first--;
		if (pair->second.second >= start)
			pair->second.second--;
	}
}

void				brackets_pairs_increment(std::vector<std::pair			\
			<t_bracket, std::pair<unsigned long int, unsigned long int>>>& 	\
			brackets_pairs, 												\
		unsigned long int start)
{
	for (auto pair = brackets_pairs.begin(); pair != brackets_pairs.end(); 	\
			pair++)
	{
		if (pair->second.first >= start)
			pair->second.first++;
		if (pair->second.second >= start)
			pair->second.second++;
	}
}
