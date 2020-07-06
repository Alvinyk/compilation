#include "craft/simple_script.h"

int main(int argc, char **argv) {
  bool verbose = false;
  if (argc > 1 && strcmp("-v", argv[1]) == 0) {
    verbose = true;
  }

  SimpleScript script(verbose);
  script.Run();
}