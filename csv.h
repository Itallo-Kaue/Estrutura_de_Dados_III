/*
    csv.h
    Leitura do arquivo de entrada conectaPoPs.csv e carga dos seus registros
    no arquivo de dados binário (funcionalidade [1]).
*/

#ifndef CSV_H
#define CSV_H

#include <stdio.h>

/* Lê o arquivo CSV informado e grava cada uma de suas linhas como um registro
   de dados no arquivo binário de saída, que é criado do zero.
   Retorna 1 em caso de sucesso e 0 se algum dos arquivos não puder ser aberto. */
int criarTabela(const char *nomeCSV, const char *nomeBin);

#endif
