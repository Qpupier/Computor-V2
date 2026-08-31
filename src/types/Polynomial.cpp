/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Polynomial.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:19:47 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/31 17:59:16 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Polynomial.hpp"
#include "Matrix.hpp"

// Utils

static Polynomial::t_term				get_term_result(					\
		const Polynomial::t_term &dividend, const Polynomial::t_term &divisor)
{
	unsigned long int	new_power;

	if (divisor.power > dividend.power)
		throw ERROR_UNEXPECTED;
	new_power = dividend.power - divisor.power;
	return ((Polynomial::t_term){
			.coefficient = *dividend.coefficient / *divisor.coefficient, 	\
			.power = new_power});
}

static void								add_term_to_vector(					\
		IType *coefficient, unsigned long int power, 						\
		std::vector<Polynomial::t_term> &vector)
{
	std::vector<Polynomial::t_term>::iterator	it(vector.begin());
	IType*										tmp;

	while (it != vector.end())
	{
		if (it->power == power)
		{
			tmp = it->coefficient;
			it->coefficient = *it->coefficient + *coefficient;
			delete tmp;
			return ;
		}
		it++;
	}
	vector.push_back((Polynomial::t_term){coefficient->clone(), power});
}

static void								add_blank_terms(					\
		std::vector<Polynomial::t_term> &terms, unsigned long int max_power)
{
	IType*	tmp;

	for (unsigned long int i = 0; i <= max_power; i++)
	{
		tmp = new Rational(0);
		add_term_to_vector(tmp, i, terms);
		delete tmp;
	}
}

static void								clean_terms(						\
		std::vector<Polynomial::t_term> &terms)
{
	std::vector<Polynomial::t_term>::const_iterator	it(terms.begin());
	unsigned long int								max_power(0);
	bool											is_empty(true);

	if (terms.empty())
		return ;
	while (it != terms.end())
	{
		if (!(*(it->coefficient)))
		{
			delete it->coefficient;
			terms.erase(it);
			continue;
		}
		if (it->power >= max_power)
		{
			max_power = it->power;
			is_empty = false;
		}
		it++;
	}
	if (is_empty)
		return free_vector_terms(terms);
	add_blank_terms(terms, max_power);
}

static void								terms_sort_powers(					\
		std::vector<Polynomial::t_term> &terms)
{
	bool (*fct)(const Polynomial::t_term&, const Polynomial::t_term&);

	fct = [](const Polynomial::t_term &a, const Polynomial::t_term &b)
	{
		return (a.power < b.power);
	};
	std::sort(terms.begin(), terms.end(), fct);
}

static std::vector<Polynomial::t_term>	euclidean_division_step(			\
		const std::vector<Polynomial::t_term> &dividend, 					\
		const std::vector<Polynomial::t_term> &divisor, 					\
		std::vector<Polynomial::t_term> &quotient)
{
	std::vector<Polynomial::t_term>	distrib;
	std::vector<Polynomial::t_term>	new_result;
	std::vector<Polynomial::t_term>	result;
	Polynomial::t_term				term_result;
	IType*							sub;

	if (dividend.size() < divisor.size())
	{
		add_terms_to_vector(dividend, result);
		return (result);
	}
	term_result = get_term_result(dividend[dividend.size() - 1], 	\
			divisor[divisor.size() - 1]);
	quotient.push_back(term_result);
	add_terms_to_vector(dividend, result);
	sub = -*term_result.coefficient;
	distrib = vector_term_coeff_multiplication(sub, term_result.power, divisor);
	add_terms_to_vector(distrib, result);
	free_vector_terms(distrib);
	delete sub;
	clean_terms(result);
	terms_sort_powers(result);
	new_result = euclidean_division_step(result, divisor, quotient);
	free_vector_terms(result);
	return (new_result);
}

static std::vector<Polynomial::t_term>	multiply_vectors(					\
		const std::vector<Polynomial::t_term> &terms1, 						\
		const std::vector<Polynomial::t_term> &terms2)
{
	std::vector<Polynomial::t_term>					result;
	std::vector<Polynomial::t_term>::const_iterator	it1(terms1.begin());
	std::vector<Polynomial::t_term>::const_iterator	it2;
	IType*											new_coefficient;

	while (it1 != terms1.end())
	{
		it2 = terms2.begin();
		while (it2 != terms2.end())
		{
			new_coefficient = *it1->coefficient * *it2->coefficient;
			add_term_to_vector(new_coefficient, it1->power + it2->power, 	\
					result);
			delete new_coefficient;
			it2++;
		}
		it1++;
	}
	return (result);
}

