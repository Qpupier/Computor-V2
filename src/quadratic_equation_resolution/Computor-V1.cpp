/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor-V1.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:03:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/22 18:08:04 by qpupier          ###   ########lyon.fr   */
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
	std::cout << COLOR_PINK << "Δ = " << *discriminant << COLOR_RESET << std::endl;
	return (discriminant);
}

static void	simplify_deepest_sqrt(t_quadratic_solutions &solutions)
{
	int	sqrt_term1;
	int	tmp;

	sqrt_term1 = solutions.sqrt_term1_factor * solutions.sqrt_term1_sqrt;
	solutions.sqrt_real = sqrt_term1 + solutions.sqrt_term2_real;
	tmp = solutions.sqrt_real;
	reduce_sqrt(&solutions.real_term2_factor[0], &tmp);
	tmp = solutions.sqrt_real;
	reduce_sqrt(&solutions.real_term2_factor[1], &tmp);
	tmp = solutions.sqrt_real;
	reduce_sqrt(&solutions.imaginary_term2_factor[0], &tmp);
	reduce_sqrt(&solutions.imaginary_term2_factor[1], &solutions.sqrt_real);
	solutions.sqrt_imaginary = sqrt_term1 + solutions.sqrt_term2_imaginary;
	tmp = solutions.sqrt_imaginary;
	reduce_sqrt(&solutions.real_term3_factor[0], &tmp);
	tmp = solutions.sqrt_imaginary;
	reduce_sqrt(&solutions.real_term3_factor[1], &tmp);
	tmp = solutions.sqrt_imaginary;
	reduce_sqrt(&solutions.imaginary_term3_factor[0], &tmp);
	reduce_sqrt(&solutions.imaginary_term3_factor[1], &solutions.sqrt_imaginary);
}

static std::string	get_sqrt_real(t_quadratic_solutions &solutions)
{
	std::string	small_sqrt;

	if (solutions.sqrt_term1_factor == -1)
		small_sqrt += "-";
	else if (solutions.sqrt_term1_factor != 1)
		small_sqrt += std::to_string(solutions.sqrt_term1_factor);
	small_sqrt += "√" + std::to_string(solutions.sqrt_term1_sqrt);
	if (solutions.sqrt_term2_real)
	{
		if (solutions.sqrt_term2_real > 0)
			small_sqrt += " + ";
		else
		{
			small_sqrt += " - ";
			solutions.sqrt_term2_real *= -1;
		}
		small_sqrt += std::to_string(solutions.sqrt_term2_real);
	}
	return (small_sqrt);
}

static std::string	get_sqrt_imaginary(t_quadratic_solutions &solutions)
{
	std::string	small_sqrt;

	if (solutions.sqrt_term1_factor == -1)
		small_sqrt += "-";
	else if (solutions.sqrt_term1_factor != 1)
		small_sqrt += std::to_string(solutions.sqrt_term1_factor);
	small_sqrt += "√" + std::to_string(solutions.sqrt_term1_sqrt);
	if (solutions.sqrt_term2_imaginary)
	{
		if (solutions.sqrt_term2_imaginary > 0)
			small_sqrt += " + ";
		else
		{
			small_sqrt += " - ";
			solutions.sqrt_term2_imaginary *= -1;
		}
		small_sqrt += std::to_string(solutions.sqrt_term2_imaginary);
	}
	return (small_sqrt);
}

void	print_real_part(t_quadratic_solutions &solutions, int index, bool reduce_sqrt)
{
	std::string sqrt_real = get_sqrt_real(solutions);
	std::string sqrt_imaginary = get_sqrt_imaginary(solutions);
	if (!solutions.real_term1[index] && !solutions.real_term2_factor[index] && !solutions.real_term3_factor[index])
	{
		std::cout << "0";
		return ;
	}
	if (solutions.real_denominator > 1)// [ ] mais pas si 1 seul term
			std::cout << "(";
	if (solutions.real_term1[index])
		std::cout << solutions.real_term1[index];
	if (solutions.real_term2_factor[index])
	{
		if (solutions.real_term1[index])
		{
			if (solutions.real_term2_factor[index] > 0)
				std::cout << " + ";
			else
			{
				std::cout << " - ";
				solutions.real_term2_factor[index] *= -1;
			}
		}
		if (solutions.real_term2_factor[index] == -1)
			std::cout << "-";
		else if (solutions.real_term2_factor[index] != 1)
			std::cout << solutions.real_term2_factor[index];
		if (solutions.real_term2_factor[index] != 1)
			std::cout << solutions.real_term2_factor[index];
		if (reduce_sqrt)
			std::cout << "√" << solutions.sqrt_real;
		else
			std::cout << "√(" << sqrt_real << ")";
	}
	if (solutions.real_term3_factor[index])
	{
		if (solutions.real_term1[index] || solutions.real_term2_factor[index])
		{
			if (solutions.real_term3_factor[index] > 0)
				std::cout << " + ";
			else
			{
				std::cout << " - ";
				solutions.real_term3_factor[index] *= -1;
			}
		}
		if (solutions.real_term3_factor[index] == -1)
			std::cout << "-";
		else if (solutions.real_term3_factor[index] != 1)
			std::cout << solutions.real_term3_factor[index];
		if (reduce_sqrt)
			std::cout << "√" << solutions.sqrt_imaginary;
		else
			std::cout << "√(" << sqrt_imaginary << ")";
	}
	if (solutions.real_denominator > 1)
		std::cout << ") / " << solutions.real_denominator;
}

