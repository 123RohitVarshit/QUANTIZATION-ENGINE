#include <stdio.h>
#include <stdint.h>

int read_string(FILE *f, char *buffer , size_t buffer_size)
{
    uint64_t str_len;
    if (fread(&str_len,sizeof(uint64_t),1,f)!=1)
    {
        printf("Error: could not read string length from file\n");
        return 0;
    }
    if (str_len >= buffer_size)
    {
        printf("Error: string length exceed buffer size\n");
        return 0;
    }
    if (fread(buffer,sizeof(char),str_len,f)!=str_len)
    {
        printf("Error: could not read string from file\n");
        return 0;
    }
    buffer[str_len]='\0';

    return 1;
}


int skip_value(FILE*f , uint32_t value_type)
{
    switch(value_type)
    {

        case 0: fseek(f, 1, SEEK_CUR); break;  // uint8
        case 1: fseek(f, 1, SEEK_CUR); break;  // int8
        case 2: fseek(f, 2, SEEK_CUR); break;  // uint16
        case 3: fseek(f, 2, SEEK_CUR); break;  // int16
        case 4: fseek(f, 4, SEEK_CUR); break;  // uint32
        case 5: fseek(f, 4, SEEK_CUR); break;  // int32
        case 6: fseek(f, 4, SEEK_CUR); break;  // float32
        case 7: fseek(f, 1, SEEK_CUR); break;  // bool
        case 8: {                               // string
            char tmp[4096];
            read_string(f, tmp, sizeof(tmp));
            break;
        }
        case 9: {  // array
        uint32_t arr_type;
        uint64_t arr_count;
        fread(&arr_type, sizeof(uint32_t), 1, f);
        fread(&arr_count, sizeof(uint64_t), 1, f);
        for (uint64_t j = 0; j < arr_count; j++) {
            skip_value(f, arr_type);  // recursively skip each element
        }
        break;
       }
        case 10: fseek(f, 8, SEEK_CUR); break; // uint64
        case 11: fseek(f, 8, SEEK_CUR); break; // int64
        case 12: fseek(f, 8, SEEK_CUR); break; // float64
        default:
            printf("Error: unknown value type %d\n",value_type);
            return 0;
    }
    return 1;
}

void read_metadata(FILE *f, uint64_t n_kv)
{
    for (uint64_t i=0;i<n_kv;i++)
    {
        char key[4096];
        if (!read_string(f,key,sizeof(key)))
        {
            printf("Error: could not read metadata key from file\n");
            return;
        }
        uint32_t value_type;
        if (fread(&value_type,sizeof(uint32_t),1,f)!=1)
        {
            printf("Error: could not read metadata value type from file\n");
            return;
        }
        printf("Metadata key: %s, value type: %d\n",key,value_type);
        if (!skip_value(f,value_type))
        {
            printf("Error: could not skip metadata value from file\n");
            return;
        }
    }
}

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

    printf("GGUF file is valid.\n");
    printf("GGUF file version: %u\n", version);
    printf("Number of tensors: %llu\n", n_tensors);
    printf("Number of key-value pairs: %llu\n", n_kv);

    read_metadata(f,n_kv);
    fclose(f);    

    return 0;
}
