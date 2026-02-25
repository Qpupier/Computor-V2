/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 15:34:40 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/25 15:55:30 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "IType.hpp"

// Output stream operator overload
std::ostream &operator<<(std::ostream &os, const IType &type)
{
	return (type.print(os));
}
