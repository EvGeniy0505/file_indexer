#include <stdio.h> 
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "hasher.h"  

block_hash_t hash_block(const uint8_t* data, size_t size) 
{
    uint8_t hash[BLAKE3_OUT_LEN] = {};
    blake3_hasher hasher = {};
    
    blake3_hasher_init(&hasher);
    blake3_hasher_update(&hasher, data, size);
    blake3_hasher_finalize(&hasher, hash, BLAKE3_OUT_LEN);

    return *(block_hash_t*)hash;
}

int compare_hashes(const void* a, const void* b) 
{
    block_hash_t ha = *(block_hash_t*)a;
    block_hash_t hb = *(block_hash_t*)b;

    return (ha < hb) ? -1 : (ha > hb) ? 1 : 0;
}

block_hash_t* calculate_file_hashes(const char* filepath, size_t* hash_count)
{
    FILE* file = fopen(filepath, "rb");

    if (!file) 
    {
        fprintf(stderr, "Failed to open file\n");
        return 0;
    }

    const size_t block_size = optimal_block_size(filepath);  // считаем хэш 1 МБ файла

    uint8_t* block = (uint8_t*) calloc(block_size, 1);
    assert(block);

    block_hash_t* all_hashes = NULL;
    size_t count = 0;

    while (fread(block, 1, block_size, file) == block_size) 
    {
        block_hash_t block_hash = hash_block(block, block_size);
        
        block_hash_t* temp = (block_hash_t*)realloc(all_hashes, (count + 1) * sizeof(block_hash_t));

        if (!temp) // можно assert, но надо закрыть файл и очистить память
        {
            free(all_hashes);
            free(block);
            fclose(file);
            return 0;
        }

        all_hashes = temp;
        all_hashes[count++] = block_hash;
    }

    free(block);
    fclose(file);
    
    *hash_count = count;
    return all_hashes;
}

file_fingerprint_t min_hashes(const block_hash_t* all_hashes, size_t hash_count, size_t k, const char* filename) 
{
    file_fingerprint_t fp = {NULL, 0};
    
    if (!all_hashes || hash_count == 0) 
    {
        return fp;
    }

    block_hash_t* sorted_hashes = (block_hash_t*) calloc(hash_count, sizeof(block_hash_t));
    assert(sorted_hashes);

    memcpy(sorted_hashes, all_hashes, hash_count * sizeof(block_hash_t));

    qsort(sorted_hashes, hash_count, sizeof(block_hash_t), compare_hashes);

    size_t selected = 0;    

    if(k < hash_count) 
    {
        selected = k;
    } 
    else
    { 
        selected = hash_count;
    }
    
    fp.min_hashes = (block_hash_t*)malloc(selected * sizeof(block_hash_t));
    if (!fp.min_hashes) 
    {
        free(sorted_hashes);
        return fp;
    }

    memcpy(fp.min_hashes, sorted_hashes, selected * sizeof(block_hash_t));
    fp.count = selected;

    strncpy(fp.filename, filename, sizeof(fp.filename) - 1);
    fp.filename[sizeof(fp.filename) - 1] = '\0';

    free(sorted_hashes);

    return fp;
}

file_fingerprint_t build_fingerprint(const char* filepath, size_t k) 
{
    size_t hash_count = 0;

    block_hash_t* all_hashes = calculate_file_hashes(filepath, &hash_count);
    if (!all_hashes) 
    {
        file_fingerprint_t empty = {NULL, 0};
        return empty;
    }

    file_fingerprint_t fp = min_hashes(all_hashes, hash_count, k, filepath);
    
    free(all_hashes);

    return fp;
}

size_t optimal_block_size(const char* filename) 
{
    struct stat st;

    stat(filename, &st);
    size_t file_size = st.st_size;

    if (file_size < 1024 * 1024) 
        return 1;                
    else if (file_size < 100 * 1024 * 1024) 
        return 12 * 1024;               
    else 
        return 1024 * 1024;     
}