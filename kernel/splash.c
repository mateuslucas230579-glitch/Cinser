#include "splash.h"
#include "console.h"
#include "delay.h"
#include <stdint.h>
#include <stddef.h>

// Arte ASCII 
static const char* const g_splash_art[] = {
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "                   :",
    "              %#  #*                                                            #@#  #",
    "              ++% %*%                                                           .+@ **+",
    "              +==%#++%                                                         #=#=#-++",
    "            %# *:-#+-*%                                                       +=+++-=* %#",
    "            *-#*#--+*-#%                                                     *+-#=-=%*=+%",
    "            #=--=*---=--%                                                   =:-=--+#=--*",
    "          %==*#---++---++=                                              ..-++---=+---+#+=%",
    "            *-:-#------#--=+:.                                        .-+*--#=-----*=-:=+",
    "          %#**#--------*++----::                                    .:----+##--------*+*#%",
    "           #*-----::--==#*------:.        . .          .           ::::---*#==:-::-----*#",
    "           %#*#%%+-:...*##=-==-:::        %.     ::.    .#        :------=*%*...::=#%%##%",
    "            #*=-----::--@+=---==---:.     ##-----::--:--**     .:---------*%--::----:=##",
    "            #+----:--::--#*----==-:---:.  =@=-   :::  --%-  .----::------#*:--------:-=#",
    "               @%*=--.   .:*-----===--------#--=-  :--=+----------------+-. .. :-+###",
    "             %=--------:  :-+#------------*-+#*#@  @%**+-#=----------=#+-. .:--------+%",
    "                 *--:::--:.:-=+%------=-=%-----==%%===----#=--------#+=-:.:--:.:-=*",
    "               #****#=-: .-..-:=+*-=%+-+=@=-:----=-------=%=+==#==+==-:..:..:-+##**#%",
    "                  #=--=#---:-: :-::-=%=##+#+-=-=---==--:+%+@#-%=:..-: --::--%+---%",
    "                     #=--#=--:--- :--::-:-#%%--======-=%%#---::--: ---:--=#--+%",
    "                        #++*+=--.----.:-*=#::-+=----=+=::%=#-:.----.--=*#++#",
    "                            %%%%**##+=## ##:-:-++==++-:-:%  *#=+*#***%%%",
    "                                        ##%: -=#++++#--.-#**",
    "                                       %%@@# %+%*==*%+@ @@@%%",
    "                                      %@%%@@@# -*--== *@@@%%@@",
    "                                      %@@@@@@.------::.@@@@@@%",
    "                                        =---@@% .:  .%@@---:",
    "                                     =-:.::...@--:-:-% ::-: .==   ..",
    "                                 #=--= :#=----------------=%-:+-:##",
    "                             %@+++%%@%+=---------------------=#%%++++%%",
    "                                   %+--+--:--:------:-------=--+#",
    "                                  *--*#=-.-%---+:---::-#----+%#+-+%",
    "                                 #*# #=::=**--**-:-*+::#%*%=-=#",
    "                                     #=*% %+-% #---%*=::%    #@#",
    "                                           @%  ##-+   %*%",
    "                                                 %",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    ""
};

static int splash_art_lines(void) {
    return (int)(sizeof(g_splash_art) / sizeof(g_splash_art[0]));
}

// Função auxiliar para calcular métricas da imagem ignorando espaços à esquerda
static void splash_get_metrics(int* out_padding, int* out_visual_width) {
    int n = splash_art_lines();
    int min_lead = 1000; // Começa grande
    int max_vis_w = 0;

    // 1. Descobre qual é a menor indentação (espaços à esquerda) em todas as linhas
    for (int i = 0; i < n; i++) {
        const char* s = g_splash_art[i];
        if (!s || !s[0]) continue; // Pula linhas vazias

        int lead = 0;
        while (s[lead] == ' ') lead++;

        // Se a linha for só espaços, ignora
        if (s[lead] == '\0') continue;

        if (lead < min_lead) min_lead = lead;
    }

    if (min_lead == 1000) min_lead = 0; // Caso a imagem seja vazia

    // 2. Calcula a largura visual máxima (tamanho total - indentação comum)
    for (int i = 0; i < n; i++) {
        const char* s = g_splash_art[i];
        if (!s || !s[0]) continue;

        int len = 0;
        while (s[len]) len++;

        // Largura visual é o comprimento total menos a gordura da esquerda
        int vis_w = len - min_lead;
        if (vis_w > max_vis_w) max_vis_w = vis_w;
    }

    *out_padding = min_lead;
    *out_visual_width = max_vis_w;
}

void splash_show(uint32_t seconds) {
    // Limpa e prepara
    console_clear();
    console_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);

    int cols = console_get_cols();
    int rows = console_get_rows();
    int art_h = splash_art_lines();
    
    // Pega as métricas corrigidas (ignorando o espaço morto à esquerda)
    int padding_left, art_w;
    splash_get_metrics(&padding_left, &art_w);

    // Centraliza baseado na largura VISUAL, não na string inteira
    int start_col = (cols > art_w) ? (cols - art_w) / 2 : 0;
    int start_row = (rows > art_h) ? (rows - art_h) / 2 : 0;

    // Variável para rastrear a última linha onde algo foi realmente desenhado
    int last_y_pos = start_row;

    // Desenha a arte linha por linha
    for (int y = 0; y < art_h && (start_row + y) < rows; y++) {
        const char* line = g_splash_art[y];
        if (!line) line = "";
        
        int len = 0;
        while(line[len]) len++;
        
        if (len <= padding_left) {
            continue;
        }

        console_set_cursor(start_col, start_row + y);
        const char* visual_part = line + padding_left;
        
        for (int x = 0; visual_part[x] && (start_col + x) < cols; x++) {
            console_putc(visual_part[x]);
        }
        
        // Atualiza a posição da última linha de conteúdo
        last_y_pos = start_row + y;
    }

    // --- Adição do Texto da Versão ---
    const char* version_text = "Cinser Kernel v0.0.7";
    
    // Calcula o comprimento do texto
    int text_len = 0;
    while (version_text[text_len]) text_len++;

    // Centraliza horizontalmente
    int text_col = (cols > text_len) ? (cols - text_len) / 2 : 0;
    // Coloca 2 linhas abaixo da arte
    int text_row = last_y_pos + 2;

    if (text_row < rows) {
        console_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK); // Cor branca para o texto
        console_set_cursor(text_col, text_row);
        console_write(version_text);
    }

    // Segura na tela
    delay_time(seconds);
}