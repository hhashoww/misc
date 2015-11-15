#include <stdio.h>

typedef struct xxx HCI_Tx_s;
/*
struct xxx {
    unsigned char Type;
    unsigned short int Opcode;
    unsigned char DataLength;
    unsigned char *data;
} __attribute__ ((__packed__));  
*/

struct xxx {
    unsigned char Type;
    unsigned short int Opcode;
    unsigned char DataLength;
    unsigned char *data;
};  

int main() {
    HCI_Tx_s data;
    printf("%ld\n", sizeof(data));
    return 0;
}
