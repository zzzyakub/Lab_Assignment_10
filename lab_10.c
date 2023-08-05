#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie {
    struct TrieNode* root;
};

// trie node structure
struct TrieNode {
    struct TrieNode* children[26]; 
    int count;
};

// creates new trie nodes
struct TrieNode* createTrieNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    for (int i = 0; i < 26; ++i) {
        newNode->children[i] = NULL;
    }
    newNode->count = 0;
    return newNode;
}

// initializes trie structure
struct Trie* createTrie() {
    struct Trie* newTrie = (struct Trie*)malloc(sizeof(struct Trie));
    newTrie->root = createTrieNode();
    return newTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word) {
    struct TrieNode* node = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i) {
        int index = word[i] - 'a';
        if (node->children[index] == NULL) {
            node->children[index] = createTrieNode();
        }
        node = node->children[index];
    }
    node->count++;
}

// deallocates trie node
void deallocateTrieNode(struct TrieNode* node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < 26; ++i) {
        if (node->children[i] != NULL) {
            deallocateTrieNode(node->children[i]);
        }
    }

    free(node);
}

// deallocate trie structure
void deallocateTrie(struct Trie* pTrie) {
    if (pTrie == NULL) {
        return;
    }

    deallocateTrieNode(pTrie->root);
    free(pTrie->root);
    free(pTrie);
}

// computes the number of occurances of the word
int numberOfOccurrences(struct Trie* pTrie, char* word) {
    struct TrieNode* node = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i) {
        int index = word[i] - 'a'; 
        if (node->children[index] == NULL) {
            return 0;
        }
        node = node->children[index];
    }
    return node->count;
}

// this function will return number of words in the dictionary
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file: %s\n", filename);
        return 0;
    }

    int numWords = 0;
    char word[256];

    while (fscanf(file, "%s", word) != EOF && numWords < 256) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char* inWords[256];
    
    //read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }
    
    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    // parse lineby line, and insert each word to the trie data structure
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // deallocates trie
    deallocateTrie(pTrie);

    // Deallocate the words in the dictionary array
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}
