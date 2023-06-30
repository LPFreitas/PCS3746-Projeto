#include <stdio.h>
#include <iostream>
using namespace std;

class MyClass
{
public:
    void myFunction()
    {
        cout << "Hello World!";
    }
};

int main()
{
    MyClass myObj;
    myObj.myFunction();
    return 0;
}
