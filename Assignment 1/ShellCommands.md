# Assignment 1: The Significance of Shell Commands

## 1. What is a Shell?

A **shell** is a command-line interpreter (CLI) that acts as the primary interface between the user and the operating system's kernel. When you type a command, the shell reads it, interprets it, and asks the kernel to perform the requested task.

**Significance:** The shell is significant because it provides a powerful, fast, and scriptable way to control every aspect of your computer. It allows you to:

- Manage files, directories, and processes.
- Automate complex, repetitive tasks by writing **shell scripts**.
- Work on remote servers or systems that do not have a graphical user interface (GUI).

The most common shell on Linux systems is `bash` (Bourne-Again SHell).

## 2. Directory Navigation & Information Commands

These commands allow you to find out where you are and move around the filesystem.

### `pwd` (Print Working Directory)

- **Significance:** Tells you the absolute path of the directory you are currently in.
- **Syntax:** `pwd`
- **Common Options:** None.
- **Example:**
  ```bash
  $ pwd
  /home/rihan/Documents/OS_Lab
  ```

### `ls` (List)

- **Significance:** Lists the contents (files and directories) of the current directory. Essential for navigating and understanding the filesystem structure.
- **Syntax:** `ls [options] [directory_path]`
- **Common Options:**
  - `-l`: **Long listing** format. Shows detailed information: permissions, links, owner, group, size, date, name.
  - `-a`: **All**. Shows all files, including hidden files (those starting with a `.`).
  - `-h`: **Human-readable**. Used with `-l` to show file sizes in KB, MB, etc. (e.g., `4.0K`, `1.2M`).
  - `-t`: Sorts by modification **time**, newest first.
  - `-r`: **Reverse** order (use with `-t` for oldest first).
  - `-S`: Sort by file **size**, largest first.
  - `-i`: Show **inode** numbers.
  - `-d`: List directories themselves, not their contents.
- **Understanding `ls -l` Output:**
  - `drwxr-xr-x`: File type (d=directory, -=file) and permissions (r=read, w=write, x=execute).
  - `2`: Number of hard links.
  - `rihan rihan`: Owner and group.
  - `4.0K`: File size.
  - `Nov 4 15:00`: Last modification date.
  - `.`: Directory name.
- **Examples:**
  ```bash
  # Simple list
  $ ls
  assignment1.c  assignment2.sh  output.txt

  # Detailed, human-readable list of all files
  $ ls -lha
  total 12K
  drwxr-xr-x 2 rihan rihan 4.0K Nov  4 15:00 .
  drwxr-xr-x 5 rihan rihan 4.0K Nov  4 14:55 ..
  -rw-r--r-- 1 rihan rihan 1.2K Nov  4 15:00 .my_config
  -rwxr-xr-x 1 rihan rihan 8.5K Nov  4 14:58 assignment1.c
  -rw-r--r-- 1 rihan rihan    0 Nov  4 14:59 output.txt

  # Sort by size, largest first
  $ ls -lhS
  -rwxr-xr-x 1 rihan rihan 8.5K Nov  4 14:58 assignment1.c
  -rw-r--r-- 1 rihan rihan 1.2K Nov  4 15:00 .my_config
  -rw-r--r-- 1 rihan rihan    0 Nov  4 14:59 output.txt

  # List only directories
  $ ls -d */
  my_project/
  ```
- **Key Study Points:**
  - File permissions: r (4), w (2), x (1) for user, group, others.
  - Hidden files start with `.` (e.g., `.bashrc`).
  - Use `ls -la` to see all files including hidden ones.

### `cd` (Change Directory)

- **Significance:** Allows you to move from your current directory to another.
- **Syntax:** `cd [directory_path]`
- **Common Options:** None, but it has special arguments.
- **Examples:**
  ```bash
  # Move into a subdirectory
  $ cd OS_Lab

  # Move up to the parent directory
  $ cd ..

  # Move to your home directory (shortcut)
  $ cd ~
  # or just:
  $ cd

  # Move to the previous directory you were in
  $ cd -
  ```

## 3. File and Directory Management

These commands are used to create, delete, copy, and move files and directories.

### `mkdir` (Make Directory)