static std::vector<Polynomial::t_term>	vector_term_multiplication(			\
		const std::vector<Polynomial::t_term> &terms1, 						\
		const std::vector<Polynomial::t_term> &terms2)
{
	std::vector<Polynomial::t_term>					result;
	std::vector<Polynomial::t_term>::const_iterator	it1(terms1.begin());
	std::vector<Polynomial::t_term>::const_iterator	it2;

	while (it1 != terms1.end())
	{
		it2 = terms2.begin();
		while (it2 != terms2.end())
		{
			unsigned long int	new_power = it1->power + it2->power;
			result.push_back((Polynomial::t_term)	\
					{*it1->coefficient * *it2->coefficient, new_power});
			it2++;
		}
		it1++;
	}
	return (result);
}

static Polynomial::t_division_result	euclidean_division(					\
		const std::vector<Polynomial::t_term> &dividend, 					\
		const std::vector<Polynomial::t_term> &divisor, 					\
		bool is_recursive = false)
{
	std::vector<Polynomial::t_term>	quotient;
	std::vector<Polynomial::t_term>	remainder;
	Polynomial::t_division_result	result;

	if (divisor.size() <= 1)
	{
		if (is_recursive)
			add_terms_to_vector(divisor, quotient);
		add_terms_to_vector(dividend, remainder);
		return ((Polynomial::t_division_result){quotient, remainder});
	}
	remainder = euclidean_division_step(dividend, divisor, quotient);
	clean_terms(quotient);
	terms_sort_powers(quotient);
	if (!is_recursive)
		return ((Polynomial::t_division_result){quotient, remainder});
	result = euclidean_division(divisor, remainder, true);
	free_vector_terms(quotient);
	free_vector_terms(remainder);
	return (result);
}

static void								print_coefficient_sign(				\
		std::ostream& os, IType** coefficient)
{
	IType*	tmp;

	if (**coefficient < 0)
	{
		os << " - ";
		tmp = *coefficient;
		*coefficient = **coefficient * (-1);
		delete tmp;
	}
	else
		os << " + ";
}

static void								print_coefficient(					\
		std::ostream &os, IType *coefficient, unsigned long int power, 		\
		bool first_term)
{
	Complex*	complex;
	bool		need_parentheses;

	if (!first_term)
		print_coefficient_sign(os, &coefficient);
	complex = dynamic_cast<Complex*>(coefficient);
	need_parentheses = complex && *complex->getReal() && *complex->getImaginary();
	if (need_parentheses)
		os << "(";
	if (power && *coefficient == -1)
		os << "-";
	else if (!power || *coefficient != 1)
		os << *coefficient;
	if (need_parentheses)
		os << ")";
	else if (complex && *complex->getImaginary() && power)
		os << " * ";
	delete coefficient;
}

static std::ostream&					print_terms(std::ostream &os, 		\
		const std::vector<Polynomial::t_term> &terms, 						\
		const std::string &name, bool alone = false)
{
	bool	first_term;

	if (!alone && terms.size() > 1)
		os << "(";
	first_term = true;
	for (std::vector<Polynomial::t_term>::const_reverse_iterator 	\
			it(terms.rbegin()); it != terms.rend(); it++)
	{
		if (!*it->coefficient)
			continue ;
		print_coefficient(os, it->coefficient->clone(), it->power, first_term);
		if (it->power)
		{
			os << name;
			if (it->power > 1)
				os << "^" << it->power;
		}
		first_term = false;
	}
	if (first_term)
		os << "0";
	if (!alone && terms.size() > 1)
		os << ")";
	return (os);
}

static Rational*						vector_gcd(							\
		const std::vector<Polynomial::t_term> &vector)
{
	Rational*	gcd;
	IType*		tmp;

	gcd = new Rational(0);
	for (std::vector<Polynomial::t_term>::const_iterator 	\
			it(vector.begin()); it != vector.end(); it++)
	{
		tmp = gcd;
		gcd = gcd->gcd(*it->coefficient);
		delete tmp;
	}
	return (gcd);
}

