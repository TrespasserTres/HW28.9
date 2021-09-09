#include<iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <future>

using namespace std;

void print(int *arr,int size)
{
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
}

void merge(int* arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;
    // создаем временные массивы
    int* left = new int[nl];
    int* right = new int[nr];

    // копируем данные во временные массивы
    for (int i = 0; i < nl; i++)
        left[i] = arr[l + i];
    for (int j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;  // начало левой части

    while (i < nl && j < nr) 
    {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j]) 
        {
            arr[k] = left[i];
            i++;
        }
        else 
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    // записываем оставшиеся элементы левой части
    while (i < nl) {
        arr[k++] = left[i++];
    }
    // записываем оставшиеся элементы правой части
    while (j < nr) {
        arr[k++] = right[j++];
    }
    delete[] left;
    delete[] right;
}

void mergesort(int* arr, int l, int r, bool n)
{
    if (l >= r)
        return;
    int m = (l + r - 1) / 2;
    if (n && (l-m > 100000))
    {
        // если элементов в левой части больше чем 10000
        // вызываем асинхронно рекурсию для правой части
        auto f = async(launch::async, [&]()
            {
                mergesort(arr, l, m, n);
            });
        mergesort(arr, m + 1, r, n);
    }
    else 
    {
        mergesort(arr, l, m, n);
        mergesort(arr, m + 1, r, n);
    }
    merge(arr, l, m, r);
}

int main(int* arrgc, char const * argv[])
{
    int N = 100000000;                                    // длинна массива
    int* rand_arr = new int[N];
    bool treadsn = false;
    srand(time(nullptr));                              // используем текущее время, чтобы сгенерировать рандомные значения
    int lef_border = 0;                                // левая граница
    int range_len = 99;                                // правая граница = range_len + left_border
    for (int i = 0; i < N; i++)
        rand_arr[i] = lef_border + rand() % range_len; // генерируем число в указанном диапазоне и записываем в массив
    //print(rand_arr, N);
    //Сортируем без деления на потоки 
    auto start = chrono::high_resolution_clock::now(); // сохраняем время начала работы алгоритма
    mergesort(rand_arr, 0, N - 1, treadsn);
    auto finish = chrono::high_resolution_clock::now(); // сохраняем время конца работы алгоритма
    chrono::duration<double> elapsed = finish - start;  // вычисляем продолжительность работы в сек. и выводим
    cout << "Elapsed time whith out threads: " << elapsed.count() << " sec" << endl;
    //print(rand_arr, N);
    //Сортируем c делением на потоки 
    treadsn = true;
    for (int i = 0; i < N; i++)
        rand_arr[i] = lef_border + rand() % range_len; // генерируем число в указанном диапазоне и записываем в массив
    //print(rand_arr, N);
    start = chrono::high_resolution_clock::now(); // сохраняем время начала работы алгоритма
    mergesort(rand_arr, 0, N - 1, treadsn);
    //print(rand_arr, N);
    finish = chrono::high_resolution_clock::now(); // сохраняем время конца работы алгоритма
    elapsed = finish - start;  // вычисляем продолжительность работы в сек. и выводим
    cout << "Elapsed time using threads: " << elapsed.count() << " sec" << endl;
    //print(rand_arr, N);
    delete[] rand_arr;
    return 0;
}