- **Significance:** Creates a new, empty directory.
- **Syntax:** `mkdir [options] <directory_name>`
- **Common Options:**
  - `-p`: **Parent**. Creates the entire path of directories if they don't already exist.
- **Examples:**
  ```bash
  # Create a single directory
  $ mkdir my_project

  # Create a nested directory structure all at once
  $ mkdir -p my_project/src/include
  ```

### `touch` (Create Empty File)

- **Significance:** Creates a new, empty file. It can also be used to update the "last modified" timestamp of an existing file.
- **Syntax:** `touch <file_name>`
- **Example:**
  ```bash
  $ touch notes.txt
  ```

### `cp` (Copy)

- **Significance:** Copies files or directories from source to destination. Essential for creating backups and duplicating content.
- **Syntax:** `cp [options] <source> <destination>`
- **Common Options:**
  - `-r`: **Recursive**. Required to copy directories and their contents.
  - `-v`: **Verbose**. Shows detailed progress of what is being copied.
  - `-i`: **Interactive**. Prompts before overwriting existing files.
  - `-u`: **Update**. Only copies if source is newer than destination or destination doesn't exist.
  - `-p`: **Preserve**. Maintains original file attributes (permissions, timestamps, ownership).
  - `-a`: **Archive**. Equivalent to `-rpP` (recursive, preserve, follow symbolic links).
- **Examples:**
  ```bash
  # Copy a file in the same directory (create a backup)
  $ cp notes.txt notes.txt.bak

  # Copy a file into another directory
  $ cp notes.txt my_project/

  # Copy an entire directory recursively
  $ cp -r my_project my_project_backup

  # Copy with verbose output
  $ cp -rv my_project my_project_backup
  my_project -> my_project_backup
  my_project/file1.txt -> my_project_backup/file1.txt

  # Interactive copy (prompts before overwrite)
  $ cp -i notes.txt existing_file.txt
  cp: overwrite 'existing_file.txt'? y

  # Archive copy (preserves all attributes)
  $ cp -a source_dir dest_dir

  # Update only newer files
  $ cp -ru source_dir/* dest_dir/
  ```
- **Key Study Points:**
  - Always use `-i` when unsure about overwriting files.
  - Use `-a` for complete backups that preserve all file attributes.
  - `-r` is required for directories; without it, `cp` treats directories as files.

### `mv` (Move)

- **Significance:** Used to **move** a file/directory to a new location or to **rename** it.
- **Syntax:** `mv <source> <destination>`
- **Examples:**
  ```bash
  # Rename a file
  $ mv notes.txt important_notes.txt

  # Move a file into a directory
  $ mv important_notes.txt my_project/
  ```

### `rm` (Remove)

- **Significance:** Permanently deletes files and directories. **Irrecoverable operation - use extreme caution!**
- **Syntax:** `rm [options] <file_name>`
- **Common Options:**
  - `-r`: **Recursive**. Required to delete directories and all their contents.
  - `-f`: **Force**. Deletes without prompting for confirmation, ignores non-existent files.
  - `-i`: **Interactive**. Prompts before each deletion for confirmation.
  - `-v`: **Verbose**. Shows what is being deleted.
  - `-d`: **Directory**. Allows deletion of empty directories (rarely used, prefer `rmdir`).
- **Examples:**
  ```bash
  # Delete a single file
  $ rm notes.txt.bak

  # Interactive deletion (safe mode)
  $ rm -i important_file.txt
  rm: remove regular file 'important_file.txt'? y

  # Force delete (ignores errors and prompts)
  $ rm -f temp_file.txt non_existent_file.txt

  # Delete an entire directory recursively (VERY DANGEROUS!)
  $ rm -rf my_project_backup

  # Verbose deletion
  $ rm -rv old_directory/
  removed 'old_directory/file1.txt'
  removed 'old_directory/file2.txt'
  removed directory 'old_directory/'
  ```
- **Key Study Points:**
  - **Never use `rm -rf /` or `rm -rf /*`** - this will destroy your entire system!
  - Always double-check paths before using `-r` or `-f`.
  - Use `-i` for important deletions to avoid accidents.
  - Deleted files cannot be recovered from trash - they are permanently gone.
  - Consider using `trash-cli` or similar tools for safer file deletion.

### `rmdir` (Remove Directory)