static IType*							function_operator_term(				\
		const std::vector<Polynomial::t_term> &terms, const IType &other)
{
	IType*	result;
	IType*	power;
	IType*	term;
	IType*	tmp;

	result = new Rational();
	for (std::vector<Polynomial::t_term>::const_iterator 	\
			it(terms.begin()); it != terms.end(); it++)
	{
		power = other ^ Rational(static_cast<long long int>(it->power));
		term = *it->coefficient * *power;
		delete power;
		tmp = result;
		result = *result + *term;
		delete tmp;
		delete term;
	}
	return (result);
}

static void								divide_constant_factor(				\
		const Rational &factor, std::vector<Polynomial::t_term> &terms, 	\
		std::vector<Polynomial::t_term> &dividers)
{
	IType	*tmp;

	for (std::vector<Polynomial::t_term>::iterator it(terms.begin()); 		\
			it != terms.end(); it++)
	{
		tmp = it->coefficient;
		it->coefficient = *it->coefficient / factor;
		delete tmp;
	}
	for (std::vector<Polynomial::t_term>::iterator it(dividers.begin()); 	\
			it != dividers.end(); it++)
	{
		tmp = it->coefficient;
		it->coefficient = *it->coefficient / factor;
		delete tmp;
	}
}

static bool								is_it_different_variables(			\
		const Polynomial& p1, const Polynomial& p2)
{
	if ((p1.getTerms().size() <= 1 && p1.getDividers().size() <= 1) 	\
			|| (p2.getTerms().size() <= 1 && p2.getDividers().size() <= 1))
		return (false);
	return (to_lower(p1.getName()) != to_lower(p2.getName()));
}

static void								second_division(					\
		std::vector<Polynomial::t_term> &terms, 							\
		std::vector<Polynomial::t_term> &dividers, 							\
		const std::vector<Polynomial::t_term> &remainder)
{
	Polynomial::t_division_result	reduced;

	reduced = euclidean_division(terms, remainder);
	free_vector_terms(terms);
	terms = reduced.quotient;
	free_vector_terms(reduced.remainder);
	reduced = euclidean_division(dividers, remainder);
	free_vector_terms(dividers);
	dividers = reduced.quotient;
	free_vector_terms(reduced.remainder);
}

static void								vectors_multiplication(				\
		const std::vector<Polynomial::t_term> &a, 							\
		const std::vector<Polynomial::t_term> &b, 							\
		std::vector<Polynomial::t_term> &result)
{
	std::vector<Polynomial::t_term>	distributivity;

	for (std::vector<Polynomial::t_term>::const_iterator it(a.begin()); 	\
			it != a.end(); it++)
	{
		distributivity = vector_term_coeff_multiplication(it->coefficient, 	\
				it->power, b);
		add_terms_to_vector(distributivity, result);
		free_vector_terms(distributivity);
	}
}

static IType*							sum_square_coefficient_terms(		\
		const std::vector<Polynomial::t_term>& terms)
{
	IType*	square;
	IType*	sum;
	IType*	tmp;

	sum = new Rational();
	for (const auto& term : terms)
	{
		tmp = sum;
		square = *term.coefficient * *term.coefficient;
		sum = *sum + *square;
		delete square;
		delete tmp;
	}
	return (sum);
}

static IType*							terms_sqrt_sub_pairs(				\
		IType* result_coefficient_num, 										\
		const std::vector<Polynomial::t_term>& terms, 						\
		unsigned long int sqrt_degree, unsigned long int i)
{
	for (unsigned long int j(1); j <= i - 1; j++)
	{
		IType*	pair = *terms[sqrt_degree - j].coefficient 	\
				* *terms[sqrt_degree - i + j].coefficient;
		IType*	ptr = result_coefficient_num;
		result_coefficient_num = *result_coefficient_num - *pair;
		delete ptr;
		delete pair;
	}
	return (result_coefficient_num);
}

static void								terms_sqrt_loop(					\
		const std::string& name, 											\
		const std::vector<Polynomial::t_term>& terms, Polynomial*& result, 	\
		unsigned long int sqrt_degree)
{
	unsigned long int	size(sqrt_degree * 2);
	IType*				result_coefficient_den;
	IType*				result_coefficient_num;
	IType*				tmp;

	for (unsigned long int i(1); i < sqrt_degree + 1; i++)
	{
		result_coefficient_num = terms_sqrt_sub_pairs(						\
				terms[size - i].coefficient->clone(), result->getTerms(), 	\
				sqrt_degree, i);
		result_coefficient_den 												\
				= *result->getTerms()[sqrt_degree].coefficient * 2;
		tmp = result;
		result = *result + Polynomial(name, (Polynomial::t_term)			\
				{*result_coefficient_num / *result_coefficient_den, 		\
					sqrt_degree - i});
		delete result_coefficient_num;
		delete result_coefficient_den;
		delete tmp;
	}
}

