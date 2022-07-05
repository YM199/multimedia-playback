#ifndef __MULTICAST_H__
#define __MULTICAST_H__

#define MCAST_ADDR  "224.0.0.88"
#define CLIENT_PORT 8000
#define SERVER_PORT CLIENT_PORT

void server_multicast(void);
void client_multicast(void);
#endif