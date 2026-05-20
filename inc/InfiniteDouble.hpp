/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteDouble.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:38:24 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/20 22:12:29 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFINITEDOUBLE_HPP
# define INFINITEDOUBLE_HPP

# include "InfiniteInt.hpp"

class	InfiniteDouble
{
	public:
		// Constructors and destructor
		InfiniteDouble(const InfiniteInt &integer_part, 					\
				const InfiniteInt &decimal_part = InfiniteInt(0, false), 	\
				bool is_negative = false, bool is_decimal_infinite = false);
		InfiniteDouble(void): InfiniteDouble(InfiniteInt()) {};
		InfiniteDouble(const unsigned long long int value, 					\
				bool is_integer_part = true): 								\
					InfiniteDouble(InfiniteInt(value, is_integer_part)) {};
		InfiniteDouble(const InfiniteDouble &other): 						\
				InfiniteDouble(other.getIntegerPart(), 						\
				other.getDecimalPart(), other.getIsNegative(), 				\
				other.getIsDecimalInfinite()) {};
		InfiniteDouble(const std::string integer_part, 						\
				const std::string decimal_part = "0", 						\
				bool is_negative = false): 									\
					InfiniteDouble(InfiniteInt(integer_part, false), 		\
						InfiniteInt(decimal_part, false, false), 			\
						is_negative) {};
		~InfiniteDouble(void) {};

		// Operator overloads
		explicit			operator bool(void) const;
		InfiniteDouble&		operator=(const InfiniteDouble &other);
		bool				operator==(const InfiniteDouble &other) const;
		bool				operator!=(const InfiniteDouble&other) const;
		bool				operator<(const InfiniteDouble &other) const;
		bool				operator<=(const InfiniteDouble &other) const;
		bool				operator>(const InfiniteDouble &other) const;
		bool				operator>=(const InfiniteDouble &other) const;
		InfiniteDouble		operator+(const InfiniteDouble &other) const;
		void				operator+=(const InfiniteDouble &other);
		InfiniteDouble&		operator++(void);
		InfiniteDouble		operator++(int);
		InfiniteDouble		operator-(void) const;
		InfiniteDouble		operator-(const InfiniteDouble &other) const;
		void				operator-=(const InfiniteDouble &other);
		InfiniteDouble&		operator--(void);
		InfiniteDouble		operator--(int);
		InfiniteDouble		operator*(const InfiniteDouble &other) const;
		void				operator*=(const InfiniteDouble &other);
		InfiniteDouble		operator/(const InfiniteDouble &other) const;
		void				operator/=(const InfiniteDouble &other);
		InfiniteDouble		operator%(const InfiniteDouble &other) const;
		void				operator%=(const InfiniteDouble &other);
		InfiniteDouble		operator^(const InfiniteDouble &other) const;
		void				operator^=(const InfiniteDouble &other);

		// Getters
		InfiniteInt	getDecimalPart(void) const;
		InfiniteInt	getIntegerPart(void) const;
		bool		getIsDecimalInfinite(void) const;
		bool		getIsNegative(void) const;

		// Setters
		void	setDecimalPart(const InfiniteInt &decimal_part);
		void	setIntegerPart(const InfiniteInt &integer_part);
		void	setIsDecimalInfinite(bool is_decimal_infinite);
		void	setIsNegative(bool is_negative);

		// Methods
		void		push_back_decimal(unsigned char digit);
		void		push_back_integer(unsigned char digit);
		void		reduce(void);

		//Constants
		static constexpr unsigned char	MAX_PRECISION = 15;

	private:
		// Members
		InfiniteInt	_integer_part;
		InfiniteInt	_decimal_part;
		bool		_isDecimalInfinite;
		bool		_isNegative;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const InfiniteDouble &num);

#endif
