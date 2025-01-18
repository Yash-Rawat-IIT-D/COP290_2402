#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;      // Pointer to the dynamically allocated array
    int size;        // Number of elements in the array
    int capacity;    // Maximum number of elements the array can hold
} DynamicArray;

// Function to initialize the dynamic array
void initArray(DynamicArray *arr) {
    arr->size = 0;
    arr->capacity = 10; // Initial capacity
    arr->data = (char *)malloc(arr->capacity * sizeof(char));
    if (arr->data == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
}

// Function to resize the array (doubling the capacity)
void resizeArray(DynamicArray *arr, int new_capacity) {
    arr->capacity = new_capacity;
    arr->data = (char *)realloc(arr->data, arr->capacity * sizeof(char));
    if (arr->data == NULL) {
        printf("Memory reallocation failed\n");
        exit(1);
    }
}

// Function to add an element at the end of the array
void push_back(DynamicArray *arr, char value) {
    if (arr->size == arr->capacity) {
        resizeArray(arr, arr->capacity * 2);  // Resize if the array is full
    }
    arr->data[arr->size] = value;
    arr->size++;
}

// Function to remove an element from the end of the array
void pop_back(DynamicArray *arr) {
    if (arr->size > 0) {
        arr->size--;
        if (arr->size > 0 && arr->size <= arr->capacity / 2) {
            resizeArray(arr, arr->capacity / 2);  // Resize if the array is less than half full
        }
    } else {
        printf("Array is empty, cannot pop\n");
    }
}

// Function to access an element by index
char at(DynamicArray *arr, int index) {
    if (index >= 0 && index < arr->size) {
        return arr->data[index];
    } else {
        printf("Index out of bounds\n");
        exit(1);  // Or return some error value
    }
}

// Function to free the dynamically allocated memory
void freeArray(DynamicArray *arr) {
    free(arr->data);
}

int main() {
    DynamicArray arr;
    initArray(&arr);

    // Add elements
    push_back(&arr, 'A');
    push_back(&arr, 'B');
    push_back(&arr, 'C');
    push_back(&arr, 'D');

    // Access elements by index
    printf("Element at index 3: %c\n", at(&arr, 3));

    // Remove an element from the end
    pop_back(&arr);

    // Access the last element after removal
    printf("Element at index 2 after pop: %c\n", at(&arr, 2));

    // Free the dynamically allocated memory
    freeArray(&arr);

    return 0;
}
