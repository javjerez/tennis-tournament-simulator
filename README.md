# tennis-tournament-simulator
A simple C++ console based tennis tournament simulator created as part of a first year programming course.

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

To compile and run the project, you'll need a **C++ compiler**.

```bash
g++ -o tennis TENIS_TOURNAMENT_JJR.cpp

```bash
./tennis

A basic players.txt file is required

You will need a basic 'players.txt' file to get started.

- The structure for each player is: <INITIALS> <ABILITY> <SPEED> <MATCHES_WON> <MATCHES_LOST>

The program stops reading when it finds "XXX". Here is a sample of the 'players.txt' file:

```bash
JJR 3 4 0 0
ABC 2 3 0 0
XYZ 4 4 0 0
XXX


