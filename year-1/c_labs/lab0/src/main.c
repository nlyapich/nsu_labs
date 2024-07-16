#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

int check_correctness1(int b1, int b2);
int check_correctness2(int b1, char number_b1[]);
int len_before_dot(char number_b1[]);
int len_after_dot(char number_b1[], int len_before_dot);

void translate_in_10(char number_b1[], int b1, long long int* whole_number_in_10, double* fractional_number_in_10);
void translate_whole_number_in_b2(long long int whole_number_in_10, int b2, char* number_b2_bd);
void translate_fractional_number_in_b2(double fractional_number_in_10, int b2, char* number_b2_ad);
void reverse(char* number_b2, int len);

int main(void){
    int b1 = 0, b2 = 0;
    if (scanf("%d %d\n", &b1, &b2) != 2){
        printf("bad input");
        return 0;
    }
    
    if (!check_correctness1(b1, b2)){
        return 0;
    }

    char number_b1[15] = {'\0'};

    if (fgets(number_b1, sizeof(number_b1), stdin) == NULL){
        printf("bad input");
        return 0;
    }
    number_b1[strlen(number_b1) - 1] = '\0';

    if (!check_correctness2(b1, number_b1)){
        return 0;
    }

    if (b1 == b2){
        printf("%s", number_b1);
        return 0;
    }

    long long int whole_number_in_10 = 0; double fractional_number_in_10 = 0;
    translate_in_10(number_b1, b1, &whole_number_in_10, &fractional_number_in_10);

    char number_b2_bd[60] = {'\0'}; number_b2_bd[0] = '0';
    translate_whole_number_in_b2(whole_number_in_10, b2, &number_b2_bd[0]);

    char number_b2_ad[13] = {'\0'};
    translate_fractional_number_in_b2(fractional_number_in_10, b2, &number_b2_ad[0]);
    
    printf("%s.%s", number_b2_bd, number_b2_ad);

    return 0;
}

void translate_whole_number_in_b2(long long int whole_number_in_10, int b2, char* number_b2_bd){
    int len = 0;
    while (whole_number_in_10 != 0){
        char p[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', '\0'};
        *number_b2_bd = p[whole_number_in_10 % b2];
        number_b2_bd++;
        whole_number_in_10 /= b2;
        len++;
    }
    number_b2_bd -= len;
    reverse(number_b2_bd, len);
    
}

void reverse(char* number_b2, int len){
    char number_b2_reverse[60] = {'\0'};
    for (int j = 0; j < len; j++){
        number_b2_reverse[j] = *number_b2;
        number_b2++;
    }
    number_b2 -= len;
    for (int j = 0; j < len; j++){
        *number_b2 = number_b2_reverse[len - 1 - j];
        number_b2++;
    }
}

void translate_fractional_number_in_b2(double fractional_number_in_10, int b2, char* number_b2_ad){
    for (int j = 0; j < 12; j++){
        char p[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', '\0'};
        *number_b2_ad = p[(int)(fractional_number_in_10 * b2)];
        number_b2_ad++;
        fractional_number_in_10 = (fractional_number_in_10 * b2) - (double)(int)(fractional_number_in_10 * b2);
    }
}

int check_correctness1(int b1, int b2){
    if ((b1 > 16)||(b1 < 2)||(b2 > 16)||(b2 < 2)){
        printf("bad input");
        return 0;
    }
    return 1;
}
int check_correctness2(int b1, char number_b1[]){
    int len = 14;
    for (int i = 0; i < len; i++){
        int h = 0;
        char now = tolower(number_b1[i]); char next = tolower(number_b1[i + 1]);
        if ((now == '.') || (now == '\0')){
            h = 1;
        }
        if (b1 > 10){
            if ((now - 'a' >= 0) && (now - 'a' <= b1 - 1 - 10)){
                h = 1;
            }
            if ((now - '0' >= 0) && (now - '0' <= 9)){
                h = 1;
            }
        } else {
            if ((now - '0' >= 0) && (now - '0' <= b1 - 1)){
                h = 1;
            }
        }

        if ((now == '.') && (next == '.')){
            h = 0;
        }

        if (h == 0){
                printf("bad input");
                return 0;
        }
    }
    if ((number_b1[0] == '.') || (number_b1[strlen(number_b1) - 1] == '.')){
        printf("bad input");
        return 0;
    }
    return 1;
}
int len_before_dot(char number_b1[]){
    int len_before_dot = 0;
    for(int i = 0; i < 14; i++){
        if ((number_b1[i] == '\0') || (number_b1[i] == '.')){
            len_before_dot = i;
            break;
        }
    }
    return len_before_dot;
}
int len_after_dot(char number_b1[], int len_before_dot){
    int len_after_dot = 0;
    if (number_b1[len_before_dot] == '.'){
        for(int i = len_before_dot + 1; i < 14; i++){
            if (number_b1[i] == '\0'){
                len_after_dot = i;
                break;
            }
        }
    }
    return len_after_dot;
}
void translate_in_10(char number_b1[], int b1, long long int* whole_number_in_10, double* fractional_number_in_10){
    int lbd = len_before_dot(number_b1);
    char v;
    for (int i = 0; i < lbd; i++){
        if (tolower(number_b1[i]) - 'a' < 0){
            v = '0';
        } else {
            v = 'a' - 10;
        }
        *whole_number_in_10 += (tolower(number_b1[i]) - v) * pow(b1, (lbd - (i + 1)));
    }

    int lad = len_after_dot(number_b1, lbd);
    if (lad != 0){
        for(int i = lbd + 1; i < lad; i++){
            if (tolower(number_b1[i]) - 'a' < 0){
                v = '0';
            } else {
                v = 'a' - 10;
            }
            *fractional_number_in_10 += (double)(tolower(number_b1[i]) - v) / powl(b1, (i - lbd));
        }
    }
}
