#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "kyk_utils.h"
#include "kyk_script.h"

#define KYK_SC_PUBK_MAX_LEN 1000
#define KYK_SC_MAX_LEN 2000

int main()
{
    unsigned char sc_pubk[KYK_SC_PUBK_MAX_LEN];
    char *addr = "1KFHE7w8BhaENAswwryaoccDb6qcT6DbYY";
    char *sc_sig_hex = "4830450221008738f6f97f3cafe1dfc79c63b7d33b356be47e5983f854203c59c45566a8c96802201190857c5dd7d1f63bb7ea30addb116ee7cdae8c588bf73df2dcf046e11888f4014104e7e8eeb1d060e2ae611e2e6253b7948d7c02b7ea7c2dce09d939225a5c39d86553b07acf5073b1dc1e432cdf574d587b2800e0c3d4e43fcb3a1f84bfdf602a1e";
    uint8_t *sc_sig;
    uint8_t sc[KYK_SC_MAX_LEN];
    size_t sc_pubk_len, sc_sig_len, sc_len;
    int verified = 0;

    /* https://blockexplorer.com/api/rawtx/21f7c74240a92be7f95f8e8262849bc27cb3395859b0d6f82f23fc02619aefe4 */
    char *tx_hex = "01000000010000000000000000000000000000000000000000000000000000000000000000ffffffff640350ce0637e4b883e5bda9e7a59ee4bb99e9b1bc0af6561b0ced2eab1458dbfa51f7a0112cda01cf4c14d473e7b3e16f91cde48402000000f09f909f124d696e6564206279206465666963696f3132000000000000000000000000000000000000000000d3c4000001e6aac64d000000001976a914c825a1ecf2a6830c4401620c3a16f1995057c2ab88ac9cf95c3a";
    size_t tx_len;
    uint8_t *tx;


    /* 从 hex 字符串直接拷贝 scriptSig 到内存 */
    sc_sig = kyk_alloc_hex(sc_sig_hex, &sc_sig_len);
    kyk_print_hex("scriptSig Hex", sc_sig, sc_sig_len);

    /* 从比特币地址中提取 pay-to-pubkey-hash 脚本 */
    sc_pubk_len = p2pkh_sc_from_address(sc_pubk, addr);

    kyk_print_hex("scriptPubKey Hex ", sc_pubk, sc_pubk_len);

    sc_len = kyk_combine_sc(sc, sc_sig, sc_sig_len, sc_pubk, sc_pubk_len);
    kyk_print_hex("script Hex", sc, sc_len);

    tx = kyk_alloc_hex(tx_hex, &tx_len);
    kyk_print_hex("tx", tx, tx_len);

    verified = kyk_run_sc(sc, sc_len, tx, tx_len);

    printf("++++++++%d\n", verified);

    free(sc_sig);
    free(tx);

}
