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

    /* 从 hex 字符串直接拷贝 ScriptSig 到内存 */
    sc_sig = kyk_alloc_hex(sc_sig_hex, &sc_sig_len);
    kyk_print_hex("scriptSig Hex", sc_sig, sc_sig_len);

    /* 从比特币地址中提取 pay-to-pubkey-hash 脚本 */
    sc_pubk_len = p2pkh_sc_from_address(sc_pubk, addr);

    kyk_print_hex("scriptPubKey Hex ", sc_pubk, sc_pubk_len);

    sc_len = kyk_combine_sc(sc, sc_sig, sc_sig_len, sc_pubk, sc_pubk_len);
    kyk_print_hex("script Hex", sc, sc_len);

    kyk_run_sc(sc, sc_len);

    free(sc_sig);

}
