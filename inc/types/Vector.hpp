/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 13:42:16 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/30 11:54:32 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Rational.hpp"
# include "Complex.hpp"
# include "Polynomial.hpp"
# include "computor-v2.hpp"

class	Vector: public IType
{
	public:
		// Constructors and destructor
		Vector(unsigned long int size);
		Vector(void): Vector(0) {};
		Vector(std::string str, t_data &data);
		Vector(const Token &token, t_data &data): 	\
				Vector(token.getValue(), data) {};
		Vector(const Vector &other): _vector(other._vector) {};
		Vector(const IType &other);
		~Vector(void);

		// Operator overloads
		explicit	operator bool() const;
		Vector&		operator=(const Vector &other);
		Vector		operator=(const IType &other);
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
		IType*		operator[](unsigned long int index) const;
		IType*		operator[](unsigned long int index);
		IType*		operator+(const IType &other) const;
		Vector*		operator+(const Vector &other) const;
		Vector*		operator+(const Rational &other) const;
		Vector*		operator+(const Complex &other) const;
		Matrix*		operator+(const Matrix &other) const;
		IType*		operator+(const Polynomial &other) const;
		Vector*		operator+(const Real &other) const;
		Vector*		operator+(const long long int value) const;
		Vector*		operator-(void) const;
		IType*		operator-(const IType &other) const;
		Vector*		operator-(const Vector &other) const;
		Vector*		operator-(const Rational &other) const;
		Vector*		operator-(const Complex &other) const;
		Matrix*		operator-(const Matrix &other) const;
		IType*		operator-(const Polynomial &other) const;
		Vector*		operator-(const Real &other) const;
		Vector*		operator-(const long long int value) const;
		IType*		operator*(const IType &other) const;
		Vector*		operator*(const Vector &other) const;
		Vector*		operator*(const Rational &other) const;
		Vector*		operator*(const Complex &other) const;
		Matrix*		operator*(const Matrix &other) const;
		IType*		operator*(const Polynomial &other) const;
		Vector*		operator*(const Real &other) const;
		Vector*		operator*(const long long int value) const;
		IType*		operator/(const IType &other) const;
		Vector*		operator/(const Vector &other) const;
		Vector*		operator/(const Rational &other) const;
		Vector*		operator/(const Complex &other) const;
		Matrix*		operator/(const Matrix &other) const;
		IType*		operator/(const Polynomial &other) const;
		Vector*		operator/(const Real &other) const;
		Vector*		operator/(const long long int value) const;
		IType*		operator%(const IType &other) const;
		Vector*		operator%(const Vector &other) const;
		Vector*		operator%(const Rational &other) const;
		Vector*		operator%(const Complex &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		Vector*		operator%(const Real &other) const;
		Vector*		operator%(const long long int value) const;
		IType*		operator^(const IType &other) const;
		Vector*		operator^(const Rational &other) const;
		Vector*		operator^(const long long int value) const;

		// Getters
		InfiniteFloat	getRoundedValue(unsigned long int index) const;

		// Setters
		void			setValue(unsigned long int index, IType* value);

		// Methods
		std::string		to_string(void) const;
		std::size_t		size(void) const;
		bool			empty(void) const;
		bool			in_C(void) const;
		bool			in_D(void) const;
		bool			in_M(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		std::ostream&	print(std::ostream &os) const;
		IType*			arccos(void) const;
		IType*			arcsin(void) const;
		IType*			arctan(void) const;
		IType*			clone(void) const;
		IType*			cos(void) const;
		IType*			e(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			matrix_operator(const IType &other) const;
		IType*			norm(void) const;
		IType*			sin(void) const;
		IType*			sqrt(void) const;
		IType*			tan(void) const;
		Rational*		gcd(const IType &ther) const;
		Rational*		gcd(const Vector &other) const;
		Rational*		gcd(const Rational &other) const;
		Rational*		gcd(const Complex &other) const;
		Rational*		gcd(const Matrix &other) const;
		void			print_rounded	\
				(const std::string var = std::string()) const;
		void			push_back(IType* value);

	private:
		// Members
		std::vector<IType*>	_vector;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream& os, const Vector& vector);

#endif
