# Autocorrect & Autocomplete
## Introduction
**Trie** is a type of k-ary search tree used for storing and searching a specific key from a set. Using Trie, search complexities can be brought to optimal limit (key length). The trie node consists of an array named children of pointers to nodes of size 26, a bool value which indicates if that node is end of word or not, an  integer frequency which indicates how many times the word occurs in the given text file.It also contains an int used,which indicates if the word has already been checked for autocorrect. Each word from the text file is scanned and stored in the Trie using the insert function(working explained in analysis).
>For Auto-Complete- Then the program reads user input and calls checker function. If the word is present in Trie, the checker function returns true,otherwise it calls printAutoSuggestions.It passes the pointer of the node where the last letter in the query string is present.Now, printAutoSuggesstions traverses down all the possible children of that node,recursively calling itself till it reaches an end of word.Then, it will print the new word where the recursion stopped.

>For Auto-Correct- We will check if the word entered by user is present in Trie or not.If not, we will call edits1 function. This function will generate all possible words obtained by making one change to the input word(deletion,insertion,transpose or replace).The it will pass all these words to select-suggesstions() function.Then the edits1 function will call itself again with all the newly generated words as parameters.This will give us all the words obtained after making two changes to the input word. These are again passed to the select-suggestions function.

>The select-suggestions function chooses all the words which are actually present in the dictionary, and then chooses the ones with the highest frequency in the file.It then arranges the word in a array of specified size in increasing order of their frequencies(it also calls a sorting function for this purpose).

## How to Run
Open terminal in the working directory and enter the command 
```
gcc project.c
```
This will generate a a.exe execuatble
type ./a.exe (or ./a.out for linux and mac).
***Make sure the text file is named as "big.txt"and is present in the same folder as the C file.***
On running the executable, It will ask the user for input. Just enter any word and you will get results accordingly.
Press any number to exit program.
## Credits
Nishad Dhuri - 2021CSB1116

Vipul Patil - 2021CSB1141

Darshan Kawale - 2021CSB1080