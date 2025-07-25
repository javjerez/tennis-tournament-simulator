# 🎾 Tennis Tournament Simulator

This project was developed as part of one of my first university courses in Computer Science. It is a C++ console based simulation of a tennis tournament, allowing players to be registered, simulate matches and provide statistics about each amtch.

## Features

Despite being in a single file with no modularization, this project focused on:

- Register and store tennis players.
- Simulate matches between two players.
- Ball hitting and player movement based on probability adn ability points.
- ASCII-based court visualization.
- Detailed match statistics.

## How to Run

### 🛠️ Requirements

To compile and run the project, a C++ compiler is required. The project can be compiled with the following instruction:

```bash
g++ -o tennis TENIS_TOURNAMENT_JJR.cpp

./tennis
```

A basic 'players.txt' file is required, if the file does not exist, it is automatically created.
The structure for each player is the following:

```txt
<INITIALS> <ABILITY> <SPEED> <MATCHES_WON> <MATCHES_LOST>
```

The program stops reading when it finds "XXX", here is a sample of the 'players.txt' file:

```txt
JJR 3 4 0 0
ABC 2 3 0 0
XYZ 4 4 0 0
XXX
```


