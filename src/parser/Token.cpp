/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 19:15:11 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/23 17:02:20 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

// Operator overloads

Token& Token::operator=(const Token &other)
{
	if (this != &other)
	{
		this->_value = other._value;
		this->_type = other._type;
	}
	return (*this);
}


// Getters

std::string		Token::getValue(void) const
{
	return (_value);
}

Token::t_token	Token::getType(void) const
{
	return (_type);
}


// Setters

void	Token::setValue(const std::string &value)
{
	this->_value = value;
}

void	Token::setType(t_token type)
{
	this->_type = type;
}


// Functions

void			define_token_types(std::map<const Token::t_token, 	\
		std::regex> &tokens_types)
{
	std::string	b(TOKEN_BEGIN);
	std::string	e(TOKEN_END);

	tokens_types[Token::E_TOKEN_LEFT_PARENTHESES] = std::regex(b + "\\(" + e);
	tokens_types[Token::E_TOKEN_RIGHT_PARENTHESES] = std::regex(b + "\\)" + e);
	tokens_types[Token::E_TOKEN_PIPE] = std::regex(b + "\\|" + e);
	tokens_types[Token::E_TOKEN_NUMBER] = std::regex(b + TOKEN_NUMBER + e);
	tokens_types[Token::E_TOKEN_IMAGINARY] 				\
			= std::regex(b + TOKEN_IMAGINARY + e);
	tokens_types[Token::E_TOKEN_POLYNOMIAL] 			\
			= std::regex(b + TOKEN_VARIABLE + e);
	tokens_types[Token::E_TOKEN_OPERATOR_INVERSE] 		\
			= std::regex(b + TOKEN_OPERATOR_INVERSE + e);
	tokens_types[Token::E_TOKEN_OPERATOR_FACTORIAL] 	\
			= std::regex(b + TOKEN_OPERATOR_FACTORIAL + e);
	tokens_types[Token::E_TOKEN_OPERATOR] = std::regex(b + TOKEN_OPERATOR + e);
	tokens_types[Token::E_TOKEN_MATRIX] = std::regex(b + TOKEN_MATRIX + e);
	tokens_types[Token::E_TOKEN_VECTOR] = std::regex(b + TOKEN_VECTOR + e);
	tokens_types[Token::E_TOKEN_WHITESPACE] 			\
			= std::regex(b + TOKEN_WHITESPACE + e);
	tokens_types[Token::E_TOKEN_QUESTION] = std::regex(b + TOKEN_QUESTION + e);
}

Token::t_token	get_token_type(const std::string &token, 			\
		const std::map<const Token::t_token, std::regex> &tokens_types)
{
	for (std::map<const Token::t_token, std::regex>::const_iterator 	\
			it(tokens_types.begin()); it != tokens_types.end(); ++it)
		if (std::regex_match(token, it->second))
			return (it->first);
	return (Token::E_TOKEN_ERROR);
}
