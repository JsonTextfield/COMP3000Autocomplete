#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#define MAX_STRING_LENGTH 100

using namespace std;

int main(){
	ifstream infile;
    
    cout << "Enter a command\n";
   	stringstream ss;
   	
   	char string[MAX_STRING_LENGTH];
   	
   	cin >> string;
   	ss << "bash -c 'compgen -A function -abck " << string << " > outputfile'";
    
    system(ss.str().c_str());
   
    infile.open("outputfile");
   	cout << infile.rdbuf() << "\n";
    infile.close();
       
    return 0;
}
