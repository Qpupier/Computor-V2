/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rounded_solutions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 17:27:47 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/13 17:24:19 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

static void	sqrt_calculations(bool reduce_sqrt, 					\
		const t_quadratic_solutions &solutions, double &sqrt_real, 	\
		double &sqrt_imaginary)
{
	double	term1;

	if (reduce_sqrt)
	{
		sqrt_real = std::sqrt(solutions.sqrt_real);
		sqrt_imaginary = std::sqrt(solutions.sqrt_imaginary);
	}
	else
	{
		term1 = solutions.sqrt_term1_factor 	\
				* std::sqrt(solutions.sqrt_term1_sqrt);
		sqrt_real = std::sqrt(term1 + solutions.sqrt_term2_real);
		sqrt_imaginary = std::sqrt(term1 + solutions.sqrt_term2_imaginary);
	}
}

static void	rounded_calculations(const t_quadratic_solutions &solutions, 	\
		double sqrt_real, double sqrt_imaginary, double x_real[2], 			\
		double x_imaginary[2])
{
	for (int i = 0; i < 2; i++)
	{
		x_real[i] = (solutions.real_term1[i] 						\
				+ solutions.real_term2_factor[i] * sqrt_real 		\
				+ solutions.real_term3_factor[i] * sqrt_imaginary) 	\
				/ solutions.real_denominator[i];
		x_imaginary[i] = (solutions.imaginary_term1[i] 				\
				+ solutions.imaginary_term2_factor[i] * sqrt_real 	\
				+ solutions.imaginary_term3_factor[i] * sqrt_imaginary) \
				/ solutions.imaginary_denominator[i];
	}
	if ((x_imaginary[0] == 0 && x_imaginary[1] == 0 						\
				&& x_real[0] > x_real[1]) 									\
			|| (x_imaginary[0] != 0 && x_imaginary[1] != 0 					\
				&& x_real[0] * x_real[0] + x_imaginary[0] * x_imaginary[0] 	\
				> x_real[1] * x_real[1] + x_imaginary[1] * x_imaginary[1]))
	{
		std::swap(x_real[0], x_real[1]);
		std::swap(x_imaginary[0], x_imaginary[1]);
	}
}

static void	print_solution(std::string var, double x_real[2], 	\
		double x_imaginary[2], int i, bool print_solution_number)
{
	std::cout << var;
	if (print_solution_number)
		std::cout << i + 1;
	if (x_real[i] == int(x_real[i]) && x_imaginary[i] == int(x_imaginary[i]))
		std::cout << " = ";
	else
		std::cout << " ≈ ";
	if (x_real[i] != 0 || x_imaginary[i] == 0)
		std::cout << x_real[i];
	if (x_imaginary[i] > 0)
	{
		if (x_real[i] != 0)
			std::cout << " + ";
		std::cout << x_imaginary[i] << "i";
	}
	else if (x_imaginary[i] < 0)
	{
		if (x_real[i] != 0)
			std::cout << " - ";
		else
			std::cout << "-";
		std::cout << -x_imaginary[i] << "i";
	}
}

void		print_rounded_solutions(								\
		const t_quadratic_solutions &solutions, bool reduce_sqrt, 	\
		std::string var, int nb_solutions)
{
	double	sqrt_real;
	double	sqrt_imaginary;
	double	x_real[2];
	double	x_imaginary[2];

	sqrt_calculations(reduce_sqrt, solutions, sqrt_real, sqrt_imaginary);
	rounded_calculations(solutions, sqrt_real, sqrt_imaginary, 	\
			x_real, x_imaginary);
	if (nb_solutions == 2)
		std::cout << COLOR_DIM;
	else
		std::cout << COLOR_BOLD;
	print_solution(var, x_real, x_imaginary, 0, nb_solutions == 2);
	if (nb_solutions == 2)
	{
		std::cout << std::endl;
		print_solution(var, x_real, x_imaginary, 1, nb_solutions == 2);
	}
	std::cout << COLOR_RESET << std::endl;
}
