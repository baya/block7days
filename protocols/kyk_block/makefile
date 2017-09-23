kyk_tx0_objs = kyk_tx0.o varint.o kyk_tx.o beej_pack.o kyk_sha.o kyk_utils.o
ec_test_objs = kyk_ecdsa_test.o kyk_ecdsa.o kyk_utils.o
addr_test_objs = kyk_address_test.o kyk_ecdsa.o kyk_utils.o kyk_sha.o kyk_base58.o
make_addr_objs = kyk_make_address_test.o kyk_ecdsa.o kyk_utils.o kyk_sha.o kyk_base58.o kyk_address.o
pem_to_addr_objs = pem_to_address.o kyk_ecdsa.o kyk_utils.o kyk_sha.o kyk_base58.o kyk_address.o
sc_pubkey_test_objs = kyk_sc_pubkey_test.o kyk_ecdsa.o kyk_utils.o kyk_sha.o kyk_base58.o kyk_script.o beej_pack.o
exe_p2pkh_sc_test_objs = kyk_exe_p2pkh_sc_test.o kyk_utils.o kyk_script.o kyk_base58.o kyk_ecdsa.o kyk_sha.o beej_pack.o kyk_ser.o varint.o
print_coinbase_objs = print_coinbase.o kyk_utils.o
sig_parse_test_objs = sig_parse_test.o kyk_utils.o
kyk_ser_test_objs = kyk_ser_test.o kyk_ser.o kyk_utils.o beej_pack.o varint.o
kyk_gens_tx_objs = kyk_gens_tx.o varint.o kyk_tx.o beej_pack.o kyk_sha.o kyk_utils.o kyk_address.o kyk_base58.o kyk_script.o kyk_ecdsa.o
parse_block_objs = parse_block.o varint.o kyk_tx.o beej_pack.o kyk_utils.o

i_paths = /usr/local/opt/openssl/include
l_paths = /usr/local/opt/openssl/lib

CC = clang


kyk_tx0: $(kyk_tx0_objs)
	$(CC) -o kyk_tx0.out $(kyk_tx0_objs) -L$(l_paths) -lcrypto

kyk_ecdsa_test: $(ec_test_objs)
	$(CC) -Wall -o kyk_ecdsa_test.out $(ec_test_objs) -L$(l_paths) -lcrypto

kyk_address_test: $(addr_test_objs)
	$(CC) -Wall -o kyk_address_test.out $(addr_test_objs) -L$(l_paths) -lcrypto

kyk_make_address_test: $(make_addr_objs)
	$(CC) -Wall -o kyk_make_address_test.out $(make_addr_objs) -L$(l_paths) -lcrypto

pem_to_address: $(pem_to_addr_objs)
	$(CC) -Wall -o pem_to_address.out $(pem_to_addr_objs) -L$(l_paths) -lcrypto

kyk_sc_pubkey_test: $(sc_pubkey_test_objs)
	$(CC) -Wall -o kyk_sc_pubkey_test.out $(sc_pubkey_test_objs) -L$(l_paths) -lcrypto

kyk_exe_p2pkh_sc_test: $(exe_p2pkh_sc_test_objs)
	$(CC) -Wall -o  kyk_exe_p2pkh_sc_test.out $(exe_p2pkh_sc_test_objs) -L$(l_paths) -lcrypto

sig_parse_test: $(sig_parse_test_objs)
	$(CC) -Wall -o  sig_parse_test.out $(sig_parse_test_objs) -L$(l_paths) -lcrypto

kyk_ser_test: $(kyk_ser_test_objs)
	$(CC) -Wall -o  kyk_ser_test.out $(kyk_ser_test_objs) -L$(l_paths) -lcrypto

print_coinbase: $(print_coinbase_objs)
	$(CC) -o print_coinbase.out $(print_coinbase_objs)

kyk_gens_tx: $(kyk_gens_tx_objs)
	$(CC) -o kyk_gens_tx.out $(kyk_gens_tx_objs) -L$(l_paths) -lcrypto

parse_block: $(parse_block_objs)
	$(CC) -o parse_block.out $(parse_block_objs)


kyk_tx0.o: kyk_tx.h
	$(CC) -c kyk_tx0.c -I$(i_paths)

kyk_ecdsa_test.o: kyk_ecdsa_test.c kyk_ecdsa.h
	$(CC) -c kyk_ecdsa_test.c -I$(i_paths)

kyk_address_test.o: kyk_address_test.c kyk_ecdsa.h kyk_utils.h
	$(CC) -c kyk_address_test.c -I$(i_paths)

kyk_make_address_test.o: kyk_make_address_test.c
	$(CC) -c kyk_make_address_test.c -I$(i_paths)

pem_to_address.o: pem_to_address.c
	$(CC) -c pem_to_address.c -I$(i_paths)

kyk_sc_pubkey_test.o: kyk_sc_pubkey_test.c kyk_script.h
	$(CC) -c kyk_sc_pubkey_test.c -I$(i_paths)

kyk_exe_p2pkh_sc_test.o: kyk_exe_p2pkh_sc_test.c kyk_script.h kyk_utils.h
	$(CC) -c kyk_exe_p2pkh_sc_test.c -I$(i_paths)

sig_parse_test.o: sig_parse_test.c kyk_ecdsa.h kyk_utils.h
	$(CC) -c sig_parse_test.c -I$(i_paths)

kyk_ser_test.o: kyk_ser_test.c beej_pack.h kyk_utils.h
	$(CC) -c kyk_ser_test.c -I$(i_paths)

kyk_gens_tx.o: kyk_gens_tx.c
	$(CC) -c kyk_gens_tx.c -I$(i_paths)

parse_block.o: parse_block.c
	$(CC) -c parse_block.c


print_coinbase.o: kyk_utils.h


varint.o: varint.h
beej_pack.o: beej_pack.h
kyk_tx.o: kyk_tx.h varint.h
kyk_utils.o: kyk_utils.h
kyk_sha.o: kyk_sha.c kyk_sha.h
	$(CC) -c kyk_sha.c -I$(i_paths)
kyk_ecdsa.o: kyk_ecdsa.c kyk_ecdsa.h kyk_sha.h kyk_sha.c
	$(CC) -c kyk_ecdsa.c -I$(i_paths)
kyk_base58.o: kyk_base58.c kyk_base58.h
	$(CC) -c kyk_base58.c -I$(i_paths)
kyk_address.o: kyk_address.c kyk_address.h
	$(CC) -c kyk_address.c -I$(i_paths)
kyk_script.o: kyk_script.c kyk_script.h
	$(CC) -c kyk_script.c -I$(i_paths)
kyk_ser.o: kyk_ser.c kyk_ser.h
	$(CC) -c kyk_ser.c -I$(i_paths)


clean:
	-rm -f *.out *.o