

#include "BigInteger.h"


BigInteger::BigInteger() :isNegative_(false) {
	data_.emplace_back(0);
}

BigInteger::BigInteger(const int x) {
	construct(x);
}
BigInteger::BigInteger(const unsigned int x) {
	construct(x);
}
BigInteger::BigInteger(const long long x) {
	construct(x);
}

BigInteger::BigInteger(const unsigned long long x) {
	construct(x);
}

BigInteger::BigInteger(const double x) {
	char buff[310];
	double y = x;
	if (x > 0)y -= 0.5;
	else if (x < 0)y += 0.5;
	sprintf(buff, "%.0f", y);
	new(this) BigInteger(buff);
}

BigInteger::BigInteger(const std::string &x) {
	auto pos = x.size();
	decltype(pos) end = 0;
	if (x[0] == '-') {
		isNegative_ = true;
		end = 1;
	}
	else {
		isNegative_ = false;
	}

	while (x[end] == '0'&&end != pos - 1) ++end;

	while (pos-- != end) {

		if (!('0' <= x[pos] && x[pos] <= '9')) {
			throw std::invalid_argument("invalid BigInteger char*\n");
		}
		data_.emplace_back(x[pos] - '0');
	}
	if (data_.empty())data_.push_back(0);
	if (this->size() == 1 && this->data_.front() == 0)
		this->isNegative_ = false;
}



std::istream& operator>>(std::istream& in, BigInteger &x) {
	std::string tmp;
	in >> tmp;
	x = BigInteger(tmp);
	return in;
}


std::ostream& operator<<(std::ostream& out, const BigInteger& x) {
	return out << x.to_string();
}



const bool operator==(const BigInteger& a, const BigInteger& b) {
	return a.compare(b) == 0;
}
const bool operator!=(const BigInteger& a, const BigInteger& b) {
	return a.compare(b) != 0;
}

const bool operator<(const BigInteger& a, const BigInteger& b) {
	return a.compare(b) < 0;
}
const bool operator<=(const BigInteger& a, const BigInteger& b) {
	return a.compare(b) <= 0;
}

const bool operator>(const BigInteger& a, const BigInteger& b) {
	return a.compare(b) > 0;
}

const bool operator>=(const BigInteger& a, const BigInteger& b) {
	return a.compare(b) >= 0;
}


const BigInteger operator+(const BigInteger& a, const BigInteger& b) {
	BigInteger x;
	if (a.isNegative_ == b.isNegative_) {
		x = a;
		x.unsigned_add(b);
	}
	else {
		int flag = a.unsigned_cmp(b);
		if (flag == 0) {
			x = 0;
		}
		else if (flag == 1) {
			x = a;
			x.unsigned_sub(b);
		}
		else {
			x = b;
			x.unsigned_sub(a);
		}
	}
	return x;
}

const BigInteger operator-(const BigInteger& a, const BigInteger& b) {
	BigInteger x;
	if (a.isNegative_ != b.isNegative_) {
		x = a;
		x.unsigned_add(b);
	}
	else {
		int flag = a.unsigned_cmp(b);
		if (flag == 0) {
			x = 0;
		}
		else if (flag == 1) {
			x = a;
			x.unsigned_sub(b);
		}
		else {
			x = b;
			x.isNegative_ = !x.isNegative_;
			x.unsigned_sub(a);
		}
	}
	return x;
}

const BigInteger operator*(const BigInteger& a, const BigInteger& b) {
	BigInteger x(a);
	x.isNegative_ = a.isNegative_ ^ b.isNegative_;
	x.unsigned_mul(b);
	if (x.size() == 1 && x.data_.front() == 0)x.isNegative_ = false;
	return x;
}

const BigInteger operator/(const BigInteger& a, const BigInteger& b) {

	int flag = a.unsigned_cmp(b);
	if (flag < 0) {
		return 0;
	}
	else if (flag == 0) {
		return 1;
	}
	else {
		BigInteger tmp(a);
		tmp.unsigned_div(b);
		tmp.isNegative_ = a.isNegative_ ^ b.isNegative_;
		return tmp;
	}

}

const BigInteger operator%(const BigInteger& a, const BigInteger& b) {
	int flag = a.unsigned_cmp(b);
	if (flag < 0) {
		return a;
	}
	else if (flag == 0) {
		return 0;
	}
	else {
		BigInteger tmp(a);
		tmp.unsigned_mod(b);
		return tmp;
	}
}

