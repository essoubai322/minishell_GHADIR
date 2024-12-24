## PRESENTATION ABOUT MY PROJECT (MINISHELL) (Azzeddine Moubine "Parsing" | Ayoub Sebaai "EXECUTION")

<img width="11204" alt="2mb" src="https://github.com/user-attachments/assets/a904321e-e988-40bf-8547-a96c0c89b3e7">

## CLICK HERE TO ZOOM IT : https://www.tldraw.com/ro/NvsI859xXdObw8dKl9Vy0?d=v-5431.429.13866.6428.uH0w8XnZixu5yu7qB6l7O


## 1. Parsing
The left side of the chart is labeled as "Parsing" and represents the steps involved in tokenizing and interpreting the command-line input. Key elements include:

**Tokenization:**

Commands are split into tokens (e.g., commands, options, redirections).
Example: ls -la | cat > file.txt is broken down into:
value: ls, type: WORD
value: -la, type: WORD
value: |, type: PIPE
and so on.
Double Quotes & Escaping:

Handling of special cases such as quoted strings ("some text") and escaped characters (\).
Example: Parsing between single and double quotes, ensuring tokens retain their intended value.
Validation:

Checks for invalid syntax, such as unmatched quotes or improper token sequences.

## PARSE Phase (Left Side)
### Input Command:

The user inputs a command like ls -la | cat > file.txt.
This input is passed to the shell for processing.
**Tokenization:**

The input is broken into tokens using spaces or special symbols as delimiters.
For example:
- ls -> type: WORD
- -la -> type: WORD
- | -> type: PIPE
- cat -> type: WORD
- '>' -> type: RED
- file.txt -> type: WORD.
**Quotes and Escapes:**

Handles double quotes, single quotes, and escape characters (\) within the command.
Example:
"hello world" remains as a single token.
Escaped characters like \n are resolved correctly.
**Validation:**

Validates the structure of tokens:
Checks for mismatched quotes.
Ensures proper syntax (e.g., no invalid pipes or redirections).
**Parse Tree Creation:**

Tokens are grouped into a structured form, like a parse tree, which defines the order and hierarchy of operations.
Example:
- ls -la is the first command.
- | indicates a pipe to cat.
- '>' redirects cat's output to file.txt.

## 2. Execution
The right side of the chart is labeled as "Execution" and details how commands are executed once parsed. Key elements include:

**Forking:**

A new process is created for executing the command using fork().
Redirection:

Input/output redirection (>, <) is handled by duplicating file descriptors.
**Example:**
> old_fd[0] = dup(STDIN_FILENO) for input redirection.
> old_fd[1] = dup(STDOUT_FILENO) for output redirection.
**Builtin Commands:**

Some commands (like cd or exit) are executed directly within the shell without forking.
Execve:

For non-builtin commands, the program uses execve() to replace the current process with the command to be executed.
If the execve() call fails, it falls back to error handling.
Signal Handling:

Signals like ctrl-c and ctrl-\ are set up to handle interruptions and terminate processes appropriately.
Why is this important?
This flowchart provides a clear visual overview of how a shell processes user input from the moment it is typed to the execution of commands. Understanding this flow is essential for debugging or implementing your own shell. It explains:

> The structure of a shell program.
> How tokens are created and validated.
> How processes are managed using forking and redirection.
> How errors are handled at various stages.

# EXECUTION Phase (Right Side)
***Signal Setup:***

Signals like ctrl-c (to interrupt) and ctrl-\ (to quit) are configured.
Redirection Handling:

Input and output redirections (>, <) are processed.
Duplicates file descriptors (dup2) to link files or streams correctly.
Handles errors in redirection (e.g., invalid files).
Forking:

- The shell creates a child process using fork().
- The parent process remains active to manage the shell.
- The child process executes the command.
**Built-in Commands:**

If the command is built-in (e.g., cd, exit), it is executed directly without forking.
External Commands:

For external commands, the child process:
Searches for the command's binary in the system's PATH.
Uses execve() to replace the current process image with the command's image.
**Error Handling:**

If execve() fails (e.g., command not found), an error message is displayed.
Piping:

If there’s a pipe (|), the output of the first command becomes the input of the second:
Example:
ls -la | cat: Output of ls -la is passed to cat.
**Final Execution:**

After handling all steps, the process executes the command or pipeline.

ey Flow Example:
For ls -la | cat > file.txt:

Parsing Phase:

- ls → WORD
- -la → WORD
- | → PIPE
- cat → WORD
- '>' → RED
- file.txt → WORD.
****Parse tree groups commands as:****
- ls -la → first command.
- Pipe to cat.
- Redirect cat output to file.txt.
**Execution Phase:**

> ls -la runs in a child process.
> Its output is piped to cat in another process.
> cat redirects its output to file.txt.
