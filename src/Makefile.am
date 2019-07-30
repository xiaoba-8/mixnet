AUTOMAKE_OPTIONS= \
  foreign \
  subdir-objects
  
INCLUDES=-I../include

bin_PROGRAMS=mix_tcp_server_daemon_demo \
	mix_tcp_server_demo \
	mix_tcp_client_demo \
	mix_udp_server_daemon_demo \
	mix_udp_server_demo \
	mix_udp_client_demo 
	
mix_tcp_server_daemon_demo_SOURCES= \
  tcp_server_daemon_demo.cpp
mix_tcp_server_daemon_demo_LDADD=../lib/libmixnet.la
mix_tcp_server_daemon_demo_LDFLAGS=-pthread

mix_tcp_server_demo_SOURCES= \
  tcp_server_demo.cpp
mix_tcp_server_demo_LDADD=../lib/libmixnet.la
mix_tcp_server_demo_LDFLAGS=-pthread

mix_tcp_client_demo_SOURCES= \
  tcp_client_demo.cpp
mix_tcp_client_demo_LDADD=../lib/libmixnet.la
mix_tcp_client_demo_LDFLAGS=-pthread

mix_udp_server_daemon_demo_SOURCES= \
  udp_server_daemon_demo.cpp
mix_udp_server_daemon_demo_LDADD=../lib/libmixnet.la
mix_udp_server_daemon_demo_LDFLAGS=-pthread

mix_udp_server_demo_SOURCES= \
  udp_server_demo.cpp
mix_udp_server_demo_LDADD=../lib/libmixnet.la
mix_udp_server_demo_LDFLAGS=-pthread

mix_udp_client_demo_SOURCES= \
  udp_client_demo.cpp
mix_udp_client_demo_LDADD=../lib/libmixnet.la
mix_udp_client_demo_LDFLAGS=-pthread
