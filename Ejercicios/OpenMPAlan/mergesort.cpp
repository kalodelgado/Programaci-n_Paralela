# include <cstdlib>
# include <iomanip>
# include <ctime>
# include <algorithm>
# include <math.h>
# include <omp.h>
# include <iostream>
# include <vector>
using namespace std;

# define MAX 2147483647

void sort(const vector<int>& array);

bool validaCntHilos(int ch);
bool validaCntTerminos(long long ct);

int main() 
{
	long long n = 0;
	int thread_count = 0;
	vector<int> data;
	vector<int> local_data;
	vector< vector<int> > thread_vec;
	int my_id;
	int my_first, my_last;
	int p = 1;

	while (!validaCntHilos(thread_count) || !validaCntTerminos(n)) {
		cout << "Digite la cantidad de hilos ( >=1 ): ";
		cin >> thread_count;
		cout << "Digite el tamaño del vector ( >=1 ): ";
		cin >> n;
	}

	data.resize(n);
	thread_vec.resize(thread_count); //inicializacion vector de vectores
	
	#pragma omp parallel num_threads(thread_count) default(none) shared (n, thread_count, thread_vec, data) private (my_id, my_first, my_last, local_data) 
	{
		//Establece los rangos de los hilos
		my_id = omp_get_thread_num();
		my_first = (my_id * n) / thread_count;
		if (my_id == thread_count - 1)
			my_last = n;
		else
			my_last = ((my_id + 1) * n) / thread_count;

		local_data.resize(my_last - my_first);

		srand(time(NULL) * my_id);
		#pragma omp for
		for (int i = 0; i < (int)local_data.size(); i++)
			local_data[i] = n * rand() % (100);

		sort(local_data);
		//Copiamos el vec ordenado en el vector de vectores
		thread_vec.at(my_id) = local_data;
		
		if(my_id % 2 == 0)
			std::merge( thread_vec[my_id].begin(), thread_vec[my_id].end(), 
						thread_vec[my_id + 1].begin(), thread_vec[my_id + 1].end(), 
						data.begin() + ( (int)thread_vec[my_id].size() * my_id) );	
	}
	
	while(pow(2,p) < thread_count)
	{
		#pragma omp parallel num_threads( (int)(thread_count / pow(2,p) ) ) default(none) shared (thread_vec, data, p) private (my_id) 
		{
			my_id = omp_get_thread_num();
			if( my_id % 2 == 0)
			{
				int f_vec_pos = thread_vec[my_id].size() * pow(2,p) * my_id;
				int s_vec_pos = thread_vec[my_id].size() * pow(2,p) * (my_id + 1);
				int size = s_vec_pos - f_vec_pos;
				
				std::merge(
							data.begin() + f_vec_pos,
							data.begin() + f_vec_pos + size, 
							data.begin() + s_vec_pos, 
							data.begin() + s_vec_pos + size, 
							data.begin() + f_vec_pos
							);
			}
		}
		p++;
	}
	
	for (int i = 0; i < (int)data.size(); i++)
        printf("%d ", data[i]);
	
	cin >> n;
	return 0;
}

void sort(const vector<int>& array)
{ 
	std::sort(array.begin(), array.end() );
}

bool validaCntHilos(int ch) {
	return ch >= 1;
}

// ct es la cantidad de términos de la sumatoria
bool validaCntTerminos(long long ct) {
	return ct >= 1;
}
