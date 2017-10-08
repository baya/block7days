#ifndef KYK_MKL_TREE_H__
#define KYK_MKL_TREE_H__

#define MKL_NODE_BODY_LEN 32


enum mkltree_node_type {
    ROOT_ND_T,
    BR_ND_T,
    LEAF_ND_T
};

struct kyk_mkltree_node{
    struct kyk_mkltree_node *child_lft;
    struct kyk_mkltree_node *child_rgt;
    struct kyk_mkltree_node *pnt;
    uint8_t bdy[MKL_NODE_BODY_LEN];
    enum mkltree_node_type ntype;
};

struct kyk_mkltree_level{
    struct kyk_mkltree_node *nd;
    size_t len;
    size_t inx;
};

struct kyk_tx_buf {
    uint8_t *bdy;
    size_t len;
};


struct kyk_mkltree_level *create_mkl_tree(struct kyk_mkltree_level *leaf_level);
struct kyk_mkltree_level *create_mkl_leafs(struct kyk_tx_buf *buf_list, size_t len);

#endif
