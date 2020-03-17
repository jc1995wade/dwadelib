

void byte_to_hexstr(const unsigned char* source, char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;
    for (i = 0; i < sourceLen; i++){
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;
        highByte += 0x30;
        if (highByte > 0x39)
                dest[i * 2] = highByte + 0x07;
        else
                dest[i * 2] = highByte;
        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
    return ;
}
