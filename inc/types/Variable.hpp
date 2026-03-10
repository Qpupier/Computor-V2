/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:06:14 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/10 16:05:27 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLE_HPP
# define VARIABLE_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Rational.hpp"
# include "Complex.hpp"
# include "Matrix.hpp"

class	Variable: public IType
{
	public:
		// Constructors and destructor
		Variable(std::string name, IType *power2, IType *power1, IType *power0): _name(name), _power2(power2), _power1(power1), _power0(power0) {};
		Variable(std::string name): Variable(name, new Rational(0), new Rational(1), new Rational(0)) {};
		Variable(const Token &token): Variable(token.getValue()) {};
		Variable(const Variable &other): _name(other._name), _power2(other._power2->clone()), _power1(other._power1->clone()), _power0(other._power0->clone()) {};
		~Variable(void);

		// Operator overloads
		Variable&	operator=(const Variable &other);
		explicit	operator bool() const;
		IType*		operator+(const IType &other) const;
		Variable*	operator+(const Variable &other) const;
		Variable*	operator+(const Rational &other) const;
		Variable*	operator+(const Complex &other) const;
		Variable*	operator+(const Matrix &other) const;
		IType*		operator-(const IType &other) const;
		Variable*	operator-(const Variable &other) const;
		Variable*	operator-(const Rational &other) const;
		Variable*	operator-(const Complex &other) const;
		Variable*	operator-(const Matrix &other) const;
		IType*		operator*(const IType &other) const;
		Variable*	operator*(const Variable &other) const;
		Variable*	operator*(const Rational &other) const;
		Variable*	operator*(const Complex &other) const;
		Variable*	operator*(const Matrix &other) const;
		IType*		operator/(const IType &other) const;
		Variable*	operator/(const Variable &other) const;
		Variable*	operator/(const Rational &other) const;
		Variable*	operator/(const Complex &other) const;
		Variable*	operator/(const Matrix &other) const;
		IType*		operator%(const IType &other) const;
		Variable*	operator%(const Variable &other) const;
		Variable*	operator%(const Rational &other) const;
		Variable*	operator%(const Complex &other) const;
		Variable*	operator%(const Matrix &other) const;
		IType*		operator^(const IType &other) const;
		Variable*	operator^(const Variable &other) const;
		Variable*	operator^(const Rational &other) const;
		Variable*	operator^(const Complex &other) const;
		Variable*	operator^(const Matrix &other) const;

		// Getters
		std::string	getName(void) const;
		IType*		getPower2(void) const;
		IType*		getPower1(void) const;
		IType*		getPower0(void) const;

		// Methods
		IType*			matrix_operator(const IType &other) const;
		IType*			matrix_operator(const Variable &other) const;
		IType*			matrix_operator(const Matrix &other) const;
		IType*			clone(void) const;
		std::ostream&	print(std::ostream &os) const;

	private:
		// Members
		std::string	_name;
		IType*		_power2;
		IType*		_power1;
		IType*		_power0;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Variable &num);

#endif
