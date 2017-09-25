parse_tx_vinc_objs = parse_tx_vinc.o

parse_tx_vinc: $(parse_tx_vinc_objs)
	$(CC) -o parse_tx_vinc.out $(parse_tx_vinc_objs)

parse_tx_vinc.o: kyk_endian.h
