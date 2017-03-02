#include <stdio.h>

#include <stdint.h>
#include <stdbool.h>

int main() {
    
    bool bbb = (0x2 != 0);
    printf("bool = %d\n", bbb);
    return 0;
    
    uint8_t x = 0;
    uint8_t y = 1;
    printf("~x = %x\n", ~x);
    printf("~y = %x\n", ~y);
    printf("!x = %x\n", !x);
    printf("!y = %x\n", !y);
    return 0;
}