static IType*							terms_sqrt_verification(			\
		const Polynomial& ref, Polynomial* result)
{
	IType*	square;

	square = *result ^ 2;
	if (*square != ref)
	{
		delete square;
		delete result;
		throw UnsupportedError("This polynomial does not have a polynomial as square root");
	}
	delete square;
	return (result);
}

static IType*							terms_sqrt(const Polynomial& ref, 	\
		const std::vector<Polynomial::t_term>& terms)
{
	unsigned long int	size(terms.size());
	unsigned long int	sqrt_degree;
	Polynomial*			result;

	if (size == 1)
		return (terms[0].coefficient->sqrt());
	if (size % 2 == 0)
		throw UnsupportedError("This polynomial does not have a square root with coefficients in ℕ");
	sqrt_degree = (size - 1) / 2;
	result = new Polynomial(ref.getName(), (Polynomial::t_term){terms[size - 1].coefficient->sqrt(), sqrt_degree});
	terms_sqrt_loop(ref.getName(), terms, result, sqrt_degree);
	return (terms_sqrt_verification(ref, result));
}


// Constructors and destructor

Polynomial::Polynomial(std::string name, t_term term)
{
	this->_name = name;
	this->_terms.push_back(term);
	this->_dividers.push_back((t_term){new Rational(1), 0});
	this->reduce();
}

Polynomial::Polynomial(const Polynomial &other)
{
	std::vector<t_term>::const_iterator	it_terms(other._terms.begin());
	std::vector<t_term>::const_iterator	it_dividers(other._dividers.begin());

	this->_name = other._name;
	while (it_terms != other._terms.end())
	{
		this->_terms.push_back((t_term)		\
				{it_terms->coefficient->clone(), it_terms->power});
		it_terms++;
	}
	while (it_dividers != other._dividers.end())
	{
		this->_dividers.push_back((t_term)	\
				{it_dividers->coefficient->clone(), it_dividers->power});
		it_dividers++;
	}
}

Polynomial::Polynomial(const IType &other)
{
	const Polynomial*	other_polynomial;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		*this = *other_polynomial;
	else
		*this = Polynomial("", (t_term){other.clone(), 0});
	this->reduce();
}

Polynomial::~Polynomial(void)
{
	this->free();
}


// Operator overloads

Polynomial::operator bool() const
{
	std::vector<t_term>::const_iterator	it(this->_terms.begin());

	while (it != this->_terms.end())
	{
		if (*it->coefficient)
			return (true);
		it++;
	}
	return (false);
}

Polynomial&	Polynomial::operator=(const Polynomial &other)
{
	std::vector<t_term>::const_iterator	it_terms(other._terms.begin());
	std::vector<t_term>::const_iterator	it_dividers(other._dividers.begin());

	if (this != &other)
	{
		this->free();
		this->_name = other._name;
		while (it_terms != other._terms.end())
		{
			this->_terms.push_back((t_term)		\
					{it_terms->coefficient->clone(), it_terms->power});
			it_terms++;
		}
		while (it_dividers != other._dividers.end())
		{
			this->_dividers.push_back((t_term)	\
					{it_dividers->coefficient->clone(), it_dividers->power});
			it_dividers++;
		}
	}
	return (*this);
}

Polynomial	Polynomial::operator=(const IType &other)
{
	*this = Polynomial(other);
	return (*this);
}

bool		Polynomial::operator==(const Polynomial &other) const
{
	std::vector<t_term>::const_iterator	it_terms(this->_terms.begin());
	std::vector<t_term>::const_iterator	it_terms_other(other._terms.begin());
	std::vector<t_term>::const_iterator	it_dividers(this->_dividers.begin());
	std::vector<t_term>::const_iterator	it_dividers_other					\
			(other._dividers.begin());

	if (this->_name != other._name || this->_terms.size() != other._terms.size())
		return (false);
	while (it_terms != this->_terms.end())
	{
		if (*it_terms->coefficient != *it_terms_other->coefficient 			\
				|| it_terms->power != it_terms_other->power)
			return (false);
		it_terms++;
		it_terms_other++;
	}
	while (it_dividers != this->_dividers.end())
	{
		if (*it_dividers->coefficient != *it_dividers_other->coefficient 	\
				|| it_dividers->power != it_dividers_other->power)
			return (false);
		it_dividers++;
		it_dividers_other++;
	}
	return (true);
}