bool	print_imaginary_part(t_quadratic_solutions &solutions, int index, bool reduce_sqrt)
{
	if (!solutions.imaginary_term1[index] && !solutions.imaginary_term2_factor[index] && !solutions.imaginary_term3_factor[index])
		return false;
	std::string sqrt_real = get_sqrt_real(solutions);
	std::string sqrt_imaginary = get_sqrt_imaginary(solutions);
	std::cout << " + i(";// [ ] si 1 term ou plus
	if (solutions.imaginary_term1[index])
		std::cout << solutions.imaginary_term1[index];
	if (solutions.imaginary_term2_factor[index])
	{
		if (solutions.imaginary_term1[index])
		{
			if (solutions.imaginary_term2_factor[index] > 0)
				std::cout << " + ";
			else
			{
				std::cout << " - ";
				solutions.imaginary_term2_factor[index] *= -1;
			}
		}
		if (solutions.imaginary_term2_factor[index] == -1)
			std::cout << "-";
		else if (solutions.imaginary_term2_factor[index] != 1)
			std::cout << solutions.imaginary_term2_factor[index];
		if (reduce_sqrt)
			std::cout << "√" << solutions.sqrt_real;
		else
			std::cout << "√(" << sqrt_real << ")";
	}
	if (solutions.imaginary_term3_factor[index])
	{
		if (solutions.imaginary_term1[index] || solutions.imaginary_term2_factor[index])
		{
			if (solutions.imaginary_term3_factor[index] > 0)
				std::cout << " + ";
			else
			{
				std::cout << " - ";
				solutions.imaginary_term3_factor[index] *= -1;
			}
		}
		if (solutions.imaginary_term3_factor[index] == -1)
			std::cout << "-";
		else if (solutions.imaginary_term3_factor[index] != 1)
			std::cout << solutions.imaginary_term3_factor[index];
		if (reduce_sqrt)
			std::cout << "√" << solutions.sqrt_imaginary;
		else
			std::cout << "√(" << sqrt_imaginary << ")";
	}
	std::cout << ")";
	if (solutions.imaginary_denominator > 1)
		std::cout << " / " << solutions.imaginary_denominator;
	return true;
}

void	print_solutions_reduce_nothing(t_quadratic_solutions &solutions, bool reduce_sqrt)
{
	bool	has_imaginary_part;

	std::cout << "S = {";
	print_real_part(solutions, 0, reduce_sqrt);
	has_imaginary_part = print_imaginary_part(solutions, 0, reduce_sqrt);
	std::cout << ", ";
	print_real_part(solutions, 1, reduce_sqrt);
	has_imaginary_part |= print_imaginary_part(solutions, 1, reduce_sqrt);
	std::cout << "} ∈ ";
	if (has_imaginary_part)
		std::cout << "ℂ" << std::endl;
	else
		std::cout << "ℝ" << std::endl;
}

static void	test(t_quadratic_solutions &solutions, bool reduce_sqrt = false)
{
	print_solutions_reduce_nothing(solutions, reduce_sqrt);
}

