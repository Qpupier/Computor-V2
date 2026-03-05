/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v2.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:56:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/05 16:32:19 by qpupier          ###   ########lyon.fr   */
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
# define EXPONENT_INTEGER std::logic_error("Only integers (ℤ) can be used as exponents")
# define ERROR_DIVISION_BY_ZERO std::logic_error("Division by zero is impossible")
# define ERROR_MATRIX_OPERATOR std::logic_error("Matrix operator (**) can only be applied to matrices")
# define ERROR_MODULO_COMPLEX std::logic_error("Modulo operator (%) cannot be applied to complex numbers")
# define ERROR_CONVERT_COMPLEX_TO_RATIONAL std::logic_error("Cannot convert a complex number with a non-zero imaginary part to a rational number")

typedef struct	s_data
{
	std::map<std::string, std::regex>			patterns;
	std::map<const Token::t_token, std::regex>	tokens_types;
	std::map<std::string, const IType*>			stored;
}				t_data;

#endif
