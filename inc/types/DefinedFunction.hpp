/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefinedFunction.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 15:16:29 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/23 14:36:52 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTION_HPP
# define FUNCTION_HPP

# include "IType.hpp"

class	DefinedFunction: public IType
{
	public:
		// Utils
		typedef enum	e_function
		{
			E_FUNCTION_NOTHING,
			E_FUNCTION_ABSOLUTE,
			E_FUNCTION_NORM,
			E_FUNCTION_SQRT,
			E_FUNCTION_EXPONENTIAL,
			E_FUNCTION_COSINE,
			E_FUNCTION_SINE,
			E_FUNCTION_TANGENT,
			E_FUNCTION_RADIAN,
			E_FUNCTION_DEGREE,
			E_FUNCTION_FACTORIAL
		}				t_function;

		// Constructors and destructor
		DefinedFunction(t_function function): _function(function) {};
		DefinedFunction(void): _function(E_FUNCTION_NOTHING) {};
		~DefinedFunction(void) {};

		// Operator overloads
		explicit	operator bool(void) const;
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

		// Methods
		std::string		to_string(void) const;
		bool			in_C(void) const;
		bool			in_D(void) const;
		bool			in_M(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		std::ostream&	print(std::ostream &os) const;
		IType*			abs(void) const;
		IType*			clone(void) const;
		IType*			cos(void) const;
		IType*			exp(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			matrix_operator(const IType &other) const;
		IType*			norm(void) const;
		IType*			sin(void) const;
		IType*			sqrt(void) const;
		IType*			tan(void) const;
		Rational*		fact(void) const;
		Rational*		gcd(const IType &other) const;
		Real*			deg(void) const;
		Real*			rad(void) const;
		void			print_rounded(const std::string var = std::string()) const;

	private:
		t_function	_function;
};

std::ostream&	operator<<(std::ostream& os, const DefinedFunction& function);

#endif
