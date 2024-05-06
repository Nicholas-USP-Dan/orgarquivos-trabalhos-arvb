/**
 * @file funcoes_fornecidas.h
 * @brief Header file de funções fornecidas para o projeto
 * 
 * @author Cristina Dutra de Aguiar (cdac@icmc.usp.br)
 * @version 1.0
 * 
 */

#ifndef FUNC_FORN
#define FUNC_FORN

/**
 * @brief Função que imprime uma hash na tela de um arquivo binário de dados
 * 
 * Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
 * Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela).
 * 
 * @param nomeArquivoBinario Nome do arquivo a ser hasheado.
 */
void binarioNaTela(char *nomeArquivoBinario);

/**
 * @brief Funcionalidade para ler strings com aspas
 * 
 * @details A função lê strings normalmente (como scanf("%s")) caso uma entrada não tenha aspas
 * 
 * @param [out] str String a ser escrita (str já deve ter espaço alocado)
 */
void scan_quote_string(char *str);

#endif