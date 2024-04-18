#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

struct Trie {
    int count;
    struct Trie* children[ALPHABET_SIZE];
};

// Function to initialize a new Trie node
struct Trie* createTrie() {
    struct Trie* node = (struct Trie*)malloc(sizeof(struct Trie));
    node->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}

// Function to insert a word into the trie
void insert(struct Trie** ppTrie, char* word) {
    struct Trie* curr = *ppTrie;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!curr->children[index])
            curr->children[index] = createTrie();
        curr = curr->children[index];
    }
    curr->count++;
}

// Function to find the number of occurrences of a word in the trie
int numberOfOccurrences(struct Trie* pTrie, char* word) {
    struct Trie* curr = pTrie;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!curr->children[index])
            return 0;
        curr = curr->children[index];
    }
    return curr->count;
}

// Function to deallocate the trie
struct Trie* deallocateTrie(struct Trie* pTrie) {
    if (pTrie) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (pTrie->children[i])
                deallocateTrie(pTrie->children[i]);
        }
        free(pTrie);
    }
    return NULL;
}

int readDictionary(char *filename, char ***pInWords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; // Return -1 on error
    }

    // Count the number of words in the file
    int wordCount = 0;
    char buffer[100]; // Adjust buffer size according to your needs

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        wordCount++;
    }

    // Allocate memory for the array of words
    *pInWords = (char **)malloc(wordCount * sizeof(char *));
    if (*pInWords == NULL) {
        fclose(file);
        return -1; // Return -1 on memory allocation error
    }

    // Rewind to the beginning of the file
    rewind(file);

    // Read words into the array
    int index = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Remove newline character, if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Allocate memory for the word and copy it into the array
        (*pInWords)[index] = strdup(buffer);
        if ((*pInWords)[index] == NULL) {
            fclose(file);
            // Free memory allocated so far
            for (int i = 0; i < index; i++) {
                free((*pInWords)[i]);
            }
            free(*pInWords);
            return -1; // Return -1 on memory allocation error
        }
        index++;
    }

    fclose(file);

    return wordCount; // Return the number of words read
}

int main(void) {
    // read the number of words in the dictionary
    // parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    struct Trie* trie = createTrie();
    for (int i = 0; i < 5; i++)
        insert(&trie, pWords[i]);
    for (int i = 0; i < 5; i++)
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(trie, pWords[i]));
    trie = deallocateTrie(trie);
    if (trie != NULL)
        printf("There is an error in this program\n");
    return 0;
}