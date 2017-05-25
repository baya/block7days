#include <stdio.h>

typedef struct fread_arg
{
    void *buffer;
    size_t size;
    size_t count;
    FILE *stream;
} fread_arg;

void tx_fread(fread_arg *arg);

int main()
{
    unsigned int tx_v;
    unsigned char tx_vin;
    FILE *fp;
    fread_arg tx_arg, tx_arg1;

    fp = fopen("tx0.bin", "rb");
    tx_arg.buffer = &tx_v;
    tx_arg.size = sizeof(tx_v);
    tx_arg.count = 1;
    tx_arg.stream = fp;


    
    // size_t ret_code = fread(&tx_v, sizeof(tx_v), 1, fp);
    /* printf("sizeof tx_v: %lu\n", sizeof(tx_v)); */
    /* if(ret_code == 1){ */
    /* 	printf("Tx Version: %u\n", tx_v); */
    /* } else { */
    /* 	if (feof(fp)) */
    /* 	    printf("Error reading tx_v.bin: unexpected end of file\n"); */
    /* 	else if (ferror(fp)) { */
    /* 	    perror("Error reading tx_v.bin"); */
    /* 	} */
    /* } */

    tx_fread(&tx_arg);
    
    printf("Tx Version: %u\n", tx_v);

    tx_arg1.buffer = &tx_vin;
    tx_arg1.size = sizeof(tx_vin);
    tx_arg1.count = 1;
    tx_arg1.stream = fp;

    tx_fread(&tx_arg1);
    printf("Tx Inputs: %u\n", tx_vin);


    fclose(fp);
}

void tx_fread(fread_arg *arg)
{
    size_t ret_code;
    size_t count;
    FILE *fp = arg->stream;
    
    count = sizeof(arg->count);
    ret_code = fread(arg->buffer, arg->size, arg->count, fp);
    if(ret_code == count){
    } else {
	if (feof(fp))
	    printf("Error reading tx_v.bin: unexpected end of file\n");
	else if (ferror(fp)) {
	    perror("Error reading tx_v.bin");
	}
    }

}
