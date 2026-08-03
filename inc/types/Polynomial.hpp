/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Polynomial.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:06:14 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/03 14:27:12 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLYNOMIAL_HPP
# define POLYNOMIAL_HPP

# include "IType.hpp"
# include "Token.hpp"
# include "Rational.hpp"
# include "Complex.hpp"
# include "Matrix.hpp"
# include "Vector.hpp"

class	Polynomial: public IType
{
	public:
		// Utils
		typedef struct	s_term
		{
			IType*				coefficient;
			unsigned long int	power;// [ ] Est-ce le bon type ?
		}				t_term;

		typedef struct	s_division_result
		{
			std::vector<t_term>	quotient;
			std::vector<t_term>	remainder;
		}				t_division_result;

		// Constructors and destructor
		Polynomial(std::string name, t_term term);
		Polynomial(std::string name): Polynomial(name, t_term{new Rational(0), 0}) {};
		Polynomial(const Token &token): Polynomial(token.getValue(), t_term{new Rational(1), 1}) {};
		Polynomial(const Polynomial &other);
		Polynomial(const IType &other);
		~Polynomial(void);

		// Operator overloads
		explicit	operator bool() const;
		Polynomial&	operator=(const Polynomial &other);
		Polynomial	operator=(const IType &other);
		bool		operator==(const IType &other) const;
		bool		operator==(const Polynomial &other) const;
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
		Polynomial*	operator+(const Polynomial &other) const;
		IType*		operator+(const long long int value) const;
		IType*		operator-(void) const;
		IType*		operator-(const IType &other) const;
		IType*		operator-(const Polynomial &other) const;
		IType*		operator-(const long long int value) const;
		IType*		operator*(const IType &other) const;
		Polynomial*	operator*(const Polynomial &other) const;
		IType*		operator*(const long long int value) const;
		IType*		operator/(const IType &other) const;
		Polynomial*	operator/(const Polynomial &other) const;
		IType*		operator/(const long long int value) const;
		IType*		operator%(const IType &other) const;
		Polynomial*	operator%(const Polynomial &other) const;
		IType*		operator%(const long long int value) const;
		IType*		operator^(const IType &other) const;
		Polynomial*	operator^(const Rational &other) const;
		Polynomial*	operator^(const long long int value) const;

		// Getters
		std::string			getName(void) const;
		std::vector<t_term>	getTerms(void) const;
		std::vector<t_term>	getDividers(void) const;

		// Setters
		void	setName(const std::string& name);
		void	setTerms(const std::vector<t_term>& terms);

		// Methods
		std::string		to_string(void) const;
		bool			in_C(void) const;
		bool			in_D(void) const;
		bool			in_M(void) const;
		bool			in_Q(void) const;
		bool			in_Z(void) const;
		std::ostream&	print(std::ostream &os) const;
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
		Rational*		gcd(const IType &other) const;
		void			factorize_constant_factor(void);
		void			free(void);
		void			print_rounded(const std::string var 	\
				= std::string()) const;
		void			reduce(void);
		void			sort_powers(void);

	private:
		// Members
		std::string			_name;
		std::vector<t_term>	_terms;
		std::vector<t_term>	_dividers;
};

// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Polynomial &num);

// Functions
std::vector<Polynomial::t_term>	vector_term_coeff_multiplication(	\
		const IType *coefficient, const unsigned long int power, 	\
		const std::vector<Polynomial::t_term> &vector);
void							add_terms_to_vector(				\
		const std::vector<Polynomial::t_term> &terms, 				\
		std::vector<Polynomial::t_term> &vector);
void							free_vector_terms(					\
		std::vector<Polynomial::t_term> &vector);

#endif
