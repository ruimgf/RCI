#include <stdint.h>
#include <string.h>

int tcpBindListen(int server_port);

int tcpAccept(int myFd);

/**
 * [tcp_connect Realiza uma nova conexão ao servidor]
 * @param  tcp_server_ip   [Endereço do Servidor]
 * @param  tcp_server_port [Porta do Servidor]
 * @return                [Return]
 */
int tcpConnect(char * server_ip, int server_port);
/**
 * [tcp_write Trata operções de escrita por parte dos clientes]
 * @param  tcp_descriptor [identificador do servidor]
 * @param  key           [chave]
 * @param  value         [valor]
 * @param  value_length  [tamanho do valor]
 * @param  overwrite     [se quer ou não fazer overwrite]
 * @return               [-1 em caso de erro 0 no caso de sucesso]
 */
int tcpWrite(int tcp_descriptor,char * mensage, int length);
/**
 * [tcp_read operação leitura por parte do cliente]
 * @param  tcp_descriptor [identificador do servidor]
 * @param  key           [chave]
 * @param  value         [valor]
 * @param  value_length  [tamanho para receber valor]
 * @return               [nr_bytes lidos ou -1,-2 em casos de erro]
 */
int tcpRead(int tcp_descriptor, char * buffer, int length);
