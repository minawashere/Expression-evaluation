#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

typedef struct
{
    float data;
    struct Node* next;
} Node;

typedef struct
{
    Node* head;
} List;

typedef List Stack;

List* createList()
{
    List* list = malloc(sizeof(List));
    list->head = NULL;
    return list;
}

float pop(Stack *s)
{
    if (s == NULL || s->head == NULL) return NAN;

    const float x = s->head->data;
    Node* temp = (Node*)s->head->next;
    free(s->head);
    s->head = temp;
    return x;
}

void destruct_stack(Stack* s)
{
    if (s == NULL) return;
    while (s->head != NULL)
    {
        pop(s);
    }
    free(s);
}

void push(Stack* s, float value)
{
    if (s == NULL)
        return;
    Node* new = malloc(sizeof(Node));
    if (s->head == NULL)
    {
        s->head = new;
        new->next = NULL;
        new->data = value;
        return;
    }
    new->data = value;
    new->next = (struct Node*)s->head;
    s->head = new;
}

Stack* initialize()
{
    Stack* stack = createList();
    return stack;
}

float peek(const Stack* stack)
{
    return stack->head->data;
}


int is_empty(const Stack* stack)
{
    return stack->head == NULL;
}

int precedence(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
    case '%':
        return 2;
    case '^':
        return 3;
    default:
        return 0;
    }
}

int is_operator(const char op)
{
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '%' || op == '^';
}

float operation(char operator, float oa1, float oa2)
{
    switch (operator)
    {
        case '+':
            return oa1 + oa2;
        case '-':
            return oa1 - oa2;
        case '*':
            return oa1 * oa2;
        case '/':
            return oa1 / oa2;
        case '%':
            return (float) ((int) oa1 % (int)oa2);
        case '^':
            return pow(oa1, oa2);
        default:
            return 0;
    }
}

char* infix_to_postfix(char* infix)
{
    int len = (int)strlen(infix);
    char* postfix = malloc(2 * len);
    postfix[0] = '\0';
    Stack* stack = initialize();
    char* token = strtok(infix, " ");

    while (token != NULL)
    {
        if (strlen(token) == 1)
        {
            if (is_operator(token[0]))
            {
                while (!is_empty(stack) && precedence((char)peek(stack)) >= precedence(token[0]))
                {
                    char op = (char)pop(stack);
                    strncat(postfix, &op, 1);
                    strcat(postfix, " ");
                }
                push(stack, token[0]);
            }
            else if (token[0] == '(')
                push(stack, token[0]);
            else if (token[0] == ')')
                while (!is_empty(stack))
                {
                    char op = (char)pop(stack);
                    if (op == '(') break;
                    strncat(postfix, &op, 1);
                    strcat(postfix, " ");
                }
            else
            {
                strcat(postfix, token);
                strcat(postfix, " ");
            }
        }
        else
        {
            strcat(postfix, token);
            strcat(postfix, " ");
        }
        token = strtok(NULL, " ");
    }

    while (!is_empty(stack))
    {
        char op = (char)pop(stack);
        strncat(postfix, &op, 1);
        strcat(postfix, " ");
    }

    destruct_stack(stack);
    return postfix;
}

float evaluatePostfix(char* postfix)
{
    Stack* stack = initialize();
    char* token = strtok(postfix, " ");
    float oa1, oa2;
    while (token != NULL) {
        if (strlen(token) == 1 && is_operator(token[0])) {
            oa2 = pop(stack);
            oa1 = pop(stack);
            push(stack, operation(token[0], oa1, oa2));
        } else {
            sscanf(token, "%f", &oa1);
            push(stack, oa1);
        }
        token = strtok(NULL, " ");
    }
    float val = pop(stack);
    destruct_stack(stack);
    return val;
}

int main(void)
{
    char buffer[100];
    printf("Input (Infix): ");
    fgets(buffer, 100, stdin);
    size_t l = 0;
    if (buffer[(l = strlen(buffer)-1)] == '\n') {
        buffer[l] = '\0';
    }
    char* postfix = infix_to_postfix(buffer);
    printf("Output (Postfix): %s\n", postfix);
    printf("Value: %f\n", evaluatePostfix(postfix));
    free(postfix);
    printf("Press Enter to exit.");
    fgets(buffer, 3, stdin);
    return 0;
}
