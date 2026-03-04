/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v2.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:56:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/04 18:58:49 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V2_HPP
# define COMPUTOR_V2_HPP

# include <algorithm>
# include <cmath>
# include <unistd.h>

# include "IType.hpp"
# include "Token.hpp"

# define ERROR_INVALID_EXPRESSION std::logic_error("Invalid expression format")

typedef struct	s_data
{
	std::map<std::string, std::regex>			patterns;
	std::map<const Token::t_token, std::regex>	tokens_types;
	std::map<std::string, const IType*>			stored;
}				t_data;

#endif
