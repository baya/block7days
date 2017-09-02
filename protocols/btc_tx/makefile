objs = kyk_tx0.o varint.o kyk_tx.o beej_pack.o kyk_sha.o
i_paths = /usr/local/opt/openssl/include
l_paths = /usr/local/opt/openssl/lib


kyk_tx0: $(objs)
	cc -o kyk_tx0.out $(objs) -L$(l_paths) -lcrypto

kyk_tx0.o: kyk_tx.h
varint.o: varint.h
beej_pack.o: beej_pack.h
kyk_tx.o: kyk_tx.h varint.h
kyk_sha.o: kyk_sha.c kyk_sha.h
	cc -c kyk_sha.c -I$(i_paths)

clean:
	-rm -f kyk_tx0.out $(objs)