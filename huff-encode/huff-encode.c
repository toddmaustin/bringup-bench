#include "libmin.h"

#define MAX_TREE_HT 256

// Huffman tree node
typedef struct HuffmanNode {
    char data;                // Stored character (for leaf nodes)
    int freq;                 // Frequency of the character
    struct HuffmanNode *left; // Left child
    struct HuffmanNode *right;// Right child
} HuffmanNode;

// Min-Heap structure for building the tree
typedef struct {
    int size;
    int capacity;
    HuffmanNode** array;
} MinHeap;

// Function to create a new Huffman node
HuffmanNode* createNode(char data, int freq) {
    HuffmanNode* node = (HuffmanNode*)libmin_malloc(sizeof(HuffmanNode));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

// Function to create a min-heap of given capacity
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)libmin_malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffmanNode**)libmin_malloc(capacity * sizeof(HuffmanNode*));
    return minHeap;
}

// Swap two Huffman nodes
void swapNodes(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify at given index (min-heap)
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Check if heap size is one
int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Extract minimum node from heap
HuffmanNode* extractMin(MinHeap* minHeap) {
    HuffmanNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert a node into the min-heap
void insertMinHeap(MinHeap* minHeap, HuffmanNode* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

// Build the min-heap
void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size;
    for (int i = (n - 1) / 2; i >= 0; i--)
        minHeapify(minHeap, i);
}

// Create and build min-heap from given characters and their frequencies
MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; i++) {
        minHeap->array[i] = createNode(data[i], freq[i]);
    }
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// Build Huffman Tree and return its root
HuffmanNode* buildHuffmanTree(char data[], int freq[], int size) {
    HuffmanNode *left, *right, *top;
    MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
    
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        
        top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

// Recursively generate Huffman codes and store them in codes array
// codes[c] will store the Huffman code (a string of '0's and '1's) for character c.
void generateCodes(HuffmanNode* root, char arr[], int top, char codes[][MAX_TREE_HT]) {
    if (root->left) {
        arr[top] = '0';
        generateCodes(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = '1';
        generateCodes(root->right, arr, top + 1, codes);
    }
    // If node is a leaf, store the code
    if (!root->left && !root->right) {
        arr[top] = '\0';
        libmin_strcpy(codes[(unsigned char)root->data], arr);
    }
}

// Encode the input string using the generated Huffman codes
void encodeString(const char* input, char codes[][MAX_TREE_HT], char* encoded) {
    encoded[0] = '\0';
    for (int i = 0; input[i]; i++) {
        libmin_strcat(encoded, codes[(unsigned char)input[i]]);
    }
}

// Decode the encoded bit string using the Huffman tree
void decodeString(HuffmanNode* root, const char* encoded, char* decoded) {
    int idx = 0, out_idx = 0;
    HuffmanNode* curr = root;
    while (encoded[idx]) {
        if (encoded[idx] == '0')
            curr = curr->left;
        else
            curr = curr->right;
        
        // If leaf node, record the character and restart from tree root
        if (!curr->left && !curr->right) {
            decoded[out_idx++] = curr->data;
            curr = root;
        }
        idx++;
    }
    decoded[out_idx] = '\0';
}

int main() {
    // Example input string to compress
    char input[] = "this is an example for huffman encoding";
    
    libtarg_start_perf();
    // Count frequency of each character in input
    int freq[256] = {0};
    for (int i = 0; input[i]; i++) {
        freq[(unsigned char)input[i]]++;
    }
    
    // Create arrays for characters and their frequencies (for non-zero frequencies)
    int unique = 0;
    for (int i = 0; i < 256; i++)
        if (freq[i])
            unique++;
    
    char* characters = (char*)libmin_malloc(unique * sizeof(char));
    int* frequencies = (int*)libmin_malloc(unique * sizeof(int));
    
    int index = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            characters[index] = (char)i;
            frequencies[index] = freq[i];
            index++;
        }
    }
    
    // Build Huffman Tree
    HuffmanNode* root = buildHuffmanTree(characters, frequencies, unique);
    
    // Generate Huffman codes for characters
    char codes[256][MAX_TREE_HT] = {{0}};
    char arr[MAX_TREE_HT];
    generateCodes(root, arr, 0, codes);
    
    // Encode input string
    char encoded[1024] = {0};
    encodeString(input, codes, encoded);

    // Decode the encoded string
    char decoded[1024] = {0};
    decodeString(root, encoded, decoded);
    libtarg_stop_perf();

    // Print generated Huffman codes
    libmin_printf("Huffman Codes:\n");
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            libmin_printf("'%c': %s\n", i, codes[i]);
        }
    }
    
    libmin_printf("\nEncoded string:\n%s\n", encoded);
    
    // Report compression metrics:
    int inputBits = libmin_strlen(input) * 8;         // Assuming 8 bits per char originally
    int encodedBits = libmin_strlen(encoded);         // One character per bit in encoded string representation
    libmin_printf("\nCompression Metrics:\n");
    libmin_printf("Original size: %d bits\n", inputBits);
    libmin_printf("Encoded size: %d bits\n", encodedBits);
    libmin_printf("Compression ratio: %.2f%%\n", (double)encodedBits / inputBits * 100);
    
    libmin_printf("\nDecoded string:\n%s\n", decoded);
    
    // Check that the decompressed string matches the original input
    if (libmin_strcmp(input, decoded) == 0)
        libmin_printf("\nDecompression verified: decoded string matches the original input.\n");
    else
        libmin_printf("\nError: decoded string does not match the original input.\n");
    
    // Free allocated memory for characters and frequencies
    libmin_free(characters);
    libmin_free(frequencies);
    
    // Note: In a complete implementation you should also free the Huffman tree nodes.

    libmin_success();    
    return 0;
}

