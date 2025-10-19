default: relay_on relay_off

relay_on: relay_on.c
	gcc -Wall -o $@ $< -lbcm2835

relay_off: relay_off.c
	gcc -Wall -o $@ $< -lbcm2835

clean:
	rm -rf relay_on relay_off *~
