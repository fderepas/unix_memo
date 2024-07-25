Here is a way to have locally a Large Language Model (LLM) which runs on a PC.

```
curl -fsSL https://ollama.com/install.sh > install.sh # inspect the code
sh install.sh                                         # execute the code
```

Let's download [codegemma](https://ollama.com/library/codegemma:7b) from Google via [ollama](https://ollama.com/):
```
ollama pull codegemma:7b
```
Large image tend to stop, do not hesitate to make a loop, for instance for [llama3.1 70b](https://ollama.com/library/llama3.1:70b) which size is 40Gb:
```
while true; do ollama pull llama3.1:70b; sleep 300; done
```

Now we can run it:
```
ollama run codegemma
```

A prompt appears. If we ask "Write a bash script which asks for numbers and returns a sorted version if the numbers" the answer is:

```bash
#!/bin/bash

# Initialize an empty array to store the numbers
numbers=()

# Prompt the user to enter numbers
while true; do
    read -p "Enter a number (or press Enter to finish): " number
    if [[ -z "$number" ]]; then
        break
    fi
    numbers+=("$number")
done

# Sort the array of numbers
sort -n <<< "${numbers[@]}"

# Print the sorted numbers
for number in "${numbers[@]}"; do
    echo "$number"
done
```
Ok, it's lexicographic order and not numeric order ;-)  [llama3.1 8b](https://ollama.com/library/llama3.1:8b) has a close answer the numeral values and not the lexicographic order:

```bash
readarray -t sorted_numbers < <(printf '%s\n' "${numbers[@]}" | sort -n)
```

This result with Gemma was given in 30 seconds on a dell xps 15 with intel i7 with 16 cores, 32GB RAM, NVIDIA GeForce RTX™ 3050. The 16 cores were used:

![cpu load](https://github.com/fderepas/unix_memo/blob/main/img/ollama_cpu.png)

To list all images type ```ollama list```:

```
NAME            	ID          	SIZE  	MODIFIED
llama3.1:latest 	62757c860e01	4.7 GB	13 minutes ago	
codegemma:latest	0c96700aaada	5.0 GB	10 minutes ago
codegemma:7b    	0c96700aaada	5.0 GB	10 minutes ago
```
