/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 14:55:07 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/25 15:39:10 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATOR_HPP
# define OPERATOR_HPP

# include "IType.hpp"
# include "AST.hpp"
# include "Token.hpp"
# include "computor-v2.hpp"

class	Operator: public IType
{
	public:
		typedef enum	e_operator
		{
			E_ADD,
			E_SUBTRACT,
			E_MULTIPLY,
			E_DIVIDE,
			E_MODULO,
			E_POWER,
			E_MATRIX
		}				t_operator;

		// Constructors and destructor
		Operator(std::string op);
		Operator(const Token &token): Operator(token.getValue()) {};
		Operator(const Operator &other): _op(other._op) {};
		~Operator(void) {};

		// Operator overloads
		Operator	&operator=(const Operator &other);
		AST			*operator+(const IType &other) const;
		AST			*operator-(const IType &other) const;
		AST			*operator*(const IType &other) const;
		AST			*operator/(const IType &other) const;
		AST			*operator%(const IType &other) const;
		AST			*operator^(const IType &other) const;

		// Getters
		t_operator	getOperator(void) const;

		// Methods
		AST*			matrix_operator(const IType &other) const;
		std::ostream	&print(std::ostream &os) const;

	private:
		// Members
		t_operator	_op;
};

#endif
