#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100
#define MAX_THREADS 4

typedef struct
{
    int* array;
    int start;
    int end;
    int target;
    int* local_results;
    int* local_count;
    pthread_mutex_t* mutex;
} ThreadData;

void* search_thread(void* arg)
{
    ThreadData* data = (ThreadData*)arg;

    // Поиск элементов в своей части массива
    for (int i = data->start; i < data->end; i++)
    {
        if (data->array[i] == data->target)
        {
            pthread_mutex_lock(data->mutex);
            data->local_results[(*data->local_count)++] = i;
            pthread_mutex_unlock(data->mutex);
        }
    }

    return NULL;
}

int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

int main() {
    int a[] = {1, 2, 3, 3, 2, 2, 4, 5, 6, 6, 6, 66, 11, 22,22};
    int size = sizeof(a) / sizeof(a[0]);
    int target;
    int k = -1;
    int num_threads;

    printf("Введите число потоков (меньше 4) ");
    scanf("%d", &num_threads);

    if(num_threads > MAX_THREADS)
    {
        printf("ERROR\n");
        exit(EXIT_FAILURE);
    }

    printf("Массив: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\nВведите элемент для поиска: ");
    scanf("%d", &target);

    // Инициализация мьютекса
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // Выделение памяти для результатов каждого потока
    int all_results[MAX_THREADS][MAX_SIZE];
    int counts[MAX_THREADS] = {0};
    int final_results[MAX_SIZE];
    int final_count = 0;

    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];

    // Создание потоков
    int chunk_size = size / num_threads;
    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].array = a;
        thread_data[i].start = i * chunk_size;
        if (i == num_threads - 1)
        {
            thread_data[i].end = size;
        } else
        {
            thread_data[i].end = (i + 1) * chunk_size;
        }
        thread_data[i].target = target;
        thread_data[i].local_results = all_results[i];
        thread_data[i].local_count = &counts[i];
        thread_data[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, search_thread, &thread_data[i]);
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Сбор результатов из всех потоков
    for (int i = 0; i < num_threads; i++)
    {
        for (int j = 0; j < counts[i]; j++)
        {
            final_results[final_count++] = all_results[i][j];
        }
    }

    // Сортировка результатов с помощью qsort
    qsort(final_results, final_count, sizeof(int), compare_ints);

    // Вывод результатов
    if (final_count == 0)
    {
        printf("Элемент не найден %d\n", k);
    } else
    {
        printf("Найдено %d вхождений:\n", final_count);
        for (int i = 0; i < final_count; i++)
        {
            printf("Индекс: %d\n", final_results[i]);
        }
    }

    // Очистка ресурсов
    pthread_mutex_destroy(&mutex);

    return 0;
}
