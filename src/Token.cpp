/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 19:15:11 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/16 20:00:13 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

// Operator overloads
Token& Token::operator=(const Token &other)
{
	if (this != &other)
	{
		this->_token = other._token;
		this->_type = other._type;
	}
	return (*this);
}


// Getters
std::string		Token::get_token(void) const
{
	return (_token);
}

Token::t_token	Token::get_type(void) const
{
	return (_type);
}


// Setters
void	Token::set_token(const std::string &token)
{
	this->_token = token;
}

void	Token::set_type(t_token type)
{
	this->_type = type;
}


// Functions
Token::t_token	get_token_type(const std::string &token, std::map<const Token::t_token, std::regex> &tokens_types)
{
	for (const std::pair<const Token::t_token, std::regex> &pair: tokens_types)
		if (std::regex_match(token, pair.second))
			return (pair.first);
	return (Token::E_ERROR);
}
