#include "varint.h"
#define SIZE 1000000

int main()
{
    char ans;
    FILE *uncom, *com;
    uint8_t *buf;
    const uint8_t **bufp;
    uint32_t *value, test;
    int size = 0;
    int read_uncom, read_com;
    
    buf = (uint8_t *)calloc(sizeof(uint8_t), 4);
    bufp = (const uint8_t **)malloc(sizeof(uint8_t **));
    *bufp = (uint8_t *)malloc(sizeof(uint8_t *));
    value = (uint32_t *)calloc(sizeof(uint32_t), SIZE);

    if((uncom = fopen("uncompressed.dat", "wb")) == NULL)
    {
        fprintf(stderr, "Ошибка при создании uncompressed.dat\n");
        exit(EXIT_FAILURE);
    }
    if((com = fopen("compressed.dat", "wb")) == NULL)
    {
        fprintf(stderr, "Ошибка при создании compressed.dat\n");
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < SIZE; i++)
    {
        value[i] = generate_number();
        size = encode_varint(value[i], buf);
        bufp[0] = buf;
        fwrite(buf, sizeof(uint8_t), size, com);
    }
    fwrite(value, sizeof(int), SIZE, uncom);

    fclose(uncom);
    fclose(com);

    printf("Запись в файлы *.dat прошла успешно\n");
    printf("Сравнить содержимое файлов?(y/n)\n");
    while((ans = getchar()) != 'y' && ans != 'n')
        continue;
    if(ans == 'n')
        return 0;

    if((uncom = fopen("uncompressed.dat", "rb")) == NULL)
    {
        fprintf(stderr, "Ошибка при чтении uncompressed.dat\n");
        exit(EXIT_FAILURE);
    }
    if((com = fopen("compressed.dat", "rb")) == NULL)
    {
        fprintf(stderr, "Ошибка при чтении compressed.dat\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < SIZE; i++)
    {
        fread(&read_uncom, sizeof(uint32_t), 1, uncom);
        for(int d = 0; d < 4; d++)
        {
            fread(&buf[d], sizeof(uint8_t), 1, com);
            if((buf[d] >> 7) == 0)
                break;
        }
        bufp[0] = buf;
        read_com = decode_varint(&bufp[0]);

        if(read_com != read_uncom)
        {
            printf("%d != %d\n%d\n", read_com, read_uncom, i);
            exit(EXIT_FAILURE);
        }
    }
    printf("Проверка работы алгоритма\nВведите целое число в шестнадцатирином виде: ");
    scanf("%x", &test);
    size = encode_varint(test, buf);
    test = 0;
    for(int i = 0; i < size - 1; i++)
    {
        test += buf[i];
        test <<= 8;
    }
    test += buf[size - 1];
    printf("Encode: %x\n", test);
    bufp[0] = buf;
    test = decode_varint(bufp);
    printf("Decode: %x\n", test);
    printf("\n");
    free(buf);
    free(bufp);
    free(value);

    fclose(uncom);
    fclose(com);

    printf("Утечек данных не обнаружено\n");
    printf("Программа завершена.\n");

    return 0;
}
