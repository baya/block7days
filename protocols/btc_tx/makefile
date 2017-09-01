objs = kyk_tx0.o varint.o kyk_tx.o beej_pack.o

kyk_tx0: $(objs)
	cc -o kyk_tx0.out $(objs)

kyk_tx0.o: kyk_tx.h
varint.o: varint.h
beej_pack.o: beej_pack.h
kyk_tx.o: kyk_tx.h varint.h

clean:
	-rm -f kyk_tx0.out $(objs)