# SCHRAGE
Folder zawiera program podający przybliżone rozwiązanie problemu RPQ, tj. optymalnego uszeregowania zadań na jednej maszynie, dla zbioru zadań podanego poprzez argument wywołania.

Struktura danych wejściowych: każdy wiersz zawiera czas przygotowania, czas wykonywanie oraz czas dostarczenia zadania. Szukane rozwiązanie to czas potrzebny na wykonanie i dostarczenie wszystkich zadań.

Zrealizowano również wariant z podziałem zadań, podający najlepsze możliwe rozwiązanie w przypadku zniesienia ograniczenia nakazującego wykonanie całości zadania naraz. Oba warianty mogą stanowić elementy składowe optymalnego algorytmu Carliera, opierającego się o schemat podziału i ograniczeń. 

The directory contains a program calculating an approximated solution of RPQ problem (optimal scheduling of tasks on single machine) for tasks set chosen by an execution argument.

Input data format: each row is made of readiness time, processing time and delivering time. The sought solution is the time needed for processing and delivering every task.

Additionally, I have implemented an algorithm for divisible tasks, allowing to calculate the best possible order if the limitation of having to proccess all the task at once is lifted. The two variants could be used as elements of optimal Carlier algorithm, solving the RPQ problem by using a B&B scheme. 
