all: bot2048

#delcare variables
CC = g++
INCLUDE = .

bot2048: AI2048.cpp 2048.cpp
	$(CC) -o bot2048 AI2048.cpp 2048.cpp


manual:
	$(CC) -o manual2048 main.cpp 2048.cpp