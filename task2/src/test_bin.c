#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *bin, *res;
    uint8_t buf;
    uint8_t test = 0xFF;                        //0xFF = 11111111
    int check = 1;
    
    printf("Проверка на утечку данных\n");
    printf("Бинарный файл: %s\n", argv[1]);
    printf("Результат: %s\n", argv[2]);
    
    if((bin = fopen(argv[1], "rb")) == NULL)
    {
        fprintf(stderr, "Файл не найден\n");
        exit(EXIT_FAILURE);
    }
    if((res = fopen(argv[2], "wb")) == NULL)
    {
        fprintf(stderr, "Файл не найден\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; (fread(&buf, sizeof(uint8_t), 1, bin) == 1); i++)
    {
        fwrite(&buf, sizeof(uint8_t), 1, res);
        if((buf >> 6) == 3)
        {
            if(check == 1)
            {
                fwrite(&test, sizeof(uint8_t), 1, res);
                check = 0;
            }
            else
                check = 1;
        }
    }

    printf("Завершение\nЗапустите ./app decode %s res.txt\n", argv[2]);
    return 0;
}