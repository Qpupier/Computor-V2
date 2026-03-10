/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnsupportedError.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:50:43 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/09 17:46:16 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNSUPPORTEDERROR_HPP
# define UNSUPPORTEDERROR_HPP

# include <exception>

# include "computor-v2.hpp"

class UnsupportedError: public std::exception
{
	public:
		UnsupportedError(const std::string &message): _message(message) {}
		virtual const char* what() const noexcept override;

	private:
		std::string _message;
};

#endif
