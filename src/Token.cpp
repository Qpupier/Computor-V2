/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 19:15:11 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/09 12:36:49 by qpupier          ###   ########lyon.fr   */
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
	tokens_types[Token::E_LEFT_PARENTHESIS] = std::regex("^\\($");
	tokens_types[Token::E_RIGHT_PARENTHESIS] = std::regex("^\\)$");
	tokens_types[Token::E_NUMBER] = std::regex("^" TOKEN_NUMBER "$");
	tokens_types[Token::E_IMAGINARY] = std::regex("^" TOKEN_IMAGINARY "$");
	tokens_types[Token::E_VARIABLE] = std::regex("^" TOKEN_VARIABLE "$");
	tokens_types[Token::E_OPERATOR] = std::regex("^" TOKEN_OPERATOR "$");
	tokens_types[Token::E_MATRIX] = std::regex("^" TOKEN_MATRIX "$");
	tokens_types[Token::E_WHITESPACE] = std::regex("^" TOKEN_WHITESPACE "$");
	tokens_types[Token::E_QUESTION] = std::regex("^" TOKEN_QUESTION "$");
}

Token::t_token	get_token_type(const std::string &token, 			\
		const std::map<const Token::t_token, std::regex> &tokens_types)
{
	for (std::map<const Token::t_token, std::regex>::const_iterator it = tokens_types.begin(); 	\
			it != tokens_types.end(); ++it)
		if (std::regex_match(token, it->second))
			return it->first;
	return (Token::E_ERROR);
}
