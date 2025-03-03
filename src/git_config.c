#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "git_config.h"

/* -- Internal Functions -- */
void _trim_edge_whitespace_inplace(char *buffer, unsigned len) {
  unsigned i, shift;

  /* trim the right first */
  for (i = len - 1; i >= 0; i--) {
    if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n' ||
        buffer[i] == '\0') {
      buffer[i] = '\0';
    } else {
      break;
    }
  }

  /* Find first non-whitespace character */
  shift = 0;
  while (buffer[shift] == ' ' || buffer[shift] == '\t' ||
         buffer[shift] == '\n') {
    shift++;
  }

  for (i = shift; i < len; i++) {
    buffer[i - shift] = buffer[i];
  }
}

GitConfig *git_config_create_with_capacity(unsigned capacity) {
  GitConfig *config = malloc(sizeof(GitConfig));
  if (!config)
    return NULL;

  /* Allocating pointers to sections not actual sections */
  Section **sections = malloc(sizeof(Section *) * capacity);
  if (!sections) {
    free(config);
    return NULL;
  }

  config->sections = sections;
  config->capacity = capacity;
  config->length = 0;
  return config;
}

void git_config_destroy(GitConfig *config) {
  unsigned i;

  for (i=0; i<config->length; i++) {
    free(config->sections[i]);
  }

  free(config->sections);
  free(config);
}

int git_config_append_section_move(GitConfig *config, Section *section) {
  if (config->capacity == config->length) {
    fprintf(stderr, "Too many sections!\n");
    return 1;
  }

  config->sections[config->length++] = section;
  return 0;
}

GitConfig *git_config_read_from_file(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f)
    return NULL;

  GitConfig *config = git_config_create_with_capacity(10);
  if (!config) {
    fclose(f);
    return NULL;
  }
  char buffer[2048];
  unsigned i = 0;
  Section *current_section = NULL;
  while (fgets(buffer, 2048, f)) {
    if (buffer[0] == '[') {
      if (current_section) {
        if (git_config_append_section_move(config, current_section)) {
          git_config_destroy(config);
          fclose(f);
          return NULL;
        }
        current_section = NULL;
      }

      i = 0;
      while (buffer[i] != ']') {
        i++;
      }
      buffer[i] = '\0';
      current_section = section_create_from_name(buffer + 1);
      if (!current_section) {
        fprintf(stderr, "Couldn't create section.\n");
        git_config_destroy(config);
        fclose(f);
        return NULL;
      }
    } else {
      if (!current_section) {
        fprintf(stderr,
                "Config file error! Expected section name to come first.\n");
        git_config_destroy(config);
        fclose(f);
        return NULL;
      }

      _trim_edge_whitespace_inplace(buffer, 2048);

      /* String magic with how \0 is the "end" of a string */
      i = 0;
      while (buffer[i] != '=') {
        i++;
      }
      buffer[i - 1] = '\0';
      if (section_append_variable_value_pair(current_section, buffer,
                                             buffer + (i + 2))) {
        fprintf(stderr, "Couldn't add variable-value pair.\n");
        git_config_destroy(config);
        fclose(f);
        return NULL;
      }
    }

    memset(buffer, 0, 2048);
  }

  /* Need to add in the last section */
  if (current_section) {
    if (git_config_append_section_move(config, current_section)) {
      fprintf(stderr, "Couldn't save section.\n");
      git_config_destroy(config);
      fclose(f);
      return NULL;
    }
    current_section = NULL;
  }

  fclose(f);

  return config;
}

int git_config_write_to_file(GitConfig *config, FILE *f) {
  if (!f || !config) {
    return 1;
  }

  unsigned i_sections, i;
  Section *current_section;
  for (i_sections=0; i_sections<config->length; i_sections++) {
    current_section = config->sections[i_sections]; 

    /* Writing a section to a file */
    fprintf(f, "[%s]\n", current_section->name);
    for (i=0; i<current_section->length; i++) {
      fprintf(f, "\t%s = %s\n", current_section->variables[i], current_section->values[i]);
    }
  }
  return 0;
}
