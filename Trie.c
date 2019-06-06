#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//TrieNode struct
typedef struct TrieNode{
  struct TrieNode *children[26];
  bool leaf;
  int count;
} TrieNode;

//Allocates new TrieNode
TrieNode * Trie(){
  TrieNode *node = NULL;
  node = (TrieNode *)malloc(sizeof(TrieNode));
  node->leaf = false;
  node->count = 0;
  //This is needed for no segmentation faults
  for(int i = 0; i < 26; i++){
    node->children[i] = NULL;
  }
  return node;
}

//Inserts word into the Trie
void insert(TrieNode *root, char *word){
  int length = strlen(word);
  TrieNode *curr = root;
  for(int i = 0; i < length; i++){
    if(curr->children[word[i] - 'a'] == NULL){
      curr->children[word[i] - 'a'] = Trie();
    }
    curr = curr->children[word[i] - 'a'];
  }

  curr->count++;
  curr->leaf = true;
}

//Search for the word in the Trie
bool search(TrieNode *root, char *word){
  int length = strlen(word);
  TrieNode *curr = root;
  for(int i = 0; i < length; i++){
    if(curr->children[word[i] - 'a'] == NULL){
      return false;
    }
    curr = curr->children[word[i] - 'a'];
  }

  return curr->leaf;
}

//Find prefix
TrieNode * prefixExists(TrieNode *root, char *word){
  int length = strlen(word);
  TrieNode *curr = root;
  for(int i = 0; i < length; i++){
    if(curr->children[word[i] - 'a'] == NULL){
      return NULL;
    }
    curr = curr->children[word[i] - 'a'];
  }

  return curr;
}

//Get the amount of times the word was inserted into the Trie
int getCount(TrieNode *root, char *word){
  int length = strlen(word);
  TrieNode *curr = root;
  for(int i = 0; i < length; i++){
    if(curr->children[word[i] - 'a'] == NULL){
      return 0;
    }
    curr = curr->children[word[i] - 'a'];
  }

  return curr->count;
}

//Recursively displays everything in the Trie
void displayTrieRecursive(TrieNode *root, char *strTotal, int level){
  if(root->leaf == true){
    strTotal[level]  = '\0';
    printf("%s\n", strTotal);
  }
  for(int i = 0; i < 26; i++){
    if(root->children[i] != NULL){
      strTotal[level] = i + 'a';
      displayTrieRecursive(root->children[i], strTotal, level+1);
    }
  }
}

//End word when alpha not found or not a lowercase alpha.
void processString(char *buffer){
  int length = strlen(buffer);
  for(int i = 0; i < length; i++){
    if(buffer[i] < 'a' || buffer[i] > 'z'){
      buffer[i] = '\0';
    }
  }
}

//Inserts all items in a file into the trie
void importFileIntoTrie(char *filename, int size, TrieNode *root){
    FILE *file = fopen (filename, "r");
    if(file != NULL){
        char buffer[size];
        while(fgets(buffer, sizeof buffer, file) != NULL){
          processString(buffer);
          insert(root, buffer);
        }
        fclose(file);
    }
}

//Guess the word given the given prefix
void guessWord(TrieNode *root, char *word){
  int length = strlen(word);
  TrieNode *curr = prefixExists(root, word);
  char str[50];
  if(curr != NULL){
    strncpy(str, word, sizeof(word));
    displayTrieRecursive(curr, str, length);
  }else{
    printf("This prefix does not exist\n");
    return;
  }

}

void deleteTrie(TrieNode *root){
  for(int i = 0; i < 26; i++){
    if(root->children[i] != NULL){
      deleteTrie(root->children[i]);
    }
  }
  free(root);
}

int main(){
  TrieNode *root = Trie();
  importFileIntoTrie("words_alpha.txt", 50, root);

  //guessWord(root, "dog");

  char line[50];
  while (strcmp(line,"quitq") != 0){
    printf("Type quitq to quit the program.\nThis only accepts lowercase alphabetic characters.\n");
    fgets(line, 50, stdin);
    processString(line);
    printf("\nMatches found for: %s\n", line);
    guessWord(root, line);
  }

  deleteTrie(root);

  return 0;
}
