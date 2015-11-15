#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint8_t len;
    uint8_t arg[0];
} data_t;

void hal(data_t *data) {

    data_t *hal_data;
    printf("=======0===========\n");
    hal_data = data;
    printf("%x\n", hal_data->arg[0]); // 0xaa
    printf("%x\n", hal_data->arg[1]); // 0xbb
    printf("%x\n", hal_data->arg[2]); // 0xcc


    printf("=======1===========\n");
    hal_data = data + sizeof(data_t) + 3;
    printf("%x\n", hal_data->arg[0]); // 0xaa
    printf("%x\n", hal_data->arg[1]); // 0xbb
    printf("%x\n", hal_data->arg[2]); // 0xcc
    printf("%x\n", hal_data->arg[3]); // 0xcc

}

int main() {

    uint8_t buffer[255];
    uint8_t tmp[3] = {0x01, 0x02, 0x03};
    uint8_t tmp2[4] = {0x04, 0x05, 0x06, 0x07};

    data_t *ptr = (data_t *)buffer;
    ptr->len = 3;
    memcpy((uint8_t *)ptr + sizeof(data_t), tmp, 3);

    printf("%x\n", ptr->arg[0]); // 0xaa
    printf("%x\n", ptr->arg[1]); // 0xbb
    printf("%x\n", ptr->arg[2]); // 0xcc

    /***************************************/


    ptr = ptr + sizeof(data_t) + (sizeof(uint8_t) * 3);
    ptr->len = 4;
    memcpy((uint8_t *)ptr + sizeof(data_t), tmp2, 4);

    printf("%x\n", ptr->arg[0]); // 0xaa
    printf("%x\n", ptr->arg[1]); // 0xbb
    printf("%x\n", ptr->arg[2]); // 0xcc
    printf("%x\n", ptr->arg[3]); // 0xcc


    hal((data_t *)buffer);

    return 0;
}
#if 0
void hal(data_t **hal_data) {


    printf("%x\n", hal_data[1]->arg[0]); // 0xaa
    printf("%x\n", hal_data[1]->arg[1]); // 0xbb
    printf("%x\n", hal_data[1]->arg[2]); // 0xcc

    printf("%x\n", hal_data[2]->arg[0]); // 0xaa
    printf("%x\n", hal_data[2]->arg[1]); // 0xbb
    printf("%x\n", hal_data[2]->arg[2]); // 0xcc
}

int main() {

    data_t *hal_data[10];
    //data_t hal_data[10];

    uint8_t buffer[255];
    uint8_t tmp[3] = {0x01, 0x02, 0x03};

    data_t *ptr = (data_t *)buffer;
    ptr->len = 3;
    memcpy((uint8_t *)ptr + sizeof(data_t), tmp, 3);

    printf("%x\n", ptr->arg[0]); // 0xaa
    printf("%x\n", ptr->arg[1]); // 0xbb
    printf("%x\n", ptr->arg[2]); // 0xcc

    /***************************************/

    uint8_t buffer2[255];
    uint8_t tmp2[3] = {0x04, 0x05, 0x06};

    data_t *ptr2 = (data_t *)buffer2;
    ptr2->len = 3;
    memcpy((uint8_t *)ptr2 + sizeof(data_t), tmp2, 3);

    printf("%x\n", ptr2->arg[0]); // 0xaa
    printf("%x\n", ptr2->arg[1]); // 0xbb
    printf("%x\n", ptr2->arg[2]); // 0xcc
    hal_data[1] = ptr;
    hal_data[2] = ptr2;
    hal(hal_data);

    return 0;
}

#endif
