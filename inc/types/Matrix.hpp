/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:00:25 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/21 18:06:03 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_HPP
# define MATRIX_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Rational.hpp"
# include "Complex.hpp"
# include "Polynomial.hpp"

class	Matrix: public IType
{
	public:
		// Constructors and destructor
		Matrix(unsigned long long int width, unsigned long long int height);
		Matrix(void): Matrix(0, 0) {};
		Matrix(std::string str, t_data &data);
		Matrix(const Token &token, t_data &data): Matrix(token.getValue(), data) {};
		Matrix(const Matrix &other);
		Matrix(const IType &other);
		~Matrix(void);

		// Operator overloads
		Matrix&		operator=(const Matrix &other);
		explicit	operator bool() const;
		bool		operator==(const IType &other) const;
		bool		operator==(const Matrix &other) const;
		bool		operator==(const Rational &other) const;
		bool		operator==(const Complex &other) const;
		bool		operator==(const Polynomial &other) const;
		bool		operator!=(const IType &other) const;
		bool		operator<(const IType &other) const;
		bool		operator<=(const IType &other) const;
		bool		operator>(const IType &other) const;
		bool		operator>=(const IType &other) const;
		Rational*	operator[](unsigned long long int index) const;
		Rational*	operator[](unsigned long long int index);
		IType*		operator+(const IType &other) const;
		Matrix*		operator+(const Matrix &other) const;
		Matrix*		operator+(const Rational &other) const;
		Matrix*		operator+(const Complex &other) const;
		Polynomial*	operator+(const Polynomial &other) const;
		IType*		operator-(const IType &other) const;
		Matrix*		operator-(const Matrix &other) const;
		Matrix*		operator-(const Rational &other) const;
		Matrix*		operator-(const Complex &other) const;
		Polynomial*	operator-(const Polynomial &other) const;
		IType*		operator*(const IType &other) const;
		Matrix*		operator*(const Matrix &other) const;
		Matrix*		operator*(const Rational &other) const;
		Matrix*		operator*(const Complex &other) const;
		Polynomial*	operator*(const Polynomial &other) const;
		IType*		operator/(const IType &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Matrix*		operator/(const Rational &other) const;
		Matrix*		operator/(const Complex &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		IType*		operator%(const IType &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Matrix*		operator%(const Rational &other) const;
		Matrix*		operator%(const Complex &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		IType*		operator^(const IType &other) const;
		Matrix*		operator^(const Rational &other) const;

		// Getters
		unsigned long long int	getWidth(void) const;
		unsigned long long int	getHeight(void) const;

		// Setters
		void	setValue(unsigned long long int i, 	\
				unsigned long long int j, Rational *value);

		// Methods
		IType*			matrix_operator(const IType &other) const;
		Matrix*			matrix_operator(const Matrix &other) const;
		IType*			function_operator(const IType &other) const;
		IType*			clone(void) const;
		Rational*		gcd(const IType &ther) const;
		Rational*		gcd(const Matrix &other) const;
		Rational*		gcd(const Rational &other) const;
		Rational*		gcd(const Complex &other) const;
		std::ostream&	print(std::ostream &os) const;
		bool			finite_decimals(void) const;
		bool			in_Z(void) const;
		void			error(const LogicError &e);
		void			free(void);
		void			print_variable(const std::string var) const;

	private:
		// Members
		unsigned long long int	_width;
		unsigned long long int	_height;
		Rational**				_matrix;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Matrix &matrix);

// Functions
bool	is_matrix(const IType& type);

#endif
