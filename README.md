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
- > -> type: RED
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
- > redirects cat's output to file.txt.
