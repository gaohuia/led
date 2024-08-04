
build: main.c
	sdcc -mmcs51 --model-small -p89 main.c
	packihx main.ihx > main.hex

flash: build
	./stcgal.sh -p /dev/tty.wchusbserial210 -P stc89 -b 9600 main.hex

clean:
	rm -f *.asm *.lst *.map *.mem *.rel *.rst *.sym *.lk *.ihx *.hex