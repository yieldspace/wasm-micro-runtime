/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef _WASI_SOCKET_EXT_H_
#define _WASI_SOCKET_EXT_H_

#include <stddef.h>
#include <stdint.h>

/*Be a part of <wasi/api.h>*/

typedef enum {
    SOCKET_DGRAM = 0,
    SOCKET_STREAM,
} __wasi_sock_type_t;

typedef uint16_t __wasi_ip_port_t;

typedef enum { IPv4 = 0, IPv6 } __wasi_addr_type_t;

/*
 n0.n1.n2.n3
 Example:
  IP Address: 127.0.0.1
  Structure: {n0: 127, n1: 0, n2: 0, n3: 1}
*/
typedef struct __wasi_addr_ip4_t {
    uint8_t n0;
    uint8_t n1;
    uint8_t n2;
    uint8_t n3;
} __wasi_addr_ip4_t;

typedef struct __wasi_addr_ip4_port_t {
    __wasi_addr_ip4_t addr;
    __wasi_ip_port_t port; /* host byte order */
} __wasi_addr_ip4_port_t;

/*
 n0:n1:n2:n3:h0:h1:h2:h3, each 16bit value uses host byte order
 Example (little-endian system)
  IP Address fe80::3ba2:893b:4be0:e3dd
  Structure: {
    n0: 0xfe80, n1:0x0, n2: 0x0, n3: 0x0,
    h0: 0x3ba2, h1: 0x893b, h2: 0x4be0, h3: 0xe3dd
  }
*/
typedef struct __wasi_addr_ip6_t {
    uint16_t n0;
    uint16_t n1;
    uint16_t n2;
    uint16_t n3;
    uint16_t h0;
    uint16_t h1;
    uint16_t h2;
    uint16_t h3;
} __wasi_addr_ip6_t;

typedef struct __wasi_addr_ip6_port_t {
    __wasi_addr_ip6_t addr;
    __wasi_ip_port_t port; /* host byte order */
} __wasi_addr_ip6_port_t;

typedef struct __wasi_addr_t {
    __wasi_addr_type_t kind;
    union {
        __wasi_addr_ip4_port_t ip4;
        __wasi_addr_ip6_port_t ip6;
    } addr;
} __wasi_addr_t;

typedef enum { INET4 = 0, INET6 } __wasi_address_family_t;

typedef struct __wasi_addr_info_t {
    __wasi_addr_t addr;
    __wasi_sock_type_t type;
} __wasi_addr_info_t;

typedef struct __wasi_addr_info_hints_t {
    __wasi_sock_type_t type;
    __wasi_address_family_t family;
    // this is to workaround lack of optional parameters
    uint8_t hints_enabled;
} __wasi_addr_info_hints_t;

#ifdef __wasi__
/**
 * Reimplement below POSIX APIs with __wasi_sock_XXX functions.
 *
 * Keep sync with
 * <sys/socket.h>
 * <sys/types.h>
 */

#define SO_RCVTIMEO 20
#define SO_SNDTIMEO 21

struct addrinfo {
    int ai_flags;             /* Input flags.  */
    int ai_family;            /* Protocol family for socket.  */
    int ai_socktype;          /* Socket type.  */
    int ai_protocol;          /* Protocol for socket.  */
    socklen_t ai_addrlen;     /* Length of socket address.  */
    struct sockaddr *ai_addr; /* Socket address for socket.  */
    char *ai_canonname;       /* Canonical name for service location.  */
    struct addrinfo *ai_next; /* Pointer to next in list.  */
};

int
accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int
bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int
connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int
listen(int sockfd, int backlog);

ssize_t
recvmsg(int sockfd, struct msghdr *msg, int flags);

ssize_t
sendmsg(int sockfd, const struct msghdr *msg, int flags);

int
socket(int domain, int type, int protocol);

int
getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int
getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int
getsockopt(int sockfd, int level, int optname, void *__restrict optval,
           socklen_t *__restrict optlen);

int
setsockopt(int sockfd, int level, int optname, const void *optval,
           socklen_t optlen);

