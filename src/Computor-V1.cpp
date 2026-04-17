/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor-V1.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:03:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/17 18:12:15 by qpupier          ###   ########lyon.fr   */
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
		throw UnexpectedError("Invalid polynomial: non-complex coefficients");
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
	// [ ] Partie manquante
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

static void	solve_trinomial_rational(std::string variable, IType *a, IType *b, Rational *delta)
{
	Rational*	new_a;
	Rational*	new_b;

	try
	{
		new_a = new Rational(*a);
		new_b = new Rational(*b);
	}
	catch (const UnexpectedError &e)
	{
		throw UnexpectedError("Invalid polynomial: non-rational coefficients");//review with complexes
	}
	if (*delta < Rational(0))
		find_complex_solutions(new_a, new_b, delta);
	else if (!*delta)
		find_one_solution(new_a, new_b);
	else
		find_two_solutions(variable, new_a, new_b, delta);
	delete new_a;
	delete new_b;
}

static IType*	get_discriminant(IType *a, IType *b, IType *c)
{
	IType*	tmp_b2;
	IType*	tmp_4a;
	IType*	tmp_4ac;
	IType*	discriminant;

	tmp_b2 = *b * *b;
	tmp_4a = Rational(4) * *a;
	tmp_4ac = *tmp_4a * *c;
	delete tmp_4a;
	discriminant = *tmp_b2 - *tmp_4ac;
	delete tmp_b2;
	delete tmp_4ac;
	return (discriminant);
}

static void	print_solutions_resolved(int real_sqrt, int imaginary_sqrt, int real_term1, int real_term2, int real_term3, int real_denominator, int imaginary_term1, int imaginary_term2, int imaginary_term3, int imaginary_denominator)
{
	Complex* solution1;
	Complex* solution2;
	solution1 = new Complex(Rational(real_term1 - real_term2 * real_sqrt - real_term3 * imaginary_sqrt, real_denominator), Rational(imaginary_term1 + imaginary_term2 * real_sqrt - imaginary_term3 * imaginary_sqrt, imaginary_denominator));
	solution2 = new Complex(Rational(real_term1 + real_term2 * real_sqrt + real_term3 * imaginary_sqrt, real_denominator), Rational(imaginary_term1 - imaginary_term2 * real_sqrt + imaginary_term3 * imaginary_sqrt, imaginary_denominator));
	if (*solution1 == *solution2)
		std::cout << "S = {" << *solution1 << "} ∈ ";
	else
		std::cout << "S = {" << *solution1 << ", " << *solution2 << "} ∈ ";
	if (solution1->getImaginary() || solution2->getImaginary())
		std::cout << "ℂ" << std::endl;
	else
		std::cout << "ℝ" << std::endl;
	delete 	solution1;
	delete solution2;
}

static void	print_solutions(int small_factor, int small_sqrt, int final_term2, int real_term1, int real_term2, int real_term3, int real_denominator, int imaginary_term1, int imaginary_term2, int imaginary_term3, int imaginary_denominator)
{
	if (small_sqrt <= 1)
	{
		int sqrt_real = small_factor * small_sqrt + final_term2;
		int factor_real = 1;
		int sqrt_imaginary = small_factor * small_sqrt - final_term2;
		int factor_imaginary = 1;
		reduce_sqrt(&factor_real, &sqrt_real);
		reduce_sqrt(&factor_imaginary, &sqrt_imaginary);
		if (sqrt_real <= 1 && sqrt_imaginary <= 1)
		{
			print_solutions_resolved(factor_real * sqrt_real, factor_imaginary * sqrt_imaginary, real_term1, real_term2, real_term3, real_denominator, imaginary_term1, imaginary_term2, imaginary_term3, imaginary_denominator);
		}
		else if (sqrt_real <= 1)
		{
			// print_solutions_real();
		}
		else if (sqrt_imaginary <= 1)
		{
			// print_solutions_imaginary();
		}
		else
		{
			// print_solutions_half();
		}
	}
	else
	{
		// print_solutions_irreducible();
	}
	// print_rounded_solutions();
}