BigInteger& BigInteger::operator+=(const BigInteger& x) {
	if (this->isNegative_ == x.isNegative_) {
		this->unsigned_add(x);
	}
	else {
		int flag = this->unsigned_cmp(x);
		if (flag == 0) {
			*this = 0;
		}
		else if (flag == 1) {
			this->unsigned_sub(x);
		}
		else {
			BigInteger tmp(std::move(*this));
			*this = x;
			this->unsigned_sub(tmp);
		}
	}
	return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& x) {
	if (this->isNegative_ != x.isNegative_) {
		this->unsigned_add(x);
	}
	else {
		int flag = this->unsigned_cmp(x);
		if (flag == 0) {
			*this = 0;
		}
		else if (flag == 1) {
			this->unsigned_sub(x);
		}
		else {
			BigInteger tmp(std::move(*this));
			*this = x;
			this->isNegative_ = !this->isNegative_;
			this->unsigned_sub(tmp);
		}
	}
	return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& x) {
	this->isNegative_ ^= x.isNegative_;
	this->unsigned_mul(x);
	if (this->size() == 1 && this->data_.front() == 0)
		this->isNegative_ = false;
	return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& x) {
	int flag = this->unsigned_cmp(x);
	if (flag < 0) {
		*this = 0;
	}
	else if (flag == 0) {
		this->isNegative_ ^= x.isNegative_;
		*this = 1;
	}
	else {
		this->isNegative_ ^= x.isNegative_;
		this->unsigned_div(x);
	}
	return *this;
}
BigInteger& BigInteger::operator%=(const BigInteger& x) {
	int flag = this->unsigned_cmp(x);
	if (flag == 0) {
		*this = 0;
	}
	else if (flag > 0) {
		this->unsigned_mod(x);
	}
	return *this;
}


const BigInteger BigInteger::operator-() const {
	BigInteger tmp(*this);
	if (tmp.data_.size() != 1 || tmp.data_.front() != 0)
		tmp.isNegative_ = !tmp.isNegative_;
	return tmp;
}

const std::string BigInteger::to_string()const {
	std::string s;
	if (isNegative_) {
		s.push_back('-');
	}
	for (auto it = data_.crbegin(); it != data_.crend(); ++it) {
		s.push_back(*it + '0');
	}
	return s;
}

BigInteger& BigInteger::operator++() {
	return *this += 1;;
}
const BigInteger BigInteger::operator++(int) {
	BigInteger tmp(*this);
	*this += 1;
	return tmp;
}

BigInteger& BigInteger::operator--() {
	return *this -= 1;;
}
const BigInteger BigInteger::operator--(int) {
	BigInteger tmp(*this);
	*this -= 1;
	return tmp;
}

const BigInteger::size_type BigInteger::size()const {
	return data_.size();
}

//equal: 0 ; this greater: 1 ;this smaller: -1
const int BigInteger::compare(const BigInteger& x) const {
	if (this->isNegative_ != x.isNegative_) {
		return x.isNegative_ ? 1 : -1;
	}
	int ans = this->unsigned_cmp(x);
	if (this->isNegative_) {
		ans = -ans;
	}
	return ans;
}

void BigInteger::abs() {
	this->isNegative_ = false;
}

template<typename T>
void BigInteger::construct(const T value) {
	static const char digits[] = { 9,8,7,6,5,4,3,2,1,0,1,2,3,4,5,6,7,8,9 };
	static const char* zero = digits + 9;
	T i = value;
	do {
		int lsd = static_cast<int>(i % 10);
		i /= 10;
		data_.emplace_back(zero[lsd]);
	} while (i != 0);

	this->isNegative_ = value < 0;

	if (data_.empty())data_.push_back(0);
}

//equal: 0 ; this greater: 1 ;this smaller: -1
int BigInteger::unsigned_cmp(const BigInteger& x)const {
	if (this->data_.size() != x.data_.size()) {
		return this->data_.size() > x.data_.size() ? 1 : -1;
	}
	auto it1 = this->data_.crbegin();
	auto it2 = x.data_.crbegin();
	while (it1 != this->data_.crend()) {
		if (*it1 != *it2) {
			return *it1 > *it2 ? 1 : -1;
		}
		++it1;
		++it2;
	}
	return 0;
}

