CC = gcc
SMG1016_CC = arm-mv5sft-linux-gnueabi-$(CC)
CFLAGS = -Wall -g

all: semtester_host

semtester_host: semtester.c
	$(CC) $(CFLAGS) $< -o $@

semtester_arm: semtester.c
	$(SMG1016_CC) $(CFLAGS) $< -o $@
