/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Polynomial.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:19:47 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/11 14:03:46 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Polynomial.hpp"
#include "Matrix.hpp"

// Utils
static std::vector<Polynomial::t_term>	vector_term_multiplication(const std::vector<Polynomial::t_term> &terms1, const std::vector<Polynomial::t_term> &terms2)
{
	std::vector<Polynomial::t_term>					result;
	std::vector<Polynomial::t_term>::const_iterator	it1(terms1.begin());
	std::vector<Polynomial::t_term>::const_iterator	it2;

	while (it1 != terms1.end())
	{
		it2 = terms2.begin();
		while (it2 != terms2.end())
		{
			unsigned short int	new_power = it1->power + it2->power;
			result.push_back((Polynomial::t_term){*it1->coefficient * *it2->coefficient, new_power});// TODO: Verifier overflow
			it2++;
		}
		it1++;
	}
	return (result);
}

static void								add_term_to_vector(IType *coefficient, unsigned short int power, std::vector<Polynomial::t_term> &vector)
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

static void								add_terms_to_vector(const std::vector<Polynomial::t_term> &terms, std::vector<Polynomial::t_term> &vector)
{
	std::vector<Polynomial::t_term>::const_iterator	it(terms.begin());

	while (it != terms.end())
	{
		add_term_to_vector(it->coefficient, it->power, vector);
		it++;
	}
}

static std::vector<Polynomial::t_term>	vector_term_coeff_multiplication(const IType *coefficient, const unsigned short int power, const std::vector<Polynomial::t_term> &vector)
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

static void								free_vector_terms(std::vector<Polynomial::t_term> &vector)
{
	std::vector<Polynomial::t_term>::iterator	it(vector.begin());

	while (it != vector.end())
	{
		delete it->coefficient;
		it++;
	}
	vector.clear();
}

static std::vector<Polynomial::t_term>	multiply_vectors(const std::vector<Polynomial::t_term> &terms1, const std::vector<Polynomial::t_term> &terms2)
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
			add_term_to_vector(new_coefficient, it1->power + it2->power, result);
			delete new_coefficient;
			it2++;
		}
		it1++;
	}
	return (result);
}

static IType*							function_operator_term(const std::vector<Polynomial::t_term> &terms, const IType &other)
{
	IType*	result;
	IType*	power;
	IType*	term;
	IType*	tmp;

	result = new Rational();
	for (std::vector<Polynomial::t_term>::const_iterator it(terms.begin()); it != terms.end(); it++)
	{
		power = other ^ Rational(it->power);
		term = *it->coefficient * *power;
		delete power;
		tmp = result;
		result = *result + *term;
		delete tmp;
		delete term;
	}
	return (result);
}

static void								print_coefficient(std::ostream &os, IType *coefficient, unsigned short int power, bool first_term)
{
	IType*		tmp;
	Complex*	complex;
	bool		need_parenthesis;

	if (!first_term)
	{
		if (*coefficient < Rational(0))
		{
			os << " - ";
			tmp = coefficient;
			coefficient = *coefficient * Rational(-1);
			delete tmp;
		}
		else
			os << " + ";
	}
	complex = dynamic_cast<Complex*>(coefficient);
	need_parenthesis = complex && complex->getReal() && complex->getImaginary();
	if (need_parenthesis)
		os << "(";
	if (power && *coefficient == Rational(-1))
		os << "-";
	else if (!power || *coefficient != Rational(1))
		os << *coefficient;
	if (need_parenthesis)
		os << ")";
	else if (complex && complex->getImaginary())
		os << " * ";
	delete coefficient;
}

