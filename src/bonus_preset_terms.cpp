/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_preset_terms.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 16:57:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/20 17:02:27 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

void	stop_preset_terms(const std::string& name)
{
	if (std::regex_match(name, std::regex(TOKEN_NO_VARIABLE)))
		throw LogicError(std::string("\"") + name + std::string("\" is a"	\
			" pre-set variable or function and cannot be changed"));
}
