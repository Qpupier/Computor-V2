/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor-V1.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:03:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/15 13:46:19 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Polynomial.hpp"

static void	reduce_sqrt(int *factor, int *sqrt)
{
	double	sqrt_rounded;
	int		perfect_square;

	sqrt_rounded = std::sqrt(*sqrt);
	for (int i = 2; i <= sqrt_rounded; i++)
	{
		perfect_square = i * i;
		while (!(*sqrt % perfect_square))
		{
			*sqrt /= perfect_square;
			*factor *= i;
		}
	}
}

static void	solve_trinomial_complex(IType *a, IType *b, Complex *delta)
{
	double modulo;
	double p;
	double q;
	double denominator;
	double solution1_real;
	double solution1_imaginary;
	double solution2_real;
	double solution2_imaginary;
	Complex*	new_a;
	Complex*	new_b;

	try
	{
		new_a = new Complex(*a);
		delete a;
		new_b = new Complex(*b);
		delete b;
	}
	catch (const UnexpectedError &e)
	{
		throw UnexpectedError("Invalid polynomial: non-complex coefficients");//review with complexes
	}
	modulo = std::sqrt(std::pow(delta->getReal().getValue(), 2) + std::pow(delta->getImaginary().getValue(), 2));
	p = std::sqrt((modulo + delta->getReal().getValue()) / 2);
	q = std::sqrt((modulo - delta->getReal().getValue()) / 2);
	if (delta->getImaginary().getValue() < 0)
		q = -q;
	denominator = 2 * (std::pow(new_a->getReal().getValue(), 2) + std::pow(new_a->getImaginary().getValue(), 2));
	solution1_real = (new_a->getReal().getValue() * (p - new_b->getReal().getValue()) + new_a->getImaginary().getValue() * (q - new_b->getImaginary().getValue())) / denominator;
	solution1_imaginary = (new_a->getReal().getValue() * (q - new_b->getImaginary().getValue()) - new_a->getImaginary().getValue() * (p - new_b->getReal().getValue())) / denominator;
	solution2_real = (new_a->getReal().getValue() * (-p - new_b->getReal().getValue()) + new_a->getImaginary().getValue() * (-q - new_b->getImaginary().getValue())) / denominator;
	solution2_imaginary = (new_a->getReal().getValue() * (-q - new_b->getImaginary().getValue()) - new_a->getImaginary().getValue() * (-p - new_b->getReal().getValue())) / denominator;
	std::cout << "S = {" << solution1_real << " + " << solution1_imaginary << "i, " << solution2_real << " + " << solution2_imaginary << "i} ∈ ℂ" << std::endl;
	// TODO ameliorer le print
}

static void	find_complex_solutions(Rational* a, Rational* b, Rational* discriminant)
{
	double		sqrt_discriminant;
	double		solution1;
	double		solution2;

	sqrt_discriminant = std::sqrt(discriminant->getNumerator() / static_cast<double>(discriminant->getDenominator()));
	solution1 = (-b->getNumerator() / static_cast<double>(b->getDenominator()) - sqrt_discriminant) / (2 * a->getNumerator() / static_cast<double>(a->getDenominator()));
	solution2 = (-b->getNumerator() / static_cast<double>(b->getDenominator()) + sqrt_discriminant) / (2 * a->getNumerator() / static_cast<double>(a->getDenominator()));
	std::cout << "S = {" << solution1 << ", " << solution2 << "} ∈ ℂ" << std::endl;
	// TODO
}

static void	find_one_solution(Rational *a, Rational *b)
{
	double		solution1;

	if (!a || !b)
		throw UnexpectedError("Invalid polynomial: non-rational coefficients");
	solution1 = -b->getNumerator() / static_cast<double>(b->getDenominator()) / (2 * a->getNumerator() / static_cast<double>(a->getDenominator()));
	std::cout << "S = {" << solution1 << "} ∈ ℝ" << std::endl;
}

static void	print_result_root(const Rational &factor, int b, int delta_factor, int sqrt, bool positive)
{
	if (sqrt == 1)
	{
		Rational* result;

		result = factor * Rational(-b + (positive ? delta_factor : -delta_factor));
		std::cout << *result;
		delete result;
		return ;
	}
	
	
	if (positive)
		std::cout << factor.getNumerator() << "(" << -b << " + " << delta_factor << "√" << sqrt << ") / " << factor.getDenominator();
	else
		std::cout << factor.getNumerator() << "(" << -b << " - " << delta_factor << "√" << sqrt << ") / " << factor.getDenominator();
}

