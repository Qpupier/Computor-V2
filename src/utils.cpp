/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 20:00:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/09 12:30:17 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "computor-v2.hpp"

int		usage(void)
{
	std::cerr << "Usage: ./computor-v2" << std::endl;
	return (EXIT_FAILURE);
}

void	define_patterns(std::map<std::string, std::regex> &patterns)
{
	patterns[TOKEN_FULL] = std::regex(TOKEN_FULL);
	patterns[TOKEN_LIST] = std::regex(TOKEN_LIST);
	patterns[TOKEN_NEXT] = std::regex(TOKEN_NEXT);
	patterns[TOKEN_FULL_EXPRESSION] = std::regex(TOKEN_FULL_EXPRESSION);
}
