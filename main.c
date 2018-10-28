

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "minify.h"

int main(int argc, const char** argv) {
	const char *suffix = SUFFIX;
	size_t length_suffix = strlen(suffix);
	for(int i=1; i<argc; i++) {
		size_t length_arg = strlen(argv[i]);
		char name[length_arg + length_suffix + 1];
		memset(name, 0, sizeof(name));
		bool state = strcmp(suffix, argv[i] + length_arg - length_suffix);
		if(state) {
			sprintf(name, "%s%s", argv[i], suffix);
		} else {
			strncpy(name, argv[i], length_arg - length_suffix);
		}
		FILE *in = fopen(argv[i], "rb");
		if(!errno) {
			FILE *out = fopen(name, "wb");
			printf("%s: %s\n", state ? "compressing" : "decompressing", name);
			clock_t start = clock();
			size_t rounds = action(out, in, state);
			clock_t interval = clock() - start;
			printf("time elapsed: %lf seconds, %ld rounds\n",
				interval / (CLOCKS_PER_SEC / 1.0), rounds
			);
			fclose(out);
		} else {
			printf("Error: (%d) %s\n", errno, strerror(errno));
		}
		fclose(in);
		continue;
	}
	return 0;
}
