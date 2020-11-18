#include <iostream>

using namespace std;

void slivane(int * arr, int l, int m, int d)
{
    int lChast = (m - l) + 1;
    int dChast = d - m;


    ///vremeni masivi lqv-desen
    int L[lChast],D[dChast];


    ///Kopirame vuv vremenite masivi
    for(int i=0;i<lChast;i++)
        L[i] = arr[i+l];
        
    for(int j = 0; j<dChast;j++)
        D[j]=arr[(m+j)+1];



    /*for(int p=0;p<dChast;p++)
        cout<<D[p]<<" ";
*/
    ///suzdavame index za lqvata chast
    int i = 0;
    ///suzdavame index za dqsnata chast
    int j = 0;
    ///index za slivaneto
    int k = l;

    while(i<lChast&&j<dChast)
    {
        if(L[i]<=D[j])
        {
            arr[k]=L[i];
            i++;
        }
        else
        {
            arr[k]=D[j];
            j++;
        }
        k++;
    }
    ///Proverka za lqva chast ostatuk
    if(i<lChast)
    {
        for(int n = i; n<lChast;n++)
        arr[k] = L[i];
    }
    ///Proverka dqsna chast
    else
    {
        for(int m = j; m<dChast; m++)
            arr[k] = D[j];
    }
}

void mergeSort(int arr[], int l, int d)
{
    if(l < d)
    {

    int m = l+(d-l)/2;
    mergeSort(arr,l,m);
    mergeSort(arr,m+1,d);

    slivane(arr,l,m,d);
    }
}

int main()
{

    int arr[]={3,5,8,2,4,6,70};

   // slivane(arr,0,2,6);
   cout << "Predi slivaneto" << endl;
    for(int i = 0; i < 7; i++)
       cout << arr[i] << " ";

    mergeSort(arr,0,6);


    cout << endl;
    cout <<"Nareden: \n";
    for(int i = 0; i < 7; i++)
        cout<<arr[i]<<" ";

    return 0;
}
