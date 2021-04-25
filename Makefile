my-cat: my-cat.c
	gcc my-cat.c -o my-cat -Wall -Werror

my-grep: my-grep.c
	gcc my-grep.c -o my-grep -Wall -Werror

my-zip: my-zip.o ziplib.o
	gcc my-zip.o ziplib.o -o my-zip -Wall -Werror

my-zip.o: my-zip.c
	gcc my-zip.c -c -o my-zip.o -Wall -Werror

my-unzip: my-unzip.o ziplib.o
	gcc my-unzip.o ziplib.o -o my-unzip -Wall -Werror

my-unzip.o: my-unzip.c
	gcc my-unzip.c -c -o my-unzip.o -Wall -Werror

ziplib.o: ziplib.c
	gcc ziplib.c -c -Wall -Werror

