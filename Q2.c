#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX 200

struct NumStack {
    int top;
    double items[MAX];
};

struct CharStack {
    int top;
    char items[MAX];
};

void pushNum(struct NumStack *s, double x) {
    if (s->top == MAX - 1) { printf("Stack Overflow\n"); return; }
    s->items[++s->top] = x;
}

double popNum(struct NumStack *s) {
    return s->items[s->top--];
}

void pushChar(struct CharStack *s, char x) {
    if (s->top == MAX - 1) { printf("Stack Overflow\n"); return; }
    s->items[++s->top] = x;
}

char popChar(struct CharStack *s) {
    return s->items[s->top--];
}

char peek(struct CharStack *s) {
    return s->items[s->top];
}

int isEmpty(struct CharStack *s) {
    return s->top == -1;
}

int precedence(char x) {
    if (x == '^') return 3;
    if (x == '*' || x == '/') return 2;
    if (x == '+' || x == '-') return 1;
    return 0;
}

int isRightAssociative(char x) {
    return x == '^';
}

int calculate(double a, double b, char op, double *result) {
    switch (op) {
        case '+': *result = a + b; return 1;
        case '-': *result = a - b; return 1;
        case '*': *result = a * b; return 1;
        case '/':
            if (b == 0) { printf("Error: division by zero\n"); return 0; }
            *result = a / b;
            return 1;
        case '^': *result = pow(a, b); return 1;
        default:  return 0;
    }
}

int processTop(struct NumStack *nums, struct CharStack *ops) {
    if (nums->top < 1) { printf("Error: malformed expression\n"); return 0; }
    double b = popNum(nums);
    double a = popNum(nums);
    char op = popChar(ops);
    double result;
    if (!calculate(a, b, op, &result)) return 0;
    pushNum(nums, result);
    return 1;
}

int evaluate(char infix[], double *finalResult) {
    struct NumStack nums;
    struct CharStack ops;
    nums.top = -1;
    ops.top = -1;

    int i = 0;
    while (infix[i] != '\0') {

        if (isspace((unsigned char)infix[i])) {
            i++;
            continue;
        }

        if (isdigit((unsigned char)infix[i])) {
            double num = 0;
            while (isdigit((unsigned char)infix[i])) {
                num = num * 10 + (infix[i] - '0');
                i++;
            }
            pushNum(&nums, num);
            continue;   /* i already advanced past the number */
        }

        else if (infix[i] == '(') {
            pushChar(&ops, infix[i]);
        }

        else if (infix[i] == ')') {
            while (!isEmpty(&ops) && peek(&ops) != '(') {
                if (!processTop(&nums, &ops)) return 0;
            }
            if (isEmpty(&ops)) {
                printf("Error: unbalanced parentheses\n");
                return 0;
            }
            popChar(&ops);   /* discard '(' */
        }

        else if (strchr("+-*/^", infix[i])) {
            char op = infix[i];
            while (!isEmpty(&ops) &&
                   peek(&ops) != '(' &&
                   precedence(peek(&ops)) >= precedence(op) &&
                   !isRightAssociative(op)) {
                if (!processTop(&nums, &ops)) return 0;
            }
            pushChar(&ops, op);
            i++;
            continue;
        }

        else {
            printf("Error: invalid character '%c'\n", infix[i]);
            return 0;
        }

        i++;
    }

    while (!isEmpty(&ops)) {
        if (peek(&ops) == '(') {
            printf("Error: unbalanced parentheses\n");
            return 0;
        }
        if (!processTop(&nums, &ops)) return 0;
    }

    if (nums.top != 0) {
        printf("Error: malformed expression\n");
        return 0;
    }

    *finalResult = popNum(&nums);
    return 1;
}

int main() {
    char infix[MAX];

    printf("Enter infix expression: ");
    fgets(infix, sizeof(infix), stdin);
    infix[strcspn(infix, "\n")] = '\0';

    double result;
    if (!evaluate(infix, &result)) {
        return 1;
    }

    if (result == (long long)result) {
        printf("Result = %lld\n", (long long)result);
    } else {
        printf("Result = %g\n", result);
    }

    return 0;
}