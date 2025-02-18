#include <criterion/criterion.h>
#include "git_config.h"

Test(git_config_create_from_file, test_no_capacity) {
  // Arrange
  unsigned capacity = 0;
  GitConfig *config;

  // Act
  config = git_config_create_with_capacity(capacity);

  // Assert
  cr_assert(config->capacity == 0);
}
