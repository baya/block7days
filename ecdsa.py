
# copy from http://www.samlewis.me/2017/06/a-peek-under-bitcoins-hood/
from ecdsa import SECP256k1, SigningKey

def get_private_key(hex_string):
    return bytes.fromhex(hex_string.zfill(64))

def get_public_key(private_key):
    # this returns the concatenated x and y coordinates for the supplied private address
    # the prepended 04 is used to signify that it's uncompressed
    return (bytes.fromhex("04") + SigningKey.from_string(private_key, curve=SECP256k1).verifying_key.to_string())

private_key = get_private_key("FEEDB0BDEADBEEF")
public_key = get_public_key(private_key)

