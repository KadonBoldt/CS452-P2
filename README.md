# **P2 - Simple Shell**  
**CS-452: Operating Systems**

**Kadon Boldt**

## **Project Overview**  
This project implements a simple shell for an operating system course. The shell provides a command-line interface, processes user commands, and supports built-in commands like `cd`, `exit`, and `history`. It also enables executing external commands using `fork()` and `execvp()`.  

## **Compilation and Execution**

Steps to configure, build, run, and test the project.

### Building

```bash
make
```

### Testing

```bash
make check
```

### Clean

```bash
make clean
```

### Install Dependencies

In order to use git send-mail you need to run the following command:

```bash
make install-deps
```

## **Features**  

- **Built-in Commands:**  
  - `cd [directory]` → Changes the working directory.  
  - `exit` → Exits the shell.  
  - `history` → Displays previously entered commands.  

---
