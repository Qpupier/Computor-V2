/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Polynomial.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:06:14 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/05 20:17:14 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLYNOMIAL_HPP
# define POLYNOMIAL_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Rational.hpp"
# include "Complex.hpp"
# include "Matrix.hpp"

class	Polynomial: public IType
{
	public:
		// Utils
		typedef struct	s_term
		{
			IType*				coefficient;
			unsigned short int	power;
		}				t_term;

		typedef struct	s_division_result
		{
			std::vector<t_term>	quotient;
			std::vector<t_term>	remainder;
		}				t_division_result;

		// Constructors and destructor
		Polynomial(std::string name): _name(name), _terms() {};
		Polynomial(void): Polynomial("") {};
		Polynomial(std::string name, t_term term);
		Polynomial(const Token &token): Polynomial(token.getValue(), t_term{new Rational(1), 1}) {};
		Polynomial(const Polynomial &other);
		Polynomial(const IType &other);
		~Polynomial(void);

		// Operator overloads
		Polynomial&	operator=(const Polynomial &other);
		explicit	operator bool() const;
		bool		operator==(const IType &other) const;
		bool		operator==(const Polynomial &other) const;
		bool		operator==(const Rational &other) const;
		bool		operator==(const Complex &other) const;
		bool		operator==(const Matrix &other) const;
		bool		operator!=(const IType &other) const;
		bool		operator<(const IType &other) const;
		bool		operator<=(const IType &other) const;
		bool		operator>(const IType &other) const;
		bool		operator>=(const IType &other) const;
		IType*		operator+(const IType &other) const;
		Polynomial*	operator+(const Polynomial &other) const;
		Polynomial*	operator+(const Rational &other) const;
		Polynomial*	operator+(const Complex &other) const;
		Polynomial*	operator+(const Matrix &other) const;
		IType*		operator-(const IType &other) const;
		Polynomial*	operator-(const Polynomial &other) const;
		Polynomial*	operator-(const Rational &other) const;
		Polynomial*	operator-(const Complex &other) const;
		Polynomial*	operator-(const Matrix &other) const;
		IType*		operator*(const IType &other) const;
		Polynomial*	operator*(const Polynomial &other) const;
		Polynomial*	operator*(const Rational &other) const;
		Polynomial*	operator*(const Complex &other) const;
		Polynomial*	operator*(const Matrix &other) const;
		IType*		operator/(const IType &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		Polynomial*	operator/(const Rational &other) const;
		Polynomial*	operator/(const Complex &other) const;
		Polynomial*	operator/(const Matrix &other) const;
		IType*		operator%(const IType &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		Polynomial*	operator%(const Rational &other) const;
		Polynomial*	operator%(const Complex &other) const;
		Polynomial*	operator%(const Matrix &other) const;
		IType*		operator^(const IType &other) const;
		Polynomial*	operator^(const Rational &other) const;

		// Getters
		std::string			getName(void) const;
		std::vector<t_term>	getTerms(void) const;
		std::vector<t_term>	getDividers(void) const;

		// Methods
		IType*			matrix_operator(const IType &other) const;
		IType*			function_operator(const IType &other) const;
		IType*			clone(void) const;
		Rational*		gcd(const IType &other) const;
		std::ostream&	print(std::ostream &os) const;
		void			sort_powers(void);
		void			factorize_constant_factor(void);
		void			reduce(void);

	private:
		// Members
		std::string			_name;
		std::vector<t_term>	_terms;
		std::vector<t_term>	_dividers;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Polynomial &num);

// Functions
std::string	to_lower(std::string s);
void		solve_trinomial(Polynomial *polynomial);

#endif
