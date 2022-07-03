#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>

#define MAX_TASKS 500
#define MAX_MACHINES 20

using namespace std;
ifstream input_file("neh.txt");    
ofstream output_file("results.txt");
int c[MAX_TASKS+1][MAX_MACHINES+1]; //pomocnicza tablica do wyliczania wyników składowych

//Pierwotna wersja liczenia cmaxa bez akceleracji, uzywana do wyznaczania dlugosci permutacji przed jakimkolwiek szeregowaniem (123...)
int calculate_cmax(int tasks_number, int machines_number, int(&order)[MAX_TASKS], int (&lengths)[MAX_TASKS][MAX_MACHINES])
{
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



//Funkcja wyznacza elementy grafu dlugosci zadan na maszynach w przypadku liczenia od lewego górnego rogu do prawego dolnego
//W pratyce sprowadza się to do zwykłego wyznaczenia cmax ale z zapamiętywaniem wyników składowych

//Funkcja wykonuje sie jednokrotnie przed dodaniem kolejnego zadania - nie jest wykorzystywana do generowania kolejności próbnych,
//tak jak miało to miejsce wcześniej w pierwotnej wersji programu, bez akceleracji

//Wybrana nazwa funkcji nawiązuje do tego, że opisywane wyniki na prezentacji dołączonej do laboratorium zapisywano w Prawym Górnym rogu pola grafu
void init_PG(int tasks_number, int machines_number, int(&order)[MAX_TASKS], int (&lengths)[MAX_TASKS][MAX_MACHINES], int (&PG)[MAX_TASKS][MAX_MACHINES])
{
    memset(c,0,sizeof(c));
    for(int i = 1; i < tasks_number+1; i++)//maszyny
    {
        for(int j = 1; j < machines_number+1; j++)//zadania
        {
            c[i][j] = max(c[i-1][j], c[i][j-1]) +  lengths[order[i-1]-1][j-1];
            PG[i][j] = c[i][j];
        }
    }
}


//Funkcja działa analogicznie do opisywanego wyżej init_PG, z tym że liczy wartości w odwrotnej kolejnośći, 
//zapisywane w Lewym Górnym rogu pól na infografice

//Również jest wywoływana jednokrotnie dla każdego nowego zadania
void init_LG(int tasks_number, int machines_number, int(&order)[MAX_TASKS], int (&lengths)[MAX_TASKS][MAX_MACHINES], int (&LG)[MAX_TASKS][MAX_MACHINES])
{
    memset(c,0,sizeof(c));
    for(int i = tasks_number-1; i >= 0 ; i--)//maszyny
    {
        for(int j = machines_number-1; j >= 0; j--)//zadania
        {
            c[i][j] = max(c[i+1][j], c[i][j+1]) +  lengths[order[i]-1][j];
            LG[i][j] = c[i][j];
        }
    }
}

int quick_cmax(int tasks_number, int machines_number, int(&order)[MAX_TASKS], int putting_index, int (&lengths)[MAX_TASKS][MAX_MACHINES], int (&LG)[MAX_TASKS][MAX_MACHINES], int (&PG)[MAX_TASKS][MAX_MACHINES])
{

    int added_PG[MAX_MACHINES]; //tabela na kolumnę nowych wartości niezbędnych do sprawdzania długości próbnej kolejności
    int a, b, d;
    int current_cmax = 0;
    for(int i = 1; i <= machines_number; i++)
    {   
        if(putting_index != 0)//Jezeli nowe dodane task wprowadzamy na inną niż pierwszą pozycję w permutacji
        {
            a = PG[putting_index][i]; //pobierz odpowiednią wartość sąsiednią z lewej strony grafu
        }
        else{a = 0;}//w innym przypadku podstaw zero
        if (putting_index < tasks_number-1)//jezeli pozycja zadania jest inna niz ostania
        {   
            b = LG[putting_index][i-1];// pobierz odpowiednią wartość sąsiednią z prawej strony grafu
           
        }
        else{b = 0;}//w innym przypadku podstaw zero
        
        if(i == 1){d = 0;}//podstaw wartość sąsiednią z góry, o ile jest, w innym przypadku podstaw zero
        else{d = added_PG[i-2];}

        //Szukanie maksymalnej długości permutacji
        added_PG[i-1] = max(a,d) + lengths[order[putting_index]-1][i-1];
        current_cmax = max(current_cmax, added_PG[i-1] + b);
    }
    return current_cmax;  
}

//Funkcja liczy wagę zadania tj. jego całkowitą długość jako sumę długości na każdej z maszyn
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
    int temp_order[MAX_TASKS]; //tablica na przechowywanie kolejności próbnych

    //tablice na przechowywanie wyników składowych funkcji init_PG i init_LG 
    int PG[MAX_TASKS][MAX_MACHINES];  
    int LG[MAX_TASKS][MAX_MACHINES];

    for (int i = 0; i < MAX_TASKS; i++)	//zapamietanie kolejności przed przesuwaniami	
	{	
        temp_order[i] = order[i];
	}

    for(int added_task = 1; added_task <= tasks_number; added_task++)
    {   
        min_cmax = 99999999;
        for(int i = added_task-1; i >= 0; --i)
        {
            if(i+1 < added_task){swap(order[i], order[i+1]);} //generowanie kolejności próbnej

            cmax = quick_cmax(added_task,machines_number,order, i, lengths, LG, PG);

            //zapamietanie kolejności dajacej lepszy wynik
            if (cmax <= min_cmax)
            {   
                min_cmax = cmax;
                for (int k = 0; k < added_task; k++)		
	            {	
                    temp_order[k] = order[k];
	            }    
            }
        }

        //wybór kolejności dającej najlepszy wynik
        for (int i = 0; i < added_task; i++)		
	    {	
            order[i] = temp_order[i];
        }

        //Ponowne wyznaczenie wartości w polach grafu
        init_LG(added_task, machines_number, order, lengths, LG);
        init_PG(added_task, machines_number, order, lengths, PG);        
    }
    return min_cmax; 
}

