# NEH/QNEH
Folder zawiera program szeregujący zadania na wielu maszynach wyznaczając optymalną kolejność podejmowania kolejnych zadań. Wykorzystuje w tym celu algorytm NEH o złożoności O(n^3 * m) oraz jego usprawnioną wersję QNEH o złożoności O(n^2 * m).

Struktura danych wejściowych: każdy wiersz zawiera czasy wykonania sekcji danego zadania na każdej z maszyn (oś y/n - ilość zadań, oś x/m - ilość maszyn). Sekcje muszą zostać wykonane w podanej w pliku kolejności.

Programy z dopiskiem 'arg' podają czas szeregowania dla podanego w wywołaniu zestawu zadań.  
Kompilacja programu 'QNEH' w g++ z opcją -O3 umożliwia wyznaczenie kolejności dla wszystkich 120 zestawów w czasie krótszym niż 1 sekunda.

The directory contains a program scheduling multiple tasks done by multiple machines, in an optimal way. For this, it uses a NEH algorithm of time complexity O(n^3 * m) or its enhanced version, QNEH, of time complexity O(n^2 * m).

Input data format: every row is made of time sections lengths of given task on each machine (y axis/n - number of tasks, x axis/m - number of machines). The sections have to be done in the order given in the file.

The 'arg' programs require choosing a single dataset by an execution argument. Compilation of 'QNEH' program in g++ with -O3 option allows to calculate permutations for all 120 datasets in total time of less than 1s.

