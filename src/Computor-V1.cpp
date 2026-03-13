/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor-V1.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:03:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/13 15:34:07 by qpupier          ###   ########lyon.fr   */
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

static void	solve_trinomial_complex(Polynomial *polynomial, Complex *delta)
{
	double modulo;
	double p;
	double q;
	double denominator;
	double solution1_real;
	double solution1_imaginary;
	double solution2_real;
	double solution2_imaginary;
	Complex *a;
	Complex *b;

	a = dynamic_cast<Complex*>(polynomial->getPower2());
	b = dynamic_cast<Complex*>(polynomial->getPower1());
	if (!a || !b)
		throw UnexpectedError("Invalid polynomial: non-rational coefficients");
	modulo = std::sqrt(std::pow(delta->getReal().getValue(), 2) + std::pow(delta->getImaginary().getValue(), 2));
	p = std::sqrt((modulo + delta->getReal().getValue()) / 2);
	q = std::sqrt((modulo - delta->getReal().getValue()) / 2);
	if (delta->getImaginary().getValue() < 0)
		q = -q;
	denominator = 2 * (std::pow(a->getReal().getValue(), 2) + std::pow(a->getImaginary().getValue(), 2));
	solution1_real = (a->getReal().getValue() * (p - b->getReal().getValue()) + a->getImaginary().getValue() * (q - b->getImaginary().getValue())) / denominator;
	solution1_imaginary = (a->getReal().getValue() * (q - b->getImaginary().getValue()) - a->getImaginary().getValue() * (p - b->getReal().getValue())) / denominator;
	solution2_real = (a->getReal().getValue() * (-p - b->getReal().getValue()) + a->getImaginary().getValue() * (-q - b->getImaginary().getValue())) / denominator;
	solution2_imaginary = (a->getReal().getValue() * (-q - b->getImaginary().getValue()) - a->getImaginary().getValue() * (-p - b->getReal().getValue())) / denominator;
	std::cout << "S = {" << solution1_real << " + " << solution1_imaginary << "i, " << solution2_real << " + " << solution2_imaginary << "i} ∈ ℂ" << std::endl;
	// TODO ameliorer le print
}

static void	find_complex_solutions(Polynomial *polynomial, Rational *discriminant)
{
	Rational	*a;
	Rational	*b;
	double		sqrt_discriminant;
	double		solution1;
	double		solution2;

	a = dynamic_cast<Rational*>(polynomial->getPower2());
	b = dynamic_cast<Rational*>(polynomial->getPower1());
	if (!a || !b)
		throw UnexpectedError("Invalid polynomial: non-rational coefficients");
	sqrt_discriminant = std::sqrt(discriminant->getNumerator() / static_cast<double>(discriminant->getDenominator()));
	solution1 = (-b->getNumerator() / static_cast<double>(b->getDenominator()) - sqrt_discriminant) / (2 * a->getNumerator() / static_cast<double>(a->getDenominator()));
	solution2 = (-b->getNumerator() / static_cast<double>(b->getDenominator()) + sqrt_discriminant) / (2 * a->getNumerator() / static_cast<double>(a->getDenominator()));
	std::cout << "S = {" << solution1 << ", " << solution2 << "} ∈ ℂ" << std::endl;
	// TODO
}

static void	find_one_solution(Polynomial *polynomial)
{
	Rational	*a;
	Rational	*b;
	double		solution1;

	a = dynamic_cast<Rational*>(polynomial->getPower2());
	b = dynamic_cast<Rational*>(polynomial->getPower1());
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

static void	find_two_solutions(Polynomial *polynomial, Rational *discriminant)
{
	Rational	*a;
	Rational	*b;

	a = dynamic_cast<Rational*>(polynomial->getPower2());
	b = dynamic_cast<Rational*>(polynomial->getPower1());
	if (!a || !b)
		throw UnexpectedError("Invalid polynomial: non-rational coefficients");//review with complexes





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
	print_result_roots(polynomial->getName(), final, new_b, new_fac, new_sqrt);
}

static void	solve_trinomial_rational(Polynomial *polynomial, Rational *discriminant)
{
	if (*discriminant < Rational(0))
		find_complex_solutions(polynomial, discriminant);
	else if (!*discriminant)
		find_one_solution(polynomial);
	else
		find_two_solutions(polynomial, discriminant);
	delete discriminant;
}

void	solve_trinomial(Polynomial *polynomial)
{
	IType		*tmp_b2;
	IType		*tmp_4a;
	IType		*tmp_4ac;
	IType		*tmp_b2_4ac;
	Rational	*discriminant_rational;
	Complex		*discriminant_complex;

	tmp_b2 = *polynomial->getPower1() * *polynomial->getPower1();
	tmp_4a = Rational(4) * *polynomial->getPower2();
	tmp_4ac = *tmp_4a * *polynomial->getPower0();
	tmp_b2_4ac = *tmp_b2 - *tmp_4ac;
	delete tmp_b2;
	delete tmp_4a;
	delete tmp_4ac;
	discriminant_rational = dynamic_cast<Rational*>(tmp_b2_4ac);
	if (discriminant_rational)
		return solve_trinomial_rational(polynomial, discriminant_rational);
	discriminant_complex = dynamic_cast<Complex*>(tmp_b2_4ac);
	if (discriminant_complex)
		return solve_trinomial_complex(polynomial, discriminant_complex);
	delete tmp_b2_4ac;
}
