#include <stdlib.h>
#include <iostream>
using namespace std;

int compare(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

int main()
{
    int a[10];
    int i;

    for (i = 0; i < 10; i++) {
        a[i] = rand() % 1000;
        cout << a[i] << ' ';
    }
    cout << endl;
    qsort(a, 10, sizeof(int), compare);
    for (i = 0; i < 10; i++)
        cout << a[i] << ' ';
    cout << endl;
    return 0;
}
