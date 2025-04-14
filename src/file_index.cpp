#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_index.h"

void index_add_file(file_index_t* index, const char* filepath, size_t fingerprint_size) 
{
    assert(index);
    assert(filepath);

    file_fingerprint_t fp = build_fingerprint(filepath, fingerprint_size);

    index->fingerprints = (file_fingerprint_t*)realloc(index->fingerprints, 
                          (index->file_count + 1) * sizeof(file_fingerprint_t));
    assert(index->fingerprints);
    
    index->fingerprints[index->file_count++] = fp;
}

file_index_t index_ctor()
{
    return (file_index_t) {NULL, 0}; 
}

void index_dtor(file_index_t* index) 
{
    assert(index);

    for (size_t i = 0; i < index->file_count; i++) 
    {
        free(index->fingerprints[i].min_hashes);
    }
    
    free(index->fingerprints);
    index->file_count = 0;
}

float calculate_jaccard_similarity(const file_fingerprint_t* a, const file_fingerprint_t* b) 
{
    assert(a);
    assert(b);

    size_t common = 0, i = 0, j = 0;

    while (i < a->count && j < b->count) 
    {
        if (a->min_hashes[i] == b->min_hashes[j]) 
        {
            common++;
            i++;
            j++;
        } 
        else if (a->min_hashes[i] < b->min_hashes[j]) 
            i++;
        else 
            j++;
    }

    // суммируем хэш файлов, вычитаем пересечение из формулы Жакара
    size_t total = a->count + b->count - common;   

    return total ? (float)common / (float)total : 0;
}

void find_similar(const file_index_t* index, const char* query_file, float threshold, size_t fingerprint_size) 
{
    assert(index);
    assert(query_file);

    file_fingerprint_t query = build_fingerprint(query_file, fingerprint_size);

    printf("Похожие на '%s' с порогом %.2f:\n", query_file, threshold);

    for (size_t i = 0; i < index->file_count; i++)
    {
        float similarity = calculate_jaccard_similarity(&query, &index->fingerprints[i]);
        if (similarity >= threshold && strcmp(query.filename, index->fingerprints[i].filename) != 0) 
            {
                printf("  %s \nCходство: %.2f%%\n", index->fingerprints[i].filename, similarity * 100);
            }
    }
    free(query.min_hashes);
}