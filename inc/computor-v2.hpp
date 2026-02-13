/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v2.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:56:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/13 16:53:31 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_V2_HPP
# define COMPUTOR_V2_HPP

# include <iostream>
# include <regex>
# include <algorithm>

class Node;

# define TOKEN_WHITESPACE		"\\s*"
# define TOKEN_SIGN				"[-+]?"
# define TOKEN_QUESTION			TOKEN_WHITESPACE "\\?" TOKEN_WHITESPACE
# define TOKEN_LIST				"^" TOKEN_QUESTION "$"
# define TOKEN_EXPRESSION		"[^=\\?]+"
# define TOKEN_SIDE				"(?:" TOKEN_EXPRESSION "|" TOKEN_QUESTION ")"
# define TOKEN_EQUATION			TOKEN_EXPRESSION "=" TOKEN_SIDE "(?:" TOKEN_QUESTION ")?"
# define TOKEN_FULL				"^(?:" TOKEN_SIDE "|" TOKEN_EQUATION ")$"
# define TOKEN_VARIABLE			TOKEN_SIGN "(?:[a-zA-Z]{2,}|[a-hj-zA-HJ-Z])"
# define TOKEN_NUMBER			TOKEN_SIGN "\\d+(?:\\.\\d+)?"
# define TOKEN_IMAGINARY		TOKEN_SIGN "i"
// # define TOKEN_FUNCTION			TOKEN_SIGN TOKEN_VARIABLE "\\(" TOKEN_EXPRESSION "\\)"
# define TOKEN_OPERATOR			"(?:\\*\\*|[\\+\\-\\*\\/\\^%])"
# define TOKEN_RATIONAL			"[\\(\\)]|" TOKEN_NUMBER "|" TOKEN_VARIABLE "|" TOKEN_OPERATOR
# define TOKEN_MATRIX_ELEMMENT	"(?:" TOKEN_WHITESPACE "(?:" TOKEN_RATIONAL "))+" TOKEN_WHITESPACE
# define TOKEN_MATRIX_LINE		TOKEN_WHITESPACE "\\[" TOKEN_MATRIX_ELEMMENT "(?:," TOKEN_MATRIX_ELEMMENT ")*\\]" TOKEN_WHITESPACE
# define TOKEN_MATRIX			TOKEN_SIGN "\\[" TOKEN_MATRIX_LINE "(?:;" TOKEN_MATRIX_LINE ")*\\]"
# define TOKEN					"(?:" TOKEN_MATRIX "|[\\(\\)]|" TOKEN_NUMBER "|" TOKEN_VARIABLE "|" TOKEN_IMAGINARY "|" TOKEN_OPERATOR "|\\s+)"
# define TOKEN_NEXT				"^(" TOKEN "|\\?)"
# define TOKEN_FULL_EXPRESSION	"^(?:" TOKEN "+\\??|" TOKEN_QUESTION ")$"

typedef enum	e_token
{
	E_ERROR,
	E_LEFT_PARENTHESIS,
	E_RIGHT_PARENTHESIS,
	E_NUMBER,
	E_IMAGINARY,
	E_VARIABLE,
	E_OPERATOR,
	E_MATRIX,
	E_WHITESPACE,
	E_QUESTION
}				t_token;

void	compute_expression(const std::string &line);
Node	*make_ast(const std::vector<std::string> &tokens);

#endif
