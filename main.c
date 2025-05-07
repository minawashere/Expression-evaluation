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

float pop(List* list)
{
    if (list == NULL || list->head == NULL) return NAN;

    const float x = list->head->data;
    Node* temp = (Node*)list->head->next;
    free(list->head);
    list->head = temp;
    return x;
}

void destruct_stack(List* list)
{
    if (list == NULL) return;
    while (list->head != NULL)
    {
        pop(list);
    }
    free(list);
}

void push(List* list, float x)
{
    if (list == NULL)
        return;
    Node* new = malloc(sizeof(Node));
    if (list->head == NULL)
    {
        list->head = new;
        new->next = NULL;
        new->data = x;
        return;
    }
    new->data = x;
    new->next = (struct Node*)list->head;
    list->head = new;
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


int main(void)
{
    // char buffer[100];
    // fgets(buffer, 100, stdin);
    // buffer[strlen(buffer) - 1] = '\0';
    // char* postfix = infix_to_postfix(buffer);
    // printf("%s", postfix);
    // free(postfix);
    // return 0;

    Stack* stack = initialize();
    push(stack, 1);
    push(stack, 2);
    push(stack, 3);
    push(stack, 4);

    while (!is_empty(stack))
    {
        int op = pop(stack);
        printf("%d", op);
    }
    destruct_stack(stack);
}