bool		Polynomial::operator==(const IType &other) const
{
	try
	{
		Polynomial	other_polynomial(other);

		return (*this == other_polynomial);
	}
	catch(const UnexpectedError &e)
	{
	}
	return (false);
}

bool		Polynomial::operator==(const long long int value) const
{
	return (*this == Rational(value));
}

bool		Polynomial::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Polynomial::operator!=(const long long int value) const
{
	return (*this != Rational(value));
}

bool		Polynomial::operator<(const IType &other) const
{
	return (false);
	(void)other;
}

bool		Polynomial::operator<(const long long int value) const
{
	return (*this < Rational(value));
}

bool		Polynomial::operator<=(const IType &other) const
{
	return (false);
	(void)other;
}

bool		Polynomial::operator<=(const long long int value) const
{
	return (*this <= Rational(value));
}

bool		Polynomial::operator>(const IType &other) const
{
	return (false);
	(void)other;
}

bool		Polynomial::operator>(const long long int value) const
{
	return (*this > Rational(value));
}

bool		Polynomial::operator>=(const IType &other) const
{
	return (false);
	(void)other;
}

bool		Polynomial::operator>=(const long long int value) const
{
	return (*this >= Rational(value));
}

Polynomial*	Polynomial::operator+(const Polynomial &other) const
{
	std::vector<t_term>	distributivity;
	std::string			name(this->_name);
	Polynomial*			result;

	if (is_it_different_variables(*this, other))
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	if (this->getName().empty())
		name = other.getName();
	else if (!other.getName().empty() && this->getName() != other.getName())
		name = to_lower(this->getName());
	result = new Polynomial(name);
	free_vector_terms(result->_dividers);
	result->_dividers 	\
			= vector_term_multiplication(this->_dividers, other._dividers);
	vectors_multiplication(this->_terms, other._dividers, result->_terms);
	vectors_multiplication(other._terms, this->_dividers, result->_terms);
	result->reduce();
	return (result);
}

IType*		Polynomial::operator+(const IType &other) const
{
	return (*this + Polynomial(other));
}

IType*		Polynomial::operator+(const long long int value) const
{
	return (*this + Rational(value));
}

IType*		Polynomial::operator-(void) const
{
	return (*this * (-1));
}

IType*		Polynomial::operator-(const Polynomial &other) const
{
	IType*	sub;
	IType*	result;

	sub = -other;
	result = *this + *sub;
	delete sub;
	return (result);
}

IType*		Polynomial::operator-(const IType &other) const
{
	return (*this - Polynomial(other));
}

IType*		Polynomial::operator-(const long long int value) const
{
	return (*this - Rational(value));
}

Polynomial*	Polynomial::operator*(const Polynomial &other) const
{
	std::string	name(this->_name);
	Polynomial*	result;

	if (is_it_different_variables(*this, other))
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	if (this->getName().empty())
		name = other.getName();
	else if (!other.getName().empty() && this->getName() != other.getName())
		name = to_lower(this->getName());
	result = new Polynomial(name);
	free_vector_terms(result->_terms);
	free_vector_terms(result->_dividers);
	result->_terms = multiply_vectors(this->_terms, other._terms);
	result->_dividers = multiply_vectors(this->_dividers, other._dividers);
	result->reduce();
	return (result);
}

IType*		Polynomial::operator*(const IType &other) const
{
	return (*this * Polynomial(other));
}

IType*		Polynomial::operator*(const long long int value) const
{
	return (*this * Rational(value));
}

Polynomial*	Polynomial::operator/(const Polynomial &other) const
{
	std::string	name(this->_name);
	Polynomial*	result;

	if (is_it_different_variables(*this, other))
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	if (this->getName().empty())
		name = other.getName();
	else if (!other.getName().empty() && this->getName() != other.getName())
		name = to_lower(this->getName());
	result = new Polynomial(name);
	result->free();
	result->_terms = multiply_vectors(this->_terms, other._dividers);
	result->_dividers = multiply_vectors(this->_dividers, other._terms);
	try
	{
		result->reduce();
	}
	catch (...)
	{
		delete result;
		throw;
	}
	return (result);
}

