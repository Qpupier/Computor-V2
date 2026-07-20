/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefinedFunction.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 15:16:29 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/20 13:47:04 by qpupier          ###   ########lyon.fr   */
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
			E_FUNCTION_NORM,
			E_FUNCTION_ABSOLUTE,
			E_FUNCTION_SQRT
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
		bool			in_D(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		std::ostream&	print(std::ostream &os) const;
		IType*			clone(void) const;
		IType*			function_operator(const IType &other) const;
		IType*			matrix_inversion(void) const;
		IType*			matrix_operator(const IType &other) const;
		Rational*		gcd(const IType &other) const;
		void			print_rounded(const std::string var = std::string()) const;

	private:
		t_function	_function;
};

std::ostream&	operator<<(std::ostream& os, const DefinedFunction& function);

#endif
