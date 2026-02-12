/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:44:27 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/12 18:40:08 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "computor-v2.hpp"

void	stored_variables(void)
{
	std::cerr << "\033[33mListing stored variables\033[0m" << std::endl;
}

void	compute_equation(const std::string &line)
{
	std::cerr << "\033[33mComputing equation: " << line << "\033[0m" << std::endl;
}

static t_token get_token_type(const std::string &token)
{
	// std::cout << token << std::endl;
	if (std::regex_match(token, std::regex("^" TOKEN_VARIABLE "$")))
		return (E_VARIABLE);
	if (std::regex_match(token, std::regex("^" TOKEN_RATIONAL "$")))
		return (E_RATIONAL);
	if (std::regex_match(token, std::regex("^" TOKEN_IMAGINARY "$")))
		return (E_IMAGINARY);
	if (std::regex_match(token, std::regex("^" TOKEN_OPERATOR "$")))
		return (E_OPERATOR);
	if (std::regex_match(token, std::regex("^" TOKEN_MATRIX "$")))
		return (E_MATRIX);
	if (std::regex_match(token, std::regex("^" TOKEN_WHITESPACE "$")))
		return (E_WHITESPACE);
	if (token == "(")
		return (E_LEFT_PARENTHESIS);
	if (token == ")")
		return (E_RIGHT_PARENTHESIS);
	return (E_ERROR);
}

static void	semantic_verification(const std::vector<std::string> &tokens)
{
	for (size_t i = 1; i < tokens.size(); i++)
	{
		switch (get_token_type(tokens[i]))
		{
			case E_VARIABLE:
				/* code */
				break;
			
			default:
				break;
		}
	}
}

void test_bounds(const std::vector<std::string> &tokens)
{
	if (get_token_type(tokens[0]) == E_OPERATOR || get_token_type(tokens[tokens.size() - 1]) == E_OPERATOR)
		throw std::logic_error("Expression cannot start or end with an operator");
}

void remove_whitespaces(std::vector<std::string> &tokens)
{
	for (size_t i = 0; i < tokens.size();)
		if (get_token_type(tokens[i]) == E_WHITESPACE)
			tokens.erase(tokens.begin() + static_cast<long>(i));
		else
			i++;
}

void	test_whitespaces(const std::vector<std::string> &tokens)
{
	for (size_t i = 0; i < tokens.size(); i++)
		if (get_token_type(tokens[i]) == E_WHITESPACE					\
				&& !((i && get_token_type(tokens[i - 1]) == E_OPERATOR)	\
					^ (i < tokens.size() - 1 && get_token_type(tokens[i + 1]) == E_OPERATOR)))
			throw std::logic_error("Invalid expression format");
}

void	compute_expression(const std::string &line)
{
	std::string::const_iterator	start(line.begin());
	std::regex					re(TOKEN_NEXT);
	std::vector<int>			which = {1, 2, 3, 4};
	std::vector<std::string>	tokens;

	if (!std::regex_match(line, std::regex(TOKEN_FULL_EXPRESSION)))
		throw std::logic_error("Invalid expression format");
	std::cerr << "\033[33mComputing expression: " << line << "\033[0m" << std::endl;
	for (; start != line.end();)
	{
		std::sregex_token_iterator	it(start, line.end(), re, which);

		start = it->second;
		tokens.push_back(*it);
	}
	test_whitespaces(tokens);
	remove_whitespaces(tokens);
	test_bounds(tokens);
	semantic_verification(tokens);
	std::cout << "Tokens: "; for (size_t i = 0; i < tokens.size(); i++) std::cout << "\033[30m[\033[32m" << tokens[i] << "\033[30m]\033[0m"; std::cout << std::endl;
}

void	parse_line(const std::string &line)
{
	long int	nb_equal;

	nb_equal = std::count(line.begin(), line.end(), '=');
	if (nb_equal > 1)
		throw std::logic_error("Too many '=' in the expression");
	if (std::count(line.begin(), line.end(), '?') > 1)
		throw std::logic_error("Too many '?' in the expression");
	if (!std::regex_match(line, std::regex(TOKEN_FULL)))
		throw std::logic_error("Invalid expression format");
	if (nb_equal)
		compute_equation(line);
	else if (line.find('?') != std::string::npos)
		stored_variables();
	else
		compute_expression(line);
}

void	compute_line(const std::string &line)
{
	if (line.empty())
		return;
	if (std::regex_match(line, std::regex(TOKEN_LIST)))
		return stored_variables();
	try
	{
		parse_line(line);
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
	if (argc > 1)
	{
		std::cerr << "Usage: ./computor-v2" << std::endl;
		return (1);
	}
	while (true)
	{
		std::string	line;

		std::cout << "> ";
		std::getline(std::cin, line);
		if (line == "quit")
			break;
		compute_line(line);
	}
	(void)argv;
	return (0);
}