void BigInteger::unsigned_add(const BigInteger& x) {
	long long carry = 0;

	while (this->data_.size() < x.data_.size())
		this->data_.emplace_back(0);
	auto it = this->data_.begin();
	for (auto xit = x.data_.cbegin(); xit != x.data_.cend(); ++xit, ++it) {
		*it += *xit + carry;
		carry = *it / 10;
		*it %= 10;
	}

	while (carry) {
		if (it == this->data_.end()) {
			this->data_.emplace_back(carry);
			break;
		}
		*it += carry;
		carry = *it / 10;
		*it %= 10;
		++it;
	}
}

void BigInteger::unsigned_sub(const BigInteger& x) {
	int borrow = 0;
	auto it = this->data_.begin();
	for (auto xit = x.data_.cbegin(); xit != x.data_.cend(); ++xit, ++it) {
		*it += 10 - *xit + borrow;
		borrow = *it / 10 - 1;
		*it %= 10;
	}
	while (borrow) {
		*it += 10 + borrow;
		borrow = *it / 10 - 1;
		*it %= 10;
		++it;
	}
	while (this->data_.size() != 1 && this->data_.back() == 0)
		this->data_.pop_back();
}

/*
* 基于FFT的无符号乘法
*/
void BigInteger::unsigned_mul(const BigInteger& x) {
	std::vector<Byte> &a = this->data_;
	const std::vector<Byte> &b = x.data_;
	size_type len = 1;
	//长度为 2 的次幂
	while (len < a.size() * 2 || len < b.size() * 2) len *= 2;

	// 将两个多项式乘法中的三次FFT变成两次优化
	// 原理是：(a+b i)^2= (a^2-b^2) + 2abi （i 为虚数）
	// FFT系数转点值乘法 转系数之后虚部除以2（乘0.5/len）即是结果
	std::vector<complex> x1;
	for (auto i = 0; i < a.size() && i < b.size(); ++i)
		x1.emplace_back(complex(a[i], b[i]));

	if (a.size() >= b.size()) {
		for (auto i = b.size(); i < a.size(); ++i)
			x1.emplace_back(complex(a[i], 0));
	}
	else {
		for (auto i = a.size(); i < b.size(); ++i)
			x1.emplace_back(complex(0, b[i]));
	}
	//填充
	for (auto i = std::max(a.size(), b.size()); i < len; ++i)
		x1.emplace_back(complex(0, 0));

	//系数表示法转换成点值表示
	fft(x1, 1);
	//实际的运算
	for (int i = 0; i < len; ++i)
		x1[i] = x1[i] * x1[i];
	//点值表示转回系数表示法
	fft(x1, -1);

	len = a.size() + b.size();
	a.assign(len, 0);

	long long carry = 0;
	for (int i = 0; i < len; ++i) {
		long long x = static_cast<long long>(x1[i].imag() / 2 + 0.5) + carry;
		a[i] = static_cast<Byte>(x % 10);
		carry = x / 10;

	}
	while (a.size() != 1 && a.back() == 0)a.pop_back();

}

/*
* 基于FFT的无符号除法
*/
void BigInteger::unsigned_div(const BigInteger& x) {

	size_t acc = this->size() - x.size() + 16;
	std::vector<Byte> tmp = x.inverse(acc);
	size_type len = 1;
	while (len < this->size() + acc) len *= 2;

	std::vector<complex> vc(len);
	auto it = data_.crbegin();
	for (int i = 0; it != data_.crend(); ++i, ++it) {
		vc[i].real(*it);
	}
	for (int i = 0; i < acc; ++i) {
		vc[i].imag(tmp[i]);
	}
	fft(vc, 1);
	for (int i = 0; i < len; ++i) {
		vc[i] = vc[i] * vc[i];
	}
	fft(vc, -1);

	tmp.clear();
	long long carry = 0;
	for (auto it = vc.crbegin(); it != vc.crend(); ++it) {
		long long x = static_cast<long long>(std::floor(it->imag() / 2 + 0.5)) + carry;
		carry = x / 10;
		int y = x % 10;
		if (y < 0) {
			y += 10;
			--carry;
		}
		tmp.emplace_back(y);

	}
	len = data_.size() - x.size();
	if (carry) {
		++len;
		tmp.emplace_back(static_cast<char>(carry));

	}

	BigInteger othis(*this);
	data_.clear();

	for (int i = 0; i < len; ++i) {
		data_.emplace_back(tmp[tmp.size() - len + i]);
	}
	this->lack(othis, x);
}

