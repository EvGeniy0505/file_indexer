#ifndef HASHER_H
#define HASHER_H

#include "blake3.h"  

/**
 * @brief 32-битное значение хеша для блока данных
 */
typedef uint32_t block_hash_t;

/**
 * @brief Структура отпечатка файла, содержащая MinHash
 * 
 * Хранит минимальные хеши блоков файла для сравнения схожести
 */
typedef struct 
{
    block_hash_t* min_hashes;
    size_t count;
    char filename[256];
} file_fingerprint_t;

/**
 * @brief Хэширует данных файла на основе blake3 
 * 
 * @param data Указатель на блок данных
 * @param size Размер блока данных в байтах
 * @return block_hash_t Первые 32 бита хеша BLAKE3
 * 
 * @see https://github.com/BLAKE3-team/BLAKE3/blob/master/c/README.md
 */
block_hash_t hash_block(const uint8_t* data, size_t size);

/**
 * @brief Сравнивает значений хешей
 * 
 * Используется qsort() для сортировки массива хешей по возрастанию
 * 
 * @param a Указатель на первое значение хеша
 * @param b Указатель на второе значение хеша
 * @return int Результат сравнения (<0, 0, >0)
 */
int compare_hashes(const void* a, const void* b);

/**
 * @brief Вычисляет хеши для всех блоков файла. 
 * Массив хэшей это и будет отпечаток файла
 * 
 * Разбивает файл на блоки относительно размера файла и вычисляет BLAKE3 хеш для каждого
 * 
 * @param filepath Путь к исходному файлу
 * @param hash_count[out] Количество вычисленных хешей
 * @return block_hash_t* Массив хешей блоков (нужно освободить)
 * @retval NULL если файл не может быть прочитан
 */
block_hash_t* calculate_file_hashes(const char* filepath, size_t* hash_count);


/**
 * @brief Выбирает k минимальных хешей из отсортированного массива
 * 
 * Создает отпечаток, выбирая наименьшие хеши
 * 
 * @param all_hashes Отсортированный массив хешей блоков
 * @param hash_count Количество хешей в массиве
 * @param fingerprint_size Количество минимальных хешей для выбора
 * @param filename Имя исходного файла для идентификации
 * @return file_fingerprint_t Результирующая структура отпечатка
 * 
 * @see https://habr.com/ru/articles/115147/
 */
file_fingerprint_t min_hashes(const block_hash_t* all_hashes, const size_t hash_count, 
                              const size_t fingerprint_size, const char* filename);

/**
 * @brief Строит отпечаток файла с использованием MinHash
 * 
 * Объединяет операции calculate_file_hashes() и min_hashes()
 * 
 * @param filepath Путь к исходному файлу
 * @param  Количество минимальных хешей для отпечатка
 * @return file_fingerprint_t Вычисленная структура отпечатка
 */
file_fingerprint_t build_fingerprint(const char* filepath, size_t fingerprint_size);

/**
 * @brief Вычисляем оптимальный размер блока
 * @param filepath Путь к файлу
 * @param min_size Минимальный размер в байтах 
 * @return size_t оптимальный размер
 */
size_t optimal_block_size(const char* filename);

#endif  // HASHER_H