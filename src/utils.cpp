/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 20:00:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/08 17:44:30 by qpupier          ###   ########lyon.fr   */
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
	patterns[TOKEN_DELETE] = std::regex(TOKEN_DELETE);
	patterns[TOKEN_DELETE_ALL] = std::regex(TOKEN_DELETE_ALL);
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

void		print_variables(const t_data& data, const bool print_preset, 	\
		const bool print_variables)
{
	std::map<std::pair<std::string, std::string>, const IType*>				\
			::const_iterator	it(data.stored.begin());
	bool						is_constant;

	while (it != data.stored.end())
	{
		is_constant = std::find(data.preset_constants.begin(), 				\
					data.preset_constants.end(), it->first.first) 			\
				!= data.preset_constants.end();
		if (((print_preset && print_variables && is_constant) 				\
					|| (!print_preset 										\
						&& ((print_variables && it->first.second.empty() 	\
								&& !is_constant) 							\
							|| (!print_variables 							\
								&& !it->first.second.empty()))))			\
				&& it->first.first != "last")
		{
			std::cout << "    " << it->first.first;
			if (!it->first.second.empty())
				std::cout << "(" << it->first.second << ")";
			std::cout << (print_preset ? " ≈ " : " = ") << *it->second 		\
					<< std::endl;
		}
		it++;
	}
}
