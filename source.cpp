#include <iostream>
#include <math.h>
#include <omp.h>
#include <random>
#include <time.h>
#include <chrono>
using namespace std;

using ll = long long;

string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const int str_len = 6;

random_device rd;
mt19937_64 mt(rd());
uniform_int_distribution<> dist(0, chars.size());

string num2str(ll num) {
	string s;
	s.reserve(str_len);

	do {
		s.push_back(chars[num % (ll)chars.size()]);
		num /= (ll)chars.size();
	} while (num > 0);

	return s;
}

string get_random_string(int len) {
	string s = string(len, '0');
	for (auto& x : s) x = chars[dist(mt)];
	return s;
}

void test(int threads, int max_len = str_len) {
	ll cases_num = pow(chars.size(), max_len);
	auto started_at = chrono::system_clock::now();
	bool flag = 0;

	auto random_str = get_random_string(str_len)/*string(str_len, 'z')*/;

	cout << "===< PasswordCrackTest >==" << endl
		<< "StringLength: " << str_len << endl
		<< "RandomString: " << random_str << endl
		<< "NumOfCases: " << cases_num << endl << endl;

#pragma omp parallel for
	for (int i = 0; i < threads; i++) {
		if (flag) break;

		ll from = cases_num / threads * i, to = cases_num / threads * (i + 1);

		cout << "===< Thread " << i << " >===" << endl
			<< "CountFrom: " << from << endl
			<< "CountTo: " << to << endl << endl;

		for (ll cnt = from; cnt <= to; cnt++) {
			auto s = num2str(cnt);

			if (s == random_str) {
				auto elapsed = chrono::system_clock::now() - started_at;

				cout << "===< Cracked [" << i << "] >==="
					<< "String: " << s << endl
					<< "Elapsed: " << chrono::duration_cast<chrono::microseconds>(elapsed).count() / pow(10, 6) << " [sec]" << endl << endl;
				flag = 1;
			}
		}
	}
}

int main() {
	test(8);
}
