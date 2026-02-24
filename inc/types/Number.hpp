/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Number.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 13:35:44 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/23 17:43:02 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMBER_HPP
# define NUMBER_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Node.hpp"
# include "Imaginary.hpp"

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
		Node	*operator+(const IType &other) const;
		Node	*operator+(const Number &other) const;
		Node	*operator+(const Imaginary &other) const;
		Node	*operator-(const IType &other) const;
		Node	*operator-(const Number &other) const;
		Node	*operator-(const Imaginary &other) const;
		Node	*operator*(const IType &other) const;
		Node	*operator*(const Number &other) const;
		Node	*operator*(const Imaginary &other) const;
		Node	*operator/(const IType &other) const;
		Node	*operator/(const Number &other) const;
		Node	*operator/(const Imaginary &other) const;
		Node	*operator%(const IType &other) const;
		Node	*operator%(const Number &other) const;
		Node	*operator^(const IType &other) const;
		Node	*operator^(const Number &other) const;

		// Getters
		double	get_value(void) const;

		// Methods
		Node*	matrix_operator(const IType &other) const;

	private:
		// Members
		double	_value;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Number &num);

#endif
