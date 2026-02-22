#include "lib.h"

#include <errno.h>

void my_getline(char **str) {
  size_t len = 0;
  getline(str, &len, stdin);
  int size = strlen(*str);
  if ((*str)[size - 1] == '\n') {
    (*str)[size - 1] = '\0';
  }
}

int insert_abonent(Abonent *array) {
  if (array == NULL) {
    fprintf(stderr, "Передача в Функцию \"%s\" нулевого указателя\n", __func__);
    return -1;
  }

  if (array->size > 100) {
    printf("Ошибка: Справочник переполнен, (Макс 100 абонентов)\n");
    return 1;
  }

  printf("Введите имя нового абонента: ");
  char *name = NULL;
  my_getline(&name);

  printf("Введите фамилию нового абонента: ");
  char *second_name = NULL;
  my_getline(&second_name);

  printf("Введите номер телефона нового абонента: ");
  char *tel = NULL;
  my_getline(&tel);

  if (array->node == NULL) {
    list_node **current_node = &array->node;
    insert(current_node, name, second_name, tel);
  } else {
    list_node *temp = array->node;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    insert(&temp, name, second_name, tel);
  }

  ++array->size;

  return 0;
}

void insert(list_node **current_node, char *name, char *second_name,
            char *tel) {
  if (*current_node == NULL) {
    (*current_node) = malloc(sizeof(list_node));
    (*current_node)->name = name;
    (*current_node)->second_name = second_name;
    (*current_node)->tel = tel;

    (*current_node)->next = NULL;
    (*current_node)->pred = NULL;
  } else {
    list_node *new_node = malloc(sizeof(list_node));
    new_node->name = name;
    new_node->second_name = second_name;
    new_node->tel = tel;

    new_node->next = NULL;
    new_node->pred = (*current_node);

    (*current_node)->next = new_node;
  }
}

int print_all_abonents(Abonent *array) {
  if (array == NULL) {
    fprintf(stderr, "Передача в Функцию \"%s\" нулевого указателя\n", __func__);
    return -1;
  } else if (array->size == 0) {
    fprintf(stderr, "Справочник пустой\n");
    return -2;
  }

  list_node *current_node = array->node;
  int i = 1;
  while (current_node != NULL) {
    printf("Абонент %d:\n", i);
    printf("Имя абонента: %s\n", current_node->name);
    printf("Фамилия абонента: %s\n", current_node->second_name);
    printf("Номер телефона абонента: %s\n", current_node->tel);

    current_node = current_node->next;
    ++i;
  }

  return 0;
}

int search_abonent(Abonent *array) {
  if (array == NULL) {
    fprintf(stderr, "Передача в Функцию \"%s\" нулевого указателя\n", __func__);
    return -1;
  } else if (array->size == 0) {
    fprintf(stderr, "Справочник пустой\n");
    return -2;
  }

  printf("Введите имя абонента которого надо найти: ");

  char *name = NULL;
  my_getline(&name);

  list_node *current_node = array->node;
  int index = 1;
  while (current_node != NULL) {
    if (strcmp(current_node->name, name) == 0) {
      break;
    }
    current_node = current_node->next;
    ++index;
  }

  if (current_node == NULL) {
    printf("Абонента с именем %s нет в справочнике\n", name);
  } else {
    printf("Абонент %d:\n", index);
    printf("Имя абонента: %s\n", current_node->name);
    printf("Фамилия абонента: %s\n", current_node->second_name);
    printf("Номер телефона абонента: %s\n", current_node->tel);
  }

  return 0;
}

int search(list_node *current_node, const char *name) {
  int index = 1;
  while (current_node != NULL) {
    if (strcmp(current_node->name, name) == 0) {
      break;
    }
    current_node = current_node->next;
    ++index;
  }

  return index;
}

int delete_abonent(Abonent *array) {
  if (array == NULL) {
    fprintf(stderr, "Передача в Функцию \"%s\" нулевого указателя\n", __func__);
    return -1;
  } else if (array->size == 0) {
    fprintf(stderr, "Справочник пустой\n");
    return -2;
  }

  printf("Введите имя абонента которого надо удалить: ");

  char *name = NULL;
  my_getline(&name);

  list_node *current_node = array->node;

  if (search(current_node, name) > array->size) {
    fprintf(stderr, "Абонента с именем %s нет в справочнике\n", name);
    return -3;
  }

  while (current_node != NULL) {
    if (strcmp(current_node->name, name) == 0) {
      current_node = delete_node(current_node, &array->node);
      --array->size;
    } else {
      current_node = current_node->next;
    }
  }

  return 0;
}

list_node *delete_node(list_node *current_node, list_node **array_node) {
  if (current_node->next == NULL && current_node->pred == NULL) {
    free(current_node);
    current_node = NULL;
    *array_node = NULL;
  } else if (current_node->next == NULL && current_node->pred != NULL) {
    list_node *pred = current_node->pred;
    free(current_node);
    current_node = NULL;
    pred->next = NULL;
  } else if (current_node->next != NULL && current_node->pred == NULL) {
    current_node = current_node->next;
    free(current_node->pred);
    current_node->pred = NULL;
    *array_node = current_node;
  } else {
    list_node *pred = current_node->pred;
    current_node = current_node->next;

    free(current_node->pred);

    pred->next = current_node;
    current_node->pred = pred;
  }

  return current_node;
}
