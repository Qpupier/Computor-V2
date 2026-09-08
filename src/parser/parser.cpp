/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:51:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/08 17:16:04 by qpupier          ###   ########lyon.fr   */
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

static Token						create_token(							\
		std::string::const_iterator &start, 								\
		const std::string::const_iterator &end, const t_data &data)
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

static AST*							compute_AST(AST* ast, t_data &data, 	\
		bool is_right_side, const bool eval)
{
	bool	begin_alone;

	begin_alone = ast->end_of_tree();
	if (is_right_side || !ast->end_of_tree() || eval)
		ast->reduce_expression(data.stored);
	if (!is_right_side && eval && begin_alone 	\
			&& ast->getNode()->getType() == IType::E_TYPE_POLYNOMIAL)
	{
		delete ast;
		return (nullptr);
	}
	if (is_right_side && waiting_function(data.stored) && !eval)
		set_function_right(data, ast);
	return (ast);
}

AST*								compute_expression(						\
		const std::string &line, t_data &data, bool is_right_side, 			\
		const bool eval)
{
	std::vector<t_possibility>	possibilities;
	std::vector<Token>			initial_tokens;
	std::string::const_iterator	end(line.end());
	AST*						ast;

	if (!std::regex_match(line, data.patterns.at(TOKEN_FULL_EXPRESSION)))
		throw ERROR_INVALID_EXPRESSION;
	for (std::string::const_iterator start(line.begin()); start != end;)
		initial_tokens.push_back(create_token(start, end, data));
	possibilities = all_possibilities(initial_tokens);
	ast = get_the_only_possibility(possibilities, data, is_right_side, eval);
	if (!ast)
		return (nullptr);
	try
	{
		return (compute_AST(ast, data, is_right_side, eval));
	}
	catch (...)
	{
		delete ast;
		throw;
	}
	return (nullptr);
}

void								print_expression(const std::string& 	\
		line, t_data &data)
{
	std::string	result;
	AST*		ast;

	ast = compute_expression(line, data, true);
	if (!ast)
		throw UnexpectedError	\
				("Unexpected error while computing the expression");
	if (!ast->end_of_tree())
	{
		delete ast;
		throw UnexpectedError("Unexpected error: the AST is not an expression");
	}
	try {verif_preset_terms(ast);}
	catch (...)
	{
		delete ast;
		throw;
	}
	result = ast->getNode()->to_string();
	std::cout << COLOR_BOLD << result << COLOR_RESET << std::endl;
	assign_last(data, ast->getNode());
	add_history_result(data.history_results, result);
	ast->getNode()->print_rounded();
	delete ast;
}
