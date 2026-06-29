/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor-v1.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:03:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/29 10:31:50 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Polynomial.hpp"
#include "quadratic.hpp"

static IType*					get_discriminant(const IType *a, 	\
		const IType *b, const IType *c)
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

static void						find_solutions(							\
		const std::vector<Complex*> &coefficients, std::string var, 	\
		t_data &data)
{
	t_quadratic_solutions	solutions_structure;
	std::string				set;

	solutions_structure = get_solutions_structure(coefficients[0], 	\
			coefficients[1], coefficients[3]);
	set = coefficients[0]->getImaginary() 							\
				|| coefficients[1]->getImaginary() 					\
				|| coefficients[2]->getImaginary() 					\
				|| coefficients[3]->getImaginary() 					\
			? "ℂ" : "ℝ";
	delete coefficients[0];
	delete coefficients[1];
	delete coefficients[2];
	delete coefficients[3];
	print_solutions(solutions_structure, set, var, data);
}

static std::vector<Complex*>	cast_coefficients_in_complex(			\
		const IType* tmp_a, const IType* tmp_b, const IType* tmp_c, 	\
		const IType* tmp_delta)
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

void							reduce_sqrt(InfiniteInt *factor, 	\
		InfiniteInt *sqrt)
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

void							solve_trinomial(	\
		const Polynomial *polynomial, t_data &data)
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
	find_solutions(coefficients, polynomial->getName(), data);
	delete discriminant;
}
