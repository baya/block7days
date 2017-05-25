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
    FILE *fp;
    fread_arg tx_arg;

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

    fclose(tx_arg.stream);
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
