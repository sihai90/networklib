#ifndef _INET_ADDRESS_HPP
#define _INET_ADDRESS_HPP

#include <cstring>
#include <string>
#include <cstdint>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <sys/socket.h>
#include <type_traits>
#include "Logging.hpp"

class InetAddress
{
public:
  InetAddress(const std::string& ip, uint16_t port)
  {
    memset(&addr, 0, sizeof(addr));
    convertIpAddr(ip, port);
  }

  InetAddress(bool loopbackOnly = false, uint16_t port = 0)
  {
    in_addr_t ipaddr = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    addr.sin_addr.s_addr = htonl(ipaddr);
  }

  explicit InetAddress(const struct sockaddr_in& addre): addr(addre)
  {
  }



  sa_family_t family() const { return addr.sin_family; }

  const struct sockaddr* getSockAddr() const 
  { 
   //sockaddr is used for bind/connect, whereas sockaddr_in has meaningful member;
    static_assert(sizeof(struct sockaddr) == sizeof(struct sockaddr_in), "struct sockaddr,sockaddr_in not same size");
    return (const struct sockaddr*)&addr; 
  }


  std::string ipPort() const
  {
      int port = ntohs(addr.sin_port);
      std::string ipaddr(INET_ADDRSTRLEN, ' ');
      ::inet_ntop(AF_INET, &addr.sin_addr, &ipaddr[0], static_cast<socklen_t >(ipaddr.size()));
      return ipaddr + " " + std::to_string(port);
  }

 ~InetAddress()  = default;
 InetAddress(const InetAddress&) = default;
 InetAddress& operator=(const InetAddress&) = default;

 
private:
  void convertIpAddr(const std::string& ip, uint16_t port)
  {
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
    {
      LOG_FATAL <<"ip : "<< ip << "port : "<< port;
    }
  }

private:
  struct sockaddr_in addr;

  
};




#endif

