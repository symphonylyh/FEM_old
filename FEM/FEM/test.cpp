#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    /*
    int* arr[3];
    int a=0, b=0, c=0;
    arr[0] = &a;
    arr[1] = &b;
    arr[2] = &c;
    cout<< arr[2] <<endl;
    */
    int** List = new int*[3];
    List[0] = new int(0);
    int* b = new int(1); List[1] = b;
    int* c = new int(2); List[2] = c;
    cout<< *List[2] <<endl;
    return 0;
}
