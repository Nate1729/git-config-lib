#ifndef GIT_CONFIG_H
#define GIT_CONFIG_H

#include "git_config_section.h"

typedef struct {
  Section **sections;
  unsigned capacity;
  unsigned length;
} GitConfig;


GitConfig *git_config_create_with_capacity(unsigned capacity);
GitConfig *git_config_create_from_file(const char *filename);

int git_config_append_section_move(GitConfig *config, Section *section);

void git_config_destroy(GitConfig *config);

#endif /* GIT_CONFIG_H */
