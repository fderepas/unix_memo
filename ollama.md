Here is a way to have locally a Large Language Model (LLM) which runs on a PC.

```
curl -fsSL https://ollama.com/install.sh > install.sh # inspect the code
sh install.sh                                         # execute the code
```

Let's download [codegemma](https://ollama.com/library/codegemma:7b) from Google via ollama:
```
ollama pull codegemma:7b
```

Now we can run it:
```
ollama run codegemma
```

A prompt appears. If we ask "Write a bash script which asks for numbers and returns a sorted version if the numbers" the anser is:

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
