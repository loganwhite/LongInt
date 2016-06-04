#include "LongInt.h"
using namespace std;

int main()
{
	longInt a,b,c,d;
	cout<<"Please input two longInt numbers end with '#'"<<endl;
	cin>>a;
	cin>>b;
	c = a + b;
	d = a - b;
	cout<<"\tsum:\t\t"<<c;
	cout<<"\tdifference:\t"<<d;
	return 0;
}