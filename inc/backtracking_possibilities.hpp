/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking_possibilities.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 16:56:45 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/26 14:57:55 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BACKTRACKING_POSSIBILITIES_HPP
# define BACKTRACKING_POSSIBILITIES_HPP

# include "Token.hpp"

typedef std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>	t_brackets_pairs;
typedef std::vector<t_brackets_pairs>														t_list_brackets_pairs;

typedef struct	s_parenthesis_data
{
	std::vector<std::pair<t_bracket, unsigned long int>>	lasts;
	t_brackets_pairs										pairs;
}				t_parenthesis_data;

typedef struct	s_possibility
{
	std::vector<Token>	tokens;
	t_brackets_pairs	brackets_pairs;
}				t_possibility;

std::vector<t_possibility>	all_possibilities(const std::vector<Token>& tokens);
t_list_brackets_pairs		backtracking_possibilities(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos);
t_list_brackets_pairs		try_pipe_brackets(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos);
t_list_brackets_pairs		parentheses_brackets(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos, Token::t_token token_type);
t_bracket					test_external_brackets(t_possibility& p);
bool						handle_operators(t_possibility& possibility, long long int *pos);
void						add_new_possibility(t_list_brackets_pairs& results, const std::vector<Token>& tokens, t_parenthesis_data& data, const unsigned long int pos);

#endif
