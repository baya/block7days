#include <stdio.h>
#include <stdint.h>

int main()
{
  FILE *fp;
  unsigned int tx_version;

  
  uint8_t  tx_vin1;
  uint16_t tx_vin2;
  uint32_t tx_vin4;
  uint64_t tx_vin8;
  uint64_t tx_vin;

  fp = fopen("tx0.bin", "rb");
  fread(&tx_version, sizeof(tx_version), 1, fp);
  fread(&tx_vin1, sizeof(tx_vin1), 1, fp);
  if(tx_vin1 < 0xFD){
    tx_vin = tx_vin1;
  } else if(tx_vin1 == 0xFD){
    fread(&tx_vin2, sizeof(tx_vin2), 1, fp);
    tx_vin = tx_vin2;
  } else if(tx_vin1 == 0xFE) {
    fread(&tx_vin4, sizeof(tx_vin4), 1, fp);
    tx_vin = tx_vin4;
  } else if(tx_vin1 == 0xFF) {
    fread(&tx_vin8, sizeof(tx_vin8), 1, fp);
    tx_vin = tx_vin8;
  } else {
    tx_vin = 0;
  }

  printf("Tx Version: %u\n", tx_version);
  printf("Tx In-counter: %llu\n", tx_vin);
  
}
