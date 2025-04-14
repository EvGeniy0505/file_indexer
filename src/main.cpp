#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "isa-l.h"   

#include "file_index.h"
#include "hasher.h"


int main() {
    file_index_t index = {NULL, 0};
    const size_t k = 256;  // Размер отпечатка

    index_add_file(&index, "./examples/file1.bin", k);
    index_add_file(&index, "./examples/file2.bin", k);
    
    index_add_file(&index, "./examples/file1.txt", k);
    index_add_file(&index, "./examples/file2.txt", k);

    index_add_file(&index, "./examples/file1.mp4", k);
    index_add_file(&index, "./examples/file2.mp4", k);

    find_similar(&index, "./examples/query.bin", 0.5f, k);
    find_similar(&index, "./examples/query.txt", 0.5f, k);
    find_similar(&index, "./examples/query.mp4", 0.5f, k);

    index_dtor(&index);

    return 0;
}