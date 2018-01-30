#include <iostream>

using namespace std;

void stir(int & x, int * y) {
        x = 7;
        *y = 10;
        //y = new int(10);
}

int main() {
        int w;     int * v;
        v = 10;
        stir(w, v);
        cout << w << ", " << *v << endl;
        delete v;   return 0;
}
