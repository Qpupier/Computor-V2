/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Polynomial.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:06:14 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/13 19:34:49 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLYNOMIAL_HPP
# define POLYNOMIAL_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Rational.hpp"
# include "Complex.hpp"
# include "Matrix.hpp"

class	Polynomial: public IType
{
	public:
		// Constructors and destructor
		Polynomial(std::string name, IType *power2, IType *power1, IType *power0): _name(name), _power2(power2), _power1(power1), _power0(power0) {};
		Polynomial(std::string name): Polynomial(name, new Rational(0), new Rational(1), new Rational(0)) {};
		Polynomial(const Token &token): Polynomial(token.getValue()) {};
		Polynomial(const Polynomial &other): _name(other._name), _power2(other._power2->clone()), _power1(other._power1->clone()), _power0(other._power0->clone()) {};
		~Polynomial(void);

		// Operator overloads
		Polynomial&	operator=(const Polynomial &other);
		explicit	operator bool() const;
		bool		operator==(const IType &other) const;
		bool		operator==(const Polynomial &other) const;
		bool		operator==(const Rational &other) const;
		bool		operator==(const Complex &other) const;
		bool		operator==(const Matrix &other) const;
		bool		operator!=(const IType &other) const;
		bool		operator<(const IType &other) const;
		bool		operator<=(const IType &other) const;
		bool		operator>(const IType &other) const;
		bool		operator>=(const IType &other) const;
		IType*		operator+(const IType &other) const;
		Polynomial*	operator+(const Polynomial &other) const;
		Polynomial*	operator+(const Rational &other) const;
		Polynomial*	operator+(const Complex &other) const;
		Polynomial*	operator+(const Matrix &other) const;
		IType*		operator-(const IType &other) const;
		Polynomial*	operator-(const Polynomial &other) const;
		Polynomial*	operator-(const Rational &other) const;
		Polynomial*	operator-(const Complex &other) const;
		Polynomial*	operator-(const Matrix &other) const;
		IType*		operator*(const IType &other) const;
		Polynomial*	operator*(const Polynomial &other) const;
		Polynomial*	operator*(const Rational &other) const;
		Polynomial*	operator*(const Complex &other) const;
		Polynomial*	operator*(const Matrix &other) const;
		IType*		operator/(const IType &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		Polynomial*	operator/(const Rational &other) const;
		Polynomial*	operator/(const Complex &other) const;
		Polynomial*	operator/(const Matrix &other) const;
		IType*		operator%(const IType &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		Polynomial*	operator%(const Rational &other) const;
		Polynomial*	operator%(const Complex &other) const;
		Polynomial*	operator%(const Matrix &other) const;
		IType*		operator^(const IType &other) const;
		Polynomial*	operator^(const Polynomial &other) const;
		Polynomial*	operator^(const Rational &other) const;
		Polynomial*	operator^(const Complex &other) const;
		Polynomial*	operator^(const Matrix &other) const;

		// Getters
		std::string	getName(void) const;
		IType*		getPower2(void) const;
		IType*		getPower1(void) const;
		IType*		getPower0(void) const;

		// Methods
		IType*			matrix_operator(const IType &other) const;
		IType*			matrix_operator(const Polynomial &other) const;
		IType*			matrix_operator(const Matrix &other) const;
		IType*			function_operator(const IType &other) const;
		IType*			clone(void) const;
		std::ostream&	print(std::ostream &os) const;
		std::ostream&	print_polynomial(std::ostream &os, 	\
				const std::string &var) const;

	private:
		// Members
		std::string	_name;
		IType*		_power2;
		IType*		_power1;
		IType*		_power0;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Polynomial &num);

// Functions
std::string	to_lower(std::string s);
void		solve_trinomial(Polynomial *polynomial);

#endif