static void	print_result_roots(std::string var, const Rational &factor, int b, int delta_factor, int sqrt)
{
	double solution1;
	double solution2;

	solution1 = factor.getNumerator() * (-b - delta_factor * std::sqrt(sqrt)) / factor.getDenominator();
	solution2 = factor.getNumerator() * (-b + delta_factor * std::sqrt(sqrt)) / factor.getDenominator();
	std::cout << "\033[32mS = {";
	print_result_root(factor, b, delta_factor, sqrt, solution1 > solution2);
	std::cout << ", ";
	print_result_root(factor, b, delta_factor, sqrt, solution1 <= solution2);
	std::cout << "} ∈ ℝ\033[0m" << std::endl;
	std::cout << "\033[30m" << var << "1 ≃ " << std::min(solution1, solution2) << "\033[0m" << std::endl;
	std::cout << "\033[30m" << var << "2 ≃ " << std::max(solution1, solution2) << "\033[0m" << std::endl;
}

static void	find_two_solutions(std::string variable, Rational *a, Rational *b, Rational *discriminant)
{
	int factor_num = 1;
	int factor_den = 1;
	int sqrt_num = discriminant->getNumerator();
	int sqrt_den = discriminant->getDenominator();
	reduce_sqrt(&factor_num, &sqrt_num);
	reduce_sqrt(&factor_den, &sqrt_den);
	Rational factor(factor_num, factor_den * sqrt_den);
	int new_sqrt = sqrt_num * sqrt_den;
	int new_b = b->getNumerator() * factor.getDenominator();
	int new_fac = factor.getNumerator() * b->getDenominator();
	Rational factorize(new_b, new_fac);
	int test = new_b / factorize.getNumerator();
	new_b = factorize.getNumerator();
	new_fac = factorize.getDenominator();
	Rational final(test * a->getDenominator(), 2 * a->getNumerator() * b->getDenominator() * factor.getDenominator());
	print_result_roots(variable, final, new_b, new_fac, new_sqrt);
}

static void	solve_trinomial_rational(std::string variable, IType *a, IType *b, Rational *discriminant)
{
	Rational*	new_a;
	Rational*	new_b;

	try
	{
		new_a = new Rational(*a);
		delete a;
		new_b = new Rational(*b);
		delete b;
	}
	catch (const UnexpectedError &e)
	{
		throw UnexpectedError("Invalid polynomial: non-rational coefficients");//review with complexes
	}
	if (*discriminant < Rational(0))
		find_complex_solutions(new_a, new_b, discriminant);
	else if (!*discriminant)
		find_one_solution(new_a, new_b);
	else
		find_two_solutions(variable, new_a, new_b, discriminant);
	delete discriminant;
}

void	solve_trinomial(Polynomial *polynomial)
{
	std::vector<Polynomial::t_term>	terms;
	IType*							a;
	IType*							b;
	IType*							c;
	IType*							tmp_b2;
	IType*							tmp_4a;
	IType*							tmp_4ac;
	IType*							tmp_b2_4ac;
	Rational*						discriminant_rational;
	Complex*						discriminant_complex;

	a = nullptr;
	b = nullptr;
	c = nullptr;
	for (std::vector<Polynomial::t_term>::const_iterator it = terms.begin(); it != terms.end(); it++)
	{
		if (it->power == 2)
			a = it->coefficient;
		else if (it->power == 1)
			b = it->coefficient;
		else if (it->power == 0)
			c = it->coefficient;
		else
			throw ERROR_UNEXPECTED;
	}
	if (!a)
		a = new Rational(0);
	if (!b)
		b = new Rational(0);
	if (!c)
		c = new Rational(0);
	tmp_b2 = *b * *b;
	delete b;
	tmp_4a = Rational(4) * *a;
	delete a;
	tmp_4ac = *tmp_4a * *c;
	delete c;
	tmp_b2_4ac = *tmp_b2 - *tmp_4ac;
	delete tmp_b2;
	delete tmp_4a;
	delete tmp_4ac;
	discriminant_rational = dynamic_cast<Rational*>(tmp_b2_4ac);
	if (discriminant_rational)
		return solve_trinomial_rational(polynomial->getName(), a, b, discriminant_rational);
	discriminant_complex = dynamic_cast<Complex*>(tmp_b2_4ac);
	if (discriminant_complex)
		return solve_trinomial_complex(a, b, discriminant_complex);
	delete tmp_b2_4ac;
}
