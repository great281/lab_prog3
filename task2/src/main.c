#include "command.h"

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        printf("Использование:\n./coder encode <in-file-name> <out-file-name>\n./coder decode <in-file-name> <out-file-name>\n");
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1], "encode") == 0)
    {
        if(encode_file(argv[2], argv[3]) != 0)
            exit(EXIT_FAILURE);
        printf("Данные из файла %s закодированы в бинарный файл %s\nПрограмма завершена.\n", argv[2], argv[3]);
    }
    else if(strcmp(argv[1], "decode") == 0)
    {
        if(decode_file(argv[2], argv[3]) != 0)
            exit(EXIT_FAILURE);
        printf("Данные из бинарного файла %s декодированы в файл %s\nПрограмма завершена.\n", argv[2], argv[3]);
    }
    else
    {
        fprintf(stderr, "Некорректный ввод\nВведите decode или encode вместо %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}