#ifndef GIT_CONFIG_SECTION_H
#define GIT_CONFIG_SECTION_H

#define DEFAULT_SECTION_CAPACITY 2

typedef struct {
  char *name;
  char **variables;
  char **values;
  unsigned capacity;
  unsigned length;
} Section;

Section *section_create_from_name(char *name);

int section_append_variable_value_pair(Section *section, char *variable,
                                       char *value);

void section_destroy(Section *section);

#endif /* GIT_CONFIG_SECTION_H */
