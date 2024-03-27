# Отчет по лабораторной работе N 24 по курсу
# "Фундаментальная информатика"

Студент группы: M80-1107Б-21, Хомяков Иван Андреевич\
Контакты: ivan.hom.200@mail.ru\
Работа выполнена: 05.04.2022\
Преподаватель: Найденов Иван Евгеньевич\


## 1. Тема

Алгоритмы и структуры данных

## 2. Цель работы

Составить программу на языке Си выполнения заданных преобразований арифметических выражений с применением деревьев.

## 3. Задание

№ 7(постфиксная) Редуцировать выражения, заменив операцию умножения переменной на целое число n на сумму n слагаемых: a * 3 = a + a + a

## 4. Оборудование

Процессор: Intel Core i5-8265U @ 8x 3.9GH\
ОП: 7851 Мб\
НМД: 1024 Гб\
Монитор: 1920x1080\

## 5. Программное обеспечение

Операционная система семейства: **linux (ubuntu)**, версия **18.10 cosmic**\
Интерпретатор команд: **bash**, версия **4.4.19**.\
Система программирования: **--**, версия **--**\
Редактор текстов: **emacs**, версия **25.2.2**\
Утилиты операционной системы: **--**\
Прикладные системы и программы: **--**\
Местонахождение и имена файлов программ и данных на домашнем компьютере: **--**\

## 6. Идея, метод, алгоритм решения задачи

*В формах: словесной, псевдокода, графической [блок схема, диаграмма, рисунок, таблица] или формальные спецификации с пред- и постусловиями.*

Реализуем стек, чтобы преобразовать выражение в постфиксную запись. Реализуем дерево, чтобы по стеку построить дерево и уже его преобразовать. Выведем дерево.

## 7. Сценарий выполнения работы

*[план работы, первоначальный текст программы в черновике (можно на отдельном листе) и тесты либо соображения по тестированию]*

Реализуем стек и дерево, используя работу с памятью. Не будем забывать проверить на утечки при помощи библиотеки crt.\
У стека будут по 3 операции для push и top, чтобы код был удобочитаем. Можно было бы использовать дженерики, если бы они были в си, а не define)))\
В деревьях булем использовать указатель на указатель, потому что в некоторых участках кода простого указателя нам будет недостаточно.

## 8. Распечатка протокола
*Подклеить листинг окончательного варианта программы с тестовыми примерами, подписанный преподавателем.*

stach.h
```
#pragma once
#include <stdlib.h>
#include <stdio.h>
#define ll long long
#define inf 1e9

typedef struct _Value
{
    char operation;
    ll num;
    
} Value;

typedef struct _Item
{
    Value data;
    struct _Item* link;
} Item;

typedef struct _Stack
{
    ll size;
    struct _Item* top;
} Stack;

void StackCreate(Stack *s);
char Empty(Stack *s);
ll Size(Stack *s);
void Pop(Stack* s);
void DelStack(Stack* s);
void Reverse(Stack* s);
void PrintStack(Stack *s);
void CopyStack(Stack *a, Stack *b);

void PushNum(Stack *s, ll value);
void PushOper(Stack *s, char value);
void PushVal(Stack *s, Value value);

ll TopNum(const Stack* s);
char TopOper(const Stack* s);
Value TopVal(const Stack* s);
```

stack.c
```
#include "stack.h"

void StackCreate(Stack *s){
    s->size = 0;
    s->top = NULL;
    return s;
}

char Empty(Stack* s) {
    return s->top == NULL;
}

ll Size(Stack* s) {
    return s->size;
}

void PushNum(Stack* s, ll value) {

    Item *item = (Item*)malloc(sizeof(Item));
    Value data = { "", value};

    item->data = data;
    item->link = s->top;
    s->top = item;
    s->size++;

}

void PushOper(Stack* s, char value) {

    Item* item = (Item*)malloc(sizeof(Item));
    Value data = {value, inf};

    item->data = data;
    item->link = s->top;
    s->top = item;
    s->size++;

}

void PushVal(Stack *s, Value val) {
    
    Item* item = (Item*)malloc(sizeof(Item));

    item->data = val;
    item->link = s->top;
    s->top = item;
    s->size++;

}

void Pop(Stack* s) {

    Item* item = s->top;
    s->top = s->top->link;
    free(item);
    s->size--;
}

void DelStack(Stack* s) {

    Item* item;
    while (s->top != NULL) {
        item = s->top;
        s->top = s->top->link;
        free(item);
    }
    s->size = 0;
    s->top = NULL;
}

ll TopNum(Stack* s) {
    return s->top->data.num;
}
char TopOper(Stack* s) {
    return s->top->data.operation;
}
Value TopVal(Stack* s) {
    return s->top->data;
}


void Reverse(Stack* s) {

    Stack rev = *s;
    StackCreate(s);
    
    while (!Empty(&rev)) {
        PushVal(s, TopVal(&rev));
        Pop(&rev);
    }
    DelStack(&rev);
}

void PrintStack(Stack *s) {
    
    Stack scopy = *s; StackCreate(s);

    while (!Empty(&scopy)) {
        ll a = TopNum(&scopy);
        if (a == 1e9) {
            printf("%c ", TopOper(&scopy));
        }
        else {
            printf("%lld ", TopNum(&scopy));
        }
        PushVal(s, TopVal(&scopy));
        Pop(&scopy);
    }
    Reverse(s);
    DelStack(&scopy);

}

void CopyStack(Stack *in, Stack *out) {

    Item* link = in->top;
    while (link != NULL) {
        PushVal(out, link->data);
        link = link->link;
    }
    Reverse(out);

}
```

