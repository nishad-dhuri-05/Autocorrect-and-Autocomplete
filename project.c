#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX_SUGG 8
int track_sugg = 0;
int count_sugg = 0;
char suggested_word[MAX_SUGG][31];
int resp_probabilities[MAX_SUGG];
int char2index(char c)
{
    return tolower(c) - 'a';
}
struct node
{
    struct node *children[26];
    bool is_EOW;
    int freq;
    int used;
};
typedef struct node Node;
Node *ROOT;
Node *new_node()
{
    Node *temp = NULL;
    temp = (Node *)malloc(sizeof(Node));
    if (temp)
    {
        for (int i = 0; i < 26; i++)
        {
            temp->children[i] = NULL;
        }
        temp->is_EOW = false;
        temp->freq = 0;
    }
    if (temp == NULL)
    {
        printf("Failed to allocate memory");
    }
    return temp;
}
Node *insert(Node *root, char *key, int len_str)
{
    int height;
    int index = 0;
    Node *traverse = root;
    for (height = 0; height < len_str; height++)
    {
        if (key[height] == '\0')
        {
            break;
        }
        else
        {
            index = char2index(key[height]);
            if (traverse != NULL)
            {
                if (traverse->children[index] == NULL)
                    traverse->children[index] = new_node();
                traverse = traverse->children[index];
            }
        }
    }
    if (traverse != NULL)
    {
        traverse->is_EOW = true;
        traverse->freq++;
        traverse->used=0;
    }
    return root;
}
bool search(Node *root, char *key, int str_len)
{
    int height;
    int index;
    Node *traverse = root;
    for (height = 0; height < str_len; height++)
    {
        index = char2index(key[height]);
        if (!traverse->children[index])
            return false;
        traverse = traverse->children[index];
    }
    printf("%d\n",traverse->freq);
    return traverse->is_EOW;
}
int get_count(Node *root, char *key, int str_len)
{
    int height;
    int index;
    Node *traverse = root;
    for (height = 0; height < str_len; height++)
    {
        if (!isalpha(key[height]))
        {
            break;
        }
        index = char2index(key[height]);
        if (!traverse->children[index])
            return 0;
        traverse = traverse->children[index];
    }
    if(traverse->is_EOW){
    return traverse->freq;
    }
    else
    return 0;
}
int get_used(Node *root,char*key,int str_len){
    int height;
    int index;
    Node *traverse = root;
    for (height = 0; height < str_len; height++)
    {
        if (!isalpha(key[height]))
        {
            break;
        }
        index = char2index(key[height]);
        if (!traverse->children[index])
            return 0;
        traverse = traverse->children[index];
    }
    if(traverse->is_EOW){
    
    return traverse->used;
    }
    else
    return 0;
}
void array_sort()
{
    for (int i = 0; i < MAX_SUGG; i++)
    {
        for (int j = i + 1; j < MAX_SUGG; j++)
        {
            if (resp_probabilities[i] > resp_probabilities[j])
            {
                int temp_i = resp_probabilities[j];
                resp_probabilities[j] = resp_probabilities[i];
                resp_probabilities[i] = temp_i;
                for (int k = 0; k < 31; k++)
                {
                    char temp_c = suggested_word[j][k];
                    suggested_word[j][k] = suggested_word[i][k];
                    suggested_word[i][k] = temp_c;
                }
            }
        }
    }
}
void update_used_value(Node* root,char *key,int str_len){
    int height;
    int index;
    Node *traverse = root;
    for (height = 0; height < str_len; height++)
    {
        index = char2index(key[height]);
        if (!traverse->children[index])
            return ;
        traverse = traverse->children[index];
    }
    if(traverse->is_EOW){
    traverse->used=1;
    }
}
void select_suggestions(Node *root, char *s, int len_str,int track)
{
    if (track_sugg < MAX_SUGG)
    {
        int p = get_count(root, s, len_str);
        if(track==0&&p!=0){
            p=(p/100)+1;
        }
        if (p != 0&& (!get_used(root,s,len_str)))
        {
            for (int i = 0; i < len_str; i++)
            {
                suggested_word[track_sugg][i] = s[i];
            }
            resp_probabilities[track_sugg] = p;
            track_sugg++;
        }
        update_used_value(root,s,len_str);
        if (track_sugg == MAX_SUGG)
        {
            array_sort();
        }
        return;
    }
    else
    {
        int p = get_count(root, s, len_str);
        if(track==0&&p!=0){
            p=(p/100)+1;
        }
        if(!get_used(root,s,len_str)){
        
            if (p > resp_probabilities[0])
            {
                resp_probabilities[0] = p;
                for (int j = 0; j < len_str; j++)
                {
                    suggested_word[0][j] = s[j];
                }
                while (isalpha(suggested_word[0][len_str])&&len_str<31)
                {
                    suggested_word[0][len_str] = '\0';
                    len_str++;
                }
                array_sort();
            }
            update_used_value(root,s,len_str);
        
        }
    }
}
void edits1(Node *root, char s[31], int str_len,int track)
{
    for (int i = 0; i < str_len+1; i++)
    {
        char s1[18];
        char s2[18];
        for(int temp=0;temp<i;temp++){
            s1[temp]=s[temp];
        }
        for(int temp=0;temp<str_len-i;temp++){
            s2[temp]=s[temp+i];
        }
        int size_s1 = i;
        int size_s2 = str_len - i;
        char delete[31], transpose[31], replace[26][31], insert_char[26][31];
        int len_d = str_len - 1;
        int len_t = str_len;
        int len_r = str_len;
        int len_i = str_len + 1;
        // for delete
        for (int j = 0; j < size_s1; j++)
        {
            delete[j] = s1[j];
        }
        for (int j = 1; j < size_s2; j++)
        {
            delete[i+ j - 1] = s2[j];
        }
        select_suggestions(root, delete, len_d,track);
        // for transpose
        if (size_s2 >= 2)
        {
            for (int i = 0; i < size_s1; i++)
            {
                transpose[i] = s1[i];
            }
            transpose[size_s1] = s2[1];
            transpose[size_s1 + 1] = s2[0];
            for (int i = 2; i < size_s2; i++)
            {
                transpose[size_s1 + i] = s2[i];
            }
        select_suggestions(root, transpose, len_t,track);
        }
        // for replace
        for (int r = 0; r < 26; r++)
        {
            for (int j = 0; j < size_s1; j++)
            {
                replace[r][j] = s1[j];
            }
            replace[r][size_s1] = (char)('a' + r);
            for (int j = 1; j < size_s2; j++)
            {
                replace[r][size_s1 + j] = s2[j];
            }
            select_suggestions(root, replace[r], len_r,track);
        }
        for (int r = 0; r < 26; r++)
        {
            for (int j = 0; j < size_s1; j++)
            {
                insert_char[r][j] = s1[j];
            }
            insert_char[r][size_s1] = (char)('a' + r);
            for (int j = 0; j < size_s2; j++)
            {
                insert_char[r][size_s1 + 1 + j] = s2[j];
            }
            select_suggestions(root, insert_char[r], len_i,track);
        }
        if(track==1){
            edits1(root,delete,len_d,0);
            edits1(root,transpose,len_t,0);
            for(int r=0;r<26;r++){
                edits1(root,replace[r],len_r,0);
                edits1(root,insert_char[r],len_i,0);
            }
        }
    }
}
int printAutoSuggestions(Node *root, char *s, int len_str)
{
    if (count_sugg <= MAX_SUGG)
    {
        Node *traverse = root;
        int height = 0;
        if (traverse->is_EOW)
        {
            int i=0;
            while(isalpha(s[i])){
                printf("%c",s[i]);
                i++;
            }
            printf(" ");
            count_sugg++;
        }
        for (int i = 0; i < 26; i++)
        {
            if (traverse->children[i] != NULL)
            {
                s[len_str] = 'a' + i;
                printAutoSuggestions(traverse->children[i], s, len_str + 1);
                s[len_str] = '\0';
            }
        }
    }
}
bool checker(Node *root, char *key, int str_len)
{
    Node *temp = root;
    for (int height = 0; height < str_len; height++)
    {
        int index = char2index(key[height]);
        if (temp->children[index] == NULL)
        {
            printAutoSuggestions(temp, key, height);
            return false;
        }
        temp = temp->children[index];
    }
    if (temp->is_EOW)
    {
        return true;
    }
    printAutoSuggestions(temp, key, str_len);
    return false;
}
int main()
{
    FILE *fp_read = fopen("big.txt", "r");
    char c = fgetc(fp_read);
    Node *root = new_node();
    ROOT = root;
    int count = 0;
    int max_len = 0;
    while (c != EOF)
    {
        char *s;
        char *w = s;
        int len_str = 0;
        while (isalpha(c))
        {
            *s = c;
            s++;
            len_str++;
            c = fgetc(fp_read);
        }
        if (c != EOF)
        {
            if(c=='\''){
            c = fgetc(fp_read);
            c=fgetc(fp_read);
            }
            else
                c=fgetc(fp_read);
        }
        s = w;
        count++;
        if (len_str > max_len)
        {
            max_len = len_str;
        }
        root = insert(root, w, len_str);
    }
    printf("Stored successfully!!!!\n");
    char inp[31];
    printf("Enter the word that you want to check: ");
    scanf("%s", inp);
    char inp2[31];
    for(int i=0;i<31;i++){
        inp2[i]=inp[i];
    }
    int str_len = 0;
    while (isalpha(inp[str_len]))
    {
        str_len++;
    }
    printf("For Auto-Complete- ");
    if (checker(root, inp, str_len))
    {
        printf("The word is present");
    }
    printf("\n");
    if (!search(root, inp, str_len))
    {
        printf("For Auto-Correct- ");
        edits1(root, inp2, str_len,1);
        for (int i = MAX_SUGG - 1; i >= 0; i--)
        {
            if(isalpha(suggested_word[i][0]))
            printf("%s ", suggested_word[i]);
        }
        printf("\n");
    }
    else{
        printf("The word is present in the dictionary\n");
    }
}