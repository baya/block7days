#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

#include "kyk_sha.h"
#include "kyk_utils.h"
#include "beej_pack.h"

#define MKL_NODE_BODY_LEN 32


enum mkltree_node_type {
    ROOT_T,
    BR_T,
    LEAF_T
};

struct kyk_mkltree_node{
    struct kyk_mkltree_node *chid_lft;
    struct kyk_mkltree_node *chid_rgt;
    struct kyk_mkltree_node *pnt;
    uint8_t bdy[MKL_NODE_BODY_LEN];
    enum mkltree_node_type ntype;
};

struct kyk_mkltree_level{
    struct kyk_mkltree_node *nd;
    size_t len;
    uint8_t inx;
};

struct kyk_tx_buf {
    uint8_t *bdy;
    size_t len;
};

void kyk_init_mkltree_node(struct kyk_mkltree_node *nd);
void build_tx_buf_from_hex(struct kyk_tx_buf *tx_buf, const char *hexstr);
void free_tx_buf_list(struct kyk_tx_buf *buf_list, size_t len);
void kyk_set_mkl_leaf(struct kyk_mkltree_node *nd, struct kyk_tx_buf buf);
struct kyk_mkltree_level *create_mkl_leafs(struct kyk_tx_buf *buf_list, size_t len);

int main()
{
    char *tx1_hex = "01000000010000000000000000000000000000000000000000000000000000000000000000ffffffff0704ffff001d0102ffffffff0100f2052a01000000434104d46c4968bde02899d2aa0963367c7a6ce34eec332b32e42e5f3407e052d64ac625da6f0718e7b302140434bd725706957c092db53805b821a85b23a7ac61725bac00000000";
    char *tx2_hex = "0100000001c997a5e56e104102fa209c6a852dd90660a20b2d9c352423edce25857fcd3704000000004847304402204e45e16932b8af514961a1d3a1a25fdf3f4f7732e9d624c6c61548ab5fb8cd410220181522ec8eca07de4860a4acdd12909d831cc56cbbac4622082221a8768d1d0901ffffffff0200ca9a3b00000000434104ae1a62fe09c5f51b13905f07f06b99a2f7159b2225f374cd378d71302fa28414e7aab37397f554a7df5f142c21c1b7303b8a0626f1baded5c72a704f7e6cd84cac00286bee0000000043410411db93e1dcdb8a016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5cb2e0eaddfb84ccf9744464f82e160bfa9b8b64f9d4c03f999b8643f656b412a3ac00000000";
    struct kyk_tx_buf buf_list[2];
    struct kyk_mkltree_level *mkl_level1;

    build_tx_buf_from_hex(buf_list, tx1_hex);
    build_tx_buf_from_hex(buf_list+1, tx2_hex);
    mkl_level1 = create_mkl_leafs(buf_list, 2);

    for(int i=0; i < mkl_level1 -> len; i++){
	kyk_print_hex(" Hash ", mkl_level1 -> nd[i].bdy, MKL_NODE_BODY_LEN);
    };


    free_tx_buf_list(buf_list, sizeof(buf_list)/sizeof(buf_list[0]));

}


void kyk_init_mkltree_node(struct kyk_mkltree_node *nd)
{
    nd -> chid_lft = NULL;
    nd -> chid_rgt = NULL;
    nd -> pnt = NULL;
    nd -> ntype = LEAF_T;
}

struct kyk_mkltree_level *create_mkl_leafs(struct kyk_tx_buf *buf_list, size_t len)
{
    struct kyk_mkltree_level *mkl_level = malloc(sizeof(struct kyk_mkltree_level));
    struct kyk_mkltree_node *nd_list = malloc(len * sizeof(struct kyk_mkltree_node));
    struct kyk_mkltree_node *nd = nd_list;
    mkl_level -> nd = nd_list;
    mkl_level -> len = 0;
    mkl_level -> inx = 1;
    
    for(int i=0; i < len; i++){
	kyk_init_mkltree_node(nd);
	kyk_set_mkl_leaf(nd, buf_list[i]);
	mkl_level -> len++;
	nd++;
    }

    return mkl_level;
}

void kyk_set_mkl_leaf(struct kyk_mkltree_node *nd, struct kyk_tx_buf buf)
{
    kyk_dgst_hash256(nd -> bdy, buf.bdy, buf.len);
    kyk_reverse(nd -> bdy, sizeof(nd -> bdy));
}

void build_tx_buf_from_hex(struct kyk_tx_buf *tx_buf, const char *hexstr)
{
    tx_buf -> bdy = kyk_alloc_hex(hexstr, &tx_buf -> len);    
}

void free_tx_buf_list(struct kyk_tx_buf *buf_list, size_t len)
{
    for(int i = 0; i < len; i++){
	free(buf_list[i].bdy);
    }
}




    











