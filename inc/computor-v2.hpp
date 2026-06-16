/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v2.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:56:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/16 14:35:10 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V2_HPP
# define COMPUTOR_V2_HPP

# include <cmath>
# include <utility>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

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
# define ERROR_INCOMPATIBLE_TYPES		LogicError("Incompatible types for operation")
# define ERROR_SQRT_NEGATIVE			LogicError("Cannot compute square root of a negative number")
# define ERROR_OPERATOR_EXPECTED		UnexpectedError("Invalid expression: operator expected")
# define ERROR_UNEXPECTED				UnexpectedError("Impossible to complete operation")
# define UNSUPPORTED_DIVISION			UnsupportedError("Division not supported between polynomials with these powers")
# define UNSUPPORTED_EXPONENT			UnsupportedError("Exponentiation can only be positive integers")
# define UNSUPPORTED_MULTI_POLYNOMIALS	UnsupportedError("Multiple unknown polynomials are not supported")
# define CONTINUE						2
# define NOTHING						3

typedef struct	s_data
{
	std::map<std::string, std::regex>							patterns;
	std::map<const Token::t_token, std::regex>					tokens_types;
	std::map<std::pair<std::string, std::string>, const IType*>	stored;
	std::vector<std::string>									history_results;
}				t_data;

std::string	to_lower(std::string s);
int			loop(t_data &data, bool is_interactive);
int			usage(void);
void		assignation(std::string var, std::map<std::pair<std::string, std::string>, const IType*> &stored, IType* result);
void		compute_line(std::string line, t_data &data);
void		define_patterns(std::map<std::string, std::regex> &patterns);

#endif
