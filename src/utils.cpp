/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 20:00:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/21 20:02:17 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

int			usage(void)
{
	std::cerr << "Usage: ./computor-v2" << std::endl;
	return (EXIT_FAILURE);
}

void		define_patterns(std::map<std::string, std::regex> &patterns)
{
	patterns[TOKEN_FULL] = std::regex(TOKEN_FULL);
	patterns[TOKEN_LIST] = std::regex(TOKEN_LIST);
	patterns[TOKEN_NEXT] = std::regex(TOKEN_NEXT);
	patterns[TOKEN_FULL_EXPRESSION] = std::regex(TOKEN_FULL_EXPRESSION);
	patterns[TOKEN_QUESTION] = std::regex(TOKEN_QUESTION);
	patterns[TOKEN_EXPRESSION_EVAL] = std::regex(TOKEN_EXPRESSION_EVAL);
	patterns[TOKEN_EQUATION_EVAL] = std::regex(TOKEN_EQUATION_EVAL);
	patterns[TOKEN_WHITESPACE] = std::regex(TOKEN_WHITESPACE);
	patterns[TOKEN_QUIT] = std::regex(TOKEN_QUIT);
	patterns[TOKEN_HISTORY] = std::regex(TOKEN_HISTORY);
}

std::string	to_lower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), 	\
			[](unsigned char c){return (static_cast<char>(std::tolower(c)));});
	return (s);
}

void		trim_string(std::string &s)
{
	while (std::isspace(s[0]))
		s.erase(0, 1);
	while (std::isspace(s[s.size() - 1]))
		s.erase(s.size() - 1, 1);
}

void		print_variables(const 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored, const std::vector<std::string>& preset, const bool print_preset)
{
	std::map<std::pair<std::string, std::string>, const IType*>			\
			::const_iterator	it(stored.begin());

	while (it != stored.end())
	{
		if (print_preset ^ std::find(preset.begin(), preset.end(), it->first.first) != preset.end())
		{
			it++;
			continue;
		}
		std::cout << COLOR_YELLOW << "  ";
		if (!it->first.second.empty())
			std::cout << it->first.first << "(" << it->first.second << ")";
		else
			std::cout << it->first.first;
		std::cout << " = " << *it->second << COLOR_RESET << std::endl;
		it++;
	}
}
