# Minishell
A lightweight Unix shell implemented in C supporting: - Built-in commands ,external commands


<div align="center">

```
███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗
████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║
██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║
██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║
██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗
╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝
```

**A lightweight Unix shell implemented in C**

[![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/Platform-Linux-orange?style=flat-square&logo=linux)](https://www.linux.org/)
[![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)](LICENSE)
[![Build](https://img.shields.io/badge/Build-GCC-red?style=flat-square&logo=gnu)](https://gcc.gnu.org/)

</div>

---

## 📌 Overview

**Minishell** is a custom Unix shell written in C, built from scratch as part of an Emertxe Embedded Systems training program. It mimics core behavior of `bash`, supporting process creation, I/O redirection, piping, signal handling, and job control — all implemented using low-level POSIX APIs.

> _"Understanding the shell is understanding the OS."_

---

## ✨ Features

| Feature | Description |
|---|---|
| 🔧 **Command Execution** | Execute system binaries and built-in commands |
| 📂 **Built-in Commands** | `cd`, `pwd`, `echo`, `exit`, `help` |
| 🔀 **Piping** | Chain commands with `\|` (multi-pipe support) |
| 📥 **I/O Redirection** | `>`, `>>`, `<` for file-based redirection |
| 🎛️ **Signal Handling** | `SIGINT`, `SIGTSTP`, `SIGCHLD` handled gracefully |
| 💼 **Job Control** | Foreground/background process management (`&`, `fg`, `bg`) |
| 🧹 **Zombie Prevention** | Proper `wait()` and `SIGCHLD` cleanup |
| 🖥️ **Custom Prompt** | Displays current working directory dynamically |

---

## 🏗️ Project Structure

```
minishell/
├── src/
│   ├── main.c          # Entry point, REPL loop
│   ├── parser.c        # Command line tokenizer & parser
│   ├── executor.c      # fork/exec logic, pipes
│   ├── builtins.c      # Built-in command handlers
│   ├── signals.c       # Signal handler setup
│   └── jobs.c          # Job control management
├── include/
│   └── shell.h         # Header declarations
├── Makefile
└── README.md
```

---

## ⚙️ Build & Run

### Prerequisites

- GCC compiler
- Linux / Unix environment
- `make`

### Compile

```bash
git clone https://github.com/your-username/minishell.git
cd minishell
make
```

### Run

```bash
./minishell
```

### Clean Build

```bash
make clean
```

---

## 🚀 Usage Examples

```bash
# Basic command
minishell$ ls -la

# Piping
minishell$ ls -l | grep ".c" | wc -l

# I/O Redirection
minishell$ echo "Hello, Shell!" > output.txt
minishell$ cat < output.txt

# Background process
minishell$ sleep 10 &

# Change directory
minishell$ cd /home/user/projects

# Exit
minishell$ exit
```

---

## 🔬 Internals

### Execution Flow

```
User Input
    │
    ▼
Tokenizer (parse input string)
    │
    ▼
Command Table (args, redirects, pipes)
    │
    ▼
   Built-in? ──Yes──► Execute directly
    │
    No
    │
    ▼
 fork()
    ├── Child  → execvp() → run binary
    └── Parent → wait() / job tracking
```

### Signal Handling

| Signal | Behavior |
|---|---|
| `Ctrl+C` → `SIGINT` | Kills foreground process, shell continues |
| `Ctrl+Z` → `SIGTSTP` | Suspends foreground process to background |
| `SIGCHLD` | Reaps zombie child processes automatically |

---

## 🛠️ Key System Calls Used

```c
fork()       // Create child process
execvp()     // Execute a program
waitpid()    // Wait for child with options
pipe()       // Create anonymous pipe
dup2()       // Redirect file descriptors
sigaction()  // Install signal handlers
```

---

## 📚 Concepts Demonstrated

- ✅ Process management (`fork`, `exec`, `wait`)
- ✅ File descriptor manipulation (`dup2`, `pipe`)
- ✅ Signal handling with `sigaction`
- ✅ POSIX job control
- ✅ String parsing & tokenization in C
- ✅ Async-signal-safe programming
- ✅ Zombie and orphan process prevention

---

## 🧪 Tested On

- Ubuntu 22.04 LTS
- GCC 11.x
- Linux Kernel 5.x

---

## 👨‍💻 Author

**Sajid**
B.Tech Electrical Engineering | Embedded Systems Trainee @ Emertxe, Bengaluru

[![GitHub](https://img.shields.io/badge/GitHub-your--username-black?style=flat-square&logo=github)](https://github.com/nayakawadesajid111-hue)

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

<div align="center">

_Built with 💻, low-level C, and a deep dive into Unix internals._

⭐ **Star this repo if you found it useful!** ⭐

</div>
