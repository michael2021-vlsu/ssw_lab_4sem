#include <iostream>
#include "generator.h"
using namespace std;



int main(int argc, char *argv[]) {
    try {
    	auto t1 = generate<int>(234); // t1 is std::vector
    	cout << "<TEST>" << endl;
    	cout << "\t" << t1[0] << " | " << t1.size() <<  endl;


    	auto t2 = generate<string>(5);
    	cout << "<TEST>" << endl;
    	cout << "\t" << t2[0] << " | " << t2.size() <<  endl;

    	auto t3 = generate<float>(6);
    	cout << "<TEST>" << endl;
    	cout << "\t" << t3[0] << " | " << t3.size() << endl;
    } catch (const std::exception &exp) {
        cout << exp.what() << endl;
    }

    return 0;
}
