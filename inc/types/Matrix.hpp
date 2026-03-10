/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:00:25 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/10 16:30:14 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_HPP
# define MATRIX_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Rational.hpp"
# include "Complex.hpp"
# include "Variable.hpp"

class	Matrix: public IType
{
	public:
		// Constructors and destructor
		Matrix(unsigned long width, unsigned long height);
		Matrix(std::string str, t_data &data);
		Matrix(const Token &token, t_data &data): Matrix(token.getValue(), data) {};
		Matrix(const Matrix &other);
		~Matrix(void);

		// Operator overloads
		Matrix&		operator=(const Matrix &other);
		explicit	operator bool() const;
		bool		operator==(const Matrix &other) const;
		bool		operator!=(const Matrix &other) const;
		Rational*	operator[](unsigned int index) const;
		IType*		operator+(const IType &other) const;
		Matrix*		operator+(const Matrix &other) const;
		Matrix*		operator+(const Rational &other) const;
		Matrix*		operator+(const Complex &other) const;
		Variable*	operator+(const Variable &other) const;
		IType*		operator-(const IType &other) const;
		Matrix*		operator-(const Matrix &other) const;
		Matrix*		operator-(const Rational &other) const;
		Matrix*		operator-(const Complex &other) const;
		Variable*	operator-(const Variable &other) const;
		IType*		operator*(const IType &other) const;
		Matrix*		operator*(const Matrix &other) const;
		Matrix*		operator*(const Rational &other) const;
		Matrix*		operator*(const Complex &other) const;
		Variable*	operator*(const Variable &other) const;
		IType*		operator/(const IType &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Matrix*		operator/(const Rational &other) const;
		Matrix*		operator/(const Complex &other) const;
		Variable*	operator/(const Variable &other) const;
		IType*		operator%(const IType &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Matrix*		operator%(const Rational &other) const;
		Matrix*		operator%(const Complex &other) const;
		Variable*	operator%(const Variable &other) const;
		IType*		operator^(const IType &other) const;
		Matrix*		operator^(const Matrix &other) const;
		Matrix*		operator^(const Rational &other) const;
		Matrix*		operator^(const Complex &other) const;
		Variable*	operator^(const Variable &other) const;

		// Getters
		unsigned long	getWidth(void) const;
		unsigned long	getHeight(void) const;

		// Setters
		void			setValue(unsigned int i, unsigned int j, Rational *value);

		// Methods
		IType*			matrix_operator(const IType &other) const;
		Matrix*			matrix_operator(const Matrix &other) const;
		Matrix*			matrix_operator(const Rational &other) const;
		Matrix*			matrix_operator(const Complex &other) const;
		Variable*		matrix_operator(const Variable &other) const;
		std::ostream	&print(std::ostream &os) const;
		IType*			clone(void) const;
		void			error(const std::logic_error &e) const;

	private:
		// Members
		unsigned long	_width;
		unsigned long	_height;
		Rational		**_matrix;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Matrix &matrix);

#endif
