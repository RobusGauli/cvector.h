.PHONY: test

test: test.c
	@$(CC) $^ -o $@ -g -lm
	@./$@
