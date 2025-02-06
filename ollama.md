Today locally hosted Large Language Models (LLMs) can be used as man pages. For instance they provide good answers to questions such as "how to get the size of swap memory under linux?".

Here is a way to have locally a Large Language Model (LLM) which runs on a PC. First install  [ollama](https://ollama.com/):

```
curl -fsSL https://ollama.com/install.sh > install.sh # inspect manually the code
sh install.sh                                         # execute the code
```
 
Let's download [llama3.2](https://ollama.com/library/llama3.2) from Meta via [ollama](https://ollama.com/):
```
ollama pull llama3.2
```
or if you prefer Deepseek:
```
ollama run deepseek-r1:32b
```
Now we can run the model:
```
ollama run llama3.2
```
A prompt appears. We can now ask questions such as "What are memory protection keys in the context of CPU?". If we ask "Write a bash script which asks for numbers and returns a sorted version of the numbers" the answer generated locally without an internet connection is:

```bash
#!/bin/bash

# Ask for input
echo "Enter some numbers (type 'done' when finished)"
read -r num

numbers=()

while true; do
  read -r line
  if [ "$line" = "done" ]; then
    break
  fi
  numbers+=($line)
done

# Validate user input
if [ ${#numbers[@]} -eq 0 ]; then
  echo "No numbers entered!"
  exit 1
fi

# Sort the numbers
readarray -t sorted_numbers < <(printf '%s\n' "${numbers[@]}" | sort -n)

# Print the sorted list
echo "Sorted numbers:"
printf '%s ' "${sorted_numbers[@]}"
```
This result with Llama was given in 30 seconds on a Lenovo Thinkpad P14 with 16 cores, 32GB RAM. The 16 cores were used:

![cpu load](https://github.com/fderepas/unix_memo/blob/main/img/ollama_cpu.png)

To list all images type ```ollama list```:

```
NAME            	ID          	SIZE  	MODIFIED
llama3.1:latest 	62757c860e01	4.7 GB	13 minutes ago	
codegemma:latest	0c96700aaada	5.0 GB	10 minutes ago
codegemma:7b    	0c96700aaada	5.0 GB	10 minutes ago
```
