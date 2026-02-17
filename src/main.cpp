/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/17 16:58:32 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ast.hpp"

void	stored_variables(void)
{
	std::cerr << "\033[33mListing stored variables\033[0m" << std::endl;
}

void	compute_equation(const std::string &line, const std::map<std::string, std::regex> &patterns, const std::map<const Token::t_token, std::regex> &tokens_types)
{
	std::size_t	pos;

	pos = line.find('=');
	compute_expression(line.substr(0, pos), patterns, tokens_types);
	compute_expression(line.substr(pos + 1), patterns, tokens_types);
}

void	parse_line(const std::string &line, const std::map<std::string, std::regex> &patterns, const std::map<const Token::t_token, std::regex> &tokens_types)
{
	long int	nb_equal;

	nb_equal = std::count(line.begin(), line.end(), '=');
	if (nb_equal > 1)
		throw std::logic_error("Too many '=' in the expression");
	if (std::count(line.begin(), line.end(), '?') > 1)
		throw std::logic_error("Too many '?' in the expression");
	if (!std::regex_match(line, patterns.at(TOKEN_FULL)))
		throw std::logic_error(ERROR_INVALID_EXPRESSION);
	if (nb_equal)
		compute_equation(line, patterns, tokens_types);
	else if (line.find('?') != std::string::npos)
		stored_variables();
	else
		compute_expression(line, patterns, tokens_types);
}

void	compute_line(const std::string &line, const std::map<std::string, std::regex> &patterns, const std::map<const Token::t_token, std::regex> &tokens_types)
{
	if (line.empty())
		return;
	if (std::regex_match(line, patterns.at(TOKEN_LIST)))
		return stored_variables();
	try
	{
		parse_line(line, patterns, tokens_types);
		std::cout << "\033[32m  " << line << "\033[0m" << std::endl;
	}
	catch(const std::regex_error& e)
	{
		std::cerr << "\033[33mRegex error: " << e.what() << "\033[0m" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[31m" << e.what() << "\033[0m" << std::endl;
	}
}

int	main(int argc, const char **argv)
{
	std::map<std::string, std::regex>			patterns;
	std::map<const Token::t_token, std::regex>	tokens_types;

	if (argc > 1)
	{
		std::cerr << "Usage: ./computor-v2" << std::endl;
		return (EXIT_FAILURE);
	}
	patterns[TOKEN_FULL] = std::regex(TOKEN_FULL);
	patterns[TOKEN_LIST] = std::regex(TOKEN_LIST);
	patterns[TOKEN_NEXT] = std::regex(TOKEN_NEXT);
	patterns[TOKEN_FULL_EXPRESSION] = std::regex(TOKEN_FULL_EXPRESSION);
	define_token_types(tokens_types);
	while (true)
	{
		std::string	line;

		std::cout << "> ";
		std::getline(std::cin, line);
		if (line == "quit")
			break;
		compute_line(line, patterns, tokens_types);
	}
	(void)argv;
	return (EXIT_SUCCESS);
}
