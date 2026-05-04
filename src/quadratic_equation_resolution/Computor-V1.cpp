/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor-V1.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 18:03:52 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/04 18:32:54 by qpupier          ###   ########lyon.fr   */
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
	std::cout << COLOR_PINK << "Δ = " << *discriminant << COLOR_RESET 	\
			<< std::endl;
	return (discriminant);
}

static void		find_solutions(IType *tmp_a, IType *tmp_b, 	\
		IType *tmp_delta, std::string var)
{
	Complex*				a;
	Complex*				b;
	Complex*				delta;
	t_quadratic_solutions	solutions_structure;
	std::string				set;

	a = nullptr;
	b = nullptr;
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
	set = delta->getReal() > Rational(0) && !delta->getImaginary() ? "ℝ" : "ℂ";
	print_solutions(solutions_structure, set, var);
}

void			reduce_sqrt(int *factor, int *sqrt)
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

void			solve_trinomial(Polynomial *polynomial)
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
	find_solutions(a, b, discriminant, polynomial->getName());
	delete discriminant;
}
