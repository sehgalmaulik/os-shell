# Mav Shell (msh)

This project is a simplified shell program, Mav shell (msh), akin to bourne shell (bash), c-shell (csh), or korn shell (ksh). It accepts commands, forks a child process, and executes those commands. The shell runs and accepts commands until the user exits the shell.



## Features


| Command | Description |
|---------|-------------|
| **cd [directory]** | Changes the current directory to the specified directory. |
| **history** | Displays the last 15 commands entered by the user. |
| **!n** | Re-runs the nth command in the history. If the nth command does not exist, the shell displays "Command not in history.". |
| **history -p** | Displays the history along with the associated PIDs (process IDs) of the commands. |
| **exit** | Exits the shell with status zero. |
| **quit** | Exits the shell with status zero. |
| **[any other command]** | Executes the specified command if it exists in `/bin`, `/usr/bin/`, `/usr/local/bin/` or the current working directory. |

It's important to note that the shell can support up to 10 command-line parameters in addition to the command itself. If an unsupported or non-existent command is entered, the shell prints the invalid command followed by ": Command not found.".

1. Msh accepts input when it prints out a prompt of `msh>`. If a supported shell command is given, it executes the command and displays the output.
2. If an unsupported command is entered, msh prints the invalid command followed by ": Command not found."
3. Msh prints the `msh>` prompt and accepts another line of input after each command completes.
4. Msh exits with status zero if the command is "quit" or "exit".
5. If a blank line is entered, msh prints another prompt and accepts a new line of input without any other output.
6. Msh supports up to 10 command line parameters in addition to the command.
7. Msh executes any command entered. Any command in `/bin`, `/usr/bin/`, `/usr/local/bin/` and the current working directory is considered valid for testing.
8. Msh uses `fork()`, `wait()` and one of the exec family of functions for implementation. It does not use `system()`.
9. Msh supports the `cd` command to change directories. It handles `cd ..`.
10. Msh supports the `history` command which lists the last 15 commands entered by the user. Typing `!n`, where `n` is a number between 0 and 14, results in msh re-running the nth command. If the nth command does not exist then msh states "Command not in history.".
11. The history command supports a `-p` parameter. This lists the associated PID with the command.
12. If there are less than 15 commands in the history, msh only lists the commands the user has entered up to that point.

Sure, here's a table that describes the supported commands in the shell:

| Command | Description |
|---------|-------------|
| **cd [directory]** | Changes the current directory to the specified directory. |
| **history** | Displays the last 15 commands entered by the user. |
| **!n** | Re-runs the nth command in the history. If the nth command does not exist, the shell displays "Command not in history.". |
| **history -p** | Displays the history along with the associated PIDs (process IDs) of the commands. |
| **exit** | Exits the shell with status zero. |
| **quit** | Exits the shell with status zero. |
| **[any other command]** | Executes the specified command if it exists in `/bin`, `/usr/bin/`, `/usr/local/bin/` or the current working directory. |

It's important to note that the shell can support up to 10 command-line parameters in addition to the command itself. If an unsupported or non-existent command is entered, the shell prints the invalid command followed by ": Command not found.".


## Usage

```bash
msh> [command]
```

To quit the shell:
```bash
msh> quit
```
or
```bash
msh> exit
```

To change directories:
```bash
msh> cd [directory]
```

To view command history:
```bash
msh> history
```

To execute a previous command from history:
```bash
msh> !n
```
where `n` is the number of the command in the history. 

To view command history with associated PIDs:
```bash
msh> history -p
```

## Copyright 

Copyright © 2023. All rights reserved.

This software is provided for educational purposes only. It is prohibited to use this code, for any college assignment or personal use. Unauthorized distribution, modification or commercial usage is strictly forbidden. Please respect the rights of the author.


