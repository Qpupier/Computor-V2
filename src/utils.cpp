/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 20:00:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/12 16:25:07 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "computor-v2.hpp"

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
	patterns[TOKEN_QUIT] = std::regex(TOKEN_QUIT);
}

std::string	to_lower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), 	\
			[](unsigned char c){return (static_cast<char>(std::tolower(c)));});
	return (s);
}
