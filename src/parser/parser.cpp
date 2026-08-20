/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:51:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/20 14:52:03 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Polynomial.hpp"

static bool							waiting_function(const 					\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>	\
			::const_iterator	it(stored.begin());

	while (it != stored.end())
	{
		if (!it->second && !it->first.second.empty())
			return (true);
		it++;
	}
	return (false);
}

Token								create_token(							\
		std::string::const_iterator &start, 								\
		const std::string::const_iterator &end, const t_data &data)// [ ] Static?
{
	std::vector<int>			token_positions({1});
	std::sregex_token_iterator	token_null;
	std::sregex_token_iterator	it(start, end, 	\
			data.patterns.at(TOKEN_NEXT), token_positions);

	if (it == token_null)
		std::__throw_regex_error(std::regex_constants::error_complexity, 	\
				"Invalid format of TOKEN_NEXT regex");
	start = it->second;
	return Token(*it, get_token_type(*it, data.tokens_types));
}

static std::vector<t_possibility>	get_tokens(const std::string &line, 	\
		const t_data &data)
{
	std::vector<Token>	initial_tokens;
	std::string::const_iterator	end(line.end());

	for (std::string::const_iterator start(line.begin()); start != end;)
		initial_tokens.push_back(create_token(start, end, data));
	return (all_possibilities(initial_tokens));
}

AST*								compute_expression(						\
		const std::string &line, t_data &data, bool is_right_side, 			\
		const bool eval)
{
	std::vector<t_possibility>	possibilities;
	AST*				ast;

	if (!std::regex_match(line, data.patterns.at(TOKEN_FULL_EXPRESSION)))
		throw ERROR_INVALID_EXPRESSION;
	possibilities = get_tokens(line, data);
	ast = get_the_only_possibility(possibilities, data, is_right_side, eval);
	if (!ast)
		return (nullptr);
	try
	{
		if (is_right_side || !ast->end_of_tree())
			ast->reduce_expression(data.stored);
		if (is_right_side && !eval && waiting_function(data.stored))
			set_function_right(data, ast);
	}
	catch (...)
	{
		delete ast;
		throw;
	}
	return (ast);
}
