/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 19:10:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/22 18:27:15 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <regex>
# include <iostream>

# define TOKEN_BEGIN			"^"
# define TOKEN_END				"$"
# define TOKEN_WHITESPACE		"\\s*"
# define TOKEN_QUESTION			TOKEN_WHITESPACE "\\?" TOKEN_WHITESPACE
# define TOKEN_LIST				TOKEN_BEGIN TOKEN_QUESTION TOKEN_END
# define TOKEN_EXPRESSION		"[^=\\?]+"
# define TOKEN_EQUATION			TOKEN_EXPRESSION "(?:=(?:" TOKEN_EXPRESSION ")?(?:" TOKEN_QUESTION ")?)?"
# define TOKEN_EXPRESSION_EVAL	TOKEN_EXPRESSION "=" TOKEN_QUESTION
# define TOKEN_EQUATION_EVAL	TOKEN_EXPRESSION "=" TOKEN_EXPRESSION TOKEN_QUESTION
# define TOKEN_FULL				TOKEN_BEGIN "(?:" TOKEN_EXPRESSION "|" TOKEN_EQUATION ")" TOKEN_END
# define TOKEN_VARIABLE			"(?:[a-zA-Z]{2,}|[a-hj-zA-HJ-Z])"
# define TOKEN_NUMBER			"\\d+(?:\\.\\d+)?"
# define TOKEN_IMAGINARY		"i"
# define TOKEN_OPERATOR_INVERSE	"~"
# define TOKEN_OPERATOR			"(?:\\*\\*|[\\+\\-\\*\\/\\^%]|" TOKEN_OPERATOR_INVERSE ")"
# define TOKEN_RATIONAL			"[\\(\\)]|" TOKEN_NUMBER "|" TOKEN_VARIABLE "|" TOKEN_OPERATOR
# define TOKEN_MATRIX_ELEMENT	"(?:" TOKEN_WHITESPACE "(?:" TOKEN_RATIONAL "))+" TOKEN_WHITESPACE
# define TOKEN_MATRIX_LINE		TOKEN_WHITESPACE "\\[" TOKEN_MATRIX_ELEMENT "(?:," TOKEN_MATRIX_ELEMENT ")*\\]" TOKEN_WHITESPACE
# define TOKEN_MATRIX			"\\[" TOKEN_MATRIX_LINE "(?:;" TOKEN_MATRIX_LINE ")*\\]"
# define TOKEN_VECTOR			"\\[" TOKEN_MATRIX_ELEMENT "(?:," TOKEN_MATRIX_ELEMENT ")*\\]"
# define TOKEN					"(?:" TOKEN_MATRIX "|" TOKEN_VECTOR "|[\\(\\)\\|]|" TOKEN_NUMBER "|" TOKEN_VARIABLE "|" TOKEN_IMAGINARY "|" TOKEN_OPERATOR "|\\s+)"
# define TOKEN_NEXT				"^(" TOKEN "|\\?)"// [ ] token end??
# define TOKEN_FULL_EXPRESSION	TOKEN_BEGIN "(?:" TOKEN "+\\??|" TOKEN_QUESTION ")" TOKEN_END
# define TOKEN_QUIT				TOKEN_BEGIN TOKEN_WHITESPACE "quit" TOKEN_WHITESPACE TOKEN_END
# define TOKEN_HISTORY			TOKEN_BEGIN TOKEN_WHITESPACE "history" TOKEN_WHITESPACE "(\\d*)" TOKEN_WHITESPACE TOKEN_END

typedef enum	e_parenthesis
{
	E_PARENTHESIS,
	E_LEFT_PARENTHESIS,
	E_RIGHT_PARENTHESIS,
	E_NORM,
	E_LEFT_NORM,
	E_RIGHT_NORM,
	E_ABS,
	E_LEFT_ABS,
	E_RIGHT_ABS
}				t_parenthesis;

class	Token
{
	public:
		typedef enum	e_token
		{
			E_ERROR,
			E_LEFT_PARENTHESIS,
			E_RIGHT_PARENTHESIS,
			E_PIPE,
			E_LEFT_NORM,
			E_RIGHT_NORM,
			E_LEFT_ABS,
			E_RIGHT_ABS,
			E_NUMBER,
			E_IMAGINARY,
			E_POLYNOMIAL,
			E_FUNCTION,
			E_OPERATOR_INVERSE,
			E_OPERATOR,
			E_MATRIX,
			E_VECTOR,
			E_WHITESPACE,
			E_QUESTION
		}				t_token;

		// Constructors and destructor
		Token(void): _value(), _type(E_ERROR) {};
		Token(const std::string &value, t_token type): _value(value), _type(type) {};
		Token(const Token &other): _value(other._value), _type(other._type) {};
		~Token(void) {};

		// Operator overloads
		Token&	operator=(const Token &other);

		// Getters
		std::string	getValue(void) const;
		t_token		getType(void) const;

		// Setters
		void	setValue(const std::string &value);
		void	setType(t_token type);

	private:
		// Members
		std::string	_value;
		t_token		_type;
};

// Functions
Token::t_token		get_token_type(const std::string &token, const std::map<const Token::t_token, std::regex> &tokens_types);
std::vector<Token>*	adapt_tokens(std::vector<Token> &tokens, std::vector<Token> &sub_tokens, long int *pos);
void				define_token_types(std::map<const Token::t_token, std::regex> &tokens_types);

#endif