tree.h
```
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#define ll long long
#define inf 1e9

typedef struct _tnode {
    ll num;
    char oper;
    struct _tnode* left;
    struct _tnode* right;
}Tree;

char IsOperation(char c);

Tree* TreeCreate();
void TreeBuild(Tree **t, Stack *s);
void DelTree(Tree* tree);
Tree* CopyTree(Tree* in); 

void CountCopy(Tree** t, Tree** copy, ll count);
void TransformTree(Tree** in);

void PrintNum(ll value, ll indent);
void PrintOper(ll value, ll indent);
void IndCount(Tree* tree, ll indent);
void PrintTree(Tree* tree);
```

tree.c
```
#include "tree.h"

char IsOperation(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

Tree* TreeCreate() {
    
    Tree *t = (Tree*)malloc(sizeof(Tree));
    t->left = NULL;
    t->right = NULL;
    t->num = inf;
    t->oper = "";
    
    return t;
}

void TreeBuild(Tree **t, Stack *s) {
    
    if (Empty(s))
        return;

    Value val = TopVal(s);
    Pop(s);

    (*t) = TreeCreate();
    (*t)->num = val.num;
    (*t)->oper = val.operation;

    if (IsOperation((*t)->oper)) {
        TreeBuild(&(*t)->right, s);
        TreeBuild(&(*t)->left, s);
    }
    
}

void DelTree(Tree** t) {
    if (*t == NULL)
        return;

    DelTree(&(*t)->left);
    DelTree(&(*t)->right);

    free(*t);
    *t = NULL;
}

Tree* CopyTree(Tree** in) {
    
    Tree* copy = NULL;

    if (*in == NULL)
        return NULL;

    copy = TreeCreate();
    copy->oper = (*in)->oper;
    copy->num = (*in)->num;
    copy->left = CopyTree(&(*in)->left);
    copy->right = CopyTree(&(*in)->right);

    return copy;
}

void CountCopy(Tree **t, Tree **copy, ll count){

    if (count == 1) {
        (*t) = CopyTree(copy);
        return;
    }
    else {
        (*t) = TreeCreate();
        (*t)->oper = '+';
        (*t)->left = CopyTree(copy);
        CountCopy(&(*t)->right, copy, count - 1);
    }

}

void TransformTree(Tree** in) {

    if (*in == NULL || (*in)->num != inf || (*in)->left == NULL || (*in)->right == NULL)
        return;
    else {
        TransformTree(&(*in)->left);
        TransformTree(&(*in)->right);

        if ((*in)->oper == '*') {
            if ((*in)->left->num == 0 || (*in)->right->num == 0) {
                DelTree(in);
                (*in) = TreeCreate();
                (*in)->num = 0;
                return;
            }
            else if ((*in)->left->num != inf) {
                ll num = (*in)->left->num;
                Tree* t = CopyTree(&(*in)->right);
                DelTree(in);
                CountCopy(in, &t, num);

                DelTree(&t);
            }
            else if ((*in)->right->num != inf) {
                ll num = (*in)->right->num;
                Tree* t = CopyTree(&(*in)->left);
                DelTree(in);
                CountCopy(in, &t, num);

                DelTree(&t);
            }
        }
    }

}

void PrintNum(ll value, ll indent) {
    while (indent--) {
        printf(" ");
    }

    printf("%d\n", value);
}

void PrintOper(char value, ll indent) {
    while (indent--) {
        printf(" ");
    }

    printf("%c\n", value);
}

void IndCount(Tree* tree, ll indent) {
    if (tree == NULL) {
        return;
    }
    IndCount(tree->right, indent + 3);
    if (tree->num == inf)
        PrintOper(tree->oper, indent);
    else
        PrintNum(tree->num, indent);
    IndCount(tree->left, indent + 3);
}

void PrintTree(Tree* tree) {
    IndCount(tree, 0);
}
```

main.h
```
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "stack.h"
#include "tree.h"

#define ll long long

char IsNum(char c);
char IsOper(char c);
char IsVariation(char c);

ll StoI(ll* i, char* s);
void DoPostOder(Stack *s, char *oder);
void PrintTransTree(Tree** in);
```

