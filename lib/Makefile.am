AUTOMAKE_OPTIONS= \
  foreign \
  subdir-objects
  
INCLUDES=-I../include
  
lib_LTLIBRARIES=libmixnet.la 
libmixnet_la_SOURCES= \
  mix/net/i_tcp_socket_handler.cpp \
  mix/net/i_udp_socket_handler.cpp \
  mix/net/net_mutex.cpp \
  mix/net/net_mutex.h \
  mix/net/net_thread.cpp \
  mix/net/net_hread.h \
  mix/net/net_utils.cpp \
  mix/net/net_exception.cpp \
  mix/net/TcpServerDaemon.cpp \
  mix/net/TcpServerSocket.cpp \
  mix/net/Socket.cpp \
  mix/net/UdpServerDaemon.cpp \
  mix/net/UdpServerSocket.cpp

mixnetdir=$(includedir)/mix/net
mixnet_HEADERS=../include/mix/net/i_tcp_socket_handler.h \
  ../include/mix/net/i_udp_socket_handler.h \
  ../include/mix/net/net_errno.h \
  ../include/mix/net/net_utils.h \
  ../include/mix/net/net_exception.h \
  ../include/mix/net/TcpServerDaemon.h \
  ../include/mix/net/TcpServerSocket.h \
  ../include/mix/net/socket_common.h \
  ../include/mix/net/Socket.h \
  ../include/mix/net/UdpServerDaemon.h \
  ../include/mix/net/UdpServerSocket.h \
  ../include/mix/net/Task.h 