static void	test(IType *tmp_a, IType *tmp_b, IType *tmp_c, IType *tmp_delta)
{
	Complex*	a;
	Complex*	b;
	Complex*	c;
	Complex*	delta;

	a = new Complex(*tmp_a);
	b = new Complex(*tmp_b);
	c = new Complex(*tmp_c);
	delta = new Complex(*tmp_delta);


	std::cout << "delta = " << *delta << std::endl;
	Rational* delta_real_square = delta->getReal() * delta->getReal();
	Rational* delta_imaginary_square = delta->getImaginary() * delta->getImaginary();
	Rational* module_square = *delta_real_square + *delta_imaginary_square;
	std::cout << "Module² = " << *module_square << std::endl;

	int small_factor = 2;
	int small_sqrt = module_square->getNumerator() * module_square->getDenominator();
	reduce_sqrt(&small_factor, &small_sqrt);

	Rational* term_1 = new Rational(small_factor, module_square->getDenominator());
	Rational* small_gcd = term_1->gcd(delta->getReal());
	Rational* small_factor_reduced = *term_1 / *small_gcd;

	Rational* tmp = delta->getReal() * Rational(2);
	Rational* delta_real_reduced = *tmp / *small_gcd;
	
	int big_factor = 1;
	int big_sqrt = small_gcd->getNumerator() * small_gcd->getDenominator();
	reduce_sqrt(&big_factor, &big_sqrt);

	Rational* final_factor = new Rational(big_factor, 2 * small_gcd->getDenominator());
	int final_small_factor = big_sqrt * small_factor_reduced->getNumerator();
	int final_term2 = big_sqrt * delta_real_reduced->getNumerator();

	std::string big_sqrt_1 = "√(" + std::to_string(final_small_factor) + "√" + std::to_string(small_sqrt) + " + " + std::to_string(final_term2) + ")";
	std::string big_sqrt_2 = "√(" + std::to_string(final_small_factor) + "√" + std::to_string(small_sqrt) + " - " + std::to_string(final_term2) + ")";
	std::cout << "(" << *final_factor << ")" << big_sqrt_1 << std::endl;
	Rational* final_factor2 = *final_factor * Rational(delta->getImaginary() < Rational(0) ? -1 : 1);
	std::cout << "(" << *final_factor2 << ")" << big_sqrt_2 << std::endl;

	tmp = a->getReal() * Rational(-1);
	Rational* tmp1 = *tmp * b->getReal();
	Rational* tmp2 = a->getImaginary() * b->getImaginary();
	Rational* big_term_real = *tmp1 - *tmp2;

	tmp1 = a->getImaginary() * b->getReal();
	tmp2 = a->getReal() * b->getImaginary();
	Rational* big_term_imaginary = *tmp1 - *tmp2;

	Rational *a_real_square = a->getReal() * a->getReal();
	Rational *a_imaginary_square = a->getImaginary() * a->getImaginary();
	tmp1 = Rational(2) * *a_real_square;
	tmp2 = Rational(2) * *a_imaginary_square;
	Rational* denominator = *tmp1 + *tmp2;

	Rational* term_2_real = a->getReal() * *final_factor;
	Rational* term3_real = a->getImaginary() * *final_factor2;
	Rational* gcd1 = big_term_real->gcd(*term_2_real)->gcd(*term3_real)->gcd(*denominator);
	Rational* final_real_term_1 = *big_term_real / *gcd1;
	Rational* final_real_term_2 = *term_2_real / *gcd1;
	Rational* final_real_term_3 = *term3_real / *gcd1;
	Rational* final_real_denominator = *denominator / *gcd1;

	Rational* term_imaginary_2 = a->getImaginary() * *final_factor;
	Rational* term_imaginary_3 = a->getReal() * *final_factor2;
	Rational* gcd2 = big_term_imaginary->gcd(*term_imaginary_2)->gcd(*term_imaginary_3)->gcd(*denominator);
	Rational* final_imaginary_term_1 = *big_term_imaginary / *gcd2;
	Rational* final_imaginary_term_2 = *term_imaginary_2 / *gcd2;
	Rational* final_imaginary_term_3 = *term_imaginary_3 / *gcd2;
	Rational* final_imaginary_denominator = *denominator / *gcd2;

	std::cout << "(" << *final_real_term_1 << " - " << *final_real_term_2 << big_sqrt_1 << " - " << *final_real_term_3 << big_sqrt_2 << ") / " << *final_real_denominator << " + i(" << *final_imaginary_term_1 << " + " << *final_imaginary_term_2 << big_sqrt_1 << " - " << *final_imaginary_term_3 << big_sqrt_2 << ") / " << *final_imaginary_denominator << std::endl;
	std::cout << "(" << *final_real_term_1 << " + " << *final_real_term_2 << big_sqrt_1 << " + " << *final_real_term_3 << big_sqrt_2 << ") / " << *final_real_denominator << " + i(" << *final_imaginary_term_1 << " - " << *final_imaginary_term_2 << big_sqrt_1 << " + " << *final_imaginary_term_3 << big_sqrt_2 << ") / " << *final_imaginary_denominator<< std::endl;
	print_solutions(final_small_factor, small_sqrt, final_term2, final_real_term_1->getNumerator(), final_real_term_2->getNumerator(), final_real_term_3->getNumerator(), final_real_denominator->getDenominator(), final_imaginary_term_1->getNumerator(), final_imaginary_term_2->getNumerator(), final_imaginary_term_3->getNumerator(), final_imaginary_denominator->getDenominator());
}

void	solve_trinomial(Polynomial *polynomial)
{
	std::vector<Polynomial::t_term>	terms(polynomial->getTerms());
	IType*							a;
	IType*							b;
	IType*							c;
	IType*							discriminant;
	Rational*						discriminant_rational;
	Complex*						discriminant_complex;

	a = terms[2].coefficient;
	b = terms[1].coefficient;
	c = terms[0].coefficient;
	discriminant = get_discriminant(a, b, c);
	test(a, b, c, discriminant);
	return ;
	try
	{
		discriminant_rational = new Rational(*discriminant);
	}
	catch (const UnexpectedError &e)
	{
		discriminant_rational = nullptr;
	}
	try
	{
		discriminant_complex = new Complex(*discriminant);
	}
	catch (const UnexpectedError &e)
	{
		discriminant_complex = nullptr;
	}
	if (discriminant_rational)
	{
		solve_trinomial_rational(polynomial->getName(), a, b, discriminant_rational);
		delete discriminant_rational;
	}
	else if (discriminant_complex)
	{
		solve_trinomial_complex(a, b, discriminant_complex);
		delete discriminant_complex;
	}
	else
	{
		delete discriminant;
		throw UnsupportedError("Solutions can only be found in ℝ or ℂ");
	}
	delete discriminant;
}
