/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v2.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:56:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/09 12:30:09 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V2_HPP
# define COMPUTOR_V2_HPP

# include <cmath>
# include <unistd.h>

# include "IType.hpp"
# include "Token.hpp"

# define ERROR_INVALID_EXPRESSION std::logic_error("Invalid expression format")
# define ERROR_EXPONENT_INTEGER std::logic_error("Only integers (ℤ) can be used as exponents")
# define ERROR_DIVISION_BY_ZERO std::logic_error("Division by zero is impossible")
# define ERROR_MATRIX_OPERATOR std::logic_error("Matrix operator (**) can only be applied to matrices")
# define ERROR_MODULO_COMPLEX std::logic_error("Modulo operator (%) cannot be applied to complex numbers")
# define ERROR_OPERATION_MATRIX_COMPLEX std::logic_error("Impossible operation between matrix and complex number")
# define ERROR_MATRIX_DIMENSIONS std::logic_error("Matrix operation: incompatible dimensions")
# define ERROR_MATRIX_OUT_OF_RANGE std::logic_error("Matrix index out of range")
# define ERROR_OPERATOR_EXPECTED std::runtime_error("Invalid expression: operator expected")

typedef struct	s_data
{
	std::map<std::string, std::regex>			patterns;
	std::map<const Token::t_token, std::regex>	tokens_types;
	std::map<std::string, const IType*>			stored;
}				t_data;

int		usage(void);
void	define_patterns(std::map<std::string, std::regex> &patterns);

#endif
