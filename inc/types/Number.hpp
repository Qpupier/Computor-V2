/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Number.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 13:35:44 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 14:46:53 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMBER_HPP
# define NUMBER_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Node.hpp"

class	Number: public IType
{
	public:
		// Constructors and destructor
		Number(void): _value(0) {};
		Number(double value): _value(value) {};
		Number(const std::string &value): _value(std::stod(value)) {};
		Number(const Token &token): Number(token.getValue()) {};
		Number(const Node *node): Number(node->getToken()) {};
		Number(const Number &other): _value(other._value) {};
		~Number(void) {};

		// Operator overloads
		Number	&operator=(const Number &other);
		// Number	*operator-(const Number &other) const { return (new Number(*this->_value - *other._value)); };
		// Number	*operator+(const Number &other) const { return (new Number(*this->_value + *other._value)); };
		Node	*operator*(const IType &other) const;
		Node	*operator*(const Number &other) const;
		// Number	*operator/(const Number &other) const { return (new Number(*this->_value / *other._value)); };

		// Getters
		double	get_value(void) const;

	private:
		// Members
		double	_value;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Number &num);

#endif
