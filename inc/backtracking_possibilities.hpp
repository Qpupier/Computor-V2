/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking_possibilities.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 16:56:45 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/25 15:15:42 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BACKTRACKING_POSSIBILITIES_HPP
# define BACKTRACKING_POSSIBILITIES_HPP

# include "Token.hpp"

typedef struct	s_parenthesis_data
{
	std::vector<std::pair<t_bracket, unsigned long int>>								lasts;
	std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>	pairs;
}				t_parenthesis_data;

typedef struct	s_possibility
{
	std::vector<Token>																	tokens;
	std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>	brackets_pairs;
}				t_possibility;

std::vector<std::vector<std::pair<t_bracket, std::pair<unsigned long int, unsigned long int>>>>	backtracking_possibilities(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos);
std::vector<t_possibility>																		all_possibilities(const std::vector<Token>& tokens);
bool			handle_operators(t_possibility& possibility, long long int *pos);

#endif
