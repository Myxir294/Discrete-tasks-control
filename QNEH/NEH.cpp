#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
#define MAX_TASKS 501
#define MAX_MACHINES 21


using namespace std;
    ifstream input_file("neh.txt");    
    ofstream output_file("results.txt");
    
int calculate_cmax(int tasks_number, int machines_number, int(&order)[MAX_TASKS], int (&lengths)[MAX_TASKS][MAX_MACHINES])
{
    
    int c[MAX_TASKS+1][MAX_MACHINES+1];

    memset(c,0,sizeof(c));
    for(int i = 1; i < tasks_number+1; i++)//maszyny
    {
        for(int j = 1; j < machines_number+1; j++)//zadania
        {
            c[i][j] = max(c[i-1][j], c[i][j-1]) +  lengths[order[i-1]-1][j-1];

        }
    }
    int cmax = c[tasks_number][machines_number];
    
    return cmax;
}

int calculate_task_weigth(int task, int machines_number, int (&lengths)[MAX_TASKS][MAX_MACHINES])
{
    int weigth = 0;

    for(int i = 0; i < machines_number; i++)
    {
        weigth += lengths[task][i];
    }

    return weigth;
}

int process_orders(int tasks_number, int machines_number, int (&order)[MAX_TASKS], int (&lengths)[MAX_TASKS][MAX_MACHINES])
{   
    int min_cmax = 99999999;
    int cmax;
    int temp_order[MAX_TASKS];

    for (int i = 0; i < MAX_TASKS; i++)	//zapamietanie kolejnosci przed przesuwaniami	
	{	
        temp_order[i] = order[i];
	}

    for(int added_task = 1; added_task <= tasks_number; added_task++)
    {   
        min_cmax = 99999999;
        for(int i = added_task-1; i >= 0; --i)
        {
            if(i+1 < added_task){swap(order[i], order[i+1]);}

            cmax =  calculate_cmax(added_task,machines_number,order,lengths);

            if (cmax <= min_cmax)
            {   
                min_cmax = cmax;
                for (int k = 0; k < MAX_TASKS; k++)		
	            {	
                    temp_order[k] = order[k];
	            }    
            }
        }
        for (int i = 0; i < MAX_TASKS; i++)		
	    {	
            order[i] = temp_order[i];
	    }  
    }
	// output_file << "Permutation length is " << calculate_cmax(tasks_number,machines_number,order,lengths) << endl;
    // for (int i = 0; i < tasks_number; i++)		
	// {	
    //     output_file << order[i] << " ";
	// } 
    // output_file << endl;
	return min_cmax; 
}

int main()
{   

    clock_t start, time_end; //zmienne pomocnicze do przechowywania czasu dzialania programu
    double cpu_time_used;
    start = clock();    //rozpoczecie liczenia czasu (w sekundach)

    int tasks_number;
    int machines_number;
    int section_lengths[MAX_TASKS][MAX_MACHINES];
    int order[MAX_TASKS];
    int tasks_weigths[MAX_TASKS];
    string temp;

for (int k = 0; k<121; k++)
{
    //Wczytanie i przetworzenie wybranego zestawow danych
	output_file << endl << "For tasks set " << k << endl;
    input_file >> temp;
    //cout << temp << endl;
    while(temp.length() != 9)//znajdz w pliku begin wartosci dotyczacych zestawu danych
    {   
        input_file >> temp;
    }
    input_file >> tasks_number; //wczytanie liczby zadan oraz liczby maszyn
    input_file >> machines_number;
    output_file << tasks_number << " " << machines_number << endl;				
	for (int i = 0; i < tasks_number; i++) //wczytanie czasów na każdej z maszyn		
	{	
        for(int j = 0; j < machines_number; j++)
        {
            input_file >> section_lengths[i][j];   
        }
	}
    for (int i = 0; i < MAX_TASKS; i++)//inicjalizacja kolejności - bez szeregowania, 123...		
	{	
        order[i] = i+1;
	}

    //Podanie permutacji oraz czasu wykonania bez szeregowania
    output_file << "With 123... order: ";         
    output_file << calculate_cmax(tasks_number,machines_number,order,section_lengths);
    output_file << endl;
    for(int i = 0; i < tasks_number; i++)
    {
        tasks_weigths[i] = calculate_task_weigth(i,machines_number,section_lengths);
    }


    //Sortowanie zadan wg wag - zastosowano sortowanie bąbelkowe
    for(int i = 0; i<tasks_number; i++) 
    {
        for(int j = i+1; j<tasks_number; j++)
        {    
            if(tasks_weigths[j] > tasks_weigths[i]) 
            {   
                swap(tasks_weigths[i],tasks_weigths[j]);
                swap(order[i],order[j]);
            }
        }
    }
        
    output_file << endl << endl;

    //Sekwencja tempocniczych operacji majaca na celu wyeliminowanie niewlasciwego uszeregowania ciagu zadan o tej samej wadze
    int begin = 0;
    int end = 0;
    for (int i = 1; i < tasks_number; i++)	
	{	
        if(tasks_weigths[i] == tasks_weigths[i-1])
        {
            if(begin == 0) {begin = i-1;}
            end = i;
        }
        if(tasks_weigths[i] != tasks_weigths[i-1] && end != 0)
        {   
            for(int k = begin; k<=end; k++) 
            {
                for(int j = k+1; j<=end; j++)
                {       
                    if(order[j] < order[k]) 
                    {   
                        swap(order[k],order[j]);
                    }
                }
            }
            end = 0;
            begin = 0;
        } 
	}

    //Po wykonaniu powyzszych operacji zadania sa prawidlowo posortowane wg wag        
    output_file << "Initially sorted by weights: ";
    for (int i = 0; i < tasks_number; i++)		
	{	
        output_file << " " << order[i];
	}
    output_file << endl;

    //Calosc algorytmu QNEH wykonuje ponizsza funkcja 
    int min_cmax = process_orders(tasks_number,machines_number,order,section_lengths);


    //Wypisanie wyniku - dlugosci oraz permutacji
	output_file << "Permutation length is " << min_cmax << endl;
    for (int i = 0; i < tasks_number; i++)		
	{	
        output_file << order[i] << " ";
	} 
    output_file << endl;
}
    time_end = clock();
    cpu_time_used = ((double) (time_end - start)) / CLOCKS_PER_SEC;

    cout << "Execution time: " << cpu_time_used << " s" << endl;
    input_file.close();
    output_file.close();
    return 0;
}

