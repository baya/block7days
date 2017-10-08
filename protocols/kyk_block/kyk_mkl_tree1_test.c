#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kyk_utils.h"
#include "kyk_mkl_tree.h"

/* 数据来源于: https://webbtc.com/block/000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f.json */
/* mrkl_tree: [ */
/* "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b" */
/* ] */

void build_tx_buf_from_hex(struct kyk_tx_buf *tx_buf, const char *hexstr);

int main()
{
    char *tx1_hex = "01000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000";
    struct kyk_tx_buf buf_list[1];
    struct kyk_mkltree_level *leaf_level;
    struct kyk_mkltree_level *root_level;

    build_tx_buf_from_hex(buf_list, tx1_hex);
    leaf_level = create_mkl_leafs(buf_list, sizeof(buf_list) / sizeof(buf_list[0]));
    root_level = create_mkl_tree(leaf_level);

    kyk_print_mkl_tree(root_level);

}


void build_tx_buf_from_hex(struct kyk_tx_buf *tx_buf, const char *hexstr)
{
    tx_buf -> bdy = kyk_alloc_hex(hexstr, &tx_buf -> len);    
}




    











