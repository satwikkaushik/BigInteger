#ifndef BIGINTEGER_H
#define BIGINTEGER_H

typedef struct Node{
    int data;
    struct Node *next, *prev;
}Node;

struct BigInteger{
    Node *head, *tail;
    int length;
    char sign;
};

struct BigInteger initialize(char *s);

// main functions
struct BigInteger add(struct BigInteger list1, struct BigInteger list2);    
struct BigInteger sub(struct BigInteger list1, struct BigInteger list2);    
struct BigInteger mul(struct BigInteger list1, struct BigInteger list2);
struct BigInteger div1(struct BigInteger list1, struct BigInteger list2);
struct BigInteger mod(struct BigInteger list1,struct BigInteger list2);

// helper functions
void insertAtHead(struct BigInteger *list, int val);
void insertAtTail(struct BigInteger *list, int val);

int compareForMag(struct BigInteger list1, struct BigInteger list2);
void removeZeroes(struct BigInteger *list);

void display(struct BigInteger list);

#endif