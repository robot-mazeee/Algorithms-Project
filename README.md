# Introduction to Algorithms and Data Structures Project

The full project description in Portuguese can be found in [ProjectDescription.md](ProjectDescription.md).

This repository provides a set of automatic tests, which can be ran with the following commands:

```
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj1 *.c
cd public-tests
make
```

## Brief Project Description

This project consists of a parking lot management system. The system allows for the registration of vehicles, as well as inquiries and billing. Below is a list of the available commands: 

| Command | Action |
|:---:|:---|
| __q__ | Terminates the program |
| __p__ | Creates a new parking lot with a billing system or lists all existing parks |
| __e__ | Register the entrance of a vehicle in a parking lot |
| __s__ | Register the exit of a vehicle from a parking lot |
| __v__ | List all entrances and exits of a vehicle from a parking lot |
| __f__ | Displays a parking lot's revenue. |
| __r__ | Removes a parking lot from the system |
