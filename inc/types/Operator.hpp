/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 14:55:07 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/30 11:36:29 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATOR_HPP
# define OPERATOR_HPP

# include "IType.hpp"
# include "Token.hpp"

class	Operator: public IType
{
	public:
		// Types
		typedef enum	e_operator
		{
			E_ADD,
			E_SUBTRACT,
			E_MULTIPLY,
			E_DIVIDE,
			E_MODULO,
			E_MATRIX,
			E_POWER,
			E_FUNCTION,
			E_INVERSE,
			E_UNKNOWN
		}				t_operator;

		// Constructors and destructor
		Operator(std::string op);
		Operator(const Token &token): Operator(token.getValue()) {};
		Operator(const Operator &other): _op(other._op) {};
		~Operator(void) {};

		// Operator overloads
		Operator&	operator=(const Operator &other);
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

		// Getters
		t_operator	getOperator(void) const;

		// Methods
		std::string		to_string(void) const;
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
		IType*			matrix_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			sin(void) const;
		IType*			sqrt(void) const;
		IType*			tan(void) const;
		Rational*		gcd(const IType &other) const;
		void			print_rounded(const std::string var = std::string()) const;

	private:
		// Members
		t_operator	_op;
};

// Output stream operator overload
std::ostream &operator<<(std::ostream &os, const Operator &op);

#endif
