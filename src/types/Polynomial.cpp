/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Polynomial.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:19:47 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/13 17:03:20 by qpupier          ###   ########lyon.fr   */
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
			result.push_back((Polynomial::t_term){*it1->coefficient * *it2->coefficient, it1->power + it2->power});
			it2++;
		}
		it1++;
	}
	return (result);
}

static void								add_term_to_vector(IType *coefficient, unsigned int power, std::vector<Polynomial::t_term> &vector)
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
			delete coefficient;
			return ;
		}
		it++;
	}
	vector.push_back((Polynomial::t_term){coefficient, power});
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

static std::vector<Polynomial::t_term>	vector_term_coeff_multiplication(const IType *coefficient, const unsigned int power, const std::vector<Polynomial::t_term> &vector)
{
	std::vector<Polynomial::t_term>					result;
	std::vector<Polynomial::t_term>::const_iterator	it(vector.begin());

	while (it != vector.end())
	{
		add_term_to_vector(*coefficient * *it->coefficient, it->power + power, result);
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

	while (it1 != terms1.end())
	{
		it2 = terms2.begin();
		while (it2 != terms2.end())
		{
			add_term_to_vector(*it1->coefficient * *it2->coefficient, it1->power + it2->power, result);
			it2++;
		}
		it1++;
	}
	return (result);
}

static Polynomial*						polynomial_power_rational(const Polynomial *polynomial, int power)
{
	Polynomial	*result;
	Polynomial	*tmp;

	if (!power)
		return (new Polynomial(polynomial->getName(), (Polynomial::t_term){new Rational(1), 0}));
	result = new Polynomial(*polynomial);
	while (--power)
	{
		tmp = result;
		result = *result * *polynomial;
		delete tmp;
	}
	return (result);
}

static IType*							function_operator_term(const std::vector<Polynomial::t_term> &terms, const IType &other)
{
	std::vector<Polynomial::t_term>::const_iterator	it(terms.begin());
	IType*	result;
	IType*	power;
	IType*	term;

	result = new Rational(1);
	while (it != terms.end())
	{
		power = other ^ Rational(static_cast<int>(it->power));
		term = *it->coefficient * *power;
		delete power;
		result = *result + *term;
		delete term;
		it++;
	}
	return (result);
}

static std::ostream&					print_terms(std::ostream &os, const std::vector<Polynomial::t_term> &terms, const std::string &name)
{
	std::vector<Polynomial::t_term>::const_iterator	it(terms.begin());
	bool											first_term;

	//TODO: A ameliorer
	os << "(";
	first_term = true;
	while (it != terms.end())
	{
		if (!it->coefficient)
		{
			it++;
			continue ;
		}
		if (!first_term)
			os << " + ";
		os << "(" << *it->coefficient << ")";
		if (it->power)
		{
			os << name;
			if (it->power > 1)
				os << "^" << it->power;
		}
		first_term = false;
		it++;
	}
	if (first_term)
		os << "0";
	os << ")";
	return (os);
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

	// TODO: Simplify polynomials before comparing
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
	//TODO: A voir s'il faut l'implementer autrement
	return (false);
	(void)other;
}

bool		Polynomial::operator<=(const IType &other) const
{
	//TODO: A voir s'il faut l'implementer autrement
	return (false);
	(void)other;
}

bool		Polynomial::operator>(const IType &other) const
{
	//TODO: A voir s'il faut l'implementer autrement
	return (false);
	(void)other;
}

bool		Polynomial::operator>=(const IType &other) const
{
	//TODO: A voir s'il faut l'implementer autrement
	return (false);
	(void)other;
}

Polynomial*	Polynomial::operator+(const Polynomial &other) const
{
	std::vector<t_term>::const_iterator	it1(this->_terms.begin());
	std::vector<t_term>::const_iterator	it2(other._terms.begin());
	Polynomial*							result;

	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	result = new Polynomial(this->_name);
	free_vector_terms(result->_dividers);
	result->_dividers = vector_term_multiplication(this->_dividers, other._dividers);
	while (it1 != this->_terms.end())
	{
		add_terms_to_vector(vector_term_coeff_multiplication(it1->coefficient, it1->power, other._dividers), result->_terms);
		it1++;
	}
	while (it2 != other._terms.end())
	{
		add_terms_to_vector(vector_term_coeff_multiplication(it2->coefficient, it2->power, this->_dividers), result->_terms);
		it2++;
	}
	return (result);
}

Polynomial*	Polynomial::operator+(const Rational &other) const
{
	Polynomial*	result;

	result = new Polynomial(*this);
	add_terms_to_vector(vector_term_coeff_multiplication(other.clone(), 0, this->_dividers), result->_terms);
	return (result);
}

Polynomial*	Polynomial::operator+(const Complex &other) const
{
	Polynomial*	result;

	result = new Polynomial(*this);
	add_terms_to_vector(vector_term_coeff_multiplication(other.clone(), 0, this->_dividers), result->_terms);
	return (result);
}

Polynomial*	Polynomial::operator+(const Matrix &other) const
{
	Polynomial*	result;

	result = new Polynomial(*this);
	add_terms_to_vector(vector_term_coeff_multiplication(other.clone(), 0, this->_dividers), result->_terms);
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
	std::vector<t_term>::const_iterator	it1(this->_terms.begin());
	std::vector<t_term>::const_iterator	it2(other._terms.begin());
	std::vector<t_term>					tmp;
	Polynomial*							result;

	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;
	result = new Polynomial(this->_name);
	free_vector_terms(result->_dividers);
	result->_dividers = vector_term_multiplication(this->_dividers, other._dividers);
	while (it1 != this->_terms.end())
	{
		add_terms_to_vector(vector_term_coeff_multiplication(it1->coefficient, it1->power, other._dividers), result->_terms);
		it1++;
	}
	while (it2 != other._terms.end())
	{
		tmp = vector_term_coeff_multiplication(it2->coefficient, it2->power, this->_dividers);
		add_terms_to_vector(vector_term_coeff_multiplication(new Rational(-1), 0, this->_dividers), result->_terms);
		free_vector_terms(tmp);
		it2++;
	}
	return (result);
}

Polynomial*	Polynomial::operator-(const Rational &other) const
{
	Polynomial*	result;

	result = new Polynomial(*this);
	add_terms_to_vector(vector_term_coeff_multiplication(other * Rational(-1), 0, this->_dividers), result->_terms);
	return (result);
}

Polynomial*	Polynomial::operator-(const Complex &other) const
{
	Polynomial*	result;

	result = new Polynomial(*this);
	add_terms_to_vector(vector_term_coeff_multiplication(other * Rational(-1), 0, this->_dividers), result->_terms);
	return (result);
}

Polynomial*	Polynomial::operator-(const Matrix &other) const
{
	Polynomial*	result;

	result = new Polynomial(*this);
	add_terms_to_vector(vector_term_coeff_multiplication(other * Rational(-1), 0, this->_dividers), result->_terms);
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

	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;// TODO: Ce n'est pas forcement le cas si il n'y a qu'une power 0 ou des coefficients nuls
	result = new Polynomial(this->_name);
	free_vector_terms(result->_dividers);
	result->_terms = multiply_vectors(this->_terms, other._terms);
	result->_dividers = multiply_vectors(this->_dividers, other._dividers);
	return (result);
}

Polynomial*	Polynomial::operator*(const Rational &other) const
{
	Polynomial*							result;
	std::vector<t_term>::const_iterator	it(this->_terms.begin());

	result = new Polynomial(*this);
	free_vector_terms(result->_terms);
	while (it != this->_terms.end())
	{
		add_term_to_vector(*it->coefficient * other, it->power, result->_terms);
		it++;
	}
	return (result);
}

Polynomial*	Polynomial::operator*(const Complex &other) const
{
	Polynomial*							result;
	std::vector<t_term>::const_iterator	it(this->_terms.begin());

	result = new Polynomial(*this);
	free_vector_terms(result->_terms);
	while (it != this->_terms.end())
	{
		add_term_to_vector(*it->coefficient * other, it->power, result->_terms);
		it++;
	}
	return (result);
}

Polynomial*	Polynomial::operator*(const Matrix &other) const
{
	Polynomial*							result;
	std::vector<t_term>::const_iterator	it(this->_terms.begin());

	result = new Polynomial(*this);
	free_vector_terms(result->_terms);
	while (it != this->_terms.end())
	{
		add_term_to_vector(*it->coefficient * other, it->power, result->_terms);
		it++;
	}
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

	if (this->_name != other._name)
		throw UNSUPPORTED_MULTI_POLYNOMIALS;// TODO: Ce n'est pas forcement le cas si il n'y a qu'une power 0 ou des coefficients nuls
	result = new Polynomial(this->_name);
	free_vector_terms(result->_dividers);
	result->_terms = multiply_vectors(this->_terms, other._dividers);
	result->_dividers = multiply_vectors(this->_dividers, other._terms);
	return (result);
}

Polynomial*	Polynomial::operator/(const Rational &other) const
{
	Polynomial*							result;
	std::vector<t_term>::const_iterator	it(this->_dividers.begin());

	result = new Polynomial(*this);
	free_vector_terms(result->_dividers);
	while (it != this->_dividers.end())
	{
		add_term_to_vector(*it->coefficient * other, it->power, result->_dividers);
		it++;
	}
	return (result);
}

Polynomial*	Polynomial::operator/(const Complex &other) const
{
	Polynomial*							result;
	std::vector<t_term>::const_iterator	it(this->_dividers.begin());

	result = new Polynomial(*this);
	free_vector_terms(result->_dividers);
	while (it != this->_dividers.end())
	{
		add_term_to_vector(*it->coefficient * other, it->power, result->_dividers);
		it++;
	}
	return (result);
}

Polynomial*	Polynomial::operator/(const Matrix &other) const
{
	Polynomial*							result;
	std::vector<t_term>::const_iterator	it(this->_dividers.begin());

	result = new Polynomial(*this);
	free_vector_terms(result->_dividers);
	while (it != this->_dividers.end())
	{
		add_term_to_vector(*it->coefficient * other, it->power, result->_dividers);
		it++;
	}
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
	// TODO: A voir s'il faut le gerer
	// TODO: Oui, il faut le gerer, c'est le resultat de la division euclidienne
	throw UNSUPPORTED_DIVISION;
	return (nullptr);
	(void)other;
}

Polynomial*	Polynomial::operator%(const Rational &other) const
{
	// TODO: A voir s'il faut le gerer
	throw UNSUPPORTED_DIVISION;
	return (nullptr);
	(void)other;
}

Polynomial*	Polynomial::operator%(const Complex &other) const
{
	// TODO: A voir s'il faut le gerer
	throw UNSUPPORTED_DIVISION;
	return (nullptr);
	(void)other;
}

Polynomial*	Polynomial::operator%(const Matrix &other) const
{
	// TODO: A voir s'il faut le gerer
	throw UNSUPPORTED_DIVISION;
	return (nullptr);
	(void)other;
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

IType*		Polynomial::operator^(const IType &other) const
{
	Rational	power;

	try
	{
		power = Rational(other);
	}
	catch(const LogicError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	if (!power.is_integer() || power < Rational(0))
		throw UNSUPPORTED_EXPONENT;
	return (polynomial_power_rational(this, power.getNumerator()));
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

IType*			Polynomial::clone(void) const
{
	return (new Polynomial(*this));
}

std::ostream&	Polynomial::print(std::ostream &os) const
{
	print_terms(os, this->_terms, this->_name);// TODO: Pas sur que ca ca fonctionne
	os << " / ";
	print_terms(os, this->_dividers, this->_name);
	return (os);
}

std::ostream&	Polynomial::print_polynomial(std::ostream &os, const std::string &var) const
{
	// TODO: Is this function still useful?
	throw ERROR_UNEXPECTED;
	(void)var;
	return (os);
}

unsigned int	Polynomial::get_degree(void) const
{
	std::vector<t_term>::const_iterator	it_terms(this->_terms.begin());
	std::vector<t_term>::const_iterator	it_dividers(this->_dividers.begin());
	unsigned int						degree(0);

	while (it_terms != this->_terms.end())
	{
		if (it_terms->power > degree)
			degree = it_terms->power;
		it_terms++;
	}
	while (it_dividers != this->_dividers.end())
	{
		if (it_dividers->power > degree)
			degree = it_dividers->power;
		it_dividers++;
	}
	return (degree);
}

void			Polynomial::reduce(void)
{
}


// Output stream operator overload
std::ostream	&operator<<(std::ostream &os, const Polynomial &var)
{
	return (var.print(os));
}