int
getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,
            struct addrinfo **res);

void
freeaddrinfo(struct addrinfo *res);
#endif

/**
 * Accept a connection on a socket
 * Note: This is similar to `accept`
 */
int32_t
__imported_wasi_snapshot_preview1_sock_accept(int32_t arg0, int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_accept")));

static inline __wasi_errno_t
__wasi_sock_accept(__wasi_fd_t fd, __wasi_fd_t *fd_new)
{
    return (__wasi_errno_t)__imported_wasi_snapshot_preview1_sock_accept(
        (int32_t)fd, (int32_t)fd_new);
}

/**
 * Returns the local address to which the socket is bound.
 *
 * Note: This is similar to `getsockname` in POSIX
 *
 * When successful, the contents of the output buffer consist of an IP address,
 * either IP4 or IP6.
 */
int32_t
__imported_wasi_snapshot_preview1_sock_addr_local(int32_t arg0, int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_addr_local")));

static inline __wasi_errno_t
__wasi_sock_addr_local(__wasi_fd_t fd, __wasi_addr_t *addr)
{
    return (__wasi_errno_t)__imported_wasi_snapshot_preview1_sock_addr_local(
        (int32_t)fd, (int32_t)addr);
}

/**
 * Returns the remote address to which the socket is connected to.
 *
 * Note: This is similar to `getpeername` in POSIX
 *
 * When successful, the contents of the output buffer consist of an IP address,
 * either IP4 or IP6.
 */
int32_t
__imported_wasi_snapshot_preview1_sock_addr_remote(int32_t arg0, int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_addr_remote")));

static inline __wasi_errno_t
__wasi_sock_addr_remote(__wasi_fd_t fd, __wasi_addr_t *addr)
{
    return (__wasi_errno_t)__imported_wasi_snapshot_preview1_sock_addr_remote(
        (int32_t)fd, (int32_t)addr);
}

/**
 * Resolve a hostname and a service to one or more IP addresses. Service is
 * optional and you can pass empty string in most cases, it is used as a hint
 * for protocol.
 *
 * Note: This is similar to `getaddrinfo` in POSIX
 *
 * When successful, the contents of the output buffer consist of a sequence of
 * IPv4 and/or IPv6 addresses. Each address entry consists of a wasi_addr_t
 * object.
 *
 * This function fills the output buffer as much as possible, truncating the
 * entries that didn't fit into the buffer. A number of available addresses
 * will be returned through the last parameter.
 */
int32_t
__imported_wasi_snapshot_preview1_sock_addr_resolve(int32_t arg0, int32_t arg1,
                                                    int32_t arg2, int32_t arg3,
                                                    int32_t arg4, int32_t arg5)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_addr_resolve")));

static inline __wasi_errno_t
__wasi_sock_addr_resolve(const char *host, const char *service,
                         __wasi_addr_info_hints_t *hints,
                         __wasi_addr_info_t *addr_info,
                         __wasi_size_t addr_info_size,
                         __wasi_size_t *max_info_size)
{
    return (__wasi_errno_t)__imported_wasi_snapshot_preview1_sock_addr_resolve(
        (int32_t)host, (int32_t)service, (int32_t)hints, (int32_t)addr_info,
        (int32_t)addr_info_size, (int32_t)max_info_size);
}

/**
 * Bind a socket
 * Note: This is similar to `bind` in POSIX using PF_INET
 */
int32_t
__imported_wasi_snapshot_preview1_sock_bind(int32_t arg0, int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_bind")));

static inline __wasi_errno_t
__wasi_sock_bind(__wasi_fd_t fd, __wasi_addr_t *addr)
{
    return (__wasi_errno_t)__imported_wasi_snapshot_preview1_sock_bind(
        (int32_t)fd, (int32_t)addr);
}

/**
 * Close a socket (this is an alias for `fd_close`)
 * Note: This is similar to `close` in POSIX.
 */
int32_t
__imported_wasi_snapshot_preview1_sock_close(int32_t arg0)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_close")));

static inline __wasi_errno_t
__wasi_sock_close(__wasi_fd_t fd)
{
    return (__wasi_errno_t)__imported_wasi_snapshot_preview1_sock_close(
        (int32_t)fd);
}

/**
 * Initiate a connection on a socket to the specified address
 * Note: This is similar to `connect` in POSIX
 */

int32_t
__imported_wasi_snapshot_preview1_sock_connect(int32_t arg0, int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_connect")));

static inline __wasi_errno_t
__wasi_sock_connect(__wasi_fd_t fd, __wasi_addr_t *addr)
{
    return (__wasi_errno_t)__imported_wasi_snapshot_preview1_sock_connect(
        (int32_t)fd, (int32_t)addr);
}
/**
 * Retrieve the size of the receive buffer
 * Note: This is similar to `getsockopt` in POSIX for SO_RCVBUF
 */

int32_t
__imported_wasi_snapshot_preview1_sock_get_recv_buf_size(int32_t arg0,
                                                         int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_get_recv_buf_size")));

static inline __wasi_errno_t
__wasi_sock_get_recv_buf_size(__wasi_fd_t fd, __wasi_size_t *size)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_get_recv_buf_size((int32_t)fd,
                                                                 (int32_t)size);
}
/**
 * Retrieve status of address reuse on a socket
 * Note: This is similar to `getsockopt` in POSIX for SO_REUSEADDR
 */
int32_t
__imported_wasi_snapshot_preview1_sock_get_reuse_addr(int32_t arg0,
                                                      int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_get_reuse_addr")));

static inline __wasi_errno_t
__wasi_sock_get_reuse_addr(__wasi_fd_t fd, uint8_t *reuse)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_get_reuse_addr((int32_t)fd,
                                                              (int32_t)reuse);
}

/**
 * Retrieve status of port reuse on a socket
 * Note: This is similar to `getsockopt` in POSIX for SO_REUSEPORT
 */
int32_t
__imported_wasi_snapshot_preview1_sock_get_reuse_port(int32_t arg0,
                                                      int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_get_reuse_port")));

static inline __wasi_errno_t
__wasi_sock_get_reuse_port(__wasi_fd_t fd, int8_t *reuse)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_get_reuse_port((int32_t)fd,
                                                              (int32_t)reuse);
}

/**
 * Retrieve the size of the send buffer
 * Note: This is similar to `getsockopt` in POSIX for SO_SNDBUF
 */
int32_t
__imported_wasi_snapshot_preview1_sock_get_send_buf_size(int32_t arg0,
                                                         int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_get_send_buf_size")));

static inline __wasi_errno_t
__wasi_sock_get_send_buf_size(__wasi_fd_t fd, __wasi_size_t *size)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_get_send_buf_size((int32_t)fd,
                                                                 (int32_t)size);
}

/**
 * Listen for connections on a socket
 * Note: This is similar to `listen`
 */
int32_t
__imported_wasi_snapshot_preview1_sock_listen(int32_t arg0, int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_listen")));

static inline __wasi_errno_t
__wasi_sock_listen(__wasi_fd_t fd, __wasi_size_t backlog)
{
    return (__wasi_errno_t)__imported_wasi_snapshot_preview1_sock_listen(
        (int32_t)fd, (int32_t)backlog);
}

/**
 * Open a socket

 * The first argument to this function is a handle to an
 * address pool. The address pool determines what actions can
 * be performed and at which addresses they can be performed to.

 * The address pool cannot be re-assigned. You will need to close
 * the socket and open a new one to use a different address pool.

 * Note: This is similar to `socket` in POSIX using PF_INET
 */

int32_t
__imported_wasi_snapshot_preview1_sock_open(int32_t arg0, int32_t arg1,
                                            int32_t arg2, int32_t arg3)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_open")));

static inline __wasi_errno_t
__wasi_sock_open(__wasi_fd_t fd, __wasi_address_family_t af,
                 __wasi_sock_type_t socktype, __wasi_fd_t *sockfd)
{
    return (__wasi_errno_t)__imported_wasi_snapshot_preview1_sock_open(
        (int32_t)fd, (int32_t)af, (int32_t)socktype, (int32_t)sockfd);
}

/**
 * Set size of receive buffer
 * Note: This is similar to `setsockopt` in POSIX for SO_RCVBUF
 */
int32_t
__imported_wasi_snapshot_preview1_sock_set_recv_buf_size(int32_t arg0,
                                                         int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_set_recv_buf_size")));

static inline __wasi_errno_t
__wasi_sock_set_recv_buf_size(__wasi_fd_t fd, __wasi_size_t size)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_set_recv_buf_size((int32_t)fd,
                                                                 (int32_t)size);
}

/**
 * Enable/disable address reuse on a socket
 * Note: This is similar to `setsockopt` in POSIX for SO_REUSEADDR
 */
int32_t
__imported_wasi_snapshot_preview1_sock_set_reuse_addr(int32_t arg0,
                                                      int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_set_reuse_addr")));

static inline __wasi_errno_t
__wasi_sock_set_reuse_addr(__wasi_fd_t fd, uint8_t reuse)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_set_reuse_addr((int32_t)fd,
                                                              (int32_t)reuse);
}

/**
 * Enable port reuse on a socket
 * Note: This is similar to `setsockopt` in POSIX for SO_REUSEPORT
 */
int32_t
__imported_wasi_snapshot_preview1_sock_set_reuse_port(int32_t arg0,
                                                      int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_set_reuse_port")));

static inline __wasi_errno_t
__wasi_sock_set_reuse_port(__wasi_fd_t fd, uint8_t reuse)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_set_reuse_port((int32_t)fd,
                                                              (int32_t)reuse);
}

/**
 * Set size of send buffer
 * Note: This is similar to `setsockopt` in POSIX for SO_SNDBUF
 */
int32_t
__imported_wasi_snapshot_preview1_sock_set_send_buf_size(int32_t arg0)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_set_send_buf_size")));

static inline __wasi_errno_t
__wasi_sock_set_send_buf_size(__wasi_fd_t fd)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_set_send_buf_size((int32_t)fd);
}

int32_t
__imported_wasi_snapshot_preview1_sock_get_recv_timeout(int32_t arg0,
                                                        int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_get_recv_timeout")));

static inline __wasi_errno_t
__wasi_sock_get_recv_timeout(__wasi_fd_t fd, uint64_t *timeout_us)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_get_recv_timeout(
            (int32_t)fd, (int32_t)timeout_us);
}

int32_t
__imported_wasi_snapshot_preview1_sock_set_recv_timeout(int32_t arg0,
                                                        int64_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_set_recv_timeout")));

static inline __wasi_errno_t
__wasi_sock_set_recv_timeout(__wasi_fd_t fd, uint64_t timeout_us)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_set_recv_timeout(
            (int32_t)fd, (int64_t)timeout_us);
}

int32_t
__imported_wasi_snapshot_preview1_sock_get_send_timeout(int32_t arg0,
                                                        int32_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_get_send_timeout")));

static inline __wasi_errno_t
__wasi_sock_get_send_timeout(__wasi_fd_t fd, uint64_t *timeout_us)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_get_send_timeout(
            (int32_t)fd, (int32_t)timeout_us);
}

int32_t
__imported_wasi_snapshot_preview1_sock_set_send_timeout(int32_t arg0,
                                                        int64_t arg1)
    __attribute__((__import_module__("wasi_snapshot_preview1"),
                   __import_name__("sock_set_send_timeout")));

static inline __wasi_errno_t
__wasi_sock_set_send_timeout(__wasi_fd_t fd, uint64_t timeout_us)
{
    return (__wasi_errno_t)
        __imported_wasi_snapshot_preview1_sock_set_send_timeout(
            (int32_t)fd, (int64_t)timeout_us);
}

/**
 * TODO: modify recv() and send()
 * since don't want to re-compile the wasi-libc,
 * we tend to keep original implentations of recv() and send().
 */
#endif
