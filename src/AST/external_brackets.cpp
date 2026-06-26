/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_brackets.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 17:12:46 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/26 12:26:05 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

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
			if (first_token_type == Token::E_TOKEN_LEFT_PARENTHESIS 	\
					&& last_token_type == Token::E_TOKEN_RIGHT_PARENTHESIS)
				return (remove_external_brackets(p, sub_tokens, 		\
						pair, E_BRACKET_PARENTHESIS));
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
	return (build_ast(possibility, data));
	// if (brackets_type == E_BRACKET_PARENTHESIS)
	// 	return (build_ast(possibility, data));
	// if (brackets_type == E_BRACKET_NORM)
	// 	return (new AST(new Function(NORM), build_ast(possibility, data), nullptr));
	// if (brackets_type == E_BRACKET_ABS)
	// 	return (new AST(new Function(ABSOLUTE), build_ast(possibility, data), nullptr));
	(void)brackets_type;
	throw UnexpectedError("Invalid bracket type");
}
