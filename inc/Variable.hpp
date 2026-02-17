/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:47:39 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/17 18:13:45 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLE_HPP
# define VARIABLE_HPP

# include "Type.hpp"
# include "Node.hpp"

class	Variable: public Type
{
	public:
		// Constructors and destructor
		Variable(void): Type(), _name(""), _value(nullptr) {};
		Variable(const std::string &name, Node *value): Type(), _name(name), _value(value) {};
		Variable(const Variable &other): Type(other), _name(other._name), _value(other._value) {};
		~Variable(void) {};

		// Operator overloads
		Variable	&operator=(const Variable &other);

		// Getters
		std::string	get_name(void) const;

	private:
		// Members
		std::string	_name;
		Node		*_value;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Variable &var);

#endif
