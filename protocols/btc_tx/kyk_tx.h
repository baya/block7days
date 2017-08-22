#ifndef KYK_BITCOIN_KYK_TX_H
#define KYK_BITCOIN_KYK_TX_H

#include "varint.h"

/* https://bitcoin.org/en/developer-reference#raw-transaction-format */
struct kyk_tx {
    uint32_t version;
    varint_t vin_sz;          /* In-counter */
    struct kyk_txin *txin;
    varint_t vout_sz;         /* Out-counter */
    struct kyk_txout *txout;
    uint32_t lock_time;
};

struct kyk_txin{
};

struct kyk_txout{
};


#endif
