/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v2.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:56:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/10 15:50:55 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V2_HPP
# define COMPUTOR_V2_HPP

# include <iostream>
# include <regex>
# include <algorithm>

# define TOKEN_RATIONAL		"[-+]?\\d+(\\.\\d+)?"
# define TOKEN_WHITESPACE	"\\s"
# define TOKEN_IMAGINARY	"i"
# define TOKEN_VARIABLE		"([a-hj-zA-HJ-Z]|\\w{2,})"
# define TOKEN_OPERATOR		"(\\*\\*|[\\+\\-\\*\\/\\^%])"
# define TOKEN_SIDE			"([^=\\?]*|\\?)"
# define TOKEN_EQUATION		TOKEN_SIDE "=" TOKEN_SIDE
# define TOKEN_FULL			"^(" TOKEN_SIDE "|" TOKEN_EQUATION ")$"

#endif
