#include <iostream>
#include <fstream>
#include <math.h>
#define MAX 100 //Maksymalna liczba zadan, ustalona dla wygody
using namespace std;

int main()
{
	//Zmienne pomocnicze powiązane z wykorzystywanymi plikami
    ifstream file_in("data.txt");    
    ofstream file_out("results.txt");
	string data[]={"data.10:", "data.11:","data.12:","data.13:","data.14:","data.15:","data.16:","data.17:","data.18:","data.19:","data.20:"};
	
	//Zmienne pomocnicze wykorzystywane do przetwarzania oraz wczytywania danych
	int tasks_number, time[MAX], penalty[MAX], deadline[MAX];
	string temp;

	int sub_permutation_penalty = 99999; //zmienna pomocnicza o duzej wartosci aby zadzialalo szukanie minimum
    int c; //time wykonania sekwencji zadan

	//Wczytanie i przetworzenie kazdego z zestawow danych
	for (int i = 0; i < 11; i++)//dopoki sa zestawy
	{	
		file_out << "For tasks set " << data[i] << endl;
        while(temp!=data[i])//znajdz w pliku poczatek wartosci dotyczacych zestawu danych
        {
            file_in >> temp;
        }

		//Wczytanie liczby zadan oraz dotyczacych ich parametrow
		file_in >> tasks_number;				
		for (int j = 0; j < tasks_number; j++)		
		{	
			file_in >> time[j] >> penalty[j] >> deadline[j];
		}

		//Przygotowanie tablicy F, zawierajacej minimalne kary wykonania odpowiednich kombinacji zadan
		int number_of_permutations = pow(2,tasks_number);
		int* F = new int[number_of_permutations];
		F[0] = 0;

		//Przygotowanie dodatkowej tablicy przechowujacej ostatnie zadanie kazdej kombinacji z tablicy F
		int* best_last_tasks = new int[number_of_permutations];
		best_last_tasks[0] = 0;

		for (int permutation = 1; permutation < number_of_permutations; permutation++)//Wypelnienie pol tablicy F - czesc na ocene 3.0
		{
			c = 0;
			for (int j = 0; j < tasks_number; j++)//Wyznaczenie timeu wykonania dataj kombinacji zadan
			{	
				int mask = pow(2,j);
				if (permutation & mask)//Logiczne AND w celu wyluskania odpowiednich zadan z tablic danych
				{
					c += time[j];
				}
			}

			//Wyznaczenie minimalnej kary dotyczacej dataj kombinacji zadan
			F[permutation] = sub_permutation_penalty;
			for (int k = 0; k < tasks_number; k++) //Dla kazdego mozliwego ostatniego zadania
			{	
				int mask = pow(2,k);
				if (permutation & mask) //Logiczne AND w celu wyluskania odpowiednich zadan z tablic danych
				{	
					//Implementacja wzoru matematycznego opisujacego algorytm PD
					sub_permutation_penalty = F[permutation - mask] + penalty[k] * max(c - deadline[k], 0); 
					if(sub_permutation_penalty < F[permutation])
					{
						F[permutation] = sub_permutation_penalty;//Zapisanie aktualnej optymalnej kary
						best_last_tasks[permutation] = k+1; //Zapisanie aktualnego optymalnego ostatniego zadania kombinacji
					}
				}
			}
			sub_permutation_penalty = 99999;
		}

		//Wyznaczenie permutacji - czesc na ocene 4.0 (wersja dzialajaca ale prawdopodobnie nie zapisana w najprostszej formie)
		int* Permutation = new int[tasks_number];

		//Ostatnim zadaniem bedzie optymalne ostatnie zadanie dla ostatniego elementu tablicy F
		Permutation[tasks_number-1] = best_last_tasks[number_of_permutations - 1]; 

		//Zmienna pomocnicza wyznaczajaca ktora permutation z tablicy F bedzie poprzednia
		int pom = number_of_permutations - 1 - pow(2,Permutation[tasks_number-1]-1);

		//Petla umozliwiajaca wyznaczenie kolejnych zadan oraz permutacji
		for(int j = tasks_number-2; j >= 0; j--)
		{
			Permutation[j] = best_last_tasks[pom];
			pom = pom - pow(2,Permutation[j]-1);
		}

		//Wypisanie permutacji dla datago zbioru zadan
		for(int j = 0; j<tasks_number; j++)
		{
			file_out << Permutation[j] << " ";	
		}
		file_out << endl;

		//Wypisanie minimalnej kary
		file_out << "Minimal penalty: " << F[number_of_permutations - 1] << endl;

		//Zwolnienie pamieci
		delete[] Permutation;
		delete[] F;
		delete[] best_last_tasks;
	}
	file_in.close();
	file_out.close();
	return 0;	
}