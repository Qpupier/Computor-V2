/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor-V1.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:03:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/21 19:16:05 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Polynomial.hpp"
#include "quadratic.hpp"

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
	std::cout << "Δ = " << *discriminant << std::endl;
	return (discriminant);
}

static void		print_solutions_reduce_all(const t_quadratic_solutions &solutions)
{
	Complex	solution1;
	Complex	solution2;

	std::cout << "All terms reduced, solutions are:" << std::endl;
	solution1 = Complex(\
			Rational(solutions.real_term1 - solutions.real_term2_factor * solutions.sqrt_real - solutions.real_term3_factor * solutions.sqrt_imaginary, solutions.real_denominator), \
			Rational(solutions.imaginary_term1 + solutions.imaginary_term2_factor * solutions.sqrt_real - solutions.imaginary_term3_factor * solutions.sqrt_imaginary, solutions.imaginary_denominator));
	solution2 = Complex(\
			Rational(solutions.real_term1 + solutions.real_term2_factor * solutions.sqrt_real + solutions.real_term3_factor * solutions.sqrt_imaginary, solutions.real_denominator), \
			Rational(solutions.imaginary_term1 - solutions.imaginary_term2_factor * solutions.sqrt_real + solutions.imaginary_term3_factor * solutions.sqrt_imaginary, solutions.imaginary_denominator));
	if (solution1 == solution2)
		std::cout << "S = {" << solution1 << "} ∈ ";
	else
		std::cout << "S = {" << solution1 << ", " << solution2 << "} ∈ ";
	if (solution1.getImaginary() || solution2.getImaginary())
		std::cout << "ℂ" << std::endl;
	else
		std::cout << "ℝ" << std::endl;
}

static void	simplify_deepest_sqrt(t_quadratic_solutions &solutions)
{
	int	sqrt_term1;
	int	tmp;

	sqrt_term1 = solutions.sqrt_term1_factor * solutions.sqrt_term1_sqrt;
	solutions.sqrt_real = sqrt_term1 + solutions.sqrt_term2;
	tmp = solutions.sqrt_real;
	reduce_sqrt(&solutions.real_term2_factor, &tmp);
	reduce_sqrt(&solutions.imaginary_term2_factor, &solutions.sqrt_real);
	solutions.sqrt_imaginary = sqrt_term1 - solutions.sqrt_term2;
	tmp = solutions.sqrt_imaginary;
	reduce_sqrt(&solutions.real_term3_factor, &tmp);
	reduce_sqrt(&solutions.imaginary_term3_factor, &solutions.sqrt_imaginary);
}

static void	print_solutions_half_reduced(const t_quadratic_solutions &solutions)
{
	if (solutions.real_denominator > 1)
		std::cout << "(";
	if (solutions.real_term1)
		std::cout << solutions.real_term1;
	std::cout << " - ";
	if (solutions.real_term2_factor != 1)
		std::cout << solutions.real_term2_factor;
	std::cout << "√" << solutions.sqrt_real;
	std::cout << " - ";
	if (solutions.real_term3_factor != 1)
		std::cout << solutions.real_term3_factor;
	std::cout << "√" << solutions.sqrt_imaginary;
	if (solutions.real_denominator > 1)
		std::cout << ") / " << solutions.real_denominator;
	std::cout << " + i(";
	if (solutions.imaginary_term1)
		std::cout << solutions.imaginary_term1;
	std::cout << " + ";
	if (solutions.imaginary_term2_factor != 1)
		std::cout << solutions.imaginary_term2_factor;
	std::cout << "√" << solutions.sqrt_real;
	std::cout << " - ";
	if (solutions.imaginary_term3_factor != 1)
		std::cout << solutions.imaginary_term3_factor;
	std::cout << "√" << solutions.sqrt_imaginary;
	std::cout << ")";
	if (solutions.imaginary_denominator > 1)
		std::cout << " / " << solutions.imaginary_denominator;
	std::cout << std::endl;
}

typedef void	(*t_solution_printer)(const t_quadratic_solutions &solutions);

