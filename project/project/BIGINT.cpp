#include <iostream>
#include <string>

using namespace std;

class BigInt {
public:
	BigInt() {
		len = 0;
		sign = '+';
		nums = nullptr;
	}
	BigInt(char sign_digit,const char* StringNum,unsigned int len){
		sign = sign_digit;
		this->len = len;
		int i = sizeof(char);
		nums = new char[len + 1];
		strcpy(nums, StringNum);
	}
	BigInt(const BigInt &bigint) {
		this->sign = bigint.sign;
		this->len = bigint.len;
		nums = new char[bigint.len + 1];
		strcpy(this->nums, bigint.nums);
	}
	~BigInt() {
		if (nums != nullptr)
			free(nums);
	}
	const BigInt ExternedTo(unsigned int newlen)const{
		std::string buf;
		int i = 0;
		for (; i < newlen - this->len; i++)
			buf.push_back('0');
		buf.append(this->nums);
		return BigInt(this->sign, buf.c_str(), newlen);
	}
	char operator [] (size_t pos) {
		if (nums != nullptr && pos < len)
			return nums[pos];
		else
			return NULL;
	}
	const BigInt operator = (const BigInt& bigint) {
		this->sign = bigint.sign;
		this->len = bigint.len;
		nums = new char[bigint.len + 1];
		strcpy(this->nums, bigint.nums);
		return *this;
	}
	const BigInt operator + (const BigInt& num)const {
		if (this->sign < num.sign) {//正数 + 负数 转为 正数 - 正数
			return *this - BigInt('+', num.nums, num.len);
		}
		else if (this->sign > num.sign) {//负数 + 正数 转为 正数 - 正数
			return num - BigInt('+', this->nums, this->len);
		}
		else if (this->sign == '-') {//负数相加转为正数相加取反
			BigInt sum = BigInt('+', this->nums, this->len) + BigInt('+', num.nums, num.len);
			sum.sign = '-';
			return sum;
		}
		//否则做两个正数相加
		unsigned int sumlen = this->len > num.len ? this->len : num.len;
		BigInt num1 = this->ExternedTo(sumlen);
		BigInt num2 = num.ExternedTo(sumlen);
		std::string sumbuf;
		sumbuf.resize(sumlen + 1,'0');//暂存和

		for (int i = sumlen - 1; i >= 0; i--) {
			int tmp = num1[i] + num2[i] + sumbuf[i + 1] - 144;
			if (tmp >= 10) {
				sumbuf[i] = '1';//进位
				tmp = tmp - 10;
			}
			else
				sumbuf[i] = '0';//进位
			sumbuf[i + 1] = (char)(tmp + 48);//和
		}
		int i = 0;
		while (sumbuf[i] == '0')
			i++;//使sumbuf+i指向第一个不为‘0’的元素
		
		return BigInt('+', sumbuf.c_str() + i, sumlen - i + 1);
	}
	const BigInt operator -(const BigInt& num)const{
		if (this->sign < num.sign) {//正数x - 负数y 转为 正数x + 正数y
			return *this + BigInt('+', num.nums, num.len);
		}
		else if (this->sign > num.sign) {//负数 - 正数 转为 -(正数 + 正数)
			BigInt diff = num + BigInt('+', this->nums, this->len);
			diff.sign = '-';
			return diff;
		}
		else if (this->sign == '-') {//负数 - 负数 转为 正数 - 正数
			return BigInt('+', num.nums, num.len) - BigInt('+', this->nums, this->len);
		}
		else if(this->len < num.len){//小正数 - 大正数 转为 -（大正数 - 小正数）
			BigInt diff = num - *this;
			diff.sign = '-';
			return diff;
		}
		else if (this->len == num.len) {
			for (int i = 0; i < this->len; i++) {
				if (this->nums[i] > num.nums[i]) {//小正数 - 大正数 转为 -（大正数 - 小正数）
					BigInt diff = num - *this;
					diff.sign = '-';
					return diff;
				}
			}
		}
		//否则做 大正数 - 小正数
		unsigned int sumlen = this->len > num.len ? this->len : num.len;
		BigInt num1 = this->ExternedTo(sumlen);
		BigInt num2 = num.ExternedTo(sumlen);
		std::string diffbuf;//暂存差
		diffbuf.resize(sumlen + 1, '0');

		for (int i = sumlen - 1; i >= 0; i--) {
			int tmp = num1[i] - num2[i] - diffbuf[i + 1] + 48;
			if (tmp < 0) {
				diffbuf[i] = '1';//借位位
				tmp = tmp + 10;
			}
			else
				diffbuf[i] = '0';//进位
			diffbuf[i + 1] = (char)(tmp + 48);//差
		}
		int i = 0;
		while (diffbuf[i] == '0')
			i++;//使diffbuf+i指向第一个不为‘0’的元素
		//BigInt diff('+', diffbuf.c_str() + i, sumlen - i + 1);
		return BigInt('+', diffbuf.c_str() + i, sumlen - i + 1);
	}
	const BigInt operator *(const BigInt& num)const {


		return num;
	}
	friend inline ostream &operator << (ostream& outstream, const BigInt& bigint);//输出流
	friend inline istream &operator >> (istream& instream, BigInt& bigint);//输入流
	char sign;
	char* nums;
	unsigned int  len;

};

inline ostream & operator<<(ostream & outstream, const BigInt& bigint)
{
	if (bigint.sign == '-')
		outstream << '-';
	if (bigint.nums != nullptr)
		outstream << bigint.nums;
	return outstream;
}

inline istream & operator>>(istream& instream, BigInt& bigint)
{
	std::string numstr;
	instream >> numstr;

	if(numstr.front() == '-'){
		bigint.sign = '-';
		bigint.len = numstr.size() - 1;
		if (bigint.nums != nullptr)
			free(bigint.nums);
		bigint.nums = new char[bigint.len + 1];
		strcpy(bigint.nums, numstr.c_str() + 1);
	}
	else if(numstr.front() == '+'){
		bigint.sign = '+';
		bigint.len = numstr.size() - 1;
		if (bigint.nums != nullptr)
			free(bigint.nums);
		bigint.nums = new char[bigint.len + 1];
		strcpy(bigint.nums, numstr.c_str() + 1);
	}
	else if(numstr.front() > '9' || numstr.front() < '0'){//如果第一个字符不是‘+’、‘-’或数字
		cout << "error:not decimal num!" << endl;
	}
	else{
		bigint.sign = '+';
		bigint.len = numstr.size();
		if (bigint.nums != nullptr)
			free(bigint.nums);
		bigint.nums = new char[bigint.len + 1];
		strcpy(bigint.nums, numstr.c_str());
	}
	return instream;
}


int main(){
	
	freopen("lab2_test.txt", "r", stdin);
	freopen("result.txt", "w", stdout);
	
	BigInt a, b, c, d;
	cin >> a >> b;
	c = a + b;
	d = a - b;
	cout << c << endl;
	cout << d << endl;
	return 0;
}

