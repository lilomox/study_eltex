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

typedef struct {
  int size;
  Sub *arr;
} Contact_book;

Contact_book *new_contact_book() {
  Contact_book *book = (Contact_book *)malloc(sizeof(Contact_book));
  if (book == NULL) {
    return NULL;
  }

  book->size = 0;
  return book;
}

void free_book(Contact_book *book) {
  free(book->arr);
  book->arr = NULL;
  free(book);
  book = NULL;
}

void copy_sub(Sub *dest, Sub *src) {
  strcpy(dest->name, src->name);
  strcpy(dest->surename, src->surename);
  strcpy(dest->number, src->number);
}

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

void sort(Contact_book *book) {
  for (int i = book->size - 2; i >= 0; i--) {
    if (strcmp(book->arr[i + 1].name, book->arr[i].name) < 0) {
      Sub tmp;

      copy_sub(&tmp, &book->arr[i + 1]);
      copy_sub(&book->arr[i + 1], &book->arr[i]);
      copy_sub(&book->arr[i], &tmp);

      for (int j = i + 1; j < book->size; j++) {
        if (strcmp(book->arr[j].name, book->arr[j - 1].name) < 0) {
          Sub tmp2;

          copy_sub(&tmp2, &book->arr[j - 1]);
          copy_sub(&book->arr[j - 1], &book->arr[j]);
          copy_sub(&book->arr[j], &tmp2);
        }
      }
    }
  }
}

int add_sub(Contact_book *book, Sub *sb) {
  book->arr = realloc(book->arr, sizeof(Sub) * (book->size + 1));
  if (book->arr == NULL) {
    return -1;
  }

  copy_sub(&book->arr[book->size], sb);

  book->size++;
  sort(book);
  return 0;
}

void del_sub(Contact_book *book, Sub *sb) {
  int flag = 0;

  for (int i = 0; i < book->size; i++) {
    if ((strcmp(book->arr[i].name, sb->name) == 0) &&
        (strcmp(book->arr[i].surename, sb->surename) == 0)) {

      flag = 1;

      for (int j = i; j < book->size - 1; j++) {
        copy_sub(&book->arr[j], &book->arr[j + 1]);
      }

      book->size--;
      book->arr = realloc(book->arr, sizeof(Sub) * (book->size + 1));
      break;
    }
  }

  if (flag == 0) {
    printf("\n");
    printf("Contact Not found\n");
    printf("\n");
  }
}

void lookup(Contact_book *book, Sub *sb) {
  int flag = 0;

  for (int i = 0; i < book->size; i++) {
    if ((strcmp(book->arr[i].name, sb->name) == 0) &&
        (strcmp(book->arr[i].surename, sb->surename) == 0)) {
      flag = 1;
      printf("\n");
      printf("Contact [%s %s] --> %s\n", book->arr[i].name,
             book->arr[i].surename, book->arr[i].number);
    }
  }
  if (flag == 0) {
    printf("\n");
    printf("Contact Not found\n");
    printf("\n");
  }
}

void print_subs(Contact_book *book) {
  printf("--------------------------------------------------------\n");
  for (int i = 0; i < book->size; i++) {
    printf("Contact№%d - [%s %s] --> %s", i + 1, book->arr[i].name,
           book->arr[i].surename, book->arr[i].number);
    printf("--------------------------------------------------------\n");
  }
  printf("\n");
}

int main() {
  Contact_book *book = new_contact_book();

  FILE *file = fopen("book.txt", "r+");
  char mass[CONTACT_SIZE];

  while (fgets(mass, CONTACT_SIZE, file) != NULL) {
    if (strcmp(mass, "\n") != 0) {
      char *words[3];
      if (stok(mass, ' ', words) == 3) {
        Sub tmp;

        strcpy(tmp.name, words[0]);
        strcpy(tmp.surename, words[1]);
        strcpy(tmp.number, words[2]);

        add_sub(book, &tmp);
      }
    }
  }

  remove("book.txt");
  fclose(file);

  char num[NAME_SIZE];
  while (1) {
    printf("commands: 1)add contact 2)delete contact 3)print all contacts "
           "4)find contact 5)exit\n");
    printf("write command:");
    scanf("%s", num);

    system("clear");
    if (strcmp(num, "1") == 0) {
      Sub sb;

      printf("enter name: ");
      scanf("%s", sb.name);

      printf("enter surename: ");
      scanf("%s", sb.surename);

      printf("enter number: ");
      scanf("%s", sb.number);
      strcat(sb.number, "\n");

      printf("\n");
      add_sub(book, &sb);

    } else if (strcmp(num, "2") == 0) {
      Sub sb;
      printf("Enter Name and Surename of sub: ");
      scanf("%s %s", sb.name, sb.surename);
      del_sub(book, &sb);

    } else if (strcmp(num, "3") == 0) {
      print_subs(book);

    } else if (strcmp(num, "4") == 0) {
      Sub sb;
      printf("Enter Name and Surename of sub: ");
      scanf("%s %s", sb.name, sb.surename);
      lookup(book, &sb);

    } else if (strcmp(num, "5") == 0) {
      break;

    } else {
      printf("unknown Command\n");
    }
  }

  FILE *lf = fopen("book.txt", "w");

  for (int i = 0; i < book->size; i++) {
    char arr[CONTACT_SIZE];

    strcpy(arr, book->arr[i].name);
    strcat(arr, " ");

    strcat(arr, book->arr[i].surename);
    strcat(arr, " ");

    strcat(arr, book->arr[i].number);
    fprintf(lf, "%s", arr);
  }

  fclose(lf);
  free_book(book);
}
