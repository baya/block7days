#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct aa {
  int value;
} aa;

void set_value(aa item);
void set_value2(aa *ptr_item);

int main()
{
  aa item;
  set_value(item);
  printf("++++++++++%d\n", item.value);

  set_value2(&item);
  printf("?????????%d\n", item.value);
}

void set_value(aa item)
{
  item.value = 1;
}

void set_value2(aa *ptr_item)
{
  ptr_item -> value = 3;
}
