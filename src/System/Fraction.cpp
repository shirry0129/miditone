#include "Fraction.hpp"


namespace math {

	
	

	Fraction::Fraction() {}

	Fraction::Fraction(int_type n, int_type d) {
		set(n, d);
	}



	Fraction &Fraction::set(int_type n, int_type d) {

		try {

			if (d == 0)
				throw std::runtime_error("[class:Fraction] the denominator is zero");

		} catch (const std::exception& e) {
			std::cout << e.what();
			throw;
		}


		if (d < 0) {
			n *= -1;
			d *= -1;
		}

		numer = n;
		denom = d;

		return *this;
	}

	void Fraction::print() {
		std::cout << numer << '/' << denom;
	}

	Fraction &Fraction::reduce() {
		if (numer != 0) {
			int_type lcm_frac = gcd(denom, numer);
			denom /= lcm_frac;
			numer /= lcm_frac;
		}

		return *this;
	}

	Fraction Fraction::operator+() const {
		return *this;
	}

	Fraction Fraction::operator-() const {
		Fraction frac(-numer, denom);
		return frac;
	}


	Fraction & Fraction::operator=(int_type R) {
		return set(R);
	}


	Fraction & Fraction::operator+=(const Fraction & R) {
		(*this) = (*this) + R;

		return *this;
	}

	Fraction & Fraction::operator+=(int_type R) {
		return this->operator+=(Fraction(R));
	}

	Fraction & Fraction::operator-=(const Fraction & R) {
		(*this) = (*this) - R;

		return *this;
	}

	Fraction & Fraction::operator-=(int_type R) {
		return this->operator-=(Fraction(R));
	}

	Fraction & Fraction::operator*=(const Fraction & R) {
		(*this) = (*this) * R;

		return *this;
	}

	Fraction & Fraction::operator*=(int_type R) {
		return this->operator*=(Fraction(R));
	}

	Fraction & Fraction::operator/=(const Fraction & R) {
		(*this) = (*this) / R;

		return *this;
	}

	Fraction & Fraction::operator/=(int_type R) {
		return this->operator/=(Fraction(R));
	}


	int_type gcd(int_type a, int_type b) {
		if (a == 0 || b == 0)
			return 0;

		if (a < b) {
			int_type tmp = a;
			a = b;
			b = tmp;
		}

		int_type r = a%b;
		while (r != 0) {
			int_type tmp = b%r;
			b = r;
			r = tmp;
		}

		return b;
	}

	int_type lcm(int_type a, int_type b) {
		return  a * b / gcd(a, b);
	}

	void adjustDenom(Fraction & a, Fraction & b) {	

		int_type ans_denom = lcm(a.get().d, b.get().d);

		int_type a_numer = (ans_denom/a.get().d)*a.get().n;
		int_type b_numer = (ans_denom/b.get().d)*b.get().n;

		a.set(a_numer, ans_denom);
		b.set(b_numer, ans_denom);
	}




	Fraction::~Fraction() {}


}
