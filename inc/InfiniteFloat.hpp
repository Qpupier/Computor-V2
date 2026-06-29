/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteFloat.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:38:24 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/29 11:49:58 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFINITEFLOAT_HPP
# define INFINITEFLOAT_HPP

# include "InfiniteInt.hpp"

class	InfiniteFloat
{
	public:
		// Constructors and destructor
		InfiniteFloat(const InfiniteInt integer_part, 						\
				const InfiniteInt decimal_part = InfiniteInt(0, false), 	\
				bool is_negative = false);
		InfiniteFloat(void): InfiniteFloat(InfiniteInt()) {};
		InfiniteFloat(const std::vector<unsigned char> integer_digits, 		\
				const std::vector<unsigned char> decimal_digits 			\
					= std::vector<unsigned char>(), 						\
				bool is_negative = false): 									\
					InfiniteFloat(InfiniteInt(integer_digits), 				\
						InfiniteInt(decimal_digits, false, false), 			\
						is_negative) {};
		InfiniteFloat(const long long int value, 							\
				bool is_integer_part = true): 								\
					InfiniteFloat(InfiniteInt(value, is_integer_part)) {};
		InfiniteFloat(const InfiniteFloat &other): 							\
				InfiniteFloat(other.getIntegerPart(), 						\
				other.getDecimalPart(), other.getIsNegative()) {};
		InfiniteFloat(const std::string integer_part, 						\
				const std::string decimal_part = "0", 						\
				bool is_negative = false): 									\
					InfiniteFloat(InfiniteInt(integer_part, false), 		\
						InfiniteInt(decimal_part, false, false), 			\
						is_negative) {};
		~InfiniteFloat(void) {};

		// Operator overloads
		explicit			operator bool(void) const;
		InfiniteFloat&		operator=(const InfiniteFloat &other);
		InfiniteFloat&		operator=(const InfiniteInt &other);
		InfiniteFloat&		operator=(const long long int value);
		bool				operator==(const InfiniteFloat &other) const;
		bool				operator==(const InfiniteInt &other) const;
		bool				operator==(const long long int value) const;
		bool				operator!=(const InfiniteFloat&other) const;
		bool				operator!=(const InfiniteInt&other) const;
		bool				operator!=(const long long int value) const;
		bool				operator<(const InfiniteFloat &other) const;
		bool				operator<(const InfiniteInt &other) const;
		bool				operator<(const long long int value) const;
		bool				operator<=(const InfiniteFloat &other) const;
		bool				operator<=(const InfiniteInt &other) const;
		bool				operator<=(const long long int value) const;
		bool				operator>(const InfiniteFloat &other) const;
		bool				operator>(const InfiniteInt &other) const;
		bool				operator>(const long long int value) const;
		bool				operator>=(const InfiniteFloat &other) const;
		bool				operator>=(const InfiniteInt &other) const;
		bool				operator>=(const long long int value) const;
		InfiniteFloat		operator+(const InfiniteFloat &other) const;
		InfiniteFloat		operator+(const InfiniteInt &other) const;
		InfiniteFloat		operator+(const long long int value) const;
		// IType*				operator+(const IType &other) const;
		void				operator+=(const InfiniteFloat &other);
		void				operator+=(const InfiniteInt &other);
		void				operator+=(const long long int value);
		InfiniteFloat&		operator++(void);
		InfiniteFloat		operator++(int);
		InfiniteFloat		operator-(void) const;
		InfiniteFloat		operator-(const InfiniteFloat &other) const;
		InfiniteFloat		operator-(const InfiniteInt &other) const;
		InfiniteFloat		operator-(const long long int value) const;
		void				operator-=(const InfiniteFloat &other);
		void				operator-=(const InfiniteInt &other);
		void				operator-=(const long long int value);
		InfiniteFloat&		operator--(void);
		InfiniteFloat		operator--(int);
		InfiniteFloat		operator*(const InfiniteFloat &other) const;
		InfiniteFloat		operator*(const InfiniteInt &other) const;
		InfiniteFloat		operator*(const long long int value) const;
		void				operator*=(const InfiniteFloat &other);
		void				operator*=(const InfiniteInt &other);
		void				operator*=(const long long int value);
		InfiniteFloat		operator/(const InfiniteFloat &other) const;
		InfiniteFloat		operator/(const InfiniteInt &other) const;
		InfiniteFloat		operator/(const long long int value) const;
		void				operator/=(const InfiniteFloat &other);
		void				operator/=(const InfiniteInt &other);
		void				operator/=(const long long int value);
		InfiniteFloat		operator%(const InfiniteFloat &other) const;
		InfiniteFloat		operator%(const InfiniteInt &other) const;
		InfiniteFloat		operator%(const long long int value) const;
		void				operator%=(const InfiniteFloat &other);
		void				operator%=(const InfiniteInt &other);
		void				operator%=(const long long int value);
		InfiniteFloat		operator^(const InfiniteFloat &other) const;
		InfiniteFloat		operator^(const InfiniteInt &other) const;
		InfiniteFloat		operator^(const long long int value) const;
		void				operator^=(const InfiniteFloat &other);
		void				operator^=(const InfiniteInt &other);
		void				operator^=(const long long int value);

		// Getters
		InfiniteInt	getDecimalPart(void) const;
		InfiniteInt	getIntegerPart(void) const;
		bool		getIsNegative(void) const;

		// Setters
		void	setDecimalPart(const InfiniteInt &decimal_part);
		void	setIntegerPart(const InfiniteInt &integer_part);
		void	setIsNegative(bool is_negative);

		// Methods
		InfiniteFloat	sqrt(void) const;
		bool			in_D(void) const;
		void			push_back_decimal(unsigned char digit);
		void			push_back_integer(unsigned char digit);
		void			reduce(void);

		//Constants
		static constexpr unsigned char	MAX_PRECISION = 32;
		static constexpr unsigned char	PRECISION = 8;

	private:
		// Members
		InfiniteInt	_integer_part;
		InfiniteInt	_decimal_part;
		bool		_isNegative;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const InfiniteFloat &num);

#endif
