#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LIMIT 126

int DEBUG = 0;

//  Estrutura de dados híbrida de lista (next, prev) e árvore binária (left, right, parent)
struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
    //  Parent vai ser usado para percorrer a árvore de baixo para cima
    struct tree_node* parent;

    struct tree_node* next;
    struct tree_node* prev;
    //  Frequência acumulada do nó
    int f;
    //  Caractere ou conjunto de caracteres assomados
    char c[500];
};

struct tree_node* make_node() {
    struct tree_node* n = (struct tree_node*)malloc(sizeof(struct tree_node));
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    n->next = NULL;
    n->prev = NULL;
    n->f = 0;
    n->c[0] = '\0';

    return n;
}

short count_chars(char *source, int *count) {
    int i = 0;
    int total = 0;
    while (*(source + i) != '\0') {
        //  Variável auxiliar mais legível do que "*(source + i)" no intervalo [0-LIMIT]
        // char current_char = toupper(*(source + i));
        char current_char = *(source + i);
        //  aborta caso haja um caractere fora da range [A-Z] + ' '
        // if (current_char < 0 || current_char >= LIMIT) return 0;

        //  Incremeneta o contador do caractere atual
        if (*(count + (current_char)) == 0) total++;
        *(count + (current_char)) += 1;
        i++;
    }
    return total;
}

void print_count(int* count) {
    for (int i = 0; i < LIMIT; i++) {
        // printf("Teste - %d\n", i);
        // if (*(count + i) > 0 && i >= 32) printf("%c - %d\n", i, *(count + i));
    }
}

short lst_push(struct tree_node** root, struct tree_node* element) {
    if (root == NULL) return 0;
    if (*root == NULL) {
        if (DEBUG) printf("Botando %s na raiz\n", element->c);
        *root = element;
        return 1;
    }
    struct tree_node* seeker = *root;
    while (seeker->next != NULL && seeker->f < element->f) {
        seeker = seeker->next;
    }

    if (seeker->f < element->f) {
        if (DEBUG) printf("Botando %s (%d) depois de %s (%d)\n", element->c, element->f, seeker->c, seeker->f);
        element->prev = seeker;
        element->next = seeker->next;
        seeker->next = element;
    } else {
        if (DEBUG) printf("Botando %s (%d) antes de %s (%d)\n", element->c, element->f, seeker->c, seeker->f);
        if (*root == seeker) *root = element;
        element->next = seeker;
        element->prev = seeker->prev;
        if (seeker->prev != NULL) seeker->prev->next = element;
        seeker->prev = element;
    }

    return 1;
}

struct tree_node* lst_pop(struct tree_node** root) {
    if (root == NULL || *root == NULL) {
        return NULL;
    }
    struct tree_node* seeker = *root;
    *root = seeker->next;
    if (*root != NULL)
        (*root)->prev = NULL;

    return seeker;
}

/**
**  Função repsonsável por inserir ordenadamente as contagens de caracteres numa lista de folhas
**/
struct tree_node* chars_to_nodes(int *count) {
    struct tree_node* root = NULL;
    for (int i = 0; i < LIMIT; i++) {
        if (*(count + i) > 0) {
            struct tree_node* n = make_node();
            n->f = *(count + i);
            //  HACK THE PLANET
            n->c[0] = i;
            n->c[1] = '\0';
            lst_push(&root, n);
        }
    }
    return root;
}

struct tree_node* huffman_tree(struct tree_node** list, int total) {
    struct tree_node* root = NULL;
    while (total > 0) {
        //  Pega a opção mais greedy, ou seja: os caracteres com menores frequência
        struct tree_node* greedy_one = lst_pop(list);
        total--;
        struct tree_node* greedy_two = lst_pop(list);
        total--;
        if (DEBUG) printf("greedy one = %s (%d)\n", greedy_one->c, greedy_one->f);
        if (DEBUG) printf("greedy two = %s (%d)\n", greedy_two->c, greedy_two->f);

        struct tree_node* new_root = make_node();
        new_root->right = greedy_one;
        new_root->left = greedy_two;
        strcat(new_root->c, greedy_two->c);
        strcat(new_root->c, "+");
        strcat(new_root->c, greedy_one->c);
        new_root->f = greedy_one->f + greedy_two->f;

        greedy_one->parent = new_root;
        greedy_two->parent = new_root;

        lst_push(list, new_root);
        total++;
        root = new_root;
    }

    return root;
}

