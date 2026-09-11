/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/11 18:58:06 by qpupier          ###   ########lyon.fr   */
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

static void	set_preset_constants(t_data &data)
{
	Rational*	rational;

	data.stored[std::pair<std::string, std::string>("pi", std::string())] 	\
			= get_pi();
	data.preset_constants.push_back("pi");
	rational = new Rational(PRESET_E);
	data.stored[std::pair<std::string, std::string>("e", std::string())] 	\
			= new Real(*rational);
	delete rational;
	data.preset_constants.push_back("e");
}

static void	set_preset_functions(t_data &data)
{
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("sqrt", "Square root"));
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("norm (or ||<name>||)", "Norm"));
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("abs (or |<name>|)", "Absolute"));
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("cos", "Cosine"));
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("sin", "Sine"));
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("tan", "Tangent"));
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("exp", "Exponential"));
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("rad", "Degree to radian"));
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("deg", "Radian to degree"));
	data.preset_functions.push_back(	\
			std::pair<std::string, std::string>("fact (or <name>!)", "Factorial"));
}

int			main(int argc, const char **argv)
{
	t_data	data;
	int		status;

	if (argc > 1)
		return (usage());
	define_patterns(data.patterns);
	define_token_types(data.tokens_types);
	set_preset_constants(data);
	set_preset_functions(data);
	status = loop(data, isatty(STDIN_FILENO));
	free_stored(data.stored);
	return (status);
	(void)argv;
}
