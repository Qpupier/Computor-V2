/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IType.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:42:36 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/11 19:00:47 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IType_HPP
# define IType_HPP

# include <iostream>

class	IType
{
	public:
		// Destructor
		virtual ~IType(void) {};

		// Operator overloads
		virtual explicit	operator bool() const = 0;
		virtual bool		operator==(const IType &other) const = 0;
		virtual bool		operator!=(const IType &other) const = 0;
		virtual bool		operator<(const IType &other) const = 0;
		virtual bool		operator<=(const IType &other) const = 0;
		virtual bool		operator>(const IType &other) const = 0;
		virtual bool		operator>=(const IType &other) const = 0;
		virtual IType*		operator+(const IType &other) const = 0;
		virtual IType*		operator-(const IType &other) const = 0;
		virtual IType*		operator*(const IType &other) const = 0;
		virtual IType*		operator/(const IType &other) const = 0;
		virtual IType*		operator%(const IType &other) const = 0;
		virtual IType*		operator^(const IType &other) const = 0;

		// Methods
		virtual IType*			matrix_operator(const IType &other) const = 0;
		virtual IType*			clone(void) const = 0;
		virtual std::ostream	&print(std::ostream &os) const = 0;
		virtual std::ostream	&print_polynomial(std::ostream &os, 	\
				const std::string &var) const = 0;

	protected:
		// Constructor
		IType(void) {};
};

std::ostream &operator<<(std::ostream &os, const IType &type);

#endif
