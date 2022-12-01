# Makefile k projektu do IMS
# Autor : Aram Denk
# FIT VUT
# 29.11.2022

CPPFLAGS = -Wextra -Wall
CPPC = gcc

ca: ca.c
	$(CPPC) $(CPPFLAGS)  ca.c -o ca

run:
	$(CPPC) $(CPPFLAGS)  ca.c -o ca
	./ca