IType*		Polynomial::operator/(const IType &other) const
{
	return (*this / Polynomial(other));
}

IType*		Polynomial::operator/(const long long int value) const
{
	return (*this / Rational(value));
}

Polynomial*	Polynomial::operator%(const Polynomial &other) const
{
	t_division_result	division_result;
	Polynomial*			division;
	Polynomial*			result;

	division = *this / other;
	division_result = euclidean_division(division->_terms, other._terms);
	result = new Polynomial(division->_name);
	result->_terms.insert(result->_terms.end(), 	\
			division_result.remainder.begin(), division_result.remainder.end());
	delete division;
	return (result);
}

IType*		Polynomial::operator%(const IType &other) const
{
	return (*this % Polynomial(other));
}

IType*		Polynomial::operator%(const long long int value) const
{
	return (*this % Rational(value));
}

Polynomial*	Polynomial::operator^(const Rational &other) const
{
	Polynomial*	result;
	Polynomial*	tmp;
	InfiniteInt	power;

	power = other.getNumerator();
	if (!power)
		return (new Polynomial(this->getName(), 	\
			(Polynomial::t_term){new Rational(1), 0}));
	result = new Polynomial(*this);
	while (--power)
	{
		tmp = result;
		result = *result * *this;
		delete tmp;
	}
	result->reduce();
	return (result);
}

