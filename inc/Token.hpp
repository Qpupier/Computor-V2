/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 19:10:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/16 20:04:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

# include "computor-v2.hpp"

# define TOKEN_WHITESPACE		"\\s*"
# define TOKEN_QUESTION			TOKEN_WHITESPACE "\\?" TOKEN_WHITESPACE
# define TOKEN_LIST				"^" TOKEN_QUESTION "$"
# define TOKEN_EXPRESSION		"[^=\\?]+"
# define TOKEN_SIDE				"(?:" TOKEN_EXPRESSION "|" TOKEN_QUESTION ")"
# define TOKEN_EQUATION			TOKEN_EXPRESSION "=" TOKEN_SIDE "(?:" TOKEN_QUESTION ")?"
# define TOKEN_FULL				"^(?:" TOKEN_SIDE "|" TOKEN_EQUATION ")$"
# define TOKEN_VARIABLE			"(?:[a-zA-Z]{2,}|[a-hj-zA-HJ-Z])"
# define TOKEN_NUMBER			"\\d+(?:\\.\\d+)?"
# define TOKEN_IMAGINARY		"i"
// # define TOKEN_FUNCTION			TOKEN_VARIABLE "\\(" TOKEN_EXPRESSION "\\)"
# define TOKEN_OPERATOR			"(?:\\*\\*|[\\+\\-\\*\\/\\^%])"
# define TOKEN_RATIONAL			"[\\(\\)]|" TOKEN_NUMBER "|" TOKEN_VARIABLE "|" TOKEN_OPERATOR
# define TOKEN_MATRIX_ELEMMENT	"(?:" TOKEN_WHITESPACE "(?:" TOKEN_RATIONAL "))+" TOKEN_WHITESPACE
# define TOKEN_MATRIX_LINE		TOKEN_WHITESPACE "\\[" TOKEN_MATRIX_ELEMMENT "(?:," TOKEN_MATRIX_ELEMMENT ")*\\]" TOKEN_WHITESPACE
# define TOKEN_MATRIX			"\\[" TOKEN_MATRIX_LINE "(?:;" TOKEN_MATRIX_LINE ")*\\]"
# define TOKEN					"(?:" TOKEN_MATRIX "|[\\(\\)]|" TOKEN_NUMBER "|" TOKEN_VARIABLE "|" TOKEN_IMAGINARY "|" TOKEN_OPERATOR "|\\s+)"
# define TOKEN_NEXT				"^(" TOKEN "|\\?)"
# define TOKEN_FULL_EXPRESSION	"^(?:" TOKEN "+\\??|" TOKEN_QUESTION ")$"

class	Token
{
	public:
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

		// Constructors and destructor
		Token(void): _token(), _type(E_ERROR) {};
		Token(const std::string &token, t_token type): _token(token), _type(type) {};
		Token(const Token &other): _token(other._token), _type(other._type) {};
		~Token(void) {};

		// Operator overloads
		Token&	operator=(const Token &other);

		// Getters
		std::string	get_token(void) const;
		t_token		get_type(void) const;

		// Setters
		void	set_token(const std::string &token);
		void	set_type(t_token type);

	private:
		// Members
		std::string	_token;
		t_token		_type;
};

Token::t_token	get_token_type(const std::string &token, std::map<const Token::t_token, std::regex> &tokens_types);

#endif
