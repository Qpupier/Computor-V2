/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteInt.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:39:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/18 16:51:27 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#ifndef INFINITEINT_HPP
# define INFINITEINT_HPP

// # include "computor-v2.hpp"
# include <iostream>
# include <string>
# include <algorithm>
# include <vector>

class	InfiniteInt
{
	public:
		// Constructors and destructor
		InfiniteInt(const std::vector<unsigned char> &digits, 	\
				bool is_negative = false);
		InfiniteInt(void): InfiniteInt(std::vector<unsigned char>()) {};
		InfiniteInt(const InfiniteInt &other): 						\
				InfiniteInt(other.getDigits(), other.getIsNegative()) {};
		InfiniteInt(const std::string &str, bool is_negative = false);
		~InfiniteInt(void) {};

		// Operator overloads
		explicit			operator bool(void) const;
		InfiniteInt&		operator=(const InfiniteInt &other);
		const unsigned char	operator[](	\
				std::vector<unsigned char>::size_type index) const;
		unsigned char		operator[](	\
				std::vector<unsigned char>::size_type index);
		bool				operator==(const InfiniteInt &other) const;
		bool				operator!=(const InfiniteInt&other) const;
		bool				operator<(const InfiniteInt &other) const;
		bool				operator<=(const InfiniteInt &other) const;
		bool				operator>(const InfiniteInt &other) const;
		bool				operator>=(const InfiniteInt &other) const;
		InfiniteInt			operator+(const InfiniteInt &other) const;
		void				operator+=(const InfiniteInt &other);
		InfiniteInt			operator-(void) const;
		InfiniteInt			operator-(const InfiniteInt &other) const;
		void				operator-=(const InfiniteInt &other);
		InfiniteInt			operator*(const InfiniteInt &other) const;
		void				operator*=(const InfiniteInt &other);
		InfiniteInt			operator/(const InfiniteInt &other) const;
		void				operator/=(const InfiniteInt &other);
		InfiniteInt			operator%(const InfiniteInt &other) const;
		void				operator%=(const InfiniteInt &other);
		InfiniteInt			operator^(const InfiniteInt &other) const;
		void				operator^=(const InfiniteInt &other);

		// Getters
		std::vector<unsigned char>	getDigits(void) const;
		bool						getIsNegative(void) const;

		// Setters
		void	setIsNegative(bool is_negative);

		// Methods
		InfiniteInt	clone(void) const;
		void		reverse(void);
		void		reduce(void);
		void		push_back(unsigned char digit);

	private:
		// Members
		std::vector<unsigned char>	_digits;
		bool						_is_negative;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const InfiniteInt &num);

#endif
