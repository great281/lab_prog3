#include "command.h"

int encode_file(const char *in_file_name, const char *out_file_name)
{
    if(in_file_name == NULL || out_file_name == NULL)
        return -1;
    
    FILE *text, *bin;
    CodeUnits code_unit;
    uint32_t code_point;

    if((text = fopen(in_file_name, "rb")) == NULL)
    {
        fprintf(stderr, "Ошибка при чтении файла %s\n", in_file_name);
        return -1;
    }
    if((bin = fopen(out_file_name, "wb")) == NULL)
    {
        fprintf(stderr, "Ошибка при создании бинарного файла %s\n", out_file_name);
        return -1;
    }

    while(1)
    {
        if(fscanf(text, "%" SCNx32, &code_point) != 1)
        {
            printf("Достигнут конец файла %s\n", in_file_name);
            break;
        }
        
        if(encode(code_point, &code_unit) != 0)
        {
            fprintf(stderr, "Ошибка функции encode\n");
            return -1;
        }

        if(write_code_unit(bin, &code_unit) != 0)
        {
            fprintf(stderr, "Ошибка записи в бинарный файл\n");
            return -1;
        }
        
    }
    fclose(text);
    fclose(bin);

    return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name)
{
    if(in_file_name == NULL || out_file_name == NULL)
        return -1;

    FILE *text, *bin;
    CodeUnits code_unit;
    uint32_t code_point;

    if((bin = fopen(in_file_name, "rb")) == NULL)
    {
        fprintf(stderr, "Ошибка при чтении бинарного файла %s\n", in_file_name);
        return -1;
    }
    if((text = fopen(out_file_name, "wb")) == NULL)
    {
        fprintf(stderr, "Ошибка при создании файла %s\n", out_file_name);
        return -1;
    }

    while(read_next_code_unit(bin, &code_unit) == 0)
    {
        code_point = decode(&code_unit);

        if(fprintf(text, "%" PRIx32, code_point) < 1)
        {
            fprintf(stderr, "Ошибка записи в файл %s\n", out_file_name);
            return -1;
        }
        fprintf(text, "\n");
    }

    printf("Достигнут конец файла %s\n", in_file_name);
    fclose(bin);
    fclose(text);

    return 0;
}