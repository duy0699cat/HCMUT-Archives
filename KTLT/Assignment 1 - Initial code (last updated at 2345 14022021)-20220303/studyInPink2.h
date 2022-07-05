/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2022
* Author: Vu Van Tien
* Date: 10.02.2022
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef studyInPink_h
#define studyInPink_h

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

////////////////////////////////////////////////////////////////////////
///STUDENT'S ANSWER BEGINS HERE
///Complete the following functions
///DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
bool checkN(string n1, int Size) {
	if (n1.size() == Size) {
		for (int i = 0; i < Size; i++) {
			if (!isdigit(n1[i])) { return false; }
		}
		return true;
	}
	else return false;
}
string notebook1(string ntb1) {
	// Complete this function to gain point
	string l1, l2, s1;
	int count = 0;
	long long n1;
	int freq[10] = { 0,0,0,
				  0,0,0,0,
				  0,0,0 };

	//cout << "Start notebook 1 func, \ninput string is: " << ntb1 << " \n";
	ifstream myfile(ntb1);
	if (myfile.is_open()) {
		myfile >> l1;
	}
	else { return "no file \n"; }

	s1 = l1.substr(11); // cut <n1> string

	if (!checkN(s1, 3)) return "000000000";
	else {
		n1 = stoll(s1);
		if (n1 > 999 && n1 < 1) return "000000000";
	}

	//count numbers
	while (!myfile.eof() && count < n1)
	{
		int temp;
		myfile >> temp;
		if (myfile.fail()) return "read fail \n" /* handle with break or throw? */;
		// now use temp to count freqency, store in array
		count++;
		freq[temp]++;
		if (freq[temp] >= 10) freq[temp] -= 10;
	}
	//convert freq array to string for output
	ostringstream os;
	for (long long i : freq) {
		os << i;
	}

	string str(os.str());
	// cout << str;
	s1 = str;
	return s1;
}
int checkPink(string str, string pink) {
	//1st: pink
	size_t pos = 0;
	int freq = 0;
	while (pos <= str.size() && pos != string::npos) {
		pos = str.find(pink, pos);
		if (pos != string::npos) { freq++; pos++; } // pos to find next occurrence
	}
	return freq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

string notebook2(string ntb2) {
	// Complete this function to gain point
	string l1;
	int n2, count = 0, freq = 0;
	ifstream myfile(ntb2);

	if (myfile.is_open()) {
		myfile >> l1;
	}
	else { return "no file \n"; }

	if (!checkN(l1, 5)) return "1111111111"; //wrong format n2
	else {
		n2 = stoi(l1);
		if (n2 > 100 && n2 < 5) return "1111111111";
	}
	//have right n2
	//count pinks
	while (!myfile.eof() && count <= n2)
	{
		string temp;

		//myfile >> temp;
		getline(myfile, temp);
		if (myfile.fail()) return "read fail \n" /* handle with break or throw? */;
		// now use temp string to count 
		freq = freq + checkPink(temp, "pink") + checkPink(temp, "Pink");
		count++;
	}
	//freq = 999999521;
	if (to_string(freq).size() < 5) freq = freq * freq;
	string nine(10 - to_string(freq).size(), '9'); //create "99...99" string

	return to_string(freq) + nine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isPrime(long long n)
{
	// Corner cases
	if (n <= 1)  return false;
	if (n <= 3)  return true;

	// This is checked so that we can skip 
	// middle five numbers in below loop
	if (n % 2 == 0 || n % 3 == 0) return false;

	for (long long i = 5; i * i <= n; i = i + 6)
		if (n % i == 0 || n % (i + 2) == 0)
			return false;

	return true;
}

long long nextPrime(long long N)
{

	// Base case
	if (N <= 1)
		return 2;
	if (isPrime(N)) return N;
	long long prime = N;//
	bool found = false;

	// Loop continuously until isPrime returns
	// true for a number greater than n
	while (!found) {
		prime++;

		if (isPrime(prime))
			found = true;
	}

	return prime;
}
//N is a Fibonacci number if and only if 5 N^2 + 4 or 5N^2 – 4 is a perfect* square number
// A utility function that returns true if x is perfect square
bool isPerfectSquare(long long x)
{
	long long s = sqrt(x);
	return (s * s == x);
}

// Returns true if n is a Fibonacci Number, else false
bool isFibonacci(long long n)
{
	// n is Fibonacci if one of 5*n*n + 4 or 5*n*n - 4 or both
	// is a perfect square
	return isPerfectSquare(5 * n * n + 4) ||
		isPerfectSquare(5 * n * n - 4);
}

const double pheta = 0.5 * (std::sqrt(5) + 1); //golden ratio, something something binet's formula in google?

double fib(unsigned long long n)
{
	return (std::pow(pheta, n) - std::pow(1 - pheta, n)) / std::sqrt(5);
}

unsigned long long fibo_lowerbound(double N, unsigned min = 0, unsigned max = 1000)
{
	unsigned long long newpivot = (min + max) / 2;
	if (min == newpivot)
		return newpivot;

	if (fib(newpivot) <= N)
		return fibo_lowerbound(N, newpivot, max);
	else
		return fibo_lowerbound(N, min, newpivot);
}

std::pair<double, double> fibo_range(unsigned long long n)
{
	unsigned long long lbound = fibo_lowerbound(n);
	return std::make_pair(fib(lbound), fib(lbound + 1));
}

/*void display(unsigned int n)
{
	std::pair<double, double> range = fibo_range(n);
	std::cout << "Fibonacci range wrapping " << n << " is "
		<< "[" << (unsigned long long) range.first << ", " << (unsigned long long) range.second << "]"
		<< std::endl;
}*/
long long nextFib(unsigned long long n)
{
	if (isFibonacci(n)) return n;
	else {
		std::pair<double, double> range = fibo_range(n);
		return (unsigned long long) range.second;
	}
}
void swapA_B(long long& a, long long& b) {
	long long temp;
	if (a > b) { temp = a; a = b; b = temp; }
}
void sort7to9(long long& n7, long long& n8, long long& n9) {
	swapA_B(n7, n8);
	swapA_B(n8, n9); //n9 = max now?
	swapA_B(n7, n8);
}
int maxArr(long long arr[]) {
	int i, idx = 0;
	long long max = arr[0];
	for (i = 1; i < 10; i++)
		if (arr[i] >= max)
		{
			max = arr[i];
			idx = i;
		}
	return idx;
}
string notebook3(string ntb3) {
	// Complete this function to gain point
	ifstream inputStream(ntb3);
	string stemp, s2temp;
	long long temp = 0;
	long long myArray[10][10];
	for (int i = 0; i < 10; i++) {
		inputStream >> stemp;
		std::istringstream iss(stemp);
		for (int j = 0; j < 10; j++) {
			std::getline(iss, s2temp, '|');
			temp = stoi(s2temp);
			myArray[i][j] = (temp < 0) ? -temp : temp;
		}
	}
	//should have 2d array now
	//doing step1 - above main diag line of matrix
	for (int i = 0; i < 10; i++) {

		for (int j = i + 1; j < 10; j++) {
			myArray[i][j] = nextPrime(myArray[i][j]);
		}
	}
	//step 2
	for (int i = 0; i < 10; i++) {

		for (int j = 0; j < i - 1; j++) {
			myArray[i][j] = nextFib(myArray[i][j]);
		}
	}
	int outp[10];
	for (int i = 0; i < 10; i++) {
		sort7to9(myArray[i][7], myArray[i][8], myArray[i][9]);
		outp[i] = maxArr(myArray[i]);
	}
	//convert array to string for output
	ostringstream os;
	for (int i : outp) {
		os << i;
	}

	string str(os.str());
	// cout << str;

	return str;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

string getreverseStr(string str)
{
	string tstr = str;
	int n = str.length();
	char temp;
	// Swap character starting from two
	// corners
	for (int i = 0; i < n / 2; i++)
	{
		//swap(str[i], str[n - i - 1]);
		temp = tstr[i];
		tstr[i] = tstr[n - i - 1];
		tstr[n - i - 1] = temp;
	}
	return tstr;
}

string reverse_add(string a, string b) {
	long long na = stoll(getreverseStr(a));
	long long nb = stoll(getreverseStr(b));
	long long res = na + nb;
	string out = to_string(res);
	out = getreverseStr(out);
	if (out.size() > 10) out = out.substr(0, 10);
	return out;
}
string generateListPasswords(string pwd1, string pwd2, string pwd3) {
	// Complete this function to gain point
	string s12 = reverse_add(pwd1, pwd2);
	string s13 = reverse_add(pwd1, pwd3);
	string s23 = reverse_add(pwd2, pwd3);
	return pwd1 + ";" + pwd2 + ";" + pwd3 + ";" + s12 + ";" + s13 + ";" + s23 + ";" + reverse_add(s12, pwd3) + ";" + reverse_add(pwd1, s23) + ";" + reverse_add(s12, s13);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TaxiMove(string moves, int arr[100][100]) {

	string temp = moves;
	int i = 0, j = 0;
	;
	while (temp.size() > 0) {
		switch (temp[0]) {
		case 'U':
			if (i == 0) break;
			else {
				arr[i - 1][j] = arr[i][j] + 9;
				i--;
			}
			break;
		case 'D':
			if (i == 99) break;
			else {
				arr[i + 1][j] = arr[i][j] + 9;
				i++;
			}
			break;
		case 'L':
			if (j == 0) break;
			else {
				arr[i][j - 1] = arr[i][j] + 9;
				j--;
			}
			break;
		case 'R':
			if (j == 99) break;
			else {
				arr[i][j + 1] = arr[i][j] + 9;
				j++;
			}
			break;

		}
		temp = temp.substr(1);
	}
}
void mapPrt(int arr[100][100]) {
	//test
	const int PRINT_SIZE = 10;
	for (int r = 0; r < PRINT_SIZE; ++r) {
		for (int c = 0; c < PRINT_SIZE; ++c) {
			cout << arr[r][c] << " ";
		}
		cout << endl;
	}
	//
}
string Outtime(string points, int arr[100][100], bool& reached, string& outCatchUps) {
	string temp = points;
	size_t t = 0;
	int tloc[2] = { 0,0 };
	int ploc[2] = { 0,0 };
	int dist = 0;
	string out;
	reached = 0;
	while (temp.size() > 0) {
		if (temp[0] != '(') { throw"wrong input"; break; }
		else {
			temp = temp.substr(1);
			t = temp.find(',');
			if (t == string::npos || t == 0) { throw"wrong input"; break; }
			else {
				tloc[0] = stoi(temp.substr(0, t));
				//cout << tloc[0] << ",";
				temp = temp.substr(t + 1);
				t = temp.find(')');
				if (t == string::npos || t == 0) { throw"wrong input"; break; }
				else {
					tloc[1] = stoi(temp.substr(0, t));
					//cout << tloc[1] << "\n";
					temp = temp.substr(t + 1);
					//have location now
					dist = dist + (abs(ploc[0] - tloc[0]) * 14 + abs(ploc[1] - tloc[1]) * 14);
					if (dist <= arr[tloc[0]][tloc[1]]) reached = true;

					ploc[0] = tloc[0]; ploc[1] = tloc[1];

					out = out + to_string(dist);
					outCatchUps = reached ? outCatchUps + "Y" : outCatchUps + "N";
					if (temp.size() > 0) {
						if (reached) {
							while(temp.find('-')!=string::npos){
								t = temp.find('-');
								out = out + ";-";
								outCatchUps = outCatchUps + ";-";
								temp = temp.substr(t + 1);
							}
							return out;
						}
						else {
							t = temp.find('-');
							out = out + ";";
							outCatchUps = outCatchUps + ";";
							temp = temp.substr(t + 1);
						}
					}
				}
			}

		}
	}
	return out;
}
bool chaseTaxi(
	int arr[100][100],
	string points,
	string moves,
	string& outTimes,
	string& outCatchUps
) {
	// Complete this function to gain point
	//init array
	int loc[2][2] = { 0 };
	//{	{0 , 0} , loc[0][] -> before
	//	{0 , 0}	} loc[1][] -> now
	for (int i = 0; i < 100; i++) {

		for (int j = 0; j < 100; j++) {
			arr[i][j] = -9;
		}
	}
	arr[0][0] = 0;
	TaxiMove(moves, arr);
	//mapPrt(arr);//test
	//calculate outtime
	bool reached = 0;
	outTimes=Outtime(points, arr, reached,outCatchUps);
	return reached;
}

string enterLaptop(string tag, string message) {
	// Complete this function to gain point

	ifstream myfile(tag);
	string st, st2;
	int t;
	myfile >> st;
	if (st != "Email:") throw "error!";
	else{myfile >> st;
	st = st + ";";
	myfile >> st2;
	if (st2 != "Address:") throw "error!";
	else {
		myfile >> t;
		for (int i = 0; i < t; i++) { st = st + message; }
		return st;
	}
}
}

////////////////////////////////////////////////
///END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* studyInPink_h */
