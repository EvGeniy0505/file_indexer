#include "file_index.h"
#include "hasher.h"

int main() 
{
    file_index_t index = index_ctor();
    const size_t fingerprint_size = 256;  

    index_add_file(&index, "./examples/file1.bin", fingerprint_size);
    index_add_file(&index, "./examples/file2.bin", fingerprint_size);
    
    index_add_file(&index, "./examples/file1.txt", fingerprint_size);
    index_add_file(&index, "./examples/file2.txt", fingerprint_size);

    index_add_file(&index, "./examples/file1.mp4", fingerprint_size);
    index_add_file(&index, "./examples/file2.mp4", fingerprint_size);

    find_similar(&index, "./examples/query.bin", 0.5f, fingerprint_size);
    find_similar(&index, "./examples/query.txt", 0.5f, fingerprint_size);
    find_similar(&index, "./examples/query.mp4", 0.5f, fingerprint_size);

    index_dtor(&index);

    return 0;
}