- **Significance:** Deletes an **empty** directory.
- **Syntax:** `rmdir <directory_name>`
- **Example:**
  ```bash
  $ rmdir my_project
  rmdir: failed to remove 'my_project': Directory not empty
  ```

## 4. File Content Viewing

These commands let you read the contents of files.

### `cat` (Concatenate)

- **Significance:** Reads one or more files and prints their entire content to the screen.
- **Syntax:** `cat <file_name> [another_file...]`
- **Example:**
  ```bash
  $ cat notes.txt
  This is line 1.
  This is line 2.
  ```

### `less` (Pager)

- **Significance:** A program for viewing large files one page at a time. It's "less" because it doesn't load the whole file at once.
- **Syntax:** `less <file_name>`
- **Interactive Commands:**
  - `q`: Quit
  - `Spacebar`: Page down
  - `b`: Page up
  - `/search_term`: Search for text
- **Example:**
  ```bash
  $ less /var/log/syslog
  ```

### `head` and `tail`

- **Significance:** Display the beginning (`head`) or end (`tail`) of files. Essential for quickly inspecting file contents without viewing the entire file.
- **Syntax:** `head [options] <file_name>` or `tail [options] <file_name>`
- **Common Options:**
  - `-n <number>`: Specifies the number of lines to show (default is 10). Can also use `-<number>`.
  - `-c <number>`: Show specified number of bytes instead of lines.
  - `-f`: **Follow** (tail only). Keeps the file open and prints new lines as they are added - crucial for log monitoring.
  - `-F`: **Follow with retry** (tail only). Similar to `-f` but retries if file is rotated.
  - `-q`: **Quiet**. Suppresses headers when viewing multiple files.
  - `-v`: **Verbose**. Always shows headers with file names.
- **Examples:**
  ```bash
  # Show the first 5 lines of a file
  $ head -n 5 notes.txt
  # or equivalently:
  $ head -5 notes.txt

  # Show the last 20 lines of a file
  $ tail -n 20 notes.txt
  # or equivalently:
  $ tail -20 notes.txt

  # Show first 100 bytes of a binary file
  $ head -c 100 binary_file

  # Watch a log file in real-time (most common use case)
  $ tail -f /var/log/syslog

  # Monitor multiple log files
  $ tail -f /var/log/apache2/access.log /var/log/apache2/error.log

  # Show last 50 lines and follow
  $ tail -n 50 -f application.log

  # View multiple files with headers
  $ head -v file1.txt file2.txt
  ==> file1.txt <==
  Line 1 of file1
  Line 2 of file1

  ==> file2.txt <==
  Line 1 of file2
  Line 2 of file2
  ```
- **Key Study Points:**
  - Use `tail -f` for real-time log monitoring (debugging, system administration).
  - Combine with `grep` for filtered output: `tail -f logfile | grep "ERROR"`.
  - `head` is useful for checking file format or headers.
  - `-f` keeps running until interrupted (Ctrl+C).
  - Great for large files where you don't need to see everything.

## 5. Process Management

These commands help you see and manage the programs that are running. Understanding processes is crucial for system administration and debugging.

### `ps` (Process Status)

- **Significance:** Shows a "snapshot" of currently running processes. Essential for monitoring system activity and finding specific programs.
- **Syntax:** `ps [options]`
- **Common Options:**
  - `-e`: Show **every** process on the system.
  - `-f`: Show **full** format listing (includes PPID, start time, etc.).
  - `-l`: Show **long** format with more technical details.
  - `-u <user>`: Show processes for a specific user.
  - `-p <pid>`: Show information for specific process IDs.
  - `aux`: BSD-style combination (most commonly used):
    - `a`: Show processes for **all** users.
    - `u`: Show **user-oriented** format.
    - `x`: Show processes not attached to a **terminal**.
- **Understanding `ps aux` Output:**
  - `USER`: Process owner
  - `PID`: Process ID (unique identifier)
  - `%CPU`: CPU usage percentage
  - `%MEM`: Memory usage percentage
  - `VSZ`: Virtual memory size
  - `RSS`: Resident set size (physical memory)
  - `TTY`: Controlling terminal
  - `STAT`: Process state (R=running, S=sleeping, Z=zombie, etc.)
  - `START`: Start time
  - `TIME`: Total CPU time used
  - `COMMAND`: Command that started the process