int main(int argc, char *argv[])
{   
    string arg; // weryfikacja poprawności wywolania
    if (argc != 2) //jezeli nie ma argumentu wyjsciowego to zakoncz prace
    {   
        cout << "Choose a dataset, e.g. data.000" << endl;
        return -2;
    }
    else //zapisz nazwe zbioru danych podaną w wywołaniu programu
    {
        arg = argv[1];
        arg.append(":");
    }
    clock_t start, time_end; //zmienne pomocnicze do przechowywania czasu dzialania programu
    double cpu_time_used;
    start = clock();    //rozpoczecie liczenia czasu (w sekundach)

    int tasks_number;
    int machines_number;
    int section_lengths[MAX_TASKS][MAX_MACHINES];
    int order[MAX_TASKS];
    int tasks_weigths[MAX_TASKS];
    string temp;

    //Wczytanie i przetworzenie wybranego zestawow danych
	output_file << "For tasks set " << arg << endl;
    while(temp!=arg)//znajdz w pliku poczatek wartosci dotyczacych zestawu danych
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
    output_file << "In 123... order: ";         
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

    //Sekwencja pomocniczych operacji majaca na celu wyeliminowanie niewlasciwego uszeregowania ciagu zadan o tej samej wadze
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
    output_file << "Initially sorted by weigths ";
    for (int i = 0; i < tasks_number; i++)		
	{	
        output_file << " " << order[i];
	}
    output_file << endl;

    //Calosc algorytmu QNEH wykonuje ponizsza funkcja 
    int min_cmax = process_orders(tasks_number,machines_number,order,section_lengths);


    //Wypisanie wyniku - dlugosci oraz permutacji
	output_file << "Permutation length is: " << min_cmax << endl;
    for (int i = 0; i < tasks_number; i++)		
	{	
        output_file << order[i] << " ";
	} 
    output_file << endl;

    time_end = clock();
    cpu_time_used = ((double) (time_end - start)) / CLOCKS_PER_SEC;

    cout << "Execution time: " << cpu_time_used << " s" << endl;
    input_file.close();
    output_file.close();
    return 0;
}

