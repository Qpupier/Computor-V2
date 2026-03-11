/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:40:40 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/11 13:36:32 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# include <iostream>
# include <exception>

class LogicError: public std::exception
{
	public:
		LogicError(const std::string &message): _message("\033[31m" + message + "\033[0m") {}
		virtual const char* what() const noexcept override;

	private:
		std::string _message;
};

class UnexpectedError: public std::exception
{
	public:
		UnexpectedError(const std::string &message): _message("\033[31mUnexpected error: " + message + "\033[0m") {}
		virtual const char* what() const noexcept override;

	private:
		std::string _message;
};

class UnsupportedError: public std::exception
{
	public:
		UnsupportedError(const std::string &message): _message("\033[33mUnsupported: " + message + "\033[0m") {}
		virtual const char* what() const noexcept override;

	private:
		std::string _message;
};

#endif
