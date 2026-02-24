/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:47:39 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 17:29:00 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLE_HPP
# define VARIABLE_HPP

// # include "IType.hpp"
# include "Node.hpp"
// # include "computor-v2.hpp"

class	Variable: public IType
{
	public:
		// Constructors and destructor
		Variable(void): _name(), _value(nullptr) {};
		Variable(const std::string &name): _name(name), _value(nullptr) {};
		Variable(const std::string &name, Node *value): _name(name), _value(value) {};
		Variable(const Variable &other): _name(other._name), _value(other._value) {};
		~Variable(void) {};

		// Operator overloads
		Variable	&operator=(const Variable &other);
		Node		*operator+(const IType &other) const {return new Node(Token());(void)other;};
		Node		*operator+(const Variable &other) const {return new Node(Token());(void)other;};
		Node		*operator-(const IType &other) const {return new Node(Token());(void)other;};
		Node		*operator-(const Variable &other) const {return new Node(Token());(void)other;};
		Node		*operator*(const IType &other) const {return new Node(Token());(void)other;};
		Node		*operator*(const Variable &other) const {return new Node(Token());(void)other;};
		Node		*operator/(const IType &other) const {return new Node(Token());(void)other;};
		Node		*operator/(const Variable &other) const {return new Node(Token());(void)other;};
		Node		*operator%(const IType &other) const {return new Node(Token());(void)other;};
		Node		*operator%(const Variable &other) const {return new Node(Token());(void)other;};
		Node		*operator^(const IType &other) const {return new Node(Token());(void)other;};
		Node		*operator^(const Variable &other) const {return new Node(Token());(void)other;};

		// Getters
		std::string	get_name(void) const;

		// Methods
		Node*	matrix_operator(const IType &other) const;

	private:
		// Members
		std::string	_name;
		Node		*_value;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Variable &var);

#endif
