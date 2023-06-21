#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SURENAME_SIZE 20
#define NAME_SIZE 20
#define NUMBER_SIZE 20
#define CONTACT_SIZE 100

typedef struct {
  char name[NAME_SIZE];
  char surename[SURENAME_SIZE];
  char number[NUMBER_SIZE];
} Sub;

int stok(char *str, char symbol, char **words) {
  int count = 1;
  words[0] = str;
  while (*str != '\0') {
    if (*str == symbol) {
      *str = '\0';
      str++;
      words[count] = str;
      count++;
    } else {
      str++;
    }
  }
  return count;
}

void sort(Sub *contact, int size) {
  for (int i = size - 2; i >= 0; i--) {
    if (strcmp(contact[i + 1].name, contact[i].name) < 0) {
      Sub tmp;
      strcpy(tmp.name, contact[i + 1].name);
      strcpy(tmp.surename, contact[i + 1].surename);
      strcpy(tmp.number, contact[i + 1].number);

      strcpy(contact[i + 1].name, contact[i].name);
      strcpy(contact[i + 1].surename, contact[i].surename);
      strcpy(contact[i + 1].number, contact[i].number);

      strcpy(contact[i].name, tmp.name);
      strcpy(contact[i].surename, tmp.surename);
      strcpy(contact[i].number, tmp.number);

      for (int j = i + 1; j < size; j++) {
        if (strcmp(contact[j].name, contact[j - 1].name) < 0) {
          Sub tmp2;
          strcpy(tmp2.name, contact[j - 1].name);
          strcpy(tmp2.surename, contact[j - 1].surename);
          strcpy(tmp2.number, contact[j - 1].number);

          strcpy(contact[j - 1].name, contact[j].name);
          strcpy(contact[j - 1].surename, contact[j].surename);
          strcpy(contact[j - 1].number, contact[j].number);

          strcpy(contact[j].name, tmp2.name);
          strcpy(contact[j].surename, tmp2.surename);
          strcpy(contact[j].number, tmp2.number);
        }
      }
    }
  }
}

void add_sub(Sub *contact, int *size) {
  printf("enter name: ");
  scanf("%s", contact[*size].name);
  printf("enter surename: ");
  scanf("%s", contact[*size].surename);
  printf("enter number: ");
  scanf("%s", contact[*size].number);
  strcat(contact[*size].number, "\n");
  printf("\n");
  (*size)++;
  sort(contact, *size);
}

void del_sub(Sub *contact, int *size) {
  int flag = 0;
  char name[NAME_SIZE];
  char surename[SURENAME_SIZE];
  printf("Enter Name and Surename of sub: ");
  scanf("%s %s", name, surename);

  for (int i = 0; i < *size; i++) {
    if ((strcmp(contact[i].name, name) == 0) &&
        (strcmp(contact[i].surename, surename) == 0)) {
      flag = 1;
      for (int j = i; j < (*size) - 1; j++) {
        strcpy(contact[j].name, contact[j + 1].name);
        strcpy(contact[j].surename, contact[j + 1].surename);
        strcpy(contact[j].number, contact[j + 1].number);
      }
      (*size)--;
    }
  }

  if (flag == 0) {
    printf("\n");
    printf("Contact Not found\n");
    printf("\n");
  }
}

void lookup(Sub *contact, int size) {
  int flag = 0;
  char name[NAME_SIZE];
  char surename[SURENAME_SIZE];
  printf("Enter Name and Surename of sub: ");
  scanf("%s %s", name, surename);

  for (int i = 0; i < size; i++) {
    if ((strcmp(contact[i].name, name) == 0) &&
        (strcmp(contact[i].surename, surename) == 0)) {
      flag = 1;
      printf("\n");
      printf("Contact [%s %s] --> %s\n", contact[i].name, contact[i].surename,
             contact[i].number);
    }
  }
  if (flag == 0) {
    printf("\n");
    printf("Contact Not found\n");
    printf("\n");
  }
}

void print_subs(Sub *contact, int *size) {
  printf("--------------------------------------------------------\n");
  for (int i = 0; i < *size; i++) {
    printf("Contact№%d - [%s %s] --> %s", i + 1, contact[i].name,
           contact[i].surename, contact[i].number);
    printf("--------------------------------------------------------\n");
  }
  printf("\n");
}

int main() {
  Sub contacts[CONTACT_SIZE];
  int size_of_subs = 0;
  char num[NAME_SIZE];

  FILE *file = fopen("book.txt", "r+");
  char mass[CONTACT_SIZE];

  while (fgets(mass, CONTACT_SIZE, file) != NULL) {
    if (strcmp(mass, "\n") != 0) {
      char *words[CONTACT_SIZE / 3];
      if (stok(mass, ' ', words) == 3) {
        strcpy(contacts[size_of_subs].name, words[0]);
        strcpy(contacts[size_of_subs].surename, words[1]);
        strcpy(contacts[size_of_subs].number, words[2]);
        size_of_subs++;
      }
    }
  }
  // strcat(contacts[size_of_subs - 1].number, "\n");
  remove("book.txt");
  fclose(file);

  while (1) {
    printf("commands: 1)add contact 2)delete contact 3)print all contacts "
           "4)find contact 5)exit\n");
    printf("write command:");
    scanf("%s", num);

    system("clear");
    if (strcmp(num, "1") == 0) {
      add_sub(contacts, &size_of_subs);
    } else if (strcmp(num, "2") == 0) {
      del_sub(contacts, &size_of_subs);
    } else if (strcmp(num, "3") == 0) {
      print_subs(contacts, &size_of_subs);
    } else if (strcmp(num, "4") == 0) {
      lookup(contacts, size_of_subs);
    } else if (strcmp(num, "5") == 0) {
      break;
    } else {
      printf("unknown Command\n");
    }
  }

  FILE *lf = fopen("book.txt", "w");

  for (int i = 0; i < size_of_subs; i++) {
    char arr[CONTACT_SIZE];
    strcpy(arr, contacts[i].name);
    strcat(arr, " ");
    strcat(arr, contacts[i].surename);
    strcat(arr, " ");
    strcat(arr, contacts[i].number);
    fprintf(lf, "%s", arr);
  }
  fclose(lf);
}
