CC = g++
CXXFLAGS = -Wall
# OBJS = swordfish.o

swordfish : swordfish.cc swordfish.o config.h ttyreader.h
	$(CC) $(CXXFLAGS) swordfish.cc -o swordfish