- **Examples:**
  ```bash
  # Show all processes in detail (most common command)
  $ ps aux

  # Show all processes with full details
  $ ps -ef

  # Find processes for current user
  $ ps u

  # Find specific program
  $ ps aux | grep "firefox"

  # Show processes for a specific user
  $ ps -u username

  # Show details for specific PIDs
  $ ps -p 1234 5678

  # Show process tree (with pstree if available)
  $ ps -ejH  # or use: pstree
  ```

### `kill` (Terminate Process)

- **Significance:** Sends signals to processes to control their behavior. Most commonly used to terminate unwanted processes.
- **Syntax:** `kill [signal] <PID>` or `kill [signal] <process_name>`
- **Common Signals:**
  - `SIGTERM` (15, default): Polite request to terminate. Allows cleanup.
  - `SIGKILL` (9): Force kill. Cannot be ignored. Use as last resort.
  - `SIGSTOP` (19): Stop/pause process.
  - `SIGCONT` (18): Continue stopped process.
  - `SIGHUP` (1): Hang up. Often causes programs to restart/reload config.
  - `SIGINT` (2): Interrupt (Ctrl+C equivalent).
- **Examples:**
  ```bash
  # Find the PID of a stuck program
  $ ps aux | grep "stuck_app"
  rihan    12345  0.5  2.4 123456 9876 ?   Sl   15:01   0:10 ./stuck_app

  # Try to close it politely (SIGTERM)
  $ kill 12345
  # or explicitly:
  $ kill -TERM 12345

  # If that doesn't work, force kill (SIGKILL)
  $ kill -9 12345
  # or:
  $ kill -KILL 12345

  # Stop a process temporarily
  $ kill -STOP 12345

  # Resume a stopped process
  $ kill -CONT 12345

  # Kill all processes with a specific name (use carefully!)
  $ killall firefox

  # Send signal to process group
  $ kill -9 -12345  # Kill process group 12345
  ```
- **Key Study Points:**
  - Always try `SIGTERM` (15) before `SIGKILL` (9) to allow proper cleanup.
  - `SIGKILL` cannot be caught or ignored - use only when necessary.
  - Use `killall` to kill processes by name, but be specific to avoid killing wrong processes.
  - Process states: R (running), S (sleeping), D (uninterruptible sleep), Z (zombie), T (stopped).
  - Zombie processes are dead but not cleaned up by parent - usually harmless.

## 6. System, User, & Permissions

### `whoami`

- **Significance:** Tells you the username of the account you are currently logged in as.
- **Example:**
  ```bash
  $ whoami
  rihan
  ```

### `chmod` (Change Mode)

- **Significance:** Changes the **permissions** (read, write, execute) of a file or directory. This is a critical security and operational command.
- **Syntax:** `chmod <permissions> <file_name>`
- **Common Options (Symbolic Mode):**
  - `u` (user/owner), `g` (group), `o` (other), `a` (all)
  - `+` (add), `-` (remove), `=` (set exactly)
  - `r` (read), `w` (write), `x` (execute)
- **Common Options (Octal Mode):**
  - Uses numbers to represent permissions: `4` (read), `2` (write), `1` (execute).
  - `7` = `rwx` (4+2+1)
  - `6` = `rw-` (4+2)
  - `5` = `r-x` (4+1)
- **Examples:**
  ```bash
  # Make a shell script executable for the user (owner)
  $ chmod u+x my_script.sh

  # Set permissions: user(rwx), group(r-x), other(r-x)
  # This is very common for executable scripts.
  $ chmod 755 my_script.sh

  # Make a file read-only for everyone
  $ chmod 444 config.txt
  ```

## 7. The Most Important Command: `man` (Manual)

- **Significance:** Shows the official manual page for any command. This is how you learn all other commands and their options in detail.
- **Syntax:** `man <command_name>`
- **Example:**
  ```bash
  # Get all the details about the 'ls' command
  $ man ls
  ```

### Study Questions:
- What is the difference between a shell and a kernel?
- How does `ls -l` output help in understanding file permissions?
- Why is `rm -rf` considered dangerous?
- How can `tail -f` be used for log monitoring?
- What are the different signals that `kill` can send?
