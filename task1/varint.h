#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

size_t encode_varint(uint32_t value, uint8_t* buf);

uint32_t decode_varint(const uint8_t** bufp);

uint32_t generate_number();