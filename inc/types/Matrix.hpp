/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:00:25 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/11 16:41:56 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_HPP
# define MATRIX_HPP

# include "computor-v2.hpp"
# include "Token.hpp"
# include "Rational.hpp"
# include "Complex.hpp"
# include "Vector.hpp"
# include "Polynomial.hpp"

class	Matrix: public IType
{
	public:
		// Constructors and destructor
		Matrix(unsigned long int width, unsigned long int height);
		Matrix(void): Matrix(0, 0) {};
		Matrix(std::string str, t_data &data);
		Matrix(const Token &token, t_data &data): 	\
				Matrix(token.getValue(), data) {};
		Matrix(const Matrix &other);
		Matrix(const IType &other);
		Matrix(const Vector &other);
		Matrix(const Polynomial &other);
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
		IType**		operator[](unsigned long int index) const;
		IType*		operator+(const IType &other) const;
		Matrix*		operator+(const Matrix &other) const;
		Matrix*		operator+(const Rational &other) const;
		Matrix*		operator+(const Complex &other) const;
		IType*		operator+(const Polynomial &other) const;
		Matrix*		operator+(const Vector &other) const;
		Matrix*		operator+(const Real &other) const;
		Matrix*		operator+(const long long int value) const;
		Matrix*		operator-(void) const;
		IType*		operator-(const IType &other) const;
		Matrix*		operator-(const Matrix &other) const;
		Matrix*		operator-(const Rational &other) const;
		Matrix*		operator-(const Complex &other) const;
		IType*		operator-(const Polynomial &other) const;
		Matrix*		operator-(const Vector &other) const;
		Matrix*		operator-(const Real &other) const;
		Matrix*		operator-(const long long int value) const;
		IType*		operator*(const IType &other) const;
		Matrix*		operator*(const Matrix &other) const;
		Matrix*		operator*(const Rational &other) const;
		Matrix*		operator*(const Complex &other) const;
		IType*		operator*(const Polynomial &other) const;
		Matrix*		operator*(const Vector &other) const;
		Matrix*		operator*(const Real &other) const;
		Matrix*		operator*(const long long int value) const;
		IType*		operator/(const IType &other) const;
		Matrix*		operator/(const Matrix &other) const;
		Matrix*		operator/(const Rational &other) const;
		Matrix*		operator/(const Complex &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		Matrix*		operator/(const Vector &other) const;
		Matrix*		operator/(const Real &other) const;
		Matrix*		operator/(const long long int value) const;
		IType*		operator%(const IType &other) const;
		Matrix*		operator%(const Matrix &other) const;
		Matrix*		operator%(const Rational &other) const;
		Matrix*		operator%(const Complex &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		Matrix*		operator%(const Vector &other) const;
		Matrix*		operator%(const Real &other) const;
		Matrix*		operator%(const long long int value) const;
		IType*		operator^(const IType &other) const;
		Matrix*		operator^(const Rational &other) const;
		Matrix*		operator^(const long long int value) const;

		// Getters
		InfiniteDecimal		getRoundedValue(unsigned long int i, 	\
			unsigned long int j) const;
		t_type				getType(void) const;
		IType*				getValue(unsigned long int i, 			\
				unsigned long int j) const;
		unsigned long int	getWidth(void) const;
		unsigned long int	getHeight(void) const;

		// Setters
		void	setValue(unsigned long int i, 	\
				unsigned long int j, IType* value);

		// Methods
		std::string		to_string(void) const;
		bool			in_C(void) const;
		bool			in_D(void) const;
		bool			in_M(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		bool			is_null(void) const;
		bool			is_square(void) const;
		std::ostream&	print(std::ostream &os) const;
		IType*			abs(void) const;
		IType*			clone(void) const;
		IType*			cos(void) const;
		IType*			exp(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			norm(void) const;
		IType*			sin(void) const;
		IType*			sqrt(void) const;
		IType*			tan(void) const;
		Matrix*			matrix_operator(const Matrix &other) const;
		Rational*		fact(void) const;
		Rational*		gcd(const IType &ther) const;
		Rational*		gcd(const Matrix &other) const;
		Rational*		gcd(const Rational &other) const;
		Rational*		gcd(const Complex &other) const;
		Rational*		gcd(const Vector &other) const;
		Rational*		gcd(const Polynomial &other) const;
		Rational*		gcd(const Real &other) const;
		Real*			deg(void) const;
		Real*			rad(void) const;
		void			empty(void);
		void			error(const LogicError &e);
		void			free(void);
		void			print_rounded(							\
				const std::string var = std::string()) const;
		void			round(void);
		void			swap_lines(unsigned long int line1, 	\
				unsigned long int line2);

		//Constants
		static constexpr unsigned char	MAX_NEWTON_ITERATIONS = 7;

	private:
		// Members
		unsigned long int	_width;
		unsigned long int	_height;
		IType***			_matrix;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Matrix &matrix);

// Functions
bool	is_matrix(const IType& type);
Matrix*	matrix_null(unsigned long int width, unsigned long int height);

#endif