void BigInteger::unsigned_mod(const BigInteger& x) {
	BigInteger tmp(*this);
	tmp.unsigned_div(x);
	tmp.unsigned_mul(x);
	*this -= tmp;
}

/*
* 进行 FFT 和 IFFT 前的反置变换
* 长度必须为 2 的幂
*/
void BigInteger::change(std::vector<complex> &y)const {
	size_type i, j, k;
	size_type len = y.size();
	for (i = 1, j = len / 2; i < len - 1; ++i) {
		if (i < j) std::swap(y[i], y[j]);
		k = len / 2;
		while (j >= k) {
			j = j - k;
			k = k / 2;
		}
		if (j < k) j += k;
	}
}
/*
* 进行 FFT
* 长度必须为 2 的幂
* on == 1 时是 DFT，on == -1 时是 IDFT
*/
void BigInteger::fft(std::vector<complex> &y, int on) const {
	const double PI = acos(-1.0);
	change(y);
	for (int h = 2; h <= y.size(); h <<= 1) {
		complex wn(cos(2 * PI / h), sin(on * 2 * PI / h));
		for (int j = 0; j < y.size(); j += h) {
			complex w(1, 0);
			for (int k = j; k < j + h / 2; ++k) {
				complex u = y[k];
				complex t = w * y[k + h / 2];
				y[k] = u + t;
				y[k + h / 2] = u - t;
				w = w * wn;
			}
		}
	}
	if (on == -1) {
		for (int i = 0; i < y.size(); ++i) {
			y[i].real(y[i].real() / y.size());
			y[i].imag(y[i].imag() / y.size());

		}
	}
}


/*
* 求倒数
* 长度必须为 2 的幂
* 精度为 acc
*/
const std::vector<BigInteger::Byte> BigInteger::inverse(const size_t acc)const {

	double d = 0, e = 1;

	size_t nacc = std::min(data_.size(), static_cast<size_t>(8));
	for (int i = 0; i < nacc; ++i) {
		d = d + e * data_[data_.size() - i - 1];
		e *= 0.1;
	}
	d = 10. / d;
	nacc = 8;
	std::vector<Byte> tmp(nacc);
	for (int i = 0; i < nacc; ++i) {
		tmp[i] = static_cast<char>(d);
		d = (d - tmp[i]) * 10;
	}

	std::vector<complex> a;
	std::vector<complex> b;
	a.reserve(acc);
	b.reserve(acc);

	while (nacc < acc) {

		for (int i = 0; i < 2 * nacc; ++i) {
			if (data_.size() > i) {
				a.emplace_back(data_[data_.size() - i - 1]);
			}
			else {
				a.emplace_back(0);
			}
		}
		for (int i = 0; i < nacc; ++i) {
			b.emplace_back(tmp[i]);
		}

		while (a.size() < 4 * nacc)a.emplace_back(0);
		while (b.size() < 4 * nacc)b.emplace_back(0);

		fft(a, 1);
		fft(b, 1);
		for (int i = 0; i < a.size(); ++i) {
			a[i] = a[i] * b[i];
			a[i].real(20 - a[i].real());
			a[i].imag(-a[i].imag());
			b[i] = a[i] * b[i];
		}
		fft(b, -1);

		long long carry = 0;
		for (auto it = b.rbegin(); it != b.rend(); ++it) {
			long long x = static_cast<long long>(std::floor(it->real() + 0.5)) + carry;
			carry = x / 10;
			long long y = x % 10;
			if (y < 0) {
				y += 10;
				--carry;
			}

			it->real(y);
		}
		tmp.clear();
		if (carry) {
			tmp.emplace_back(static_cast<char>(carry));
			for (int i = 1; i < 2 * nacc; ++i)
				tmp.emplace_back(static_cast<char>(b[i - 1].real()));
		}
		else {
			for (int i = 0; i < 2 * nacc; ++i)
				tmp.emplace_back(static_cast<char>(b[i].real()));
		}

		a.clear();
		b.clear();
		nacc <<= 1;
	}

	return tmp;
}

/*
* 除法微调
* 除法由于精度问题可能会少一，通过该方法检测并补充
* x=a/b ,y=(x+1)*b,if(y<=a)++x;
*/
void BigInteger::lack(const BigInteger& a, const BigInteger& b) {
	BigInteger x(*this);
	x.unsigned_add(1);
	x.unsigned_mul(b);
	int flag = x.unsigned_cmp(a);
	if (flag <= 0) {
		this->unsigned_add(1);
	}
}





