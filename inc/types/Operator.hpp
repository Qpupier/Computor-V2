/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 14:55:07 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/29 11:24:23 by qpupier          ###   ########lyon.fr   */
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
		bool		operator<=(const IType &other) const;
		bool		operator>(const IType &other) const;
		bool		operator>=(const IType &other) const;
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
		IType*			matrix_operator(const IType &other) const;
		IType*			function_operator(const IType &other) const;
		IType*			clone(void) const;
		Rational*		gcd(const IType &other) const;
		std::ostream&	print(std::ostream &os) const;
		bool			finite_decimals(void) const;
		void			print_rounded(const std::string var = std::string()) const;

	private:
		// Members
		t_operator	_op;
};

#endif
