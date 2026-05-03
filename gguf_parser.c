#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <gguf_file>\n",argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1],"rb");
    if (!f)
    {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    uint32_t magic;
    fread(&magic, sizeof(uint32_t), 1, f);
    if (magic != 0x46554747) // "gguf" in little-endian
    {
        printf("Error: Invalid GGUF file (magic number mismatch)\n");
        fclose(f);
        return 1;
    }

    uint32_t version;
    fread(&version, sizeof(uint32_t),1,f);
    if (version < 2)
    {
        printf("Error: Unsupported GGUF version %u\n", version);
        return 1;
    }
    
    uint64_t n_tensors;
    fread(&n_tensors, sizeof(uint64_t),1,f);
    
    uint64_t n_kv;
    fread(&n_kv, sizeof(uint64_t),1,f);

    fclose(f);

    printf("GGUF file is valid.\n");
    printf("GGUF file version: %u\n", version);
    printf("Number of tensors: %llu\n", n_tensors);
    printf("Number of key-value pairs: %llu\n", n_kv);

    return 0;
}