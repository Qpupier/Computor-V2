/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:42:15 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/11 13:29:57 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "errors.hpp"

const char*	LogicError::what() const noexcept
{
	return (_message.c_str());
}

const char*	UnexpectedError::what() const noexcept
{
	return (_message.c_str());
}

const char*	UnsupportedError::what() const noexcept
{
	return (_message.c_str());
}
