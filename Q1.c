#include <stdio.h>
#include <ctype.h>
#include <string.h>

// here we are defining stack structure
struct Stack {
    int top;
    char items[100];
};

// defining push function and it will push into stack structure using items and top
void push(struct Stack *s, char x) {
    if (s->top == 99) {                 // guard against stack overflow
        printf("Stack Overflow\n");
        return;
    }
    s->top++;
    s->items[s->top] = x;
}

// defining pop function and it will pop top element from stack
char pop(struct Stack *s) {
    return s->items[s->top--];
}

// defining peek function and it will return what is at the top of stack
char peek(struct Stack *s) {
    return s->items[s->top];
}

// it will just check whether the stack is empty or not
int isEmpty(struct Stack *s) {
    return s->top == -1;
}

int precedence(char x) {
    switch (x) {
        case '^': return 3;
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        default:  return 0;
    }
}

// only for ^ as it is right associative
int isRightAssociative(char x) {
    if (x == '^')
        return 1;
    return 0;
}

// conversion function
void infixToPostfix(char infix[], char postfix[]) {
    struct Stack s;
    s.top = -1;
    int i = 0;
    int k = 0;

    while (infix[i] != '\0') {

        // skip whitespace (needed now that fgets can capture spaces)
        if (isspace((unsigned char)infix[i])) {
            i++;
            continue;
        }

        if (isdigit((unsigned char)infix[i])) {
            postfix[k++] = infix[i];
        }
        else if (infix[i] == '(') {
            push(&s, infix[i]);
        }
        else if (infix[i] == ')') {
            while (!isEmpty(&s) && peek(&s) != '(') {
                postfix[k++] = pop(&s);
            }
            if (!isEmpty(&s)) {
                pop(&s); // discard the matching '('
            } else {
                printf("Error: unbalanced parentheses\n");
            }
        }
        else if (infix[i] == '+' ||
                 infix[i] == '-' ||
                 infix[i] == '*' ||
                 infix[i] == '/' ||
                 infix[i] == '^') {
            while (!isEmpty(&s) &&
                   peek(&s) != '(' &&
                   precedence(peek(&s)) >= precedence(infix[i]) &&
                   !isRightAssociative(infix[i])) {
                postfix[k++] = pop(&s);
            }
            push(&s, infix[i]);
        }
        else {
            printf("Error: invalid character '%c'\n", infix[i]);
        }

        i++;
    }

    // flush remaining operators, catching unmatched '(' instead of leaking it into output
    while (!isEmpty(&s)) {
        if (peek(&s) == '(') {
            printf("Error: unbalanced parentheses\n");
            pop(&s);
            continue;
        }
        postfix[k++] = pop(&s);
    }

    postfix[k] = '\0';
}

// main function
int main() {
    char infix[100];
    char postfix[100];

    printf("Enter infix expression: ");
    fgets(infix, sizeof(infix), stdin);

    // strip trailing newline left by fgets
    infix[strcspn(infix, "\n")] = '\0';

    printf("Infix expression:   %s\n", infix);
    infixToPostfix(infix, postfix);
    printf("Postfix expression: %s\n", postfix);

    return 0;
}