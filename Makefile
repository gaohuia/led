
build: main.c ired.c ired.h tools.c tools.h
	sdcc -mmcs51 --model-small -p89 main.c -c -o main.rel
	sdcc -mmcs51 --model-small -p89 ired.c -c -o ired.rel
	sdcc -mmcs51 --model-small -p89 tools.c -c -o tools.rel
	sdcc -mmcs51 --model-small -p89 main.rel ired.rel tools.rel -o main.ihx
	packihx main.ihx > main.hex

flash: build
	./stcgal.sh -p /dev/tty.wchusbserial210 -P stc89 -b 9600 main.hex

clean:
	rm -f *.asm *.lst *.map *.mem *.rel *.rst *.sym *.lk *.ihx *.hex