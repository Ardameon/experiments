#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

int db_to_amplitude(int db)
{
	double times = pow(1.122018454f, db);
	int amplitude;

	cout << "times: " << times << endl;
	
	amplitude = (int)((times - 1) * 32768) + 32768;

	cout << "amplitude: " << amplitude << endl;

	return amplitude;
}


int main(int argc, char **argv)
{
	int db = atoi(argv[1]);

	db_to_amplitude(db);

	return 0;
}
