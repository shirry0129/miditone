
//
// 2018 (c) Nanami Yamamoto
// This class is used to treat the decimal as a fraction.
//



#ifndef _FRACTION_HPP_
#define _FRACTION_HPP_

#include <string>
#include <exception>
#include <iostream>

namespace math {

	using int_type = int64_t;

	struct frac_t {
		int_type n, d;
	};


	struct Fraction {

	public:
		Fraction();
		Fraction(int_type n, int_type d = 1);
		~Fraction();

		Fraction &set(int_type n, int_type d = 1);
		frac_t get() const { return { numer, denom }; }

		float to_f() const { return static_cast<float>(numer)/denom; }
		int_type to_i() const { return numer/denom; }

		std::string get_str() const { return std::to_string(numer) + '/' + std::to_string(denom); }

		

		void print();

		Fraction &reduce();

		Fraction operator+() const;
		Fraction operator-() const;
		Fraction &operator=(int_type R);
		Fraction &operator+=(const Fraction &R);
		Fraction &operator+=(int_type R);
		Fraction &operator-=(const Fraction &R);
		Fraction &operator-=(int_type R);
		Fraction &operator*=(const Fraction &R);
		Fraction &operator*=(int_type R);
		Fraction &operator/=(const Fraction &R);
		Fraction &operator/=(int_type R);

		explicit operator int_type() const noexcept { return to_i(); }
		explicit operator float() const noexcept { return to_f();}

	private:
		int_type numer;
		int_type denom;
	};


	int_type gcd(int_type a, int_type b);
	int_type lcm(int_type a, int_type b);

	void adjustDenom(Fraction &a, Fraction &b);


	template<class T1, class T2>
	bool operator==(const T1 &L, const T2 &R) {

		Fraction fracL(L);
		Fraction fracR(R);

		if (fracL.get().n == 0 && fracR.get().n == 0)
			return true;

		adjustDenom(fracL, fracR);

		return (fracL.get().n == fracR.get().n);
	}

	template<class T1, class T2>
	bool operator!=(const T1 &L, const T2 &R) {
		Fraction fracL(L);
		Fraction fracR(R);

		return !(fracL == fracR);
	}

	template<class T1, class T2>
	bool operator<(const T1 &L, const T2 &R) {
		Fraction fracL(L);
		Fraction fracR(R);

		adjustDenom(fracL, fracR);

		return (fracL.get().n < fracR.get().n);
	}


	template<class T1, class T2>
	bool operator>(const T1 &L, const T2 &R) {
		Fraction fracL(L);
		Fraction fracR(R);

		return fracR < fracL;
	}

	
	template<class T1, class T2>
	bool operator<=(const T1 &L, const T2 &R) {
		Fraction fracL(L);
		Fraction fracR(R);

		return !(fracL > fracR);
	}

	template<class T1, class T2>
	bool operator>=(const T1 &L, const T2 &R) {
		Fraction fracL(L);
		Fraction fracR(R);

		return !(fracL < fracR);
	}


	template<class T1, class T2>
	Fraction operator+(const T1 &L, const T2 &R) {
		Fraction fracL(L);
		Fraction fracR(R);

		adjustDenom(fracL, fracR);
		int_type ans_numer = fracL.get().n + fracR.get().n;

		return Fraction(ans_numer, fracL.get().d);
	}

	template<class T1, class T2>
	Fraction operator-(const T1 &L, const T2 &R) {
		Fraction fracL(L);
		Fraction fracR(R);

		adjustDenom(fracL, fracR);
		int_type ans_numer = fracL.get().n - fracR.get().n;

		return Fraction(ans_numer, fracL.get().d);
	}


	template<class T1, class T2>
	Fraction operator*(const T1 &L, const T2 &R) {
		Fraction fracL(L);
		Fraction fracR(R);

		int_type n = fracL.get().n * fracR.get().n;
		int_type d = fracL.get().d * fracR.get().d;

		return Fraction(n, d);
	}

	template<class T1, class T2>
	Fraction operator/(const T1 &L, const T2 &R) {
		Fraction fracL(L);
		Fraction fracR(R);

		try {

			if (fracR == 0)
				throw std::runtime_error("[class:Fraction] divide by zero");

		} catch (const std::exception& e) {
			std::cout << e.what();
			throw;
		}

		int_type n = fracL.get().n * fracR.get().d;
		int_type d = fracL.get().d * fracR.get().n;

		return Fraction(n, d);
	}

}


#endif // !_FRACTION_HPP_

