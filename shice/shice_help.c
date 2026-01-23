#include "console.h"
#include "shice/shice_help.h"

void shice_cmd_help(void) {
    console_write("Shice Shell - comandos disponiveis:\n");
    console_write("  help   - mostra esta ajuda\n");
    console_write("  clear  - limpa a tela\n");
    console_write("  echo   - repete o texto (ex: echo oi)\n");
    console_write("  ver    - mostra versao do Cinser\n");
    console_write("  ui     - (opcional) entra no desktop UI (se compilado)\n");
}
