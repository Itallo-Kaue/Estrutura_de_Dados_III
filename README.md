# Trabalho Introdutório — Estrutura de Dados III

Manipulação de um arquivo de dados de registros de tamanho fixo, com acesso
sequencial e direto por RRN, remoção lógica encadeada em pilha e
reaproveitamento de espaço nas inserções.

## Autores

| Nome                       | NUSP     |
| -------------------------- | -------- |
| Ítallo Kauê Barbosa Santos | 16839029 |
| Natália Yumi Wanabi        | 13725566 |

## Compilação e execução

```
make          # gera o executável programaTrabalho
make run      # compila e executa
make clean    # remove os .o e o executável
```

A entrada é lida da entrada padrão e começa sempre pelo número da
funcionalidade.

## Organização do arquivo de dados

O arquivo binário é formado por um registro de cabeçalho seguido dos registros
de dados, todos de tamanho fixo. Toda a leitura e escrita é feita **campo a
campo**, nunca gravando a struct inteira de uma vez.

**Cabeçalho — 17 bytes**

| campo       | tipo | bytes | descrição                                  |
| ----------- | ---- | ----- | ------------------------------------------ |
| `status`    | char | 1     | `'0'` inconsistente, `'1'` consistente     |
| `topoPilha` | int  | 4     | RRN do topo da pilha de removidos, ou `-1` |
| `proxRRN`   | int  | 4     | próximo RRN disponível no fim do arquivo   |
| `nroRegRem` | int  | 4     | quantidade de registros removidos          |
| `nroPares`  | int  | 4     | quantidade de pares carregados do CSV      |

**Registro de dados — 18 bytes**

| campo               | tipo | bytes | descrição                        |
| ------------------- | ---- | ----- | -------------------------------- |
| `removido`          | char | 1     | `'0'` ou `'1'`                   |
| `encadeamentoPilha` | int  | 4     | RRN do próximo removido, ou `-1` |
| `idPoPs`            | int  | 4     | não pode ser nulo                |
| `idPoPsConectado`   | int  | 4     | não pode ser nulo                |
| `velocidade`        | int  | 4     | `-1` quando nulo                 |
| `unidadeMedida`     | char | 1     | `'$'` quando nulo                |

O byte offset de um registro é `17 + RRN * 18`, o que permite o acesso direto
usado na funcionalidade [4].

A pilha de removidos não existe em memória: o topo é o campo `topoPilha` do
cabeçalho e o encadeamento é o campo `encadeamentoPilha` de cada registro
removido — uma lista encadeada gravada dentro do próprio arquivo.

## Funcionalidades

| #   | operação                                 | entrada                                                       |
| --- | ---------------------------------------- | ------------------------------------------------------------- |
| 1   | carrega o CSV em um arquivo binário novo | `1 arquivo.csv arquivo.bin`                                   |
| 2   | lista todos os registros não removidos   | `2 arquivo.bin`                                               |
| 3   | busca por critérios                      | `3 arquivo.bin n` + n linhas `m campo valor ...`              |
| 4   | busca por RRN                            | `4 arquivo.bin rrn`                                           |
| 5   | remoção lógica                           | `5 arquivo.bin n` + n linhas `m campo valor ...`              |
| 6   | inserção com reaproveitamento de espaço  | `6 arquivo.bin n` + n linhas com os 4 campos                  |
| 7   | atualização de registros                 | `7 arquivo.bin n` + n pares de linhas (busca / novos valores) |

Os campos de busca aceitos são `idPoPs`, `idPoPsConectado`, `velocidade` e
`unidadeMedida`. A unidade de medida é escrita entre aspas na entrada e na
saída; um campo ausente é informado como `NULO`.

Quando o arquivo não pode ser processado, a saída é
`Falha no processamento do arquivo.`; quando uma busca não encontra nada,
`Registro inexistente.`

As funcionalidades que alteram o arquivo (1, 5, 6 e 7) terminam chamando
`BinarioNaTela`.

## Módulos

| arquivo                | responsabilidade                                             |
| ---------------------- | ------------------------------------------------------------ |
| `main.c`               | lê a opção e chama a funcionalidade correspondente           |
| `funcionalidades.c/.h` | as sete funcionalidades: leitura dos parâmetros e saída      |
| `csv.c/.h`             | carga do arquivo de dados a partir do CSV                    |
| `registro.c/.h`        | leitura e escrita campo a campo do cabeçalho e dos registros |
| `busca.c/.h`           | leitura dos critérios, comparação e varredura do arquivo     |
| `pilha.c/.h`           | pilha de registros removidos (empilhar e desempilhar)        |
| `util.c/.h`            | impressão de um registro na saída padrão                     |
| `fornecidas.c/.h`      | funções fornecidas pelo professor e seus protótipos          |

## Observações de implementação

- O separador do CSV é aceito tanto como vírgula quanto como ponto e vírgula, e
  a quebra de linha pode ser LF ou CRLF.
- A função fornecida `ScanQuoteString` é usada apenas no campo entre aspas: para
  um valor sem aspas ela lê o primeiro caractere e chama `scanf("%s")` para o
  resto, o que faz um valor de um único dígito ser concatenado com a palavra
  seguinte da entrada. Os campos numéricos são lidos separadamente.
- O `nroPares` é gravado na carga do CSV e não é alterado pelas remoções nem
  pelas inserções.
