Today locally hosted Large Language Models (LLMs) can be used as man pages. For instance they provide good answers to questions such as "how to get the size of swap memory under linux?".

Here is a way to have locally a Large Language Model (LLM) which runs on a PC. First install  [ollama](https://ollama.com/):

```
curl -fsSL https://ollama.com/install.sh > install.sh # inspect manually the code
sh install.sh                                         # execute the code
```
 
Let's download [llama3.1 8b](https://ollama.com/library/llama3.1:8b) from Meta via [ollama](https://ollama.com/):
```
ollama pull llama3.1
```
Large image tend to stop, do not hesitate to make a loop, for instance for [llama3.1 70b](https://ollama.com/library/llama3.1:70b) from Meta which size is 40Gb the following script is useful:
```
while true; do ollama pull llama3.1:70b; sleep 300; done
```

Now we can run it:
```
ollama run llama3.1
```

A prompt appears. If we ask "Write a bash script which asks for numbers and returns a sorted version of the numbers" the answer generated locally without an internet connection is:

```bash
#!/bin/bash

# Ask the user to enter numbers until they type 'done'
numbers=()
while true; do
  read -p "Enter a number, or type 'done' to finish: " num
  if [ "$num" = "done" ]; then
    break
  fi
  if [[ $num =~ ^[0-9]+$ ]]; then
    numbers+=("$num")
  else
    echo "That's not a valid number. Please try again."
  fi
done

# Sort the numbers and print them out
readarray -t sorted_numbers < <(printf '%s\n' "${numbers[@]}" | sort -n)

echo "You entered:"
for num in "${sorted_numbers[@]}"; do
  echo "$num"
done
```
This result with Llama was given in 30 seconds on a dell xps 15 with intel i7 with 16 cores, 32GB RAM, NVIDIA GeForce RTX™ 3050. The 16 cores were used:

![cpu load](https://github.com/fderepas/unix_memo/blob/main/img/ollama_cpu.png)

To list all images type ```ollama list```:

```
NAME            	ID          	SIZE  	MODIFIED
llama3.1:latest 	62757c860e01	4.7 GB	13 minutes ago	
codegemma:latest	0c96700aaada	5.0 GB	10 minutes ago
codegemma:7b    	0c96700aaada	5.0 GB	10 minutes ago
```
