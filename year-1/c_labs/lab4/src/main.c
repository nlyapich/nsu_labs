#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE  "in.txt"
#define OUTPUT_FILE "out.txt"
#define SIZE_OF_STR 1000

typedef struct Stack_Item{
	void* sym;
	struct Stack_Item* next;
} StackItem;
typedef struct _Stack{
	StackItem* top;
} Stack;

Stack* create(){
	Stack* s = (Stack*)malloc(sizeof(Stack));
	s->top = NULL;
	return s;
}
void makenull(Stack* s){
	StackItem* p;
	while (s->top){
		p = s->top;
		s->top = p->next;
		free(p);
	}
	free(s);
}
void* top(Stack* s){
	if (s->top)
		return (s->top->sym);
	else
		return 0;
}
void* pop(Stack* s){
	StackItem* p = s->top;
	void* a = p->sym;
	s->top = p->next;
	free(p);
	return a;
}
void push(Stack* s, char a){
	StackItem* p = (StackItem*)malloc(sizeof(StackItem));
	if (p){
		p->sym = a;
		p->next = s->top;
		s->top = p;
	}
}
int empty(Stack* s){
	return (s->top == NULL);
}

int check_operation(char cur){
	if (cur == '+' || cur == '-' || cur == '*' || cur == '/') return 1;
	return 0;
}

int priority_operation(char operation){
	if (operation == '+' || operation == '-') return 0;
	if (operation == '*' || operation == '/') return 1;
	return -1;
}

int get_postfix_str(char* str, FILE* fin){
	Stack* stack = create();
	char cur;
	for (int j = 0; j < SIZE_OF_STR, (cur != '\n' && cur != EOF); j++){
		cur = fgetc(fin);
		if (cur == ' ') continue;
		else if (cur - '0' <= 9 && cur - '0' >= 0){
			*str = cur;
			str++;
		}
		else {
			if (*(str - 1) - '0' <= 9 && *(str - 1) - '0' >= 0){
				*str = ' ';
				str++;
			}
			if (cur == '(')
				push(stack, cur);
			else if (cur == ')'){
				while ((*str = pop(stack)) != '('){
					if (!(*str)) return 1; // NULL -> '(' не обнаружена в стеке, был некорректный ввод
					str++;
				}
				*str = '\0';
			}
			else if (check_operation(cur)){
				char top_now;
				while (!empty(stack)){
					top_now = top(stack);
					if (check_operation(top_now) && priority_operation(top_now) - priority_operation(cur) >= 0){
						*str = pop(stack);
						str++;
					} else
						break;
				}
				push(stack, cur);
			}
			else if (cur == '\n' || cur == EOF) break;
			else {
				free(stack);
				return 1;
			}
		}
	}
	while(!empty(stack)){
		if (top(stack) == '(') return 1;
		*str = pop(stack);
		str++;
	}
	makenull(stack);
	return 0;
}

float get_number(char operation, float a, float b){
	if (operation == '+') return a + b;
	if (operation == '-') return a - b;
	if (operation == '*') return a * b;
	if (operation == '/') return a / b;
}

void calc(FILE* fin, FILE* fout){
	char str[SIZE_OF_STR + 1] = {'\0'};

	if (get_postfix_str(&str[0], fin)){
		fprintf(fout, "syntax error");
		printf("syntax error");
		return;
	}

	int k = 0;
	while(*(str + k) != '\0'){
		printf("%c ", *(str + k));
		k++;
	}
	Stack* stack = create();
	push(stack, str[0]);
	push(stack, str[1]);
	size_t cur = 2;
	while (str[cur] != '\0'){
		if (!check_operation(str[cur])){
			push(stack, str[cur]);
		} else {
			char operation = str[cur];
			char now = 0;
			float a = 0.0;
			while (now != ' '){
				a = a*10.0 + (float)now;
				now = pop(stack);
			}
			now = 0;
			float b = 0.0;
			while (now != ' '){
				b = b*10.0 + (float)now;
				now = pop(stack);
			}

			if (operation == '/' && a == 0.0){
				fprintf(fout, "division by zero\n");
				printf("division by zero\n");
				makenull(stack);
				return;
			}
			float new = get_number(operation, b, a);
			push(stack, new);
		}
		cur++;
	}

	float answer = pop(stack);
	makenull(stack);

	fprintf(fout, "%f\n", answer);
	printf("%f\n", answer);
}

int main(void){
	FILE* fin;
	FILE* fout;
	fin = fopen(INPUT_FILE, "r");
	fout = fopen(OUTPUT_FILE, "w+");

	calc(fin, fout);

	fclose(fin);
	fclose(fout);

	return 0;
}
