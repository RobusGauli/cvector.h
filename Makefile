.PHONY: test format

test: test.c
	@$(CC) $^ -o $@ -g -lm
	@./$@

format:
	@clang-format -i src/cvector.h test.c -style=file
