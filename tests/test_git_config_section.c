#include <criterion/criterion.h>
#include "git_config_section.h"

Test(section_create_from_name, null_name_pointer) {
  // Arrange
  char *name = NULL;
  Section *section;

  // Act
  section = section_create_from_name(name);

  // Assert
  cr_assert(section == NULL);
}

Test(section_create_from_name, empty_name) {
  // Arrange 
  char *name = "";
  Section *section;

  // Act 
  section = section_create_from_name(name);

  // Assert
  cr_assert(section == NULL);
}
