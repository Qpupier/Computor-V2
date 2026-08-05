/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v1_bonus.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 13:06:18 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/05 13:23:17 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "quadratic.hpp"

void					find_solutions(										\
		const std::vector<Complex*> &coefficients, std::string var, 		\
		t_data &data)
{
	t_quadratic_solutions	solutions_structure;
	std::string				set;

	solutions_structure = get_solutions_structure(coefficients[0], 	\
			coefficients[1], coefficients[3]);
	set = *coefficients[0]->getImaginary() 							\
				|| *coefficients[1]->getImaginary() 				\
				|| *coefficients[2]->getImaginary() 				\
				|| *coefficients[3]->getImaginary() 				\
			? "ℂ" : "ℝ";
	print_solutions(solutions_structure, set, var, data);
}

std::vector<Complex*>	cast_coefficients_in_complex(const IType* tmp_a, 	\
		const IType* tmp_b, const IType* tmp_c, const IType* tmp_delta)
{
	Complex*	a;
	Complex*	b;
	Complex*	c;
	Complex*	delta;

	a = nullptr;
	b = nullptr;
	c = nullptr;
	try
	{
		a = new Complex(*tmp_a);
		b = new Complex(*tmp_b);
		c = new Complex(*tmp_c);
		delta = new Complex(*tmp_delta);
	}
	catch (const UnexpectedError &e)
	{
		delete a;
		delete b;
		delete c;
		throw UnsupportedError("Solutions can only be found in ℝ or ℂ");
	}
	return (std::vector<Complex*>{a, b, c, delta});
}

void					reduce_sqrt(InfiniteInt *factor, InfiniteInt *sqrt)
{
	InfiniteFloat	sqrt_rounded;
	InfiniteInt		perfect_square;

	sqrt_rounded = InfiniteFloat(*sqrt).sqrt();
	for (InfiniteInt i(2); InfiniteFloat(i) <= sqrt_rounded; i++)
	{
		perfect_square = i * i;
		while (!(*sqrt % perfect_square))
		{
			*sqrt /= perfect_square;
			*factor *= i;
		}
	}
}
