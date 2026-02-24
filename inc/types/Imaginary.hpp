/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Imaginary.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:22:00 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/23 18:11:35 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGINARY_HPP
# define IMAGINARY_HPP

# include "IType.hpp"
# include "Node.hpp"
// # include "Number.hpp"

class	Number;

class	Imaginary: public IType
{
	public:
		// Constructors and destructor
		Imaginary(void) {};
		Imaginary(const Imaginary &other) {(void)other;};
		~Imaginary(void) {};

		// Operator overloads
		Imaginary	&operator=(const Imaginary &other);
		Node		*operator+(const IType &other) const;
		Node		*operator+(const Imaginary &other) const;
		Node		*operator+(const Number &other) const;
		Node		*operator-(const IType &other) const;
		Node		*operator-(const Imaginary &other) const;
		Node		*operator-(const Number &other) const;
		Node		*operator*(const IType &other) const;
		Node		*operator*(const Imaginary &other) const;
		Node		*operator*(const Number &other) const;
		Node		*operator/(const IType &other) const;
		Node		*operator/(const Imaginary &other) const;
		Node		*operator/(const Number &other) const;
		Node		*operator%(const IType &other) const;
		Node		*operator^(const IType &other) const;
		Node		*operator^(const Imaginary &other) const;
		Node		*operator^(const Number &other) const;

		// Methods
		Node*	matrix_operator(const IType &other) const;
};

#endif
