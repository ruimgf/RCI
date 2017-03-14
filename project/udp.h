#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * [udp_connect Realiza uma nova conexão ao servidor]
 * @param  udp_server_ip   [Endereço do Servidor]
 * @param  udp_server_port [Porta do Servidor]
 * @return                [Return]
 */
int udpConnect();
/**
 * [udp_write Trata operções de escrita por parte dos clientes]
 * @param  udp_descriptor [identificador do servidor]
 * @param  key           [chave]
 * @param  value         [valor]
 * @param  value_length  [tamanho do valor]
 * @param  overwrite     [se quer ou não fazer overwrite]
 * @return               [-1 em caso de erro 0 no caso de sucesso]
 */

int udp_write_to(int udp_descriptor,char* mensage, int length,char * ip, int port);



/**
 * [udp_read operação leitura por parte do cliente]
 * @param  udp_descriptor [identificador do servidor]
 * @param  key           [chave]
 * @param  value         [valor]
 * @param  value_length  [tamanho para receber valor]
 * @return               [nr_bytes lidos ou -1,-2 em casos de erro]
 */
int udpRead(int udp_descriptor, char * buffer, int length);
