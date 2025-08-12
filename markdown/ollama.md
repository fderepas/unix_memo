Today locally hosted Large Language Models (LLMs) can be used as man pages. For instance they provide good answers to questions such as "how to get the size of swap memory under linux?".

Here is a way to have locally a Large Language Model (LLM) which runs on a PC. First install  [ollama](https://ollama.com/):

```
curl -fsSL https://ollama.com/install.sh > install.sh # inspect manually the code
sh install.sh                                         # execute the code
```
 
Let's download [llama4:16x17b](https://ollama.com/library/llama4:16x17b) from Meta via [ollama](https://ollama.com/):
```
ollama pull llama4:16x17b
```
or if you prefer Deepseek:
```
ollama run deepseek-r1:32b
```
Now we can run the model:
```
ollama run llama4:16x17b
```
A prompt appears. We can now ask questions such as "What are memory protection keys in the context of CPU?". If we ask "Write a bash script which asks for numbers and returns a sorted version of the numbers" the answer generated locally without an internet connection is:

```bash
#!/bin/bash

# Function to get numbers from user
get_numbers() {
    numbers=()
    while true; do
        read -p "Enter a number (or 'done' to finish): " input
        if [[ $input =~ ^done$ ]]; then
            break
        elif [[ $input =~ ^[0-9]+([.][0-9]+)?$ ]]; then
            numbers+=($input)
        else
            echo "Invalid input. Please enter a number or 'done' to finish."
        fi
    done
    echo "${numbers[@]}"
}

# Function to sort numbers
sort_numbers() {
    local numbers=("$@")
    local sorted_numbers=($(printf "%s\n" "${numbers[@]}" | sort -n))
    echo "${sorted_numbers[@]}"
}

# Main function
main() {
    echo "Enter numbers (type 'done' to finish):"
    numbers=($(get_numbers))
    sorted_numbers=($(sort_numbers "${numbers[@]}"))
    echo "Sorted numbers: ${sorted_numbers[@]}"
}

# Call main function
main
```
This result with Llama4 was given in 5 seconds on recent powerful computer.
