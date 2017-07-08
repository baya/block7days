typedef struct s_block *t_block;

struct s_block {
  size_t  size;
  t_block next;
  int     free;
};


int main()
{
  t_block b;
  b = sbrk(0);
  size_t size = 100;

  sbrk(sizeof(struct s_block) + size);

  b->size = size;
}


t_block find_block(t_block *last, size_t size){
  
}
