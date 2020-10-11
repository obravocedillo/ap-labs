# word-reverser build & test automation

APP_NAME=base64
LIB_NAME=logger

build:
	gcc -c ${APP_NAME}.c -o ${APP_NAME}.o
	gcc -c ${LIB_NAME}.c -o ${LIB_NAME}.o
	gcc    ${LIB_NAME}.o ${APP_NAME}.o  -o ${APP_NAME}

files:
	curl -Ok http://textfiles.com/stories/vgilante.txt
	curl -Ok http://textfiles.com/stories/sick-kid.txt
	curl -Ok http://textfiles.com/stories/aesop11.txt
	head -c 2147483648 </dev/urandom > megafile.txt

test: build files
	 @echo Test 1
	./${APP_NAME} --encode vigilante
	./${APP_NAME} --decode vigilante-encode
	@echo Test 2
	./${APP_NAME} --encode sick-kid
	./${APP_NAME} --decode sick-kid-encoded
	@echo Test 3
	./${APP_NAME} --encode aesop11
	./${APP_NAME} --decode aesop11-encoded
	@echo Test 4 - megafile - 2 Gb
	./${APP_NAME} --encode megafile
	./${APP_NAME} --decode megafile-encoded
	@echo Test 5 - failed test
	./${APP_NAME} --decode non-existing-file
clean:
	rm -rf *.o ${APP_NAME} *-encoded.txt vgilante.txt sick-kid.txt aesop11.txt megafile.txt