static void simplify_factors(t_quadratic_solutions &solutions)
{
	for (int i = 0; i < 2; i++)
	{
		if (!solutions.sqrt_real)
		{
			solutions.real_term2_factor[i] = 0;
			solutions.imaginary_term2_factor[i] = 0;
		}
		if (!solutions.sqrt_imaginary)
		{
			solutions.real_term3_factor[i] = 0;
			solutions.imaginary_term3_factor[i] = 0;
		}
		if (!solutions.real_term2_factor[i] || solutions.sqrt_real <= 1)
		{
			solutions.real_term1[i] += solutions.real_term2_factor[i] * solutions.sqrt_real;
			solutions.real_term2_factor[i] = 0;
		}
		if (!solutions.real_term3_factor[i] || solutions.sqrt_imaginary <= 1)
		{
			solutions.real_term1[i] += solutions.real_term3_factor[i] * solutions.sqrt_imaginary;
			solutions.real_term3_factor[i] = 0;
		}
		if (!solutions.imaginary_term2_factor[i] || solutions.sqrt_real <= 1)
		{
			solutions.imaginary_term1[i] += solutions.imaginary_term2_factor[i] * solutions.sqrt_real;
			solutions.imaginary_term2_factor[i] = 0;
		}
		if (!solutions.imaginary_term3_factor[i] || solutions.sqrt_imaginary <= 1)
		{
			solutions.imaginary_term1[i] += solutions.imaginary_term3_factor[i] * solutions.sqrt_imaginary;
			solutions.imaginary_term3_factor[i] = 0;
		}
		if (solutions.sqrt_real == solutions.sqrt_imaginary)
		{
			solutions.real_term2_factor[i] += solutions.real_term3_factor[i];
			solutions.real_term3_factor[i] = 0;
			solutions.imaginary_term2_factor[i] += solutions.imaginary_term3_factor[i];
			solutions.imaginary_term3_factor[i] = 0;
		}
	}
}

static void simplify_factors2(t_quadratic_solutions &solutions)
{
	for (int i = 0; i < 2; i++)
	{
		if (!solutions.sqrt_term2_real)
		{
			solutions.real_term2_factor[i] += solutions.real_term3_factor[i];
			solutions.real_term3_factor[i] = 0;
			solutions.imaginary_term2_factor[i] += solutions.imaginary_term3_factor[i];
			solutions.imaginary_term3_factor[i] = 0;
		}
	}
}

static void	print_rounded_solutions(t_quadratic_solutions &solutions, bool reduce_sqrt)
{
	double	sqrt_real;
	double	sqrt_imaginary;
	double	term1;
	double	x_real[2];
	double	x_imaginary[2];

	if (reduce_sqrt)
	{
		sqrt_real = std::sqrt(solutions.sqrt_real);
		sqrt_imaginary = std::sqrt(solutions.sqrt_imaginary);
	}
	else
	{
		term1 = solutions.sqrt_term1_factor * std::sqrt(solutions.sqrt_term1_sqrt);
		sqrt_real = std::sqrt(term1 + solutions.sqrt_term2_real);
		sqrt_imaginary = std::sqrt(term1 + solutions.sqrt_term2_imaginary);
	}
	for (int i = 0; i < 2; i++)
	{
		x_real[i] = (solutions.real_term1[i] + solutions.real_term2_factor[i] * sqrt_real + solutions.real_term3_factor[i] * sqrt_imaginary) / solutions.real_denominator;
		x_imaginary[i] = (solutions.imaginary_term1[i] + solutions.imaginary_term2_factor[i] * sqrt_real + solutions.imaginary_term3_factor[i] * sqrt_imaginary) / solutions.imaginary_denominator;
	}
	if ((x_imaginary[0] == 0 && x_imaginary[1] == 0 && x_real[0] > x_real[1]) || (x_imaginary[0] != 0 && x_imaginary[1] != 0 && x_real[0] * x_real[0] + x_imaginary[0] * x_imaginary[0] > x_real[1] * x_real[1] + x_imaginary[1] * x_imaginary[1]))
	{
		std::swap(x_real[0], x_real[1]);
		std::swap(x_imaginary[0], x_imaginary[1]);
	}
	std::cout << COLOR_DIM;
	for (int i = 0; i < 2; i++)
	{
		std::cout << "x" << (i + 1);
		if (x_real[i] == int(x_real[i]) && x_imaginary[i] == int(x_imaginary[i]))
			std::cout << " = ";
		else
			std::cout << " ≈ ";
		std::cout << x_real[i];
		if (x_imaginary[i] > 0)
			std::cout << " + " << x_imaginary[i] << "i";
		else if (x_imaginary[i] < 0)
			std::cout << " - " << -x_imaginary[i] << "i";
		std::cout << std::endl;
	}
	std::cout << COLOR_RESET;
}

static void	print_solutions(t_quadratic_solutions& solutions)
{
	bool	reduce_sqrt;

	if (!solutions.sqrt_term1_factor || solutions.sqrt_term1_sqrt <= 1)
	{
		simplify_deepest_sqrt(solutions);
		simplify_factors(solutions);
		reduce_sqrt = true;
	}
	else
	{
		simplify_factors2(solutions);
		reduce_sqrt = false;
	}
	std::cout << COLOR_GREEN;
	test(solutions, reduce_sqrt);
	std::cout << COLOR_RESET;
	print_rounded_solutions(solutions, reduce_sqrt);
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
	find_solutions(a, b, discriminant);
	delete discriminant;
}
