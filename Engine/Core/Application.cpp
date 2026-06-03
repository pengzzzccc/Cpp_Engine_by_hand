#include "Application.h"
#include <iostream>
#include <vector>

using namespace std;

void Application::Run(int n)
{
    string s = "Hello Engine in string";
    cout << s << endl;

    cout << "" << endl;

    vector<int> intV;
    intV.push_back(n);
    intV.push_back(3);
    for (int i = 0; i < intV.size(); i++)
    {
        cout << intV[i] << endl;
    }
}

int main()
{
    Application app;
    app.Run(100);
    return 0;
}