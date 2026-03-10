/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnsupportedError.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:54:09 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/09 17:46:41 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "UnsupportedError.hpp"

const char* UnsupportedError::what() const noexcept
{
	return (_message.c_str());
}
