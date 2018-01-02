#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

class BigInt {
public:
	BigInt() :sign('+'), isZero(true), len(0), nums(NULL) {}
	BigInt(char sign_digit,const char* StringNum,unsigned int len){
		sign = sign_digit;
		this->len = len;
		if(len == 0){
			isZero = true;
			nums = NULL;
		}
		else {
			isZero = false;
			nums = new char[len + 1];
			for (int i = 0; i < len; i++)
				nums[i] = StringNum[i];
			nums[len] = '\0';
		}
	}
	BigInt(const BigInt &bigint) {
		this->sign = bigint.sign;
		this->isZero = bigint.isZero;
		this->len = bigint.len;
		if (bigint.isZero == true) {
			nums = NULL;
			return;
		}
		nums = new char[bigint.len + 1];
		for (int i = 0; i < bigint.len; i++)
			nums[i] = bigint.nums[i];
		nums[len] = '\0';
	}
	~BigInt() {
		if (nums != NULL)
			free(nums);
	}
	const BigInt operator = (const BigInt& bigint) {
		this->sign = bigint.sign;
		this->isZero = bigint.isZero;
		this->len = bigint.len;
		if (this->nums != NULL)
			free(nums);
		if (bigint.isZero == true) {
			this->nums = NULL;
			return *this;
		}
		nums = new char[bigint.len + 1];
		for (int i = 0; i < bigint.len; i++)
			nums[i] = bigint.nums[i];
		nums[len] = '\0';
		return *this;
	}
	const BigInt ExternedTo(unsigned int newlen)const{
		std::string buf;
		buf.append(newlen - this->len, '0');
		buf.append(this->nums);
		return BigInt(this->sign, buf.c_str(), buf.size());
	}
	const char	operator [] (size_t pos)const{
		if (nums != NULL && pos < len)
			return nums[pos];
		else
			return 0;
	}
	const BigInt operator << (size_t shamt)const{
		if (this->isZero == true)
			return BigInt('+', "", 0);
		std::string buf;
		buf.append(this->nums);
		buf.append(shamt, '0');
		return BigInt(this->sign, buf.c_str(), buf.size());
	}
	const BigInt operator +	(const BigInt& num)const {
		if (num.isZero == true)
			return *this;
		else if (this->isZero == true) {
			return num;
		}
		if (this->sign < num.sign) {//���� + ���� תΪ ���� - ����
			return *this - BigInt('+', num.nums, num.len);
		}
		else if (this->sign > num.sign) {//���� + ���� תΪ ���� - ����
			return num - BigInt('+', this->nums, this->len);
		}
		else if (this->sign == '-') {//�������תΪ�������ȡ��
			BigInt sum = BigInt('+', this->nums, this->len) + BigInt('+', num.nums, num.len);
			sum.sign = '-';
			return sum;
		}
		//�����������������
		unsigned int sumlen = this->len > num.len ? this->len : num.len;
		BigInt num1 = this->ExternedTo(sumlen);
		BigInt num2 = num.ExternedTo(sumlen);
		std::string sumbuf;
		sumbuf.resize(sumlen + 1,'0');//�ݴ��

		for (int i = sumlen - 1; i >= 0; i--) {
			int tmp = num1[i] + num2[i] + sumbuf[i + 1] - 144;
			if (tmp >= 10) {
				sumbuf[i] = '1';//��λ
				tmp = tmp - 10;
			}
			else
				sumbuf[i] = '0';//��λ
			sumbuf[i + 1] = (char)(tmp + 48);//��
		}
		for (int i = 0; i < sumbuf.size(); i++) {
			if (sumbuf[i] != '0')
				return BigInt('+', sumbuf.c_str() + i, sumlen - i + 1);
		}
		return BigInt('+', "",0);

	}
	const BigInt operator -	(const BigInt& num)const{
		if (num.isZero == true)
			return *this;
		else if(this->isZero == true){
			if (num.sign == '+')
				return BigInt('-', num.nums, num.len);
			else
				return BigInt('+', num.nums, num.len);
		}
		if (this->sign < num.sign) {//����x - ����y תΪ ����x + ����y
			return *this + BigInt('+', num.nums, num.len);
		}
		else if (this->sign > num.sign) {//���� - ���� תΪ -(���� + ����)
			BigInt diff = num + BigInt('+', this->nums, this->len);
			diff.sign = '-';
			return diff;
		}
		else if (this->sign == '-') {//���� - ���� תΪ ���� - ����
			return BigInt('+', num.nums, num.len) - BigInt('+', this->nums, this->len);
		}
		else if(this->len < num.len){//С���� - ������ תΪ -�������� - С������
			BigInt diff = num - *this;
			diff.sign = '-';
			return diff;
		}
		else if (this->len == num.len) {
			for (int i = 0; i < this->len; i++) {
				if (this->nums[i] == num.nums[i])
					continue;
				else if (this->nums[i] < num.nums[i]) {//С���� - ������ תΪ -�������� - С������
					BigInt diff = num - *this;
					diff.sign = '-';
					return diff;
				}
				else
					break;
			}
		}
		//������ ������ - С����
		unsigned int difflen = this->len > num.len ? this->len : num.len;
		BigInt num1 = this->ExternedTo(difflen);
		BigInt num2 = num.ExternedTo(difflen);
		std::string diffbuf;//�ݴ��
		diffbuf.resize(difflen + 1, '0');

		for (int i = difflen - 1; i >= 0; i--) {
			int tmp = num1[i] - num2[i] - diffbuf[i + 1] + 48;
			if (tmp < 0) {
				diffbuf[i] = '1';//��λλ
				tmp = tmp + 10;
			}
			else
				diffbuf[i] = '0';//��λ
			diffbuf[i + 1] = (char)(tmp + 48);//��
		}
		
		for (int i = 0; i < diffbuf.size(); i++) {
			if (diffbuf[i] != '0')
				return BigInt('+', diffbuf.c_str() + i, difflen - i + 1);
		}
		return BigInt('+', "", 0);
	}
	const BigInt operator *	(const BigInt& num)const {
		char procuctsign = this->sign == num.sign ? '+' : '-';
		if (this->isZero == true || num.isZero == true)//�����һ������Ϊ0
			return BigInt(procuctsign, "", 0);//����0
		if (this->len < 10 && num.len < 10) {
			long long product = atoll(this->nums) * atoll(num.nums);
			char buffer[20] = "";
			sprintf(buffer, "%lld", product);
			return BigInt(procuctsign, buffer, strlen(buffer));//����С��9λ�����Ļ�
		}
		BigInt A, B, C, D;
		unsigned int biggerlen = this->len > num.len ? this->len : num.len;
		biggerlen += biggerlen % 2;//
		unsigned int factorlen = biggerlen / 2;

		BigInt num1 = this->ExternedTo(biggerlen);
		BigInt num2 = num.ExternedTo(biggerlen);
		if(this->len > factorlen)
			A = BigInt('+', num1.nums, factorlen);
		if (num.len > factorlen)
			C = BigInt('+', num2.nums, factorlen);
		B = BigInt('+', num1.nums + factorlen, factorlen);
		D = BigInt('+', num2.nums + factorlen, factorlen);
		BigInt AC = A * C;
		BigInt BD = B * D;
		BigInt A_B = A - B;
		BigInt D_C = D - C;
		BigInt A_B_D_C = A_B*D_C;
		BigInt product = (AC << 2 * factorlen) + ((A_B_D_C + AC + BD) << factorlen) + BD;
		product.sign = procuctsign;
		return product;

	}
	friend inline ostream &operator << (ostream& outstream, const BigInt& bigint);//�����
	friend inline istream &operator >> (istream& instream, BigInt& bigint);//������
public:
	char sign;
	bool isZero;
	unsigned int  len;
	char* nums;
};

