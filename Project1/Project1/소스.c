#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 1000
#define MAXstring 200000
#define BUFFER_LENGTH 100

typedef struct Word {
	char *form;
	char *mean;
}Word;
typedef struct Word_dict {
	char word[20];
	int word_count;
	Word samewords[100];
}W_D;
W_D dict[MAXstring];
char ignore[MAX] = ") ";
int dictnum = -1;
int isokay(char *s);
void process_command();
int read_line(FILE *fp, char str[], int n);
void size();
void find(char* tmp);
void handle_load(char *tmp);
void load(FILE *in_fp);
int binarySearch(char* target, int begin, int end);

int count = 0;
int main(void) {

	process_command();

	return 0;

}

int isokay(char *s) {
	if (strcmp(dict[dictnum].word, s) == 0)
		return dictnum;
	return -1;
}
void process_command() {

	char command_line[BUFFER_LENGTH];
	char *command;
	while (1) {
		printf("$ ");

		if (read_line(stdin, command_line, BUFFER_LENGTH) <= 0)
			continue;

		command = strtok(command_line, " ");
		if (strcmp(command, "read") == 0) {
			char *tmp = strtok(NULL, " ");
			if (strlen(tmp) == 0)
				continue;
			handle_load(tmp);
		}
		else if (strcmp(command, "size") == 0)
			size();
		else if (strcmp(command, "find") == 0) {
			char *tmp = strtok(NULL, "\n");
			if (strlen(tmp) == 0)
				continue;
			find(tmp);
		}
		else if (strcmp(command, "exit") == 0)
			break;

	}

}
int read_line(FILE *fp, char str[], int n) {
	int ch, i = 0;
	while ((ch = fgetc(fp)) != '\n'&&ch != EOF)
		if (i < n - 1)
			str[i++] = ch;
	str[i] = '\0';
	return i;
}
void size() {
	printf("%d\n", count);
}
void find(char* tmp) {

	int j = 0;
	printf("%s\n", tmp);
	int result = binarySearch(tmp, 0, dictnum);

}
int binarySearch(char* target, int begin, int end) {
	if (begin > end) {
		printf("Not found.\n");
		printf("%s\n", dict[end].word);
		printf("---\n");
		printf("%s\n", dict[begin].word);
		return -1;
	}

	else {
		int middle = (begin + end) / 2;
		if (strcmpi(dict[middle].word, target) == 0) {
			printf("Found %d items.\n", dict[middle].word_count);
			for (int j = 0; j < dict[middle].word_count; j++) {
				printf("%s (%s %s\n", dict[middle].word, dict[middle].samewords[j].form, dict[middle].samewords[j].mean);
			}
			return middle;
		}
		else if (strcmpi(dict[middle].word, target) > 0)
			return binarySearch(target, begin, middle - 1);
		else
			return binarySearch(target, middle + 1, end);
	}
}
void handle_load(char *tmp) {

	FILE *in_fp = fopen(tmp, "r");
	if (in_fp == NULL) {
		printf("No such file exists.\n");
		return;
	}
	load(in_fp);
	fclose(in_fp);

}
void load(FILE *in_fp) {
	char buffer[MAX];
	char *token[3];
	char *tmp;
	char *tmp2;
	while (!feof(in_fp)) {
		fgets(buffer, MAX, in_fp);
		if (strcmp(buffer, "\n") != 0) {
			token[0] = strtok(buffer, "(");
			token[0][strlen(token[0]) - 1] = '\0';
			tmp = strtok(NULL, "\n");
			if (tmp[0] == ')') {
				token[1] = strdup("");
				tmp2 = strtok(tmp, " ");
				token[2] = strtok(NULL, "\n");
			}
			else
			{
				token[1] = strtok(tmp, ")");
				token[2] = strtok(NULL, "\n");
			}

			int result = isokay(token[0]);

			if (dictnum == -1 || result == -1) {
				dictnum++; strcpy(dict[dictnum].word, token[0]);
				dict[dictnum].word_count = 1;
			}
			else
				dict[result].word_count++;
			dict[dictnum].samewords[dict[dictnum].word_count - 1].form = strdup(token[1]);
			dict[dictnum].samewords[dict[dictnum].word_count - 1].mean = strdup(token[2]);
			strcat(dict[dictnum].samewords[dict[dictnum].word_count - 1].form, ")");
			count++;
		}

	}
}