main.c
```
#define _CRT_SECURE_NO_WARNINGS

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "main.h"

int main() {

    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);

    char oder[300];
    Stack postod; StackCreate(&postod);
    Tree* t = NULL;
    Tree* copyt = TreeCreate();

    printf("0 - Exit\n");
    printf("1 - Input oder\n");
    printf("2 - Print oder\n");
    printf("3 - Print postfix oder\n");
    printf("4 - Print result oder\n");
    printf("5 - Print postfix tree\n");
    printf("6 - Print result tree\n");

    int c;
    printf("do: ");
    scanf("%d", &c);
    while (c != 0) {
        switch (c)
        {
            case 1:
                printf("Input oder: ");
                scanf("%s", &oder);

                DelStack(&postod); 
                DelTree(&t); DelTree(&copyt);

                DoPostOder(&postod, oder);

                Stack s; StackCreate(&s);
                CopyStack(&postod, &s);
                
                TreeBuild(&t, &s);
                copyt = CopyTree(&t);
                TransformTree(&t);
    
                DelStack(&s);
                break;
            case 2:
                if (!Empty(&postod))
                    printf("%s\n", oder);
                break;
            case 3:
                Reverse(&postod);
                PrintStack(&postod);
                Reverse(&postod);
                printf("\n");
                break;
            case 4:
                PrintTransTree(&t);
                printf("\n");
                break;
            case 5:
                printf("\n");
                PrintTree(copyt);
                printf("\n");
                break;
            case 6:
                printf("\n");
                PrintTree(t);
                printf("\n");
                break;
            default:
                break;
        }
        printf("do: ");
        scanf("%d", &c);
    }

    DelTree(&t);    
    DelTree(&copyt);
    DelStack(&postod);

    if (_CrtDumpMemoryLeaks())
        printf("memory leak!\n");
    else
        printf("all is ok\n");
    
    return 0;

}

char IsNum(char c) {
    return (c >= '0' && c <= '9');
}
char IsVariation(char c) {
    return (c >= 'a' && c <= 'z');
}
char IsOper(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}
int RankOper(char c) {
    if (c == '(')
        return 0;
    if (c == '+' || c == '-')
        return 1;
    return 2;
}

ll StoI(ll *i, char *s) {

    ll l = *i, r;
    ll res = 0;
    while (IsNum(s[*i])) {
        res = res * 10 + (s[*i] - '0');
        *i = *i + 1;
    }
    *i = *i - 1;
    
    return res;
}

void DoPostOder(Stack* result, char* oder) {

    ll i = 0, num;
    Stack operations; StackCreate(&operations);

    while (oder[i] != '\0') {

        if (IsNum(oder[i])) {
            num = StoI(&i, oder);
            PushNum(result, num);
        }
        else if (oder[i] == '(') {
            PushOper(&operations, oder[i]);
        }
        else if (oder[i] == ')') {
            while (!Empty(&operations) && TopOper(&operations) != '(') {
                PushOper(result, TopOper(&operations));
                Pop(&operations);
            }
            Pop(&operations);
        }
        else if (IsOper(oder[i])) {

            while (!Empty(&operations) && RankOper(oder[i]) <= RankOper(TopOper(&operations))) {
                PushOper(result, TopOper(&operations));
                Pop(&operations);
            }

            PushOper(&operations, oder[i]);
        }
        else if (IsVariation(oder[i])) {
            PushOper(result, oder[i]);
        }
        i++;
    }

    while (!Empty(&operations)) {
        PushOper(result, TopOper(&operations));
        Pop(&operations);
    }
    DelStack(&operations);

}

void PrintTransTree(Tree** in) {

    if (*in == NULL)
        return;

    if ((*in)->left != NULL) {
        if ((*in)->left->left != NULL)
            printf("(");

        PrintTransTree(&(*in)->left);

        if ((*in)->left->left != NULL)
            printf(")");
    }

    if ((*in)->num == inf)
        printf("%c", (*in)->oper);
    else
        printf("%lld", (*in)->num);

    if ((*in)->right != NULL)
    {
        if ((*in)->right->left != NULL)
            printf("(");

        PrintTransTree(&(*in)->right);

        if ((*in)->right->left != NULL)
            printf(")");
    }

}
```

## 9. Дневник отладки

*Должен содержать дату и время сеансов отладки и основные события (ошибки в сценарии и программе, нестандартные ситуации) и краткие комментарии к ним. В дневнике отладки приводятся сведения об использовании других ЭВМ, существенном участии преподавателя и других лиц в написании и отладке программы.*

| № | Лаб. или дом. | Дата | Время | Событие | Действие по исправлению | Примечание |
| - | ------------- | ---- | ----- | ------- | ----------------------- | ---------- |
| 0 | дом | 11.04.2022 | 20:01 | утечка памяти | поискал | оказывается выделял участок памяти и сразу ще присваивал указателю ссылку на другую память |

## 10. Замечания автора по существу работы


## 11. Выводы

Я создал стек, дерево с нуля, поработал с выделением и очисткой памяти. Было весело, но немного больно, это явно поможет мне в дальнейшем.