struct tree_node* tree_search(struct tree_node* root, char c) {
    if (root == NULL) return NULL;
    //  TODO: REMOVER ESSE HACK NOJENTO
    if (*(root->c) == c && *(root->c + 1) == '\0') return root;

    struct tree_node* current = NULL;
    current = tree_search(root->left, c);
    if (current != NULL) return current;
    current = tree_search(root->right, c);
    if (current != NULL) return current;

    return NULL;
}

void invert(char* word, int size) {
    char aux;
    for (int i = 0; i < size / 2; i++) {
        aux = *(word + i);
        *(word + i) = *(word + (size - 1 - i));
        *(word + (size - 1 - i)) = aux;
    }
}

char* bin_from_tree(struct tree_node* root, char c) {
    char* bin = (char*)malloc(sizeof(char) * 7);
    int size = 0;
    struct tree_node* leaf = tree_search(root, c);
    while (leaf->parent != NULL) {
        *(bin + size) = (leaf == leaf->parent->right) + '0';
        size++;
        leaf = leaf->parent;
    }
    *(bin + size) = '\0';
    //  Como o caminho pela árvore é inverso, é preciso inverter a string
    invert(bin, size);

    return bin;
}

void explain(struct tree_node* root) {
    if (root == NULL) return;

    printf("============> elemento = %s\n", root->c);
    explain(root->left);
    explain(root->right);
}

