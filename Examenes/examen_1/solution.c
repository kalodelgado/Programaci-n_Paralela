#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
		size_t x;
		size_t y;
	} position_t;

typedef struct {
		size_t thread_count;
		size_t rows;
		size_t colums;
		pthread_mutex_t mutex;
		char** mat;
		size_t perimetro;
		size_t area;
		position_t pos_inicial;
		position_t pos_final;
	}shared_data_t;

typedef struct {
		size_t thread_id;
		shared_data_t* shared_data;
	}thread_data_t;

size_t calculate_start(size_t rows, size_t thread_id, size_t thread_count){
	return (rows / thread_count * thread_id);
}

size_t calculate_finish(size_t rows, size_t thread_id, size_t thread_count){
	if (thread_id == thread_count -1)
		return rows;
	return (rows / thread_count * (thread_id + 1) );
}

size_t calc_perimetro(size_t x_1, size_t y_1, size_t x_2, size_t y_2){
	size_t p = 0;	
	for(size_t i = x_1; i <= x_2; ++i)
		for(size_t j = y_1; j <= y_2; ++j)
		{
			if ( !( i != x_1 && i != x_2 && j != y_1 && j != y_2) )
				++p;
		}
	return p;
}

void* run(void* data){
	thread_data_t* info = (thread_data_t*)data;
	shared_data_t* sh_data = info->shared_data;
	
	size_t start = calculate_start(sh_data->rows, info->thread_id, sh_data->thread_count);
	size_t end = calculate_finish(sh_data->rows, info->thread_id, sh_data->thread_count);
	
	size_t perimetro = 0;
	
	for(size_t row = start; row < end; ++row)
	{
		for (size_t colum = 0; colum < sh_data->colums; ++colum)
		{
			if (sh_data->mat[row][colum] != 'x')
			{
				position_t temp_position_i;
				position_t temp_position_f;
				
				temp_position_i.x = row;
				temp_position_i.y = colum;
				
				size_t row_1 = row + 1;
					
				while(row_1 < sh_data->rows)
				{
					if(sh_data->mat[row_1][colum] != 'x')
					{
						//Se mueve por las columnas, hacia la derecha
						size_t colum_1 = temp_position_i.y + 1;
						while(colum_1 < sh_data->colums)
						{
							if(sh_data->mat[row_1][colum_1] != 'x')
							{
								temp_position_f.x = row_1;
								temp_position_f.y = colum_1;
								
								//Sube por las filas
								row_1 = temp_position_f.x - 1;
								while(sh_data->mat[row_1][colum_1] != 'x' && row_1 != temp_position_i.x)
								{
									--row_1;
								}
											
								if(row_1 == temp_position_i.x)
								{
									if(sh_data->mat[row_1][temp_position_f.y] != 'x')
									{
										//Se mueve hacia la izquierda
										colum_1 = temp_position_f.y - 1;
										while(sh_data->mat[row_1][colum_1] != 'x' && colum_1 != temp_position_i.y)
										{
											--colum_1;
										}
										
										if (colum_1 == temp_position_i.y)
										{
											perimetro = calc_perimetro(temp_position_i.x, temp_position_i.y, temp_position_f.x, temp_position_f.y);
											
											pthread_mutex_lock(&sh_data->mutex);
											if( perimetro > sh_data->perimetro)
											{
												sh_data->perimetro = perimetro;
												
												sh_data->pos_inicial.x = temp_position_i.x;
												sh_data->pos_inicial.y = temp_position_i.y;
												
												sh_data->pos_final.x = temp_position_f.x;
												sh_data->pos_final.y = temp_position_f.y;
											}
											pthread_mutex_unlock(&sh_data->mutex);
										}
									}
								}
								colum_1 = temp_position_f.y + 1;
								row_1 = temp_position_f.x;
							}
							else
							{
								colum_1 = sh_data->colums;
							}
						}
					row_1++;
					}
					else
					{
						row_1 = sh_data->rows;
					}
				}
			}
		}
	}
	
	return NULL;
}	

int main ()
{
	shared_data_t shared_data;
	
	scanf("%zu %zu\n", &shared_data.rows, &shared_data.colums);
			
	shared_data.mat = (char**)malloc( shared_data.rows * sizeof(char*) );	
	for (size_t index = 0; index < shared_data.rows; ++index)
		shared_data.mat[index] = (char*)malloc( shared_data.colums * sizeof(char) );
	
	for (size_t row = 0; row < shared_data.rows; ++row)
	{
		for (size_t col = 0; col < shared_data.colums; ++col)
			scanf("%c", &shared_data.mat[row][col]);
		scanf("\n");
	}
	
	shared_data.perimetro = 0;
	shared_data.thread_count = sysconf(_SC_NPROCESSORS_ONLN);
	pthread_mutex_init(&shared_data.mutex, NULL);
	
	if(shared_data.rows < shared_data.thread_count)
		shared_data.thread_count = shared_data.rows;
	
	pthread_t* threads = (pthread_t*)malloc( shared_data.thread_count * sizeof(pthread_t) );
	thread_data_t* thread_data = (thread_data_t*)malloc( shared_data.thread_count * sizeof(thread_data_t) );
	
	for(size_t index = 0; index < shared_data.thread_count; ++index){
		thread_data[index].thread_id = index;
		thread_data[index].shared_data = &shared_data;
		
		pthread_create(&threads[index], NULL, run, &thread_data[index]);
	}
	
	for(size_t index = 0; index < shared_data.thread_count; ++index)
		pthread_join(threads[index], NULL);
	
	if(shared_data.perimetro != 0)
		printf("%zu (%zu,%zu)-(%zu,%zu)\n"
			, shared_data.perimetro, shared_data.pos_inicial.x + 1, shared_data.pos_inicial.y + 1, shared_data.pos_final.x + 1, shared_data.pos_final.y + 1);
	else
		printf("impossible\n");
		
	pthread_mutex_destroy(&shared_data.mutex);
	free(thread_data);
	free(threads);
	free(shared_data.mat);

	return 0;
}
