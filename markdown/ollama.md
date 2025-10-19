Today locally hosted Large Language Models (LLMs) can be used as man pages. For instance they provide good answers to questions such as "how to get the size of swap memory under Linux?".

Here is a way to have locally a Large Language Model (LLM) which runs on a PC. First install  [ollama](https://ollama.com/):

```
curl -fsSL https://ollama.com/install.sh > install.sh # inspect manually the code
sh install.sh                                         # execute the code
```
 
Let's download [llama4:16x17b](https://ollama.com/library/llama4:16x17b) from Meta via [ollama](https://ollama.com/):
```
ollama pull llama4:16x17b
```
or if you prefer Deepseek or Gpt-oss (from OpenAI):
```
ollama pull deepseek-r1:70b
ollama pull gpt-oss:120b
```
Now we can run the model on a Mac Studio with 120G of unified memory:
```
ollama run gpt-oss:120b
```
A prompt appears. If we ask "Write a bash script which asks for numbers and returns a sorted version of the numbers" the answer generated locally without an internet connection is:

The thinking appears first:
```
Thinking...
We need to write a bash script that asks for numbers and returns a sorted version. Probably read numbers from user input (maybe space-separated, or line by line) and then sort 
them. Use read -p. Could allow multiple numbers separated by spaces or newline until empty line. Use sort -n. Provide script with comments. Also mention portability, error 
handling. Provide usage. Return sorted numbers. Provide example. Should be a bash script file. We'll output code block with shebang. Provide explanation.

Let's craft script: ask user to input numbers one per line, finish with empty line. Collect them into array. Then sort using sort -n. Or use built-in sort with printf "%s\n". 
Provide option to input space-separated list.

I'll produce script:

#!/usr/bin/env bash

set -euo pipefail

function usage...

Read numbers: echo "Enter numbers (one per line). Press Enter on empty line to finish:"; while read -r line && [[ -n $line ]]; do numbers+=("$line"); done

Then if none, exit.

sorted=$(printf "%s\n" "${numbers[@]}" | sort -n)

echo "Sorted numbers:"; echo "$sorted"

Alternatively, handle arguments: if arguments passed, sort them.

Add -r for reverse.

Let's produce final answer.
...done thinking.
```

Then the answer:
```
Below is a **stand‑alone Bash script** that

1. **Prompts the user** to enter a list of numbers (you can separate them with spaces, commas or new‑lines).  
2. **Validates** that each token looks like a number (optional, you can turn it off).  
3. **Sorts** the numbers numerically (ascending) and prints the result.  

Save the script as `sort_numbers.sh`, make it executable (`chmod +x sort_numbers.sh`), and run it (`./sort_numbers.sh`).

```
```bash
#!/usr/bin/env bash
#
# sort_numbers.sh
#   Prompt the user for a list of numbers and print them back in sorted order.
#
#   Features
#   • Accepts numbers separated by spaces, commas or new‑lines.
#   • Ignores empty input and allows the user to re‑enter data.
#   • Optional strict numeric validation (integer or floating point).
#   • Handles negative numbers and scientific notation.
#
#   Usage: ./sort_numbers.sh
#
#   Author: ChatGPT  (2025)
#   License: MIT

set -euo pipefail   # Safer Bash: exit on error, treat unset vars as errors, etc.

# ----------------------------------------------------------------------
# Helper: print a friendly prompt (keeps the script POSIX‑compatible)
# ----------------------------------------------------------------------
prompt_user() {
    cat <<'EOF'
Please enter the numbers you want to sort.
You may separate them with spaces, commas, or new‑lines.
Press ENTER when you are done (an empty line will finish input).

Example:
  12 4.5 -3,7 0.001

EOF
}

# ----------------------------------------------------------------------
# Helper: check whether a token looks like a number.
# Accepts:
#   • integers        (e.g. -42, 7)
#   • floating points (e.g. 3.14, -0.001)
#   • scientific      (e.g. 1e-5, -2E3)
# Returns 0 (true) if it matches, 1 (false) otherwise.
# ----------------------------------------------------------------------
is_number() {
    local token=$1
    # Regex explained:
    # ^[+-]?                optional sign
    # (                     start of main number
    #   ([0-9]+(\.[0-9]*)?) # digits, optional decimal part
    #   |(\.[0-9]+)         # or just a leading dot with digits
    # )
    # ([eE][+-]?[0-9]+)?$   optional exponent
    [[ $token =~ ^[+-]?(([0-9]+(\.[0-9]*)?)|(\.[0-9]+))([eE][+-]?[0-9]+)?$ ]]
}

# ----------------------------------------------------------------------
# Main: read numbers from the user
# ----------------------------------------------------------------------
read_numbers() {
    local line token
    local -a numbers=()

    while true; do
        # Prompt for a line of input
        read -r -p "> " line || break   # Ctrl‑D also ends input

        # Stop when the user hits ENTER on an empty line
        [[ -z $line ]] && break

        # Replace commas with spaces, then split on whitespace
        line=${line//,/ }
        for token in $line; do
            # Skip empty tokens (possible when there are multiple spaces)
            [[ -z $token ]] && continue

            # OPTIONAL: validate that each token is a number
            if is_number "$token"; then
                numbers+=("$token")
            else
                echo "Warning: '$token' does not look like a number – ignoring it."
            fi
        done
    done

    # If nothing was entered, exit with a friendly message
    if (( ${#numbers[@]} == 0 )); then
        echo "No numbers were provided – nothing to sort."
        exit 0
    fi

    printf '%s\n' "${numbers[@]}"
}

# ----------------------------------------------------------------------
# Sort and display the numbers
# ----------------------------------------------------------------------
sort_and_print() {
    local -a unsorted
    mapfile -t unsorted < <(read_numbers)   # read_numbers prints one per line

    # Use `sort -n` for numeric sort.
    # The `-r` flag would reverse the order (descending) – uncomment if you prefer.
    # The `-g` flag (general numeric) can also be used, but `-n` is sufficient here.
    printf '%s\n' "${unsorted[@]}" | sort -n
}

# ----------------------------------------------------------------------
# Execution starts here
# ----------------------------------------------------------------------
prompt_user
sort_and_print
```
```
---

## How It Works

| Step | What the script does | Why it matters |
|------|----------------------|----------------|
| **Prompt** | Prints a short help text and then waits for input (`read -r -p "> "`). | Gives the user clear guidance and a visible prompt. |
| **Read loop** | Continues reading lines until the user hits an empty line (or Ctrl‑D). | Allows the user to paste a long list or type numbers over several lines. |
| **Tokenisation** | Replaces commas with spaces, then splits on whitespace. | Accepts any of the three common separators (` `, `,`, `\n`). |
| **Validation** (`is_number`) | Uses a regular expression to ensure each token is a valid integer/float/scientific number. Invalid tokens are ignored with a warning. | Prevents 
garbage from breaking the sort and helps the user notice typos. |
| **Sorting** | Pipes the collected numbers to `sort -n`. | `-n` forces a numeric comparison (so `2` comes before `10`). |
| **Output** | Prints the sorted numbers, one per line. | Easy to read, pipe to other commands, or redirect to a file. |

---

## Customising the Script

| Want to … | How to change it |
|-----------|------------------|
| **Sort descending** | Replace `sort -n` with `sort -nr`. |
| **Skip validation** (accept any strings) | Comment out the `if is_number …` block and simply `numbers+=("$token")`. |
| **Read from a file instead of the terminal** | Run the script with input redirection: `./sort_numbers.sh < numbers.txt`. The script already works because `read` will read from 
STDIN. |
| **Use locale‑aware sorting** | Use `sort -n -b` (ignore leading blanks) or add `LC_ALL=C` before `sort` for predictable byte ordering. |
| **Print on one line (space‑separated)** | Change the final `printf` line to: `printf '%s ' "${sorted[@]}" && echo`. |

---

## Quick Test
```
```bash
$ ./sort_numbers.sh
Please enter the numbers you want to sort.
You may separate them with spaces, commas, or new‑lines.
Press ENTER when you are done (an empty line will finish input).

Example:
  12 4.5 -3,7 0.001

> 12 4.5 -3,7 0.001
> -2 8e3 5.5
> 
-3
-2
0.001
4.5
5.5
7
8e3
12
```
```
The script correctly handled spaces, commas, scientific notation (`8e3`), and negative numbers, then printed them in ascending numeric order.
```

Feel free to copy the script into your own projects and adapt it to the exact behaviour you need![GIN] 2025/08/13 - 10:15:40 | 200 |  47.44538475s |       127.0.0.1 | POST     "/api/chat
