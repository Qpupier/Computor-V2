/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteInt.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:39:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/15 18:24:39 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#ifndef INFINITEINT_HPP
# define INFINITEINT_HPP

// # include "computor-v2.hpp"
// # include <string>
# include <algorithm>
# include <vector>

class	InfiniteInt
{
	public:
		// Constructors and destructor
		InfiniteInt(void): _digits(1), _is_negative(false) {};
		InfiniteInt(const std::vector<unsigned short int> &digits, 	\
					bool is_negative = false): 							\
				_digits(digits), _is_negative(is_negative) {};
		InfiniteInt(const InfiniteInt &other): _digits(other._digits), 	\
				_is_negative(other._is_negative) {};
		~InfiniteInt(void) {};

		// Operator overloads
		InfiniteInt&	operator=(const InfiniteInt &other);
		explicit		operator bool() const;
		bool			operator==(const InfiniteInt &other) const;
		bool			operator!=(const InfiniteInt&other) const;
		bool			operator<(const InfiniteInt &other) const;
		bool			operator<=(const InfiniteInt &other) const;
		bool			operator>(const InfiniteInt &other) const;
		bool			operator>=(const InfiniteInt &other) const;
		InfiniteInt*	operator+(const InfiniteInt &other) const;
		InfiniteInt*	operator-(void) const;
		InfiniteInt*	operator-(const InfiniteInt &other) const;
		InfiniteInt*	operator*(const InfiniteInt &other) const;
		InfiniteInt*	operator/(const InfiniteInt &other) const;
		InfiniteInt*	operator%(const InfiniteInt &other) const;
		InfiniteInt*	operator^(const InfiniteInt &other) const;

		// Getters
		std::vector<unsigned short int>	getDigits(void) const;
		bool							getIsNegative(void) const;

		// Methods
		InfiniteInt*	clone(void) const;
		void			push_back(unsigned short int digit);
		void			reverse(void);
		// std::ostream&	print(std::ostream &os) const;

	private:
		// Members
		std::vector<unsigned short int>	_digits;
		bool							_is_negative;
};

// Output stream operator overload
// std::ostream&	operator<<(std::ostream &os, const Rational &num);

#endif
