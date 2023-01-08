c++ -std=c++11 -Wall -o ./bin/player1 ./players/player1.cpp
c++ -std=c++11 -Wall -o ./bin/player2 ./players/player2.cpp
gcc -Wall -o ./bin/judge ./src/judge.cpp
cd bin
./judge player1 player2
cd ..