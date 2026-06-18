/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:00:25 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/18 11:57:28 by qpupier          ###   ########lyon.fr   */
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
		explicit	operator bool() const;
		Matrix&		operator=(const Matrix &other);
		Matrix		operator=(const IType &other);
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
		Rational*	operator[](unsigned long long int index) const;
		Rational*	operator[](unsigned long long int index);
		IType*		operator+(const IType &other) const;
		Matrix*		operator+(const Matrix &other) const;
		Matrix*		operator+(const Rational &other) const;
		Matrix*		operator+(const Complex &other) const;
		Polynomial*	operator+(const Polynomial &other) const;
		Matrix*		operator+(const long long int value) const;
		Matrix*		operator-(void) const;
		IType*		operator-(const IType &other) const;
		Matrix*		operator-(const Matrix &other) const;
		Matrix*		operator-(const Rational &other) const;
		Matrix*		operator-(const Complex &other) const;
		Polynomial*	operator-(const Polynomial &other) const;
		Matrix*		operator-(const long long int value) const;
		IType*		operator*(const IType &other) const;
		Matrix*		operator*(const Matrix &other) const;
		Matrix*		operator*(const Rational &other) const;
		Matrix*		operator*(const Complex &other) const;
		Polynomial*	operator*(const Polynomial &other) const;
		Matrix*		operator*(const long long int value) const;
		IType*		operator/(const IType &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Matrix*		operator/(const Rational &other) const;
		Matrix*		operator/(const Complex &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		Matrix*		operator/(const long long int value) const;
		IType*		operator%(const IType &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Matrix*		operator%(const Rational &other) const;
		Matrix*		operator%(const Complex &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		Matrix*		operator%(const long long int value) const;
		IType*		operator^(const IType &other) const;
		Matrix*		operator^(const Rational &other) const;
		Matrix*		operator^(const long long int value) const;

		// Getters
		Rational				getValue(unsigned long long int i, 			\
				unsigned long long int j) const;
		InfiniteDouble			getRoundedValue(unsigned long long int i, 	\
				unsigned long long int j) const;
		unsigned long long int	getWidth(void) const;
		unsigned long long int	getHeight(void) const;

		// Setters
		void	setValue(unsigned long long int i, 	\
				unsigned long long int j, Rational value);

		// Methods
		IType*			clone(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_operator(const IType &other) const;
		Matrix*			matrix_operator(const Matrix &other) const;
		IType*			matrix_inversion(void) const;
		Rational*		gcd(const IType &ther) const;
		Rational*		gcd(const Matrix &other) const;
		Rational*		gcd(const Rational &other) const;
		Rational*		gcd(const Complex &other) const;
		std::ostream&	print(std::ostream &os) const;
		std::string		to_string(void) const;
		bool			values_in_D(void) const;
		bool			values_in_Z(void) const;
		void			error(const LogicError &e);
		void			free(void);
		void			print_rounded(const std::string var = std::string()) const;
		void			swap_lines(unsigned long long int line1, unsigned long long int line2);

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
