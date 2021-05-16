#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"

typedef struct{
    char *kod;
    char *kelime;
}Node;

int main(int argc, char **argv)
{
    FILE *f;
    IS girdi, kilit;
    JRB agacKod, agacKelime, yedek;
    Node *n;
    int i, j;
    char *tmp;
    if (argc != 4) { fprintf(stderr, "Parametre hatasi(kripto islem giris_metni cikis_metni)\n"); exit(1); }

    girdi = new_inputstruct(argv[2]);
    kilit = new_inputstruct("./.kilit");
    if (girdi == NULL) {
        printf("giris dosyası yok/açılamadı!");
        perror(argv[2]);
        exit(1);
    }
    if (kilit == NULL) {
        printf(".kilit dosyası yok/açılamadı!");
        exit(1);
    }
    agacKod = make_jrb();
    agacKelime = make_jrb();
    while(get_line(kilit) >= 0) {
        for (i = 0; i < kilit->NF && kilit->NF>1; i++) {
            if (i==2){
                printf(".kilit dosyası bozuk\n");
                return 1;
                //exit(1);
            }
            n = malloc(sizeof(Node));
            tmp = malloc((strlen(kilit->fields[i])-2)*sizeof(char));
            for ( j = 1; j < strlen(kilit->fields[i])-2; j++)
            {
                tmp[j-1]=kilit->fields[i][j];
            }
            n->kelime=tmp;
            i++;
            tmp = malloc((strlen(kilit->fields[i])-1)*sizeof(char));
            for ( j = 1; j < strlen(kilit->fields[i])-1; j++)
            {
                if (kilit->fields[i][j] == '"')break;
                tmp[j-1]=kilit->fields[i][j];
            }
            n->kod=tmp;
            if (strcmp(argv[1],"-d")==0)
                jrb_insert_str(agacKod, strdup(n->kod), new_jval_v((void *) n));
            if (strcmp(argv[1],"-e")==0)
                jrb_insert_str(agacKelime, strdup(n->kelime), new_jval_v((void *) n));
        }
    } 
    if (strcmp(argv[1],"-e")==0)
    {
        if ((f = fopen (strdup(argv[3]), "w")) == NULL) {
            printf("Dosya açma hatası!");
            exit(1);
        }
        while(get_line(girdi) >= 0) {
            for (i = 0; i < girdi->NF; i++) {
                yedek = jrb_find_str(agacKelime, strdup(girdi->fields[i]));
                if (yedek==NULL)
                {
                    fputs(girdi->fields[i],f);
                    fputc(' ',f);
                }else{
                    n = malloc(sizeof(Node));
                    n = yedek->val.v;
                    fputs(n->kod,f);
                    fputc(' ',f);
                }
            }
        }
        fclose(f);

    }
    if (strcmp(argv[1],"-d")==0)
    {
        if ((f = fopen (strdup(argv[3]), "w")) == NULL) {
            printf("Dosya açma hatası!");
            exit(1);
        }
        while(get_line(girdi) >= 0) {
            for (i = 0; i < girdi->NF; i++) {
                yedek = jrb_find_str(agacKod, strdup(girdi->fields[i]));
                if (yedek==NULL)
                {
                    fputs(girdi->fields[i],f);
                    fputc(' ',f);
                }else{
                    n = malloc(sizeof(Node));
                    n = yedek->val.v;
                    fputs(n->kelime,f);
                    fputc(' ',f);
                }
            }
        }
        fclose(f);
    }
    jettison_inputstruct(girdi);
    jettison_inputstruct(kilit);
    return 0;
}