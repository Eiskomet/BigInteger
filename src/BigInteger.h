#pragma warning(disable:4996)
#ifndef COMET_BIG_INTRGER_H
#define COMET_BIG_INTRGER_H

#include<algorithm>
#include<complex>
#include<cmath>
#include<iostream>
#include<string>
#include<vector>


class BigInteger {
public:
	using Byte = char;
	using size_type = std::vector<Byte>::size_type;

	BigInteger();
	BigInteger(const int x);
	BigInteger(const unsigned int x);
	BigInteger(const long long x);
	BigInteger(const unsigned long long x);
	BigInteger(const double x);
	BigInteger(const std::string &x);



	friend std::istream& operator>>(std::istream& in, BigInteger& x);
	friend std::ostream& operator<<(std::ostream& out, const BigInteger& x);

	friend const bool operator==(const BigInteger& a, const BigInteger& b);
	friend const bool operator!=(const BigInteger& a, const BigInteger& b);

	friend const bool operator<(const BigInteger& a, const BigInteger& b);
	friend const bool operator<=(const BigInteger& a, const BigInteger& b);

	friend const bool operator>(const BigInteger& a, const BigInteger& b);
	friend const bool operator>=(const BigInteger& a, const BigInteger& b);

	friend const BigInteger operator+(const BigInteger& a, const BigInteger& b);
	friend const BigInteger operator-(const BigInteger& a, const BigInteger& b);

	friend const BigInteger operator*(const BigInteger& a, const BigInteger& b);

	friend const BigInteger operator/(const BigInteger& a, const BigInteger& b);
	friend const BigInteger operator%(const BigInteger& a, const BigInteger& b);


	BigInteger& operator+=(const BigInteger& x);
	BigInteger& operator-=(const BigInteger& x);
	BigInteger& operator*=(const BigInteger& x);
	BigInteger& operator/=(const BigInteger& x);
	BigInteger& operator%=(const BigInteger& x);

	const BigInteger operator-()const;

	BigInteger& operator++();
	const BigInteger operator++(int);

	BigInteger& operator--();
	const BigInteger operator--(int);



	const std::string to_string()const;

	const size_type size()const;

	//equal: 0 ; this greater: 1 ;this smaller: -1
	const int compare(const BigInteger& x)const;

	void abs();

private:
	using complex = std::complex<double>;


	template<typename T>
	void construct(T x);

	//equal: 0 ; this greater: 1 ;this smaller: -1
	int unsigned_cmp(const BigInteger& x)const;

	void unsigned_add(const BigInteger& x);

	void unsigned_sub(const BigInteger& x);

	void unsigned_mul(const BigInteger& x);

	void unsigned_div(const BigInteger& x);

	void unsigned_mod(const BigInteger& x);


	/*
	* 进行 FFT 和 IFFT 前的反置变换
	* 长度必须为 2 的幂
	*/
	void change(std::vector<complex> &y)const;
	/*
	* 进行 FFT
	* 长度必须为 2 的幂
	* on == 1 时是 DFT，on == -1 时是 IDFT
	*/
	void fft(std::vector<complex> &y, int on)const;

	/*
	* 求倒数
	* 长度必须为 2 的幂
	* 精度为 acc
	*/
	const std::vector<Byte> inverse(const size_t acc)const;

	/*
	* 除法微调
	* 除法由于精度问题可能会少一，通过该方法检测并补充
	* x=a/b ,y=(x+1)*b,if(y<=a)++x;
	*/
	void lack(const BigInteger& a, const BigInteger& b);

	std::vector<Byte> data_;
	bool isNegative_;
};



#endif
