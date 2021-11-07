/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* TODO: structure definitions */
struct TrieNode {
	struct TrieNode* next[26];
	int wordCount;
};

/* NOTE: int return values can be used to indicate errors (typically non-zero)
   or success (typically zero return value) */

/* TODO: change this return type */
void indexPage(const char* url);

int addWordOccurrence(const char* word, struct TrieNode* curr);

void printTrieContents(struct TrieNode* curr, char* wordSoFar);

int freeTrieMemory(struct TrieNode* curr);

int getText(const char* srcAddr, char* buffer, const int bufSize);

int main(int argc, char** argv){
  /* TODO: write the (simple) main function

  /* argv[1] will be the URL to index, if argc > 1 */
	indexPage(argv[1]);
  
	return 0;
}

/* TODO: define the functions corresponding to the above prototypes */
struct TrieNode* init_trie(){
	
	struct TrieNode* curr = (struct TrieNode*)malloc(sizeof(struct TrieNode));
	
	for(int i =0; i < 26; i++){
		curr->next[i] = NULL;
	}
	curr->wordCount = 0;
	
	return curr;
}

/* TODO: change this return type */
void indexPage(const char* url) {
	
	char buffer[300000];
	char newBuffer[300000];
	int newBuffercount = 0;
	struct TrieNode* curr = init_trie();

	getText(url, buffer, 300000);
	for(int i = 0; i < 300000 && buffer[i] != '\0'; i++){
		char temp = buffer[i];
		if(buffer[i] >= 65 && buffer[i] <= 90){
			temp += 32;
			newBuffer[newBuffercount] = temp;
			newBuffercount++;
		}
		else if(buffer[i] >= 97 && buffer[i] <= 122){
		  newBuffer[newBuffercount] = temp;
		  newBuffercount++;
		}
		else{
			if(newBuffer[newBuffercount - 1] != '\n'){
				newBuffer[newBuffercount] = '\n';
				newBuffercount++;
			}
		}
	}

	newBuffer[newBuffercount] = '\0';
	
	printf("%s\n", url);
	
	char *token = strtok(newBuffer, "\n");
	while(token != NULL){
		printf("\t%s\n", token);
		addWordOccurrence(token, curr);
		
		token = strtok(NULL, "\n");
	}
	
	
	
	printTrieContents(curr,"");
	
	freeTrieMemory(curr);
	
	
	return;
}

int addWordOccurrence(const char* word, struct TrieNode* curr) {

	int len = strlen(word);
	if(len == 1) {
		int letter = word[0] -97;
    //Jimmy revise >>>
    if(curr->next[letter] == NULL){
      struct TrieNode* ptr;
      ptr = init_trie();
      curr->next[letter] = ptr;
    }
    //<<< 
		curr->next[letter]->wordCount++;
		return curr->next[letter]->wordCount;
	}
	
	char newWord[len];
	
	for(int n = 0; n < len - 1; n++) {
		newWord[n] = word[n + 1];
	}
	
	newWord[len - 1] = '\0';
	int letter = word[0] -97;
	
	//Jimmy revise >>>
	if(curr->next[letter] == NULL){
		struct TrieNode* ptr;
		ptr = init_trie();
		curr->next[letter] = ptr;
	}
	//<<<
	
	addWordOccurrence(newWord, curr->next[letter]);
	
	return 0;
}

void printTrieContents(struct TrieNode* curr, char* wordSoFar) {
	
	if(curr->wordCount > 0){
		printf("%s: %d\n", wordSoFar, curr->wordCount);
	}
	
	int len = strlen(wordSoFar);

	char newWord[len + 1];
	for(int i = 0; i < len; i++) {
		newWord[i] = wordSoFar[i];
	}
	newWord[len + 1] = '\0';
	
	for(int n = 0; n < 26; n++) {
		if(curr->next[n] != NULL) {
			newWord[len] = (char) 97 + n;
			printTrieContents(curr->next[n], newWord);
		}
	}

	return;
}

int freeTrieMemory(struct TrieNode* curr) {
	
	int numChildrenFreed = 0;

	for(int n = 0; n < 26; n++) {
		if(curr->next[n] != NULL) {
			freeTrieMemory(curr->next[n]);
			numChildrenFreed++;
		}
	}

	free(curr);

	return numChildrenFreed;
}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
