#ifndef FILE_INDEX_H
#define FILE_INDEX_H

#include "hasher.h"

/**
 * @brief Структура для хранения отпечатков файлов
 */
typedef struct 
{
    file_fingerprint_t* fingerprints;
    size_t file_count;
} file_index_t;

/**
 * @brief Добавляет файл в структуру индексов
 * 
 * @param index Указатель на структуру индекса
 * @param filepath Путь к файлу для индексации
 * @param fingerprint_size Количество минимальных хешей для сохранения в отпечатке
 * 
 * @note Функция вычисляет MinHash-отпечаток файла и сохраняет его в индекс.
 * Значение fingerprint_size - от 64 до 256 для баланса точности. 
 */
void index_add_file(file_index_t* index, const char* filepath, size_t fingerprint_size);

/**
 * @brief Освобождает ресурсы, занятые индексом
 * 
 * @return file_index_t Струтура для хранения отпечатков файлов
 */
file_index_t index_ctor();

/**
 * @brief Освобождает ресурсы, занятые индексом
 * 
 * @param index Указатель на структуру индекса
 * 
 * @note Функция освобождает память, выделенную для всех отпечатков в индексе,
 * но не удаляет сам индекс (его нужно освободить отдельно).
 */
void index_dtor(file_index_t* index);

/**
 * @brief Вычисляет коэффициент Жаккара для двух отпечатков файлов
 * 
 * @param a Указатель на первый отпечаток
 * @param b Указатель на второй отпечаток
 * @return float Коэффициент сходства в диапазоне [0.0, 1.0]
 * 
 * @note Возвращает 0.0 если хотя бы один из отпечатков пуст.
 * Коэффициент 1.0 означает идентичные файлы, 0.0 - полностью разные.
 * Хоть в задаче файлы должны быть одинакового размера, 
 * но программа работает и для файлов любых размеров
 */
float calculate_jaccard_similarity(const file_fingerprint_t* a, const file_fingerprint_t* b);

/**
 * @brief Находит файлы в индексе, похожие на заданный файл
 * 
 * @param index Указатель на индекс для поиска
 * @param query_file Путь к файлу-запросу
 * @param threshold Порог сходства (от 0.0 до 1.0)
 * @param fingerprint_size Количество хешей для отпечатка файла-запроса
 * 
 * @note Функция выводит на экран список файлов из индекса, чей коэффициент
 * Жаккара с файлом-запросом >= threshold.
 * 
 * @example Пример вызова:
 * @code
 * find_similar(&index, "file.bin", 0.8f, 64); // Найти файлы сходные на 80%+
 * @endcode
 */
void find_similar(const file_index_t* index, const char* query_file, 
                  float threshold, size_t fingerprint_size);

#endif  // FILE_INDEX_H