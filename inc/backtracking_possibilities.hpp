/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking_possibilities.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 16:56:45 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/22 17:32:02 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BACKTRACKING_POSSIBILITIES_HPP
# define BACKTRACKING_POSSIBILITIES_HPP

# include "Token.hpp"

typedef struct	s_parenthesis_data
{
	std::vector<std::pair<t_parenthesis, unsigned long int>>	lasts;
	// std::vector<t_parenthesis>	all;
	std::vector<std::pair<t_parenthesis, std::pair<unsigned long int, unsigned long int>>>	pairs;
}				t_parenthesis_data;

std::vector<std::vector<std::pair<t_parenthesis, std::pair<unsigned long int, unsigned long int>>>>	backtracking_possibilities(const std::vector<Token>& tokens, t_parenthesis_data data, const unsigned long int pos);
std::vector<std::vector<Token>>	all_possibilities(const std::vector<Token>& tokens);

#endif
