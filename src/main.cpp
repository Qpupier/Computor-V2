/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/21 13:46:49 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Rational.hpp"
#include "Real.hpp"

static void	free_stored(const 	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>	\
			::const_iterator	it(stored.begin());

	while (it != stored.end())
	{
		delete it->second;
		it++;
	}
}

int			main(int argc, const char **argv)
{
	t_data		data;
	int			status;
	Rational*	rational;

	if (argc > 1)
		return (usage());
	define_patterns(data.patterns);
	define_token_types(data.tokens_types);
	rational = new Rational(PRESET_PI);
	data.stored[std::pair<std::string, std::string>("pi", std::string())] 	\
			= new Real(*rational);
	delete rational;
	rational = new Rational(PRESET_E);
	data.stored[std::pair<std::string, std::string>("e", std::string())] 	\
			= new Real(*rational);
	delete rational;
	status = loop(data, isatty(STDIN_FILENO));
	free_stored(data.stored);
	return (status);
	(void)argv;
}
