#include <cstdlib>
#include <iostream>
#include <fstream>
#define MAX 100

using namespace std;

int algorithm(int N, int* R, int* P, int* Q, int* X)
{   
    int time = 0;
    int current_task_time[MAX];
    int temp = 999999;
    int max_delivery_time = 0;
    int scheduling_state[MAX]; //-1 dla indeksu oznacza ze zadanie o tym indeksie zostalo uszeregowane 
    for(int i = 0; i<N; i++)
    {
        scheduling_state[i] = i;//domyslnie zadne nie jest uszeregowane
    }
    int chosen_task_index;

    for(int j = 0; j<N; j++)
    {
        for(int i = 0; i<N; i++)//przeszukaj tablice w celu znalezienia nastepnego
        {
            if(scheduling_state[i] != -1) //jezeli element nie ma jeszcze przypisanej pozycji
            {   
                current_task_time[i] = max(0, R[i] - time);//Sprawdzenie za ile czasu zadanie bedzie dostepne
                if(current_task_time[i] < temp)//Wybor zadania dostepnego najszybciej
                {   
                    temp = current_task_time[i];
                    max_delivery_time = Q[i];
                    chosen_task_index = i;
                }
                else if(current_task_time[i] == temp)//Jezeli kilka zadan bedzie dostepnych jednoczesnie
                {
                    if(max_delivery_time < Q[i])//Wybor na podstawie czasu dostarczenia
                    {   
                        max_delivery_time = Q[i];
                        chosen_task_index = i;
                    }       
                }
            }
        }

        //Dodanie elementu do permutacji
        scheduling_state[chosen_task_index] = -1;
        X[j] = chosen_task_index;

        //Zaktualizowanie zmiennej modelujacej uplyw czasu
        time = max(time,R[chosen_task_index]) + P[chosen_task_index];
        max_delivery_time = 0;
        temp = 999999;
    }

    /////////////////////////////

    //Liczenie cmax
    int m = 0;
    int c = 0;
    for(int n = 0; n< N; n++)
    {
        int i = X[n]; 
        m = max(m,R[i])+P[i];
        c = max(m + Q[i], c);
    }
    return c;
}
int main(int argc, char *argv[])
{
    string arg;
    if (argc != 2)
    {   
        cout << "Choose a dataset, e.g. data.000";
        return -2;
    }
    else
    {
        arg = argv[1];
        arg.append(":");
    }
    ifstream input_file("data.txt");
    int N, R[100], P[100], Q[100], X[100]; 
    string temp;
    while(temp!=arg)
    {
        input_file >> temp;
    }
    input_file >> N;
    for(int i = 0; i < N; i++)
    {
        input_file >> R[i] >> P[i] >> Q[i];
    }

    int c = algorithm(N,R,P,Q,X);

    cout << "The calculated order: " << endl;
    for(int i = 0; i<N; i++)
    {
        cout << X[i]+1 << " ";
    }
    cout << endl << "The processing time of the order: " << c << endl;

    input_file.close();
    return 0;
}