inline ostream & operator<<(ostream & outstream, const BigInt& bigint)
{
	if (bigint.sign == '-')
		outstream << '-';
	if (bigint.isZero == true)
		outstream << '0';
	else if (bigint.nums != NULL)
		outstream << bigint.nums;
	return outstream;
}

inline istream & operator>>(istream& instream, BigInt& bigint)
{
	std::string numstr;
	instream >> numstr;
	int offset = 0;
	if(numstr.at(0) == '-' || numstr.at(0) == '+'){
		bigint.sign = numstr.at(0);
		offset = 1;
	}
	else
		bigint.sign = '+';
	for (int i = offset; i < numstr.size(); i++) {
		if (numstr[i] != '0') {
			bigint.isZero = false;
			bigint.len = numstr.size() - i;
			if (bigint.nums != NULL)
				free(bigint.nums);
			bigint.nums = new char[bigint.len + 1];
			for (int j = 0; j < bigint.len; j++)
				bigint.nums[j] = numstr[j + i];
			bigint.nums[bigint.len] = '\0';
			return instream;
		}
	}
	bigint.isZero = true;
	bigint.len = 0;
	if (bigint.nums != NULL)
		free(bigint.nums);
	bigint.nums = NULL;
	return instream;
}

int main(){
	std::ifstream cin("lab2_test.txt");
	if (0 == cin.is_open()) {
		cout << "�Ҳ����ļ�lab2_test.txt!" << endl;
		getchar();
		return 0;
	}
	BigInt a, b, result;
	cin >> a >> b;
	cout << "+ : " << a + b << endl;
	cout << "- : " << a - b << endl;
	cout << "* : " << a * b << endl << endl;

	getchar();
	return 0;
}