static std::ostream&					print_terms(std::ostream &os, const std::vector<Polynomial::t_term> &terms, const std::string &name, bool alone = false)
{
	bool	first_term;

	if (!alone && terms.size() > 1)
		os << "(";
	first_term = true;
	for (std::vector<Polynomial::t_term>::const_reverse_iterator it(terms.rbegin()); it != terms.rend(); it++)
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

static void								add_blank_terms(std::vector<Polynomial::t_term> &terms, unsigned short int max_power)
{
	IType*	tmp;

	for (unsigned short int i = 0; i <= max_power; i++)
	{
		tmp = new Rational(0);
		add_term_to_vector(tmp, i, terms);
		delete tmp;
	}
}

static void								clean_terms(std::vector<Polynomial::t_term> &terms)
{
	std::vector<Polynomial::t_term>::const_iterator	it(terms.begin());
	unsigned short int								max_power(0);
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

static void								terms_sort_powers(std::vector<Polynomial::t_term> &terms)
{
	bool (*fct)(const Polynomial::t_term&, const Polynomial::t_term&);

	fct = [](const Polynomial::t_term &a, const Polynomial::t_term &b)
	{
		return (a.power < b.power);
	};
	std::sort(terms.begin(), terms.end(), fct);
}

static Polynomial::t_term				get_term_result(const Polynomial::t_term &dividend, const Polynomial::t_term &divisor)
{
	unsigned short int	new_power;

	if (divisor.power > dividend.power)
		throw ERROR_UNEXPECTED;
	new_power = dividend.power - divisor.power;
	return ((Polynomial::t_term){
			.coefficient = *dividend.coefficient / *divisor.coefficient, 	\
			.power = new_power});
}

static std::vector<Polynomial::t_term>	euclidean_division_step(const std::vector<Polynomial::t_term> &dividend, const std::vector<Polynomial::t_term> &divisor, std::vector<Polynomial::t_term> &quotient)
{
	Polynomial::t_term				term_result;
	IType*							sub;
	std::vector<Polynomial::t_term>	result;
	std::vector<Polynomial::t_term>	new_result;
	std::vector<Polynomial::t_term>	distributivity;

	if (dividend.size() < divisor.size())
	{
		add_terms_to_vector(dividend, result);
		return (result);
	}
	term_result = get_term_result(dividend[dividend.size() - 1], divisor[divisor.size() - 1]);
	quotient.push_back(term_result);
	add_terms_to_vector(dividend, result);
	sub = *term_result.coefficient * Rational(-1);
	distributivity = vector_term_coeff_multiplication(sub, term_result.power, divisor);
	add_terms_to_vector(distributivity, result);
	free_vector_terms(distributivity);
	delete sub;
	clean_terms(result);
	terms_sort_powers(result);
	new_result = euclidean_division_step(result, divisor, quotient);
	free_vector_terms(result);
	return (new_result);
}

static Polynomial::t_division_result	euclidean_division(const std::vector<Polynomial::t_term> &dividend, const std::vector<Polynomial::t_term> &divisor, bool is_recursive = false)
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

static void								divide_constant_factor(const Rational &factor, std::vector<Polynomial::t_term> &terms, std::vector<Polynomial::t_term> &dividers)
{
	IType	*tmp;

	for (std::vector<Polynomial::t_term>::iterator it(terms.begin()); it != terms.end(); it++)
	{
		tmp = it->coefficient;
		it->coefficient = *it->coefficient / factor;
		delete tmp;
	}
	for (std::vector<Polynomial::t_term>::iterator it(dividers.begin()); it != dividers.end(); it++)
	{
		tmp = it->coefficient;
		it->coefficient = *it->coefficient / factor;
		delete tmp;
	}
}

static bool								is_it_different_variables(const Polynomial& p1, const Polynomial& p2)
{
	if (p1.getTerms().size() <= 1 || p2.getTerms().size() <= 1)
		return (false);
	return (p1.getName() != p2.getName());
}


// Constructors and destructor
Polynomial::Polynomial(std::string name, t_term term)
{
	this->_name = name;
	this->_terms.push_back(term);
	this->_dividers.push_back((t_term){new Rational(1), 0});
}

Polynomial::Polynomial(const Polynomial &other)
{
	std::vector<t_term>::const_iterator	it_terms(other._terms.begin());
	std::vector<t_term>::const_iterator	it_dividers(other._dividers.begin());

	this->_name = other._name;
	while (it_terms != other._terms.end())
	{
		this->_terms.push_back((t_term){it_terms->coefficient->clone(), it_terms->power});
		it_terms++;
	}
	while (it_dividers != other._dividers.end())
	{
		this->_dividers.push_back((t_term){it_dividers->coefficient->clone(), it_dividers->power});
		it_dividers++;
	}
}

Polynomial::Polynomial(const IType &other)
{
	const Rational		*other_rational;
	const Complex		*other_complex;
	const Matrix		*other_matrix;
	const Polynomial	*other_polynomial;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	other_rational = dynamic_cast<const Rational*>(&other);
	other_complex = dynamic_cast<const Complex*>(&other);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_polynomial)
		*this = *other_polynomial;
	else if (other_rational)
		*this = Polynomial("", (t_term){other_rational->clone(), 0});
	else if (other_complex)
		*this = Polynomial("", (t_term){other_complex->clone(), 0});
	else if (other_matrix)
		*this = Polynomial("", (t_term){other_matrix->clone(), 0});
	else
		throw ERROR_UNEXPECTED;
	this->reduce();
}

Polynomial::~Polynomial(void)
{
	std::vector<t_term>::const_iterator	it_terms(this->_terms.begin());
	std::vector<t_term>::const_iterator	it_dividers(this->_dividers.begin());

	while (it_terms != this->_terms.end())
	{
		delete it_terms->coefficient;
		it_terms++;
	}
	this->_terms.clear();
	while (it_dividers != this->_dividers.end())
	{
		delete it_dividers->coefficient;
		it_dividers++;
	}
	this->_dividers.clear();
}


// Operator overloads
Polynomial&	Polynomial::operator=(const Polynomial &other)
{
	std::vector<t_term>::const_iterator	it_terms(other._terms.begin());
	std::vector<t_term>::const_iterator	it_dividers(other._dividers.begin());

	if (this != &other)
	{
		this->~Polynomial();
		this->_name = other._name;
		while (it_terms != other._terms.end())
		{
			this->_terms.push_back((t_term){it_terms->coefficient->clone(), it_terms->power});
			it_terms++;
		}
		while (it_dividers != other._dividers.end())
		{
			this->_dividers.push_back((t_term){it_dividers->coefficient->clone(), it_dividers->power});
			it_dividers++;
		}
	}
	return (*this);
}

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

bool		Polynomial::operator==(const Polynomial &other) const
{
	std::vector<t_term>::const_iterator	it_terms(this->_terms.begin());
	std::vector<t_term>::const_iterator	it_terms_other(other._terms.begin());
	std::vector<t_term>::const_iterator	it_dividers(this->_dividers.begin());
	std::vector<t_term>::const_iterator	it_dividers_other(other._dividers.begin());

	if (this->_name != other._name || this->_terms.size() != other._terms.size())
		return (false);
	while (it_terms != this->_terms.end())
	{
		if (*it_terms->coefficient != *it_terms_other->coefficient || it_terms->power != it_terms_other->power)
			return (false);
		it_terms++;
		it_terms_other++;
	}
	while (it_dividers != this->_dividers.end())
	{
		if (*it_dividers->coefficient != *it_dividers_other->coefficient || it_dividers->power != it_dividers_other->power)
			return (false);
		it_dividers++;
		it_dividers_other++;
	}
	return (true);
}

bool		Polynomial::operator==(const Rational &other) const
{
	if (this->_terms.size() != 1 || this->_dividers.size() != 1 || *this->_dividers[0].coefficient != Rational(1) || this->_dividers[0].power)
		return (false);
	return (*this->_terms[0].coefficient == other && !this->_terms[0].power);
}

bool		Polynomial::operator==(const Complex &other) const
{
	if (this->_terms.size() != 1 || this->_dividers.size() != 1 || *this->_dividers[0].coefficient != Rational(1) || this->_dividers[0].power)
		return (false);
	return (*this->_terms[0].coefficient == other && !this->_terms[0].power);
}

bool		Polynomial::operator==(const Matrix &other) const
{
	if (this->_terms.size() != 1 || this->_dividers.size() != 1 || *this->_dividers[0].coefficient != Rational(1) || this->_dividers[0].power)
		return (false);
	return (*this->_terms[0].coefficient == other && !this->_terms[0].power);
}

bool		Polynomial::operator==(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational		*other_rational;
	const Complex		*other_complex;
	const Matrix		*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this == *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this == *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this == *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this == *other_matrix);
	throw ERROR_UNEXPECTED;
	return (false);
}

