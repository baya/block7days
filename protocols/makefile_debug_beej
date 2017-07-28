dbj_pack_objs = debug_beej_pack.o beej_pack.o

debug_beej_pack.out: $(dbj_pack_objs)
	gcc -o debug_beej_pack.out $(dbj_pack_objs)
debug_beej_pack.o: debug_beej_pack.c beej_pack.h
	gcc -c debug_beej_pack.c
beej_pack.o: beej_pack.c beej_pack.h
	gcc -c beej_pack.c

clean:
	rm $(dbj_pack_objs)