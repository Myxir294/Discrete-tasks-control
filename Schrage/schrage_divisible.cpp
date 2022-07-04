#include <cstdlib>
#include <iostream>
#include <fstream>
#define MAX 100

using namespace std;


int algorithm(int N, int* R, int* P, int* Q)
{   

    //Krok 1
    int time = 0;
    int cmax = 0;
    int t_next = 9999999;
    int j;

    //Przygotowanie zbiorow G i N
    bool tasks_ready_to_process[MAX];
    bool tasks_N[MAX];
    int G_elements_number = 0;
    int N_elements_number = N;
    for(int i = 0; i<N; i++)
    {
        tasks_ready_to_process[i] = false;
        tasks_N[i] = true;
    }

    //Skopiowanie tablicy P
    int current_P[MAX];
    for(int i=0; i<N; i++ )
    {
        current_P[i] = P[i];
    }

    //Krok 2 - petla while
    while(G_elements_number != 0 || N_elements_number != 0)
    {   
        //Krok 2.1 - odswiezenie zbioru zadan gotowych
        for(int i=0; i<N; i++)
        {   
            //Sprawdzenie czy zadanie jest gotowe
            if(tasks_N[i] == true && R[i] <= time)
            {   
                //Operacje na zbiorach
                tasks_ready_to_process[i] = true;
                ++G_elements_number;
                tasks_N[i] = false;
                --N_elements_number;
            }
        }
        //Krok 2.2 - wyznaczenie momentu nadejscia nastepnego zadania
        if(N_elements_number == 0){t_next = 9999999;}
        else
        {   
            int temp_min_r = 9999999;
            for(int i = 0; i<N; i++)
            {
                if(tasks_N[i] == true && R[i] < temp_min_r)
                {   
                    temp_min_r = R[i];
                }
            }
            t_next = temp_min_r;
        }
        //krok 2.4 - przerwa na maszynie
        if(G_elements_number == 0){time = t_next;}
        else//krok 2.5 - wyznacz zadanie j do wykonania
        {   
            //krok 2.5.1 
            int temp_max_index;
            int temp_max_q = -1;
            for(int i = 0; i<N; i++)
            {
                if(tasks_ready_to_process[i] == true && Q[i] > temp_max_q)
                {
                    temp_max_q = Q[i];
                    temp_max_index = i;
                }
            }
            j = temp_max_index;

            //krok 2.5.2 - wykonanie czesci zadania j do momentu t_next
            if(t_next < time + current_P[j])
            {
                current_P[j] = current_P[j] - (t_next - time);
                time = t_next;
            }
            //krok 2.5.3 - wykonanie calosci zadania j
            if(t_next >= time + current_P[j])
            {
                time = time + current_P[j];
                tasks_ready_to_process[j] = false;
                --G_elements_number;
                cmax = max(cmax, time + Q[j]);
            }            
        }
    }
    return cmax;
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
    int N, R[100], P[100], Q[100]; 
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

    int c = algorithm(N,R,P,Q);
    cout << "Best possible total time is: " << c;
    input_file.close();
    return 0;
}