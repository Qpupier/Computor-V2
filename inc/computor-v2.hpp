/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v2.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:56:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/05 15:42:59 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V2_HPP
# define COMPUTOR_V2_HPP

# include <cmath>
# include <utility>
# include <unistd.h>

# include "IType.hpp"
# include "Token.hpp"
# include "errors.hpp"

# define COLOR_RESET					"\033[0m"
# define COLOR_BOLD						"\033[1m"
# define COLOR_DIM						"\033[2m"
# define COLOR_ITALIC					"\033[3m"
# define COLOR_RED						"\033[31m"
# define COLOR_GREEN					"\033[32m"
# define COLOR_YELLOW					"\033[33m"
# define COLOR_BLUE						"\033[34m"
# define COLOR_PINK						"\033[35m"
# define COLOR_CYAN						"\033[36m"
# define ERROR_INVALID_EXPRESSION		LogicError("Invalid expression format")
# define ERROR_EXPONENT_INTEGER			LogicError("Only integers (ℤ) can be used as exponents")
# define ERROR_DIVISION_BY_ZERO			LogicError("Division by zero is impossible")
# define ERROR_MATRIX_OPERATOR			LogicError("Matrix operator (**) can only be applied to matrices")
# define ERROR_MODULO_COMPLEX			LogicError("Modulo operator (%) cannot be applied to complex numbers")
# define ERROR_OPERATION_MATRIX_COMPLEX	LogicError("Impossible operation between matrix and complex number")
# define ERROR_MATRIX_DIMENSIONS		LogicError("Matrix operation: incompatible dimensions")
# define ERROR_MATRIX_OUT_OF_RANGE		LogicError("Matrix index out of range")
# define ERROR_OPERATOR_EXPECTED		UnexpectedError("Invalid expression: operator expected")
# define UNSUPPORTED_DIVISION			UnsupportedError("Division not supported between polynomials with these powers")
# define UNSUPPORTED_EXPONENT			UnsupportedError("Exponentiation can only be positive integers")
# define ERROR_UNEXPECTED				UnexpectedError("Impossible to complete operation")
# define UNSUPPORTED_MULTI_POLYNOMIALS	UnsupportedError("Multiple unknown polynomials are not supported")

typedef struct	s_data
{
	std::map<std::string, std::regex>							patterns;
	std::map<const Token::t_token, std::regex>					tokens_types;
	std::map<std::pair<std::string, std::string>, const IType*>	stored;
}				t_data;

int		usage(void);
void	define_patterns(std::map<std::string, std::regex> &patterns);

#endif
