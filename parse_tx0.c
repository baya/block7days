#include <stdio.h>

int main()
{
  FILE *fp;
  unsigned int tx_version;

  fp = fopen("tx0.bin", "rb");
  fread(&tx_version, sizeof(tx_version), 1, fp);

  printf("Tx Version: %u\n", tx_version);
}
