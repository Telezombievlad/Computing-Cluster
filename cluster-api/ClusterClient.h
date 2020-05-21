// No Copyright. Vladislav Aleinik 2020
//======================================
// Computing Cluster Client
//======================================
// - Connects to computation cluster
// - Performs computations
// - Sends results back
//======================================
#ifndef COMPUTING_CLUSTER_CLIENT_HPP_INCLUDED
#define COMPUTING_CLUSTER_CLIENT_HPP_INCLUDED

//----------------
// Enable Logging
//----------------

#include "Logging.h"

//-------------
// Client Data
//-------------

#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/sysinfo.h>
#include <sys/eventfd.h>

// Bool:
typedef char bool;

struct Connection
{
	int  socket_fd;

	// Recv buffer:
	char*  recv_buffer;
	size_t bytes_recieved;
};

struct ComputeInfo
{
	void* data_pack;
	void* ret_pack;
	void* (*thread_func)(void*);
};

struct ThreadInfo
{
	bool empty;

	struct ComputeInfo in_args;
	
	pthread_t thread_id;
	cpu_set_t cpu;
	uint32_t  num_of_task;

    int  event_fd;
	bool ready;
};

enum Errors
{
    E_ERROR = -1,
    E_CACHE_INFO = -2,
    E_BADARGS = -3,
};

struct ClusterClientHandle
{
	// Eventloop:
	int epoll_fd;
	pthread_t eventloop_thr_id;
	
	// Server discovery:
	const char* server_hostname;
	bool local_discovery;

	int discovery_socket_fd;
	int discovery_timer_fd;

	struct sockaddr_in server_addr;
	
	// Connection management:
	struct Connection server_conn;

	// Task management:
	size_t requests_to_send;
	size_t in_process;
	
	struct ThreadInfo* thread_manager;
	size_t max_threads;
	
	void*  task_buffer;
	size_t task_size;
	
	void*  ret_buffer;
	size_t ret_size;

	// Task computation:
	int max_cpu;
    int cache_line_size;
	void* (*thread_func)(void*);
};

//-------------
// Computation
//-------------

void client_compute(size_t num_threads, size_t task_size, size_t ret_size, const char* master_host, void* (*thread_func)(void*));

#endif // COMPUTING_CLUSTER_CLIENT_HPP_INCLUDED
