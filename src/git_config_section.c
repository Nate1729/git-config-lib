#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "git_config_section.h"

Section *section_create_from_name(char *name) {
  if (!name)
    return NULL; /* Needs to be a valid pointer */

  if (strlen(name) == 0) {
    return NULL; /* Name cannot be empty */
  }

  Section *section = malloc(sizeof(Section));
  if (!section)
    return NULL;

  char *new_name_ptr = malloc(sizeof(char) * strlen(name));
  if (!new_name_ptr) {
    free(section);
    return NULL;
  }
  memcpy(new_name_ptr, name, strlen(name));

  char **variables, **values;
  variables = malloc(sizeof(char *) * DEFAULT_SECTION_CAPACITY);
  if (!variables) {
    free(new_name_ptr);
    free(section);
    return NULL;
  }

  values = malloc(sizeof(char *) * DEFAULT_SECTION_CAPACITY);
  if (!values) {
    free(new_name_ptr);
    free(section);
    free(variables);
    return NULL;
  }

  section->name = new_name_ptr;
  section->variables = variables;
  section->values = values;
  section->capacity = DEFAULT_SECTION_CAPACITY;
  section->length = 0;
  return section;
}

int section_append_variable_value_pair(Section *section, char *variable,
                                       char *value) {
  /* Base case for no capacity */
  assert(section->capacity > 0);


  if (section->capacity == section->length) {
    char **new_variables, **new_values;
    unsigned i;
    new_variables =
        malloc(sizeof(char *) * (section->capacity + DEFAULT_SECTION_CAPACITY));
    if (!new_variables) {
      return 1;
    }

    new_values =
        malloc(sizeof(char *) * (section->capacity + DEFAULT_SECTION_CAPACITY));
    if (!new_values) {
      free(new_variables);
      return 1;
    }

    for (i=0; i< section->capacity; i++) {
      new_variables[i] = section->variables[i];
      new_values[i] = section->values[i];
    }
    free(section->variables);
    free(section->values);
    section->variables = new_variables;
    section->values = new_values;
    section->capacity += DEFAULT_SECTION_CAPACITY;
  }

  assert(section->capacity > section->length); /* Can never be too certain */

  /* Now we can finally just append the information */
  char *new_variable, *new_value;
  new_variable = malloc(sizeof(char) * strlen(variable));
  if (!new_variable) {
    fprintf(stderr, "Memory allocation error.\n");
    return 1;
  }
  new_value = malloc(sizeof(char) * strlen(value));
  if (!new_value) {
    free(new_variable);
    fprintf(stderr, "Memory allocation error.\n");
    return 1;
  }

  strcpy(new_variable, variable);
  strcpy(new_value, value);


  section->variables[section->length] = new_variable;
  section->values[section->length] = new_value;
  section->length += 1;
  return 0;
}

void section_destroy(Section *section) {
  unsigned i;

  free(section->name);
  for (i=0; i<section->length; i++) {
    free(section->variables[i]);
    free(section->values[i]);
  }
  free(section->variables);
  free(section->values);
  free(section);
}