int main() {
    //  Texto a ser comprimido
    // char source_text[100] = "MEUNOMEERAFAELARAUJOCHINAGLIA";
    // char source_text[999] = "CASA PAPEL HOTEL PASTEL";
    // char source_text[] = "ONCEUPONAMIDNIGHTDREARYWHILEIPONDEREDWEAKANDWEARYOVERMANYAQUAINTANDCURIOUSVOLUMEOFFORGOTTENLOREWHILEINODDEDNEARLYNAPPINGSUDDENLYTHERECAMEATAPPINGASOFSOMEONEGENTLYRAPPINGRAPPINGATMYCHAMBERDOORTISSOMEVISITORIMUTTEREDTAPPINGATMYCHAMBERDOORONLYTHISANDNOTHINGMOREAHDISTINCTLYIREMEMBERITWASINTHEBLEAKDECEMBERANDEACHSEPARATEDYINGEMBERWROUGHTITSGHOSTUPONTHEFLOOREAGERLYIWISHEDTHEMORROWVAINLYIHADSOUGHTTOBORROWFROMMYBOOKSSURCEASEOFSORROWSORROWFORTHELOSTLENOREFORTHERAREANDRADIANTMAIDENWHOMTHEANGELSNAMELENORENAMELESSHEREFOREVERMOREANDTHESILKENSADUNCERTAINRUSTLINGOFEACHPURPLECURTAINTHRILLEDMEFILLEDMEWITHFANTASTICTERRORSNEVERFELTBEFORESOTHATNOWTOSTILLTHEBEATINGOFMYHEARTISTOODREPEATINGTISSOMEVISITORENTREATINGENTRANCEATMYCHAMBERDOORSOMELATEVISITORENTREATINGENTRANCEATMYCHAMBERDOORTHISITISANDNOTHINGMOREPRESENTLYMYSOULGREWSTRONGERHESITATINGTHENNOLONGERSIRSAIDIORMADAMTRULYYOURFORGIVENESSIIMPLOREBUTTHEFACTISIWASNAPPINGANDSOGENTLYYOUCAMERAPPINGANDSOFAINTLYYOUCAMETAPPINGTAPPINGATMYCHAMBERDOORTHATISCARCEWASSUREIHEARDYOUHEREIOPENEDWIDETHEDOORDARKNESSTHEREANDNOTHINGMOREDEEPINTOTHATDARKNESSPEERINGLONGISTOODTHEREWONDERINGFEARINGDOUBTINGDREAMINGDREAMSNOMORTALEVERDAREDTODREAMBEFOREBUTTHESILENCEWASUNBROKENANDTHESTILLNESSGAVENOTOKENANDTHEONLYWORDTHERESPOKENWASTHEWHISPEREDWORDLENORETHISIWHISPEREDANDANECHOMURMUREDBACKTHEWORDLENOREMERELYTHISANDNOTHINGMOREBACKINTOTHECHAMBERTURNINGALLMYSOULWITHINMEBURNINGSOONAGAINIHEARDATAPPINGSOMEWHATLOUDERTHANBEFORESURELYSAIDISURELYTHATISSOMETHINGATMYWINDOWLATTICELETMESEETHENWHATTHEREATISANDTHISMYSTERYEXPLORELETMYHEARTBESTILLAMOMENTANDTHISMYSTERYEXPLORETISTHEWINDANDNOTHINGMOREOPENHEREIFLUNGTHESHUTTERWHENWITHMANYAFLIRTANDFLUTTERINTHERESTEPPEDASTATELYRAVENOFTHESAINTLYDAYSOFYORENOTTHELEASTOBEISANCEMADEHENOTAMINUTESTOPPEDORSTAYEDHEBUTWITHMIENOFLORDORLADYPERCHEDABOVEMYCHAMBERDOORPERCHEDUPONABUSTOFPALLASJUSTABOVEMYCHAMBERDOORPERCHEDANDSATANDNOTHINGMORETHENTHISEBONYBIRDBEGUILINGMYSADFANCYINTOSMILINGBYTHEGRAVEANDSTERNDECORUMOFTHECOUNTENANCEITWORETHOUGHTHYCRESTBESHORNANDSHAVENTHOUISAIDARTSURENOCRAVENGHASTLYGRIMANDANCIENTRAVENWANDERINGFROMTHENIGHTLYSHORETELLMEWHATTHYLORDLYNAMEISONTHENIGHTSPLUTONIANSHOREQUOTHTHERAVENNEVERMOREMUCHIMARVELLEDTHISUNGAINLYFOWLTOHEARDISCOURSESOPLAINLYTHOUGHITSANSWERLITTLEMEANINGLITTLERELEVANCYBOREFORWECANNOTHELPAGREEINGTHATNOLIVINGHUMANBEINGEVERYETWASBLESSEDWITHSEEINGBIRDABOVEHISCHAMBERDOORBIRDORBEASTUPONTHESCULPTUREDBUSTABOVEHISCHAMBERDOORWITHSUCHNAMEASNEVERMOREBUTTHERAVENSITTINGLONELYONTHEPLACIDBUSTSPOKEONLYTHATONEWORDASIFHISSOULINTHATONEWORDHEDIDOUTPOURNOTHINGFARTHERTHENHEUTTEREDNOTAFEATHERTHENHEFLUTTEREDTILLISCARCELYMORETHANMUTTEREDOTHERFRIENDSHAVEFLOWNBEFOREONTHEMORROWHEWILLLEAVEMEASMYHOPESHAVEFLOWNBEFORETHENTHEBIRDSAIDNEVERMORESTARTLEDATTHESTILLNESSBROKENBYREPLYSOAPTLYSPOKENDOUBTLESSSAIDIWHATITUTTERSISITSONLYSTOCKANDSTORECAUGHTFROMSOMEUNHAPPYMASTERWHOMUNMERCIFULDISASTERFOLLOWEDFASTANDFOLLOWEDFASTERTILLHISSONGSONEBURDENBORETILLTHEDIRGESOFHISHOPETHATMELANCHOLYBURDENBOREOFNEVERNEVERMOREBUTTHERAVENSTILLBEGUILINGALLMYFANCYINTOSMILINGSTRAIGHTIWHEELEDACUSHIONEDSEATINFRONTOFBIRDANDBUSTANDDOORTHENUPONTHEVELVETSINKINGIBETOOKMYSELFTOLINKINGFANCYUNTOFANCYTHINKINGWHATTHISOMINOUSBIRDOFYOREWHATTHISGRIMUNGAINLYGHASTLYGAUNTANDOMINOUSBIRDOFYOREMEANTINCROAKINGNEVERMORETHISISATENGAGEDINGUESSINGBUTNOSYLLABLEEXPRESSINGTOTHEFOWLWHOSEFIERYEYESNOWBURNEDINTOMYBOSOMSCORETHISANDMOREISATDIVININGWITHMYHEADATEASERECLININGONTHECUSHIONSVELVETLININGTHATTHELAMPLIGHTGLOATEDOERBUTWHOSEVELVETVIOLETLININGWITHTHELAMPLIGHTGLOATINGOERSHESHALLPRESSAHNEVERMORETHENMETHOUGHTTHEAIRGREWDENSERPERFUMEDFROMANUNSEENCENSERSWUNGBYSERAPHIMWHOSEFOOTFALLSTINKLEDONTHETUFTEDFLOORWRETCHICRIEDTHYGODHATHLENTTHEEBYTHESEANGELSHEHATHSENTTHEERESPITERESPITEANDNEPENTHEFROMTHYMEMORIESOFLENOREQUAFFOHQUAFFTHISKINDNEPENTHEANDFORGETTHISLOSTLENOREQUOTHTHERAVENNEVERMOREPROPHETSAIDITHINGOFEVILPROPHETSTILLIFBIRDORDEVILWHETHERTEMPTERSENTORWHETHERTEMPESTTOSSEDTHEEHEREASHOREDESOLATEYETALLUNDAUNTEDONTHISDESERTLANDENCHANTEDONTHISHOMEBYHORRORHAUNTEDTELLMETRULYIIMPLOREISTHEREISTHEREBALMINGILEADTELLMETELLMEIIMPLOREQUOTHTHERAVENNEVERMOREPROPHETSAIDITHINGOFEVILPROPHETSTILLIFBIRDORDEVILBYTHATHEAVENTHATBENDSABOVEUSBYTHATGODWEBOTHADORETELLTHISSOULWITHSORROWLADENIFWITHINTHEDISTANTAIDENNITSHALLCLASPASAINTEDMAIDENWHOMTHEANGELSNAMELENORECLASPARAREANDRADIANTMAIDENWHOMTHEANGELSNAMELENOREQUOTHTHERAVENNEVERMOREBETHATWORDOURSIGNOFPARTINGBIRDORFIENDISHRIEKEDUPSTARTINGGETTHEEBACKINTOTHETEMPESTANDTHENIGHTSPLUTONIANSHORELEAVENOBLACKPLUMEASATOKENOFTHATLIETHYSOULHATHSPOKENLEAVEMYLONELINESSUNBROKENQUITTHEBUSTABOVEMYDOORTAKETHYBEAKFROMOUTMYHEARTANDTAKETHYFORMFROMOFFMYDOORQUOTHTHERAVENNEVERMOREANDTHERAVENNEVERFLITTINGSTILLISSITTINGSTILLISSITTINGONTHEPALLIDBUSTOFPALLASJUSTABOVEMYCHAMBERDOORANDHISEYESHAVEALLTHESEEMINGOFADEMONSTHATISDREAMINGANDTHELAMPLIGHTOERHIMSTREAMINGTHROWSHISSHADOWONTHEFLOORANDMYSOULFROMOUTTHATSHADOWTHATLIESFLOATINGONTHEFLOORSHALLBELIFTEDNEVERMORE";
    char source_text[999];
    printf("Digite o texto para compactar. Por favor, sem acentos! ");
    fgets(source_text, 999, stdin);
    int last_char = strlen(source_text) - 1;
    if (*(source_text + last_char) == '\n') {
        *(source_text + last_char) = '\0';
    }

    // char source_text[100] = "AAAAAABBBBBCCCCDDDEEEEEEEEEF";
    //  Vetor para contagem de caracteres
    //  TODO: incluir caracteres especiais, acentos, espaços e etc
    int char_count[LIMIT];
    //  Inicializando char_count com zeros em todas as posições
    for (int i = 0; i < LIMIT; i++) char_count[i] = 0;
    int total = 0;

    if (!(total = count_chars(source_text, char_count))) {
        printf("Existem caracteres ilegais na string de origem!\nNo momento só aceitamos caracteres no intervalo [A-Z]!\n\n");
        return 0;
    }

    print_count(char_count);
    //  Cria uma lista para os elementos com suas frequências ordenadas
    struct tree_node* first_leaf = chars_to_nodes(char_count);
    struct tree_node* root = NULL;


    if (DEBUG) printf("==========================\n");
    root = huffman_tree(&first_leaf, total - 1);

    if (DEBUG) explain(root);
    char r[999999]; for (int i = 0; i < 999999; i++) r[i] = '\0';
    char* code;
    for (int i = 0; i < LIMIT; i++) {
        if (char_count[i] == 0) continue;
        code = bin_from_tree(root, i);
        // printf("WARD for %c\n", i);
        printf("Binary code for %c is %s\n", i, code);
    }
    char current_char;
    current_char = source_text[0];
    int i = 0;
    printf("============================\n");
    //  Traduz a string original usando os códigos da árvore de Huffman.
    while (current_char != '\0') {
        code = bin_from_tree(root, current_char);
        strcat(r, code);
        printf("%s", code);
        i++;
        current_char = source_text[i];
    }
    printf("\n\nTamanho original: %ld bits\n", strlen(source_text) * 4);
    printf("Tamanho compactado: %ld bits\n", strlen(r));
    printf("\n\n");
    // printf("Resultado: %s\n", r);

    return 1;
}
