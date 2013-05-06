/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Global.C,v 5.15 2012-05-19 15:01:30 sin Exp $
*/

#include "Global.H"

unsigned int stream_buf_size;
unsigned int emergency_save_all;
unsigned int no_stream_save;
unsigned int no_stream_org_save;
unsigned int no_regexp_hit_save;

pid_t pid[P_MAX];

time_t gc_remove_time;
time_t now_time;

unsigned int sim_rule_num;
unsigned int sim_rule_size;
unsigned int stream_id_prev;

unsigned long int max_stream_num;
unsigned long int max_stream_size;
unsigned long int max_sum_stream_size;

pthread_mutex_t pkt_queue_mutex;
pthread_mutex_t stream0_queue_mutex;
pthread_mutex_t stream1_queue_mutex;
pthread_mutex_t stream2_queue_mutex;
pthread_mutex_t stream3_queue_mutex;

queue<Packet *> pkt_queue;
queue<Packet *> stream0_queue;
queue<Packet *> stream1_queue;
queue<Packet *> stream2_queue;
queue<Packet *> stream3_queue;

pthread_t thread_cap;
pthread_t thread_stream0;
pthread_t thread_stream1;
pthread_t thread_stream2;
pthread_t thread_stream3;

list<Stream *> end_stream_list;

pcap_t *pcap_descriptor;

//map<string,string> sql_config;

SaveMode savemode;
MallocMode mallocmode;

Config *config;
PacketPool *packet_pool;
RulePool *rule_pool;
StreamRebuild *stream_rebuild;
IpFilter *ip_filter;
HttpDecoder *http_decoder;
MatchPreFilter *match_pre_filter;
Extractor *extractor;
StreamPool *stream_pool;
#ifdef USE_POSTGRES
Pgsql *pgsql;
PgsqlSaver *pgsql_saver;
RuleLoaderPgsql *rule_loader_pgsql;
#else
RuleLoaderFile *rule_loader_file;
#endif
Master *master;
TcpConnPool *tcp_conn_pool;
PacketClean *packet_clean;
Observer *observer;
Gzip *gzip;
FileWriter *file_writer;

ResultPool *result_pool;

//RiverGate *river_gate;

pthread_mutex_t sss_cond_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sss_cond =  PTHREAD_COND_INITIALIZER;
Packet* sss_send_value;
