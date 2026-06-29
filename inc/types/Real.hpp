/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Real.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 17:41:08 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/29 10:31:50 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef REAL_HPP
# define REAL_HPP

# include "IType.hpp"
# include "InfiniteFloat.hpp"

class	Real: public IType
{
	// Constructors and destructor
	public:
		Real(void);
		Real(const Real &other);
		~Real(void) {};

		// Operator overloads
		explicit	operator bool() const;
		bool		operator==(const IType &other) const;
		bool		operator==(const long long int value) const;
		bool		operator!=(const IType &other) const;
		bool		operator!=(const long long int value) const;
		bool		operator<(const IType &other) const;
		bool		operator<(const long long int value) const;
		bool		operator<=(const IType &other) const;
		bool		operator<=(const long long int value) const;
		bool		operator>(const IType &other) const;
		bool		operator>(const long long int value) const;
		bool		operator>=(const IType &other) const;
		bool		operator>=(const long long int value) const;
		IType*		operator+(const IType &other) const;
		IType*		operator+(const long long int value) const;
		IType*		operator-(void) const;
		IType*		operator-(const IType &other) const;
		IType*		operator-(const long long int value) const;
		IType*		operator*(const IType &other) const;
		IType*		operator*(const long long int value) const;
		IType*		operator/(const IType &other) const;
		IType*		operator/(const long long int value) const;
		IType*		operator%(const IType &other) const;
		IType*		operator%(const long long int value) const;
		IType*		operator^(const IType &other) const;
		IType*		operator^(const long long int value) const;

		// Methods
		IType*			clone(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			matrix_operator(const IType &other) const;
		Rational*		gcd(const IType &other) const;
		std::ostream	&print(std::ostream &os) const;
		std::string		to_string(void) const;
		void			print_rounded(const std::string var = std::string()) const;
	
	private:
		// Members
		InfiniteFloat	_value;
};

#endif
