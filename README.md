# Introduction to Algorithms and Data Structures Project

The full project description in Portuguese can be found in [ProjectDescription.md](ProjectDescription.md).

This repository provides a set of automatic tests, which can be ran with the following commands:

```
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj1 *.c
cd public-tests
make
```

## Brief Project Description

This project implements a parking lot management system that supports vehicle registration, inquiries, and billing functionalities. Below is a list of the available commands:

| Command | Action | Usage Example
|:---:|:---|:---|
| __q__ | Terminates the program | `q`
| __p__ | Creates a new parking lot with a billing system or lists all existing parks | `p` (list), `p [ <name> <capacity> <billing-each-15-mins-first-hour> <billing-each-15-mins-after-first-hour> <max-daily-billing> ]` (create)
| __e__ | Register the entrance of a vehicle in a parking lot | `e <parking-lot-name> <license-plate> <date> <time>`
| __s__ | Register the exit of a vehicle from a parking lot | `s <parking-lot-name> <license-plate> <date> <time>`
| __v__ | List all entrances and exits of a vehicle from a parking lot | `v <license-plate>`
| __f__ | Displays a parking lot's revenue. | `f <parking-lot-name> [ <date> ]`
| __r__ | Removes a parking lot from the system | `r <parking-lot-name>`
