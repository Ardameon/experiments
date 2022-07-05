#include <iostream>
#include <cstring>

using namespace std;

int main()
{
	//char arr[20] = {0};
	char arr[] = "aaaaaaaa\0ddddddd";


	//memset(arr, 'a', sizeof(arr) - 1);
	//arr[5] = '\0';
	cout << arr << endl;

	for (const char &a : arr)
	{
		cout << a << endl;
	}

	return 0;
}