void	print_solutions_reduce_nothing(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 0 called" << std::endl; }
void	print_solutions_reduce_im2(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 1 called" << std::endl; }
void	print_solutions_reduce_im1(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 2 called" << std::endl; }
void	print_solutions_reduce_im1_im2(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 3 called" << std::endl; }
void	print_solutions_reduce_re2(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 4 called" << std::endl; }
void	print_solutions_reduce_re2_im2(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 5 called" << std::endl; }
void	print_solutions_reduce_re2_im1(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 6 called" << std::endl; }
void	print_solutions_reduce_re2_im1_im2(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 7 called" << std::endl; }
void	print_solutions_reduce_re1(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 8 called" << std::endl; }
void	print_solutions_reduce_re1_im2(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 9 called" << std::endl; }
void	print_solutions_reduce_re1_im1(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 10 called" << std::endl; }
void	print_solutions_reduce_re1_im1_im2(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 11 called" << std::endl; }
void	print_solutions_reduce_re1_re2(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 12 called" << std::endl; }
void	print_solutions_reduce_re1_re2_im2(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 13 called" << std::endl; }
void	print_solutions_reduce_re1_re2_im1(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 14 called" << std::endl; }
// static void	print_solutions_reduce_all(const t_quadratic_solutions &solutions) { print_solutions_half_reduced(solutions); std::cout << "Function 15 called" << std::endl; }

static const t_solution_printer	g_solution_printers[16] =
{
	print_solutions_reduce_nothing,
	print_solutions_reduce_im2,
	print_solutions_reduce_re2,
	print_solutions_reduce_re2_im2,
	print_solutions_reduce_re2_im1,
	print_solutions_reduce_re2_im1_im2,
	print_solutions_reduce_re1,
	print_solutions_reduce_re1_im2,
	print_solutions_reduce_re1_im1,
	print_solutions_reduce_re1_im1_im2,
	print_solutions_reduce_re1_re2,
	print_solutions_reduce_re1_re2_im2,
	print_solutions_reduce_re1_re2_im1,
	print_solutions_reduce_all
};

static void	print_solutions(t_quadratic_solutions& solutions)
{
	if (!solutions.sqrt_term1_factor || solutions.sqrt_term1_sqrt <= 1)
	{
		simplify_deepest_sqrt(solutions);
		enum	e_resolution_flags
		{
			REAL_TERM2_RESOLVED = 1 << 0,
			REAL_TERM3_RESOLVED = 1 << 1,
			IMAGINARY_TERM2_RESOLVED = 1 << 2,
			IMAGINARY_TERM3_RESOLVED = 1 << 3,
		};
		int	resolved_flags;

		resolved_flags = 0;
		if (solutions.sqrt_real <= 1)
			resolved_flags |= REAL_TERM2_RESOLVED | IMAGINARY_TERM2_RESOLVED;
		if (solutions.sqrt_imaginary <= 1)
			resolved_flags |= REAL_TERM3_RESOLVED | IMAGINARY_TERM3_RESOLVED;
		if (!solutions.real_term2_factor)
			resolved_flags |= REAL_TERM2_RESOLVED;
		if (!solutions.real_term3_factor)
			resolved_flags |= REAL_TERM3_RESOLVED;
		if (!solutions.imaginary_term2_factor)
			resolved_flags |= IMAGINARY_TERM2_RESOLVED;
		if (!solutions.imaginary_term3_factor)
			resolved_flags |= IMAGINARY_TERM3_RESOLVED;
		std::cout << "Resolved flags: " << resolved_flags << std::endl;
		g_solution_printers[resolved_flags](solutions);
		// else if (!solutions.real_term2_factor || solutions.sqrt_real <= 1)
		// {
		// 	// print_solutions_real();
		// }
		// else if (!solutions.real || solutions.sqrt_imaginary <= 1)
		// {
		// 	// print_solutions_imaginary();
		// }
	}
	else
	{
		// print_solutions_irreducible();
	}
	// print_rounded_solutions();
}

static void	find_solutions(IType *tmp_a, IType *tmp_b, IType *tmp_delta)
{
	Complex*				a;
	Complex*				b;
	Complex*				delta;
	t_quadratic_solutions	solutions_structure;

	a = nullptr;
	b = nullptr;
	delta = nullptr;
	try
	{
		a = new Complex(*tmp_a);
		b = new Complex(*tmp_b);
		delta = new Complex(*tmp_delta);
	}
	catch (const UnexpectedError &e)
	{
		if (a)
			delete a;
		if (b)
			delete b;
		throw UnsupportedError("Solutions can only be found in ℝ or ℂ");
	}
	solutions_structure = get_solutions_structure(a, b, delta);
	print_solutions(solutions_structure);
}

void	reduce_sqrt(int *factor, int *sqrt)
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

void	solve_trinomial(Polynomial *polynomial)
{
	std::vector<Polynomial::t_term>	terms(polynomial->getTerms());
	IType*							a;
	IType*							b;
	IType*							c;
	IType*							discriminant;

	a = terms[2].coefficient;
	b = terms[1].coefficient;
	c = terms[0].coefficient;
	discriminant = get_discriminant(a, b, c);
	delete c;
	find_solutions(a, b, discriminant);
	delete a;
	delete b;
	delete discriminant;
}
