/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:00:25 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/04 19:35:30 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_HPP
# define MATRIX_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Rational.hpp"

class	Matrix: public IType
{
	public:
		// Constructors and destructor
		Matrix(std::string str, t_data &data);
		Matrix(const Token &token, t_data &data): Matrix(token.getValue(), data) {};
		Matrix(const Matrix &other);
		~Matrix(void);

		// Operator overloads
		bool		operator==(const Matrix &other) const;
		bool		operator!=(const Matrix &other) const;
		Matrix&		operator=(const Matrix &other);
		Rational*	operator[](unsigned int index) const;
		IType*		operator+(const IType &other) const;
		Matrix*		operator+(const Matrix &other) const;
		IType*		operator-(const IType &other) const;
		Matrix*		operator-(const Matrix &other) const;
		IType*		operator*(const IType &other) const;
		Matrix*		operator*(const Matrix &other) const;
		IType*		operator/(const IType &other) const;
		Matrix*		operator/(const Matrix &other) const;
		IType*		operator%(const IType &other) const;
		Matrix*		operator%(const Matrix &other) const;
		IType*		operator^(const IType &other) const;
		Matrix*		operator^(const Matrix &other) const;

		// Getters
		unsigned long	getWidth(void) const;
		unsigned long	getHeight(void) const;

		// Methods
		IType*			matrix_operator(const IType &other) const;
		std::ostream	&print(std::ostream &os) const;
		IType*			clone(void) const;

	private:
		// Members
		unsigned long	_width;
		unsigned long	_height;
		Rational		**_matrix;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Matrix &matrix);

#endif
