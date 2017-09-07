#encoding:utf-8

import struct
import hashlib
from ecdsa import SECP256k1, SigningKey

# 58 character alphabet used
BASE58_ALPHABET = '123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz'

def base58_encode(version, public_address):
    """
    Gets a Base58Check string
    See https://en.bitcoin.it/wiki/Base58Check_encoding
    """
    version = bytearray.fromhex(version)
    checksum = hashlib.sha256(hashlib.sha256(version + public_address).digest()).digest()[:4]
    payload = version + public_address + checksum

    result = int.from_bytes(payload, byteorder="big")
    padding = len(payload) - len(payload.lstrip(b'\0'))
    encoded = []

    while result != 0:
        result, remainder = divmod(result, 58)
        encoded.append(BASE58_ALPHABET[remainder])

    return padding*"1" + "".join(encoded)[::-1]

def get_private_key(hex_string):
    # return bytes.fromhex(hex_string.zfill(64))
    return bytearray.fromhex(hex_string.zfill(64))

def get_public_key(private_key):
    # this returns the concatenated x and y coordinates for the supplied private address
    # the prepended 04 is used to signify that it's uncompressed
    return (bytearray.fromhex("04") + SigningKey.from_string(private_key, curve=SECP256k1).verifying_key.to_string())

def get_public_address(public_key):
    address = hashlib.sha256(public_key).digest()

    h = hashlib.new('ripemd160')
    h.update(address)
    address = h.digest()

    return address

if __name__ == "__main__":
    private_key = get_private_key("6c85f3945141a6c43793565a17c3c469c4cb078a17352c227abbc858951aa0f2")
    public_key = get_public_key(private_key)
    public_address = get_public_address(public_key)
    bitcoin_address = base58_encode("00", public_address)
    print(bitcoin_address)
