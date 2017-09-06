kyk_tx0_objs = kyk_tx0.o varint.o kyk_tx.o beej_pack.o kyk_sha.o
ec_test_objs = kyk_ecdsa_test.o kyk_ecdsa.o kyk_utils.o
addr_test_objs = kyk_address_test.o kyk_ecdsa.o kyk_utils.o kyk_sha.o kyk_base58.o
i_paths = /usr/local/opt/openssl/include
l_paths = /usr/local/opt/openssl/lib


kyk_tx0: $(kyk_tx0_objs)
	cc -o kyk_tx0.out $(kyk_tx0_objs) -L$(l_paths) -lcrypto

kyk_ecdsa_test: $(ec_test_objs)
	cc -Wall -o kyk_ecdsa_test.out $(ec_test_objs) -L$(l_paths) -lcrypto

kyk_address_test: $(addr_test_objs)
	cc -Wall -o kyk_address_test.out $(addr_test_objs) -L$(l_paths) -lcrypto

kyk_tx0.o: kyk_tx.h
kyk_ecdsa_test.o: kyk_ecdsa_test.c kyk_ecdsa.h
	cc -c kyk_ecdsa_test.c -I$(i_paths)

kyk_address_test.o: kyk_address_test.c kyk_ecdsa.h kyk_utils.h
	cc -c kyk_address_test.c -I$(i_paths)

varint.o: varint.h
beej_pack.o: beej_pack.h
kyk_tx.o: kyk_tx.h varint.h
kyk_utils.o: kyk_utils.h
kyk_sha.o: kyk_sha.c kyk_sha.h
	cc -c kyk_sha.c -I$(i_paths)
kyk_ecdsa.o: kyk_ecdsa.c kyk_ecdsa.h
	cc -c kyk_ecdsa.c -I$(i_paths)
kyk_base58.o: kyk_base58.c kyk_base58.h
	cc -c kyk_base58.c -I$(i_paths)

clean:
	-rm -f *.out *.o