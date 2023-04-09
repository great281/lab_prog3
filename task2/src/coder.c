#include "coder.h"

int encode(uint32_t code_point, CodeUnits *code_units)
{
    if(code_units == NULL)
        return -1;

    uint8_t res = 0x80;                                 //0x80 = 10000000

    if(code_point < 128)
    {
        code_units->length = 1;
        res = (res | code_point) & 0x7f;                //0x7F = 01111111
        code_units->code[0] = res;
    }
    else if(code_point < 2048)
    {
        code_units->length = 2;
        res = (res | code_point) & 0xBF;                //0xBF = 10111111
        code_units->code[1] = res;
        code_point >>= 6;
        res = (0xC0 | code_point) & 0xDF;               //0xC0 = 11000000    0xDF = 11011111
        code_units->code[0] = res;
    }
    else if(code_point < 65536)
    {
        code_units->length = 3;
        for(int i = 2; i > 0; i--)
        {
            res = (0xC0 | code_point) & 0xBF;
            code_units->code[i] = res;
            code_point >>= 6;
        }
        res = (0xE0 | code_point) & 0xEF;               //0xE0 = 11100000   0xEF = 11101111
        code_units->code[0] = res;
    }
    else if(code_point < 2097152)
    {
        code_units->length = 4;
        for(int i = 3; i > 0; i--)
        {
            res = (0xC0 | code_point) & 0xBF;
            code_units->code[i] = res;
            code_point >>= 6;
        }
        res = (0xF0 | code_point) & 0xF7;               //0xF0 = 11110000   0xF7 = 11110111
        code_units->code[0] = res;
    }
    else
        return -1;

    return 0;
}

uint32_t decode(const CodeUnits *code_unit)
{
    uint32_t res = 0;
    if(code_unit->length == 1)
    {
        res = code_unit->code[0];
    }
    else if(code_unit->length == 2)
    {
        res += code_unit->code[0] & 0x1F;                   //0x1F = 00011111
        res <<= 6;
        res += code_unit->code[1] & 0x3F;                   //0x3F = 00111111
    }
    else if(code_unit->length == 3)
    {
        res += code_unit->code[0] & 0xF;                    //0xF = 00001111
        res <<= 6;
        for(int i = 1; i < 3; i++)
        {
            res += code_unit->code[i] & 0x3F;
            if(i + 1 != 3)
                res <<= 6;
        }
    }
    else if(code_unit->length == 4)
    {
        res += code_unit->code[0] & 0x7;                    //0x7 = 00000111
        res <<= 6;
        for(int i = 1; i < 4; i++)
        {
            res += code_unit->code[i] & 0x3F;
            if(i + 1 != 4)
                res <<= 6;
        }
    }

    return res;
}

int read_next_code_unit(FILE *in, CodeUnits *code_units)
{
    uint8_t buf;
    int i, d, check = 0, shift;
    while(check == 0)
    {
        d = 0;
        shift = 0x80;
        code_units->length = 1;
        if(fread(&buf, sizeof(uint8_t), 1, in) != 1)
            return -1;
            
        for(i = 7; (buf & shift) == shift && i > 2; i--)
        {
            d++;
            shift = shift / 2;
        }
        
        if(d != 1 && i != 2)
        {
            code_units->code[0] = buf;
            check = 1;
        }
        else
        {
            check = 0;
            continue;
        }

        for(int g = 1; d > 1; g++)
        {
            if(fread(&buf, sizeof(uint8_t), 1, in) != 1)
                return -1;
            if((buf >> 7) != 1 || (buf & 0x40) == 0x40)             //0x40 == 01000000
            {
                check = 0;
                break;
            }
            else
            {
                code_units->code[g] = buf;
                code_units->length++;
                d--;
                check = 1;
            }
        }
    }

    return 0;
}

int write_code_unit(FILE *out, const CodeUnits *code_unit)
{
    if(code_unit == NULL)
        return -1;

    if(code_unit->length < 0 || code_unit->length > 4)
        return -1;

    if(fwrite(code_unit->code, sizeof(uint8_t), code_unit->length, out) != code_unit->length)
        return -1;

    return 0;
}