bool		Polynomial::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Polynomial::operator<(const IType &other) const
{
	return (false);
	(void)other;
}

bool		Polynomial::operator<=(const IType &other) const
{
	return (false);
	(void)other;
}

bool		Polynomial::operator>(const IType &other) const
{
	return (false);
	(void)other;
}

bool		Polynomial::operator>=(const IType &other) const
{
	return (false);
	(void)other;
}

Polynomial*	Polynomial::operator+(const Polynomial &other) const
{
	std::vector<t_term>	distributivity;
	Polynomial*			result;

	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	result = new Polynomial(this->_name);
	free_vector_terms(result->_dividers);
	result->_dividers = vector_term_multiplication(this->_dividers, other._dividers);
	for (std::vector<t_term>::const_iterator it(this->_terms.begin()); it != this->_terms.end(); it++)
	{
		distributivity = vector_term_coeff_multiplication(it->coefficient, it->power, other._dividers);
		add_terms_to_vector(distributivity, result->_terms);
		free_vector_terms(distributivity);
	}
	for (std::vector<t_term>::const_iterator it(other._terms.begin()); it != other._terms.end(); it++)
	{
		distributivity = vector_term_coeff_multiplication(it->coefficient, it->power, this->_dividers);
		add_terms_to_vector(distributivity, result->_terms);
		free_vector_terms(distributivity);
	}
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator+(const Rational &other) const
{
	Polynomial*			result;
	std::vector<t_term>	distributivity;

	result = new Polynomial(*this);
	distributivity = vector_term_coeff_multiplication(dynamic_cast<const IType*>(&other), 0, this->_dividers);
	add_terms_to_vector(distributivity, result->_terms);
	free_vector_terms(distributivity);
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator+(const Complex &other) const
{
	Polynomial*			result;
	std::vector<t_term>	distributivity;

	result = new Polynomial(*this);
	distributivity = vector_term_coeff_multiplication(dynamic_cast<const IType*>(&other), 0, this->_dividers);
	add_terms_to_vector(distributivity, result->_terms);
	free_vector_terms(distributivity);
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator+(const Matrix &other) const
{
	Polynomial*			result;
	std::vector<t_term>	distributivity;

	result = new Polynomial(*this);
	distributivity = vector_term_coeff_multiplication(dynamic_cast<const IType*>(&other), 0, this->_dividers);
	add_terms_to_vector(distributivity, result->_terms);
	free_vector_terms(distributivity);
	result->reduce();
	return (result);
}

IType*		Polynomial::operator+(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational		*other_rational;
	const Complex		*other_complex;
	const Matrix		*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this + *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator-(const Polynomial &other) const
{
	Polynomial*	sub;
	Polynomial*	result;

	sub = other * Rational(-1);
	result = *this + *sub;
	delete sub;
	return (result);
}

Polynomial*	Polynomial::operator-(const Rational &other) const
{
	Polynomial*			result;
	IType*				sub;
	std::vector<t_term>	distributivity;

	result = new Polynomial(*this);
	sub = other * Rational(-1);
	distributivity = vector_term_coeff_multiplication(sub, 0, this->_dividers);
	delete sub;
	add_terms_to_vector(distributivity, result->_terms);
	free_vector_terms(distributivity);
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator-(const Complex &other) const
{
	Polynomial*			result;
	IType*				sub;
	std::vector<t_term>	distributivity;

	result = new Polynomial(*this);
	sub = other * Rational(-1);
	distributivity = vector_term_coeff_multiplication(sub, 0, this->_dividers);
	delete sub;
	add_terms_to_vector(distributivity, result->_terms);
	free_vector_terms(distributivity);
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator-(const Matrix &other) const
{
	Polynomial*			result;
	IType*				sub;
	std::vector<t_term>	distributivity;

	result = new Polynomial(*this);
	sub = other * Rational(-1);
	distributivity = vector_term_coeff_multiplication(sub, 0, this->_dividers);
	delete sub;
	add_terms_to_vector(distributivity, result->_terms);
	free_vector_terms(distributivity);
	result->reduce();
	return (result);
}

IType*		Polynomial::operator-(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational		*other_rational;
	const Complex		*other_complex;
	const Matrix		*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this - *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator*(const Polynomial &other) const
{
	Polynomial*	result;

	if (is_it_different_variables(*this, other))
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	result = new Polynomial(this->_name);
	free_vector_terms(result->_terms);
	free_vector_terms(result->_dividers);
	result->_terms = multiply_vectors(this->_terms, other._terms);
	result->_dividers = multiply_vectors(this->_dividers, other._dividers);
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator*(const Rational &other) const
{
	Polynomial*	result;
	IType*		new_coefficient;

	result = new Polynomial(*this);
	free_vector_terms(result->_terms);
	for (std::vector<t_term>::const_iterator it(this->_terms.begin()); it != this->_terms.end(); it++)
	{
		new_coefficient = *it->coefficient * other;
		add_term_to_vector(new_coefficient, it->power, result->_terms);
		delete new_coefficient;
	}
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator*(const Complex &other) const
{
	Polynomial*	result;
	IType*		new_coefficient;

	result = new Polynomial(*this);
	free_vector_terms(result->_terms);
	for (std::vector<t_term>::const_iterator it(this->_terms.begin()); it != this->_terms.end(); it++)
	{
		new_coefficient = *it->coefficient * other;
		add_term_to_vector(new_coefficient, it->power, result->_terms);
		delete new_coefficient;
	}
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator*(const Matrix &other) const
{
	Polynomial*	result;
	IType*		new_coefficient;

	result = new Polynomial(*this);
	free_vector_terms(result->_terms);
	for (std::vector<t_term>::const_iterator it(this->_terms.begin()); it != this->_terms.end(); it++)
	{
		new_coefficient = *it->coefficient * other;
		add_term_to_vector(new_coefficient, it->power, result->_terms);
		delete new_coefficient;
	}
	result->reduce();
	return (result);
}

IType*		Polynomial::operator*(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational		*other_rational;
	const Complex		*other_complex;
	const Matrix		*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this * *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator/(const Polynomial &other) const
{
	Polynomial*	result;

	if (is_it_different_variables(*this, other))
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	result = new Polynomial(this->_name);
	free_vector_terms(result->_terms);
	free_vector_terms(result->_dividers);
	result->_terms = multiply_vectors(this->_terms, other._dividers);
	result->_dividers = multiply_vectors(this->_dividers, other._terms);
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator/(const Rational &other) const
{
	Polynomial*	result;
	IType*		new_coefficient;

	result = new Polynomial(*this);
	free_vector_terms(result->_dividers);
	for (std::vector<t_term>::const_iterator it(this->_dividers.begin()); it != this->_dividers.end(); it++)
	{
		new_coefficient = *it->coefficient * other;
		add_term_to_vector(new_coefficient, it->power, result->_dividers);
		delete new_coefficient;
	}
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator/(const Complex &other) const
{
	Polynomial*	result;
	IType*		new_coefficient;

	result = new Polynomial(*this);
	free_vector_terms(result->_dividers);
	for (std::vector<t_term>::const_iterator it(this->_dividers.begin()); it != this->_dividers.end(); it++)
	{
		new_coefficient = *it->coefficient * other;
		add_term_to_vector(new_coefficient, it->power, result->_dividers);
		delete new_coefficient;
	}
	result->reduce();
	return (result);
}

Polynomial*	Polynomial::operator/(const Matrix &other) const
{
	Polynomial*	result;
	IType*		new_coefficient;

	result = new Polynomial(*this);
	free_vector_terms(result->_dividers);
	for (std::vector<t_term>::const_iterator it(this->_dividers.begin()); it != this->_dividers.end(); it++)
	{
		new_coefficient = *it->coefficient * other;
		add_term_to_vector(new_coefficient, it->power, result->_dividers);
		delete new_coefficient;
	}
	result->reduce();
	return (result);
}

IType*		Polynomial::operator/(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational		*other_rational;
	const Complex		*other_complex;
	const Matrix		*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this / *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator%(const Polynomial &other) const
{
	Polynomial*			division;
	Polynomial*			result;
	t_division_result	division_result;

	division = *this / other;
	division_result = euclidean_division(division->_terms, other._terms);
	result = new Polynomial(division->_name);
	result->_terms.insert(result->_terms.end(), 	\
			division_result.remainder.begin(), division_result.remainder.end());
	delete division;
	return (result);
}

Polynomial*	Polynomial::operator%(const Rational &other) const
{
	return (*this 							\
			% Polynomial(this->getName(), 	\
				(Polynomial::t_term){other.clone(), 0}));
}

Polynomial*	Polynomial::operator%(const Complex &other) const
{
	return (*this 							\
			% Polynomial(this->getName(), 	\
				(Polynomial::t_term){other.clone(), 0}));
}

Polynomial*	Polynomial::operator%(const Matrix &other) const
{
	return (*this 							\
			% Polynomial(this->getName(), 	\
				(Polynomial::t_term){other.clone(), 0}));
}

IType*		Polynomial::operator%(const IType &other) const
{
	const Polynomial	*other_polynomial;
	const Rational		*other_rational;
	const Complex		*other_complex;
	const Matrix		*other_matrix;

	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this % *other_polynomial);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Polynomial*	Polynomial::operator^(const Rational &other) const
{
	Polynomial	*result;
	Polynomial	*tmp;
	int			power;

	power = other.getNumerator();
	if (!power)
		return (new Polynomial(this->getName(), (Polynomial::t_term){new Rational(1), 0}));
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
	if (!power.is_integer() || power < Rational(0))
		throw UNSUPPORTED_EXPONENT;
	return (*this ^ power);
}


// Getters
std::string						Polynomial::getName(void) const
{
	return (this->_name);
}

std::vector<Polynomial::t_term>	Polynomial::getTerms(void) const
{
	return (this->_terms);
}

std::vector<Polynomial::t_term>	Polynomial::getDividers(void) const
{
	return (this->_dividers);
}


// Setters
void	Polynomial::setName(const std::string& name)
{
	this->_name = name;
}


// Methods
IType*			Polynomial::matrix_operator(const IType &other) const
{
	try
	{
		Matrix	matrix(*this);
		Matrix	other_matrix(other);

		return (matrix.matrix_operator(other_matrix));
	}
	catch (const LogicError &e)
	{
		throw ERROR_MATRIX_OPERATOR;
	}
	return (nullptr);
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

Rational*		Polynomial::gcd(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

IType*			Polynomial::clone(void) const
{
	return (new Polynomial(*this));
}

std::ostream&	Polynomial::print(std::ostream &os) const
{
	if (this->_dividers.size() == 1 && *this->_dividers[0].coefficient == Rational(1) && !this->_dividers[0].power)
		print_terms(os, this->_terms, this->_name, true);
	else
	{
		print_terms(os, this->_terms, this->_name);
		os << " / ";
		print_terms(os, this->_dividers, this->_name);
	}
	return (os);
}

void			Polynomial::print_variable(const std::string var) const
{
	(void)var;
}

void			Polynomial::sort_powers(void)
{
	terms_sort_powers(this->_terms);
	terms_sort_powers(this->_dividers);
}

void			Polynomial::factorize_constant_factor(void)
{
	Rational	*gcd_dividend;
	Rational	*gcd_divisor;
	Rational	*gcd;
	IType		*tmp;

	gcd_dividend = new Rational(0);
	gcd_divisor = new Rational(0);
	for (std::vector<Polynomial::t_term>::const_iterator it(this->_terms.begin()); it != this->_terms.end(); it++)
	{
		tmp = gcd_dividend;
		gcd_dividend = gcd_dividend->gcd(*it->coefficient);
		delete tmp;
	}
	for (std::vector<Polynomial::t_term>::const_iterator it(this->_dividers.begin()); it != this->_dividers.end(); it++)
	{
		tmp = gcd_divisor;
		gcd_divisor = gcd_divisor->gcd(*it->coefficient);
		delete tmp;
	}
	gcd = gcd_dividend->gcd(*gcd_divisor);
	delete gcd_dividend;
	delete gcd_divisor;
	divide_constant_factor(*gcd, this->_terms, this->_dividers);
	delete gcd;
}

void			Polynomial::reduce(void)
{
	Polynomial::t_division_result	division_result;
	Polynomial::t_division_result	reduced;

	clean_terms(this->_terms);
	clean_terms(this->_dividers);
	this->sort_powers();
	if (this->_dividers.empty())
	{
		delete this;
		throw ERROR_DIVISION_BY_ZERO;
	}
	division_result = euclidean_division(this->_terms, this->_dividers, true);
	if (division_result.quotient.empty())
	{
		reduced = euclidean_division(this->_terms, division_result.remainder);
		free_vector_terms(this->_terms);
		this->_terms = reduced.quotient;
		free_vector_terms(reduced.remainder);
		reduced = euclidean_division(this->_dividers, division_result.remainder);
		free_vector_terms(this->_dividers);
		this->_dividers = reduced.quotient;
		free_vector_terms(reduced.remainder);
	}
	free_vector_terms(division_result.quotient);
	free_vector_terms(division_result.remainder);
	this->factorize_constant_factor();
}

bool			Polynomial::finite_decimals(void) const
{
	for (std::vector<t_term>::const_iterator it(this->_terms.begin()); 		\
			it != this->_terms.end(); it++)
		if (!it->coefficient->finite_decimals())
			return (false);
	for (std::vector<t_term>::const_iterator it(this->_dividers.begin()); 	\
			it != this->_dividers.end(); it++)
		if (!it->coefficient->finite_decimals())
			return (false);
	return (true);
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Polynomial &var)
{
	return (var.print(os));
}
