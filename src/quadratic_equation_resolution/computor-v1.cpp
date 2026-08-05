/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v1.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:03:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/05 18:45:20 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Polynomial.hpp"
#include "quadratic.hpp"

static IType*	get_discriminant(const IType *a, const IType *b, const IType *c)
{
	IType*	tmp_b2;
	IType*	tmp_4a;
	IType*	tmp_4ac;
	IType*	discriminant;

	tmp_b2 = *b * *b;
	tmp_4a = *a * 4;
	tmp_4ac = *tmp_4a * *c;
	delete tmp_4a;
	discriminant = *tmp_b2 - *tmp_4ac;
	delete tmp_b2;
	delete tmp_4ac;
	std::cout << COLOR_PINK << "Δ = " << *discriminant << COLOR_RESET 	\
			<< std::endl;
	return (discriminant);
}

static IType*	get_real_solution(const IType& neg_b, 					\
		const IType& sqrt_delta, const IType& denominator, bool positive = true)
{
	IType*	numerator;
	IType*	solution;

	numerator = positive ? neg_b + sqrt_delta : neg_b - sqrt_delta;
	solution = *numerator / denominator;
	delete numerator;
	return (solution);
}

static void		print_real_solutions(std::string var, 					\
		const IType& solution1, const IType& solution2, t_data &data)
{
	std::string 		result;
	unsigned short int	nb_solutions(2);

	if (solution1 == solution2)
		nb_solutions = 1;
	std::cout << COLOR_GREEN 											\
			<< (nb_solutions == 1 ? "One solution" : "Two solutions") 	\
			<< " in ℝ:" << COLOR_RESET << std::endl;
	std::cout << var << (nb_solutions > 1 ? "1" : "") << " ≈ " 			\
			<< solution1 << std::endl;
	if (nb_solutions == 2)
		std::cout << var << "2 ≈ " << solution2 << std::endl;
	result = std::string(COLOR_GREEN) + var + " ≈ " + solution1.to_string();
	if (nb_solutions == 2)
		result += " or " + var + " ≈ " + solution2.to_string();
	result += std::string(COLOR_RESET);
	data.history_results.push_back(result);
}

static void		find_real_solutions(									\
		const std::vector<Complex*> &coefficients, std::string var, 	\
		t_data &data)
{
	IType*	denominator;
	IType*	neg_b;
	IType*	solution1;
	IType*	solution2;
	IType*	sqrt_delta;

	neg_b = -*coefficients[1];
	sqrt_delta = coefficients[3]->sqrt();
	denominator = *coefficients[0] * 2;
	solution1 = get_real_solution(*neg_b, *sqrt_delta, *denominator, true);
	solution2 = get_real_solution(*neg_b, *sqrt_delta, *denominator, false);
	delete neg_b;
	delete sqrt_delta;
	delete denominator;
	print_real_solutions(var, *solution1, *solution2, data);
	delete solution1;
	delete solution2;
}

void			solve_trinomial(const Polynomial *polynomial, t_data &data)
{
	std::vector<Polynomial::t_term>	terms(polynomial->getTerms());
	std::vector<IType*>				tmp_coefficients(					\
			{terms[2].coefficient, terms[1].coefficient, terms[0].coefficient});
	std::vector<Complex*>			coefficients;
	IType*							discriminant;

	discriminant = get_discriminant(tmp_coefficients[0], 				\
			tmp_coefficients[1], tmp_coefficients[2]);
	coefficients = cast_coefficients_in_complex(tmp_coefficients[0], 	\
			tmp_coefficients[1], tmp_coefficients[2], discriminant);
	if (polynomial->in_Q())
		find_solutions(coefficients, polynomial->getName(), data);
	else
		find_real_solutions(coefficients, polynomial->getName(), data);
	delete coefficients[0];
	delete coefficients[1];
	delete coefficients[2];
	delete coefficients[3];
	delete discriminant;
}