IType*		Polynomial::operator^(const IType &other) const
{
	Rational	power;

	try
	{
		power = Rational(other);
	}
	catch(const UnexpectedError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	if (!power.in_Z() || power < 0)
		throw UNSUPPORTED_EXPONENT;
	return (*this ^ power);
}

Polynomial*	Polynomial::operator^(const long long int value) const
{
	return (*this ^ Rational(value));
}


// Getters

std::vector<Polynomial::t_term>	Polynomial::getDividers(void) const
{
	return (this->_dividers);
}

std::vector<Polynomial::t_term>	Polynomial::getTerms(void) const
{
	return (this->_terms);
}

std::string						Polynomial::getName(void) const
{
	return (this->_name);
}

IType::t_type					Polynomial::getType(void) const
{
	return (IType::t_type::E_TYPE_POLYNOMIAL);
}


// Setters

void	Polynomial::setName(const std::string& name)
{
	this->_name = name;
}

void	Polynomial::setTerms(const std::vector<t_term>& terms)
{
	free_vector_terms(this->_terms);
	this->_terms = terms;
}


// Methods

std::string		Polynomial::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

bool			Polynomial::is_constant(void) const
{
	if (this->_terms.size() == 1 && this->_dividers.size() == 1 	\
			&& *this->_dividers[0].coefficient == 1)
		return (true);
	return (false);
}

bool			Polynomial::in_C(void) const
{
	for (const auto& term : this->_terms)
		if (!term.coefficient->in_C())
			return (false);
	for (const auto& divider : this->_dividers)
		if (!divider.coefficient->in_C())
			return (false);
	return (true);
}

bool			Polynomial::in_D(void) const
{
	for (const auto& term : this->_terms)
		if (!term.coefficient->in_D())
			return (false);
	for (const auto& divider : this->_dividers)
		if (!divider.coefficient->in_D())
			return (false);
	return (true);
}

bool			Polynomial::in_M(void) const
{
	for (const auto& term : this->_terms)
		if (!term.coefficient->in_M())
			return (false);
	for (const auto& divider : this->_dividers)
		if (!divider.coefficient->in_M())
			return (false);
	return (true);
}

bool			Polynomial::in_Q(void) const
{
	for (const auto& term : this->_terms)
		if (!term.coefficient->in_Q())
			return (false);
	for (const auto& divider : this->_dividers)
		if (!divider.coefficient->in_Q())
			return (false);
	return (true);
}

bool			Polynomial::in_Z(void) const
{
	for (const auto& term : this->_terms)
		if (!term.coefficient->in_Z())
			return (false);
	for (const auto& divider : this->_dividers)
		if (!divider.coefficient->in_Z())
			return (false);
	return (true);
}

std::ostream&	Polynomial::print(std::ostream &os) const// [ ] Enlever les parentheses dans le resultat de 4 / var
{
	if (this->_dividers.size() == 1 					\
			&& *this->_dividers[0].coefficient == 1 	\
			&& !this->_dividers[0].power)
		print_terms(os, this->_terms, this->_name, true);
	else
	{
		print_terms(os, this->_terms, this->_name);
		os << " / ";
		print_terms(os, this->_dividers, this->_name);
	}
	return (os);
}

IType*			Polynomial::abs(void) const
{
	if (!this->is_constant())
		throw UnsupportedError("Absolute function is not defined for "
				"polynomials, use the norm function instead");
	return (this->getTerms()[0].coefficient->abs());
}

IType*			Polynomial::clone(void) const
{
	return (new Polynomial(*this));
}

IType*			Polynomial::cos(void) const
{
	if (!this->is_constant())
		throw UnsupportedError("Cosine of a polynomial is undefined");
	return (this->getTerms()[0].coefficient->cos());
}

IType*			Polynomial::exp(void) const
{
	if (!this->is_constant())
		throw UnsupportedError("Exponential of a polynomial is undefined");
	return (this->getTerms()[0].coefficient->exp());
}

IType*			Polynomial::function_operator(const IType &other) const
{
	IType*	numerator;
	IType*	denominator;
	IType*	result;

	numerator = function_operator_term(this->_terms, other);
	denominator = function_operator_term(this->_dividers, other);
	result = *numerator / *denominator;
	delete numerator;
	delete denominator;
	return (result);
}

IType*			Polynomial::matrix_operator(const IType &other) const
{
	if (!this->is_constant())
		throw ERROR_MATRIX_OPERATOR;
	return (this->getTerms()[0].coefficient->matrix_operator(other));
}

IType*			Polynomial::matrix_inversion(void) const
{
	if (!this->is_constant())
		throw ERROR_MATRIX_INVERSION_SQUARE;
	return (this->getTerms()[0].coefficient->matrix_inversion());
}

IType*			Polynomial::norm(void) const
{
	IType*	denominator_sum;
	IType*	numerator_sum;
	IType*	result;
	IType*	sum;

	numerator_sum = sum_square_coefficient_terms(this->_terms);
	denominator_sum = sum_square_coefficient_terms(this->_dividers);
	sum = *numerator_sum / *denominator_sum;
	delete numerator_sum;
	delete denominator_sum;
	result = sum->sqrt();
	delete sum;
	return (result);
}

IType*			Polynomial::sin(void) const
{
	if (!this->is_constant())
		throw UnsupportedError("Sine of a polynomial is undefined");
	return (this->getTerms()[0].coefficient->sin());
}

IType*			Polynomial::sqrt(void) const
{
	IType*	numerator_sqrt;
	IType*	denominator_sqrt;
	IType*	result;

	numerator_sqrt = terms_sqrt(*this, this->_terms);
	denominator_sqrt = terms_sqrt(*this, this->_dividers);
	result = *numerator_sqrt / *denominator_sqrt;
	delete numerator_sqrt;
	delete denominator_sqrt;
	return (result);
}

IType*			Polynomial::tan(void) const
{
	if (!this->is_constant())
		throw UnsupportedError("Tangent of a polynomial is undefined");
	return (this->getTerms()[0].coefficient->tan());
}

Rational*		Polynomial::fact(void) const
{
	if (!this->is_constant())
		throw ERROR_FACTORIAL_FUNCTION;
	return (this->getTerms()[0].coefficient->fact());
}

Rational*		Polynomial::gcd(const Polynomial &other) const
{
	Rational*	gcd;
	Rational*	result;

	if (!this->in_Q() || !other.in_Q() || this->getTerms().empty() 	\
			|| other.getTerms().empty())
		return (new Rational(1));
	gcd = vector_gcd(this->_terms);
	result = gcd->gcd(*vector_gcd(other._terms));
	delete gcd;
	return (result);
}

Rational*		Polynomial::gcd(const Rational &other) const
{
	return (Polynomial(other).gcd(*this));
}

Rational*		Polynomial::gcd(const Complex &other) const
{
	return (Polynomial(other).gcd(*this));
}

Rational*		Polynomial::gcd(const Matrix &other) const
{
	return (Polynomial(other).gcd(*this));
}

Rational*		Polynomial::gcd(const Vector &other) const
{
	return (Polynomial(other).gcd(*this));
}

Rational*		Polynomial::gcd(const Real &other) const
{
	return (new Rational(1));
	(void)other;
}

Rational*		Polynomial::gcd(const IType &other) const
{
	const Polynomial*	other_polynomial;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Matrix*		other_matrix;
	const Vector*		other_vector;
	const Real*			other_real;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (this->gcd(*other_polynomial));
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (this->gcd(*other_rational));
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (this->gcd(*other_complex));
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (this->gcd(*other_matrix));
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (this->gcd(*other_vector));
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (this->gcd(*other_real));
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Real*			Polynomial::deg(void) const
{
	if (!this->is_constant())
		throw ERROR_DEGREE_FUNCTION;
	return (this->getTerms()[0].coefficient->deg());
}

Real*			Polynomial::rad(void) const
{
	if (!this->is_constant())
		throw ERROR_RADIAN_FUNCTION;
	return (this->getTerms()[0].coefficient->rad());
}

void			Polynomial::factorize_constant_factor(void)
{
	Rational*	gcd_dividend;
	Rational*	gcd_divisor;
	Rational*	gcd;

	if (!this->in_Q())
		return ;
	gcd_dividend = vector_gcd(this->_terms);
	gcd_divisor = vector_gcd(this->_dividers);
	gcd = gcd_dividend->gcd(*gcd_divisor);
	delete gcd_dividend;
	delete gcd_divisor;
	divide_constant_factor(*gcd, this->_terms, this->_dividers);
	delete gcd;
}

void			Polynomial::free(void)
{
	free_vector_terms(this->_terms);
	free_vector_terms(this->_dividers);
}

void			Polynomial::print_rounded(const std::string var) const
{
	(void)var;
}

void			Polynomial::reduce(void)
{
	Polynomial::t_division_result	division_result;

	clean_terms(this->_terms);
	clean_terms(this->_dividers);
	this->sort_powers();
	if (this->_dividers.empty())
	{
		this->free();
		throw ERROR_DIVISION_BY_ZERO;
	}
	division_result = euclidean_division(this->_terms, this->_dividers, true);
	if (division_result.quotient.empty())
		second_division(this->_terms, this->_dividers, 				\
				division_result.remainder);
	free_vector_terms(division_result.quotient);
	free_vector_terms(division_result.remainder);
	if (this->in_Q() || this->in_C() || this->in_M())
		this->factorize_constant_factor();
	if (this->_terms.size() == 1 && this->_dividers.size() == 1 	\
			&& *this->_dividers[0].coefficient != 1)
		this->reduce_constant();
}

void			Polynomial::reduce_constant(void)
{
	IType*	new_type;
	
	new_type = *this->_terms[0].coefficient 	\
			/ *this->_dividers[0].coefficient;
	this->free();
	this->_terms.push_back((t_term){new_type, 0});
	this->_dividers.push_back((t_term){new Rational(1), 0});
}

void			Polynomial::sort_powers(void)
{
	terms_sort_powers(this->_terms);
	terms_sort_powers(this->_dividers);
}


// Output stream operator overload

std::ostream	&operator<<(std::ostream &os, const Polynomial &var)
{
	return (var.print(os));
}


// Functions

void							add_terms_to_vector(				\
		const std::vector<Polynomial::t_term> &terms, 				\
		std::vector<Polynomial::t_term> &vector)
{
	std::vector<Polynomial::t_term>::const_iterator	it(terms.begin());

	while (it != terms.end())
	{
		add_term_to_vector(it->coefficient, it->power, vector);
		it++;
	}
}

std::vector<Polynomial::t_term>	vector_term_coeff_multiplication(	\
		const IType *coefficient, const unsigned long int power, 	\
		const std::vector<Polynomial::t_term> &vector)
{
	std::vector<Polynomial::t_term>					result;
	std::vector<Polynomial::t_term>::const_iterator	it(vector.begin());
	IType*											new_coefficient;

	while (it != vector.end())
	{
		new_coefficient = *coefficient * *it->coefficient;
		add_term_to_vector(new_coefficient, power + it->power, result);
		delete new_coefficient;
		it++;
	}
	return (result);
}

void							free_vector_terms(					\
		std::vector<Polynomial::t_term>& vector)
{
	std::vector<Polynomial::t_term>::iterator	it(vector.begin());

	while (it != vector.end())
	{
		delete it->coefficient;
		it++;
	}
	vector.clear();
}
