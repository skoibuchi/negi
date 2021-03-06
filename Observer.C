/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Observer.C,v 5.11 2012-05-01 09:14:49 sin Exp $
*/
#include "Observer.H"
#include "Global.H"

Observer::Observer(){
	SetDefault();
	//Print HTTP decoding info
	cout << "time(real)"<< "	" << "time(packet)" << "	" << "	" << "disorder_count" <<"	" << "stream_created_num" << "	"<< "stream_current_num" << endl;
}

Observer::~Observer(){
	return;
}

void Observer::SetDefault(){

		num_packet_created = 0;
		num_packet_deleted = 0;
		num_stream_created = 0;
		num_stream_deleted = 0;
		num_tcpconn_created = 0;
		num_tcpconn_deleted = 0;
		max_stream_size = 0;
		mpf_mem_size = 48+ 16*sim_rule_num + sim_rule_size; //ActiveRules + TempBuf + State + Stream

	return;
}

void Observer::Show(){
	cout << "----Observer result------------------" << endl;
	cout << "Packet created: " << num_packet_created << endl;
	cout << "Packet deleted: " << num_packet_deleted << endl;
	cout << "Packet memory mallocd(byte): " << num_packet_mallocd << endl;
	cout << "Packet memory freed(byte)  : " << num_packet_freed << endl;
	cout << "Packet memory INUSE  : " << num_packet_mem_inuse << endl;
	cout <<endl;
	cout << "Stream created: " << num_stream_created << endl;
	cout << "Stream deleted: " << num_stream_deleted << endl;
	cout << "Stream memory mallocd(byte): " << stream_mem_mallocd << endl;
	cout << "Stream memory freed(byte)  : " << stream_mem_freed << endl;
	cout << "Stream memory INUSE  : " << stream_mem_inuse << endl;

	cout <<endl;
	cout << "TcpConn created: " << num_tcpconn_created << endl;
	cout << "TcpConn deleted: " << num_tcpconn_deleted << endl;
	cout <<endl;
	cout << "----Observer END------------------" << endl;
}

void Observer::ShowMem(timeval timestamp){
	if(offset_time.tv_sec == 0 && offset_time.tv_usec == 0) offset_time = timestamp;

	unsigned long int stream_num = 0;
	unsigned long int sum_stream_size = 0;
	unsigned long int gzip_stream_num = 0;

	for(list<Stream *>::iterator it = stream_pool->GetStreamFirstIt(); it != stream_pool->GetStreamLastIt(); it++){
		stream_num++;
		sum_stream_size += (*it)->GetRetrievedContentSize();
		if((*it)->GetRetrievedContentSize() >= max_stream_size){
			max_stream_size =(*it)->GetRetrievedContentSize();
		}
		if((int)((*it)->GetHttpCompress())){
			gzip_stream_num++;
		}
	}
	if(stream_num * 5 + gzip_stream_num * 64 > max_stream_num) max_stream_num = stream_num*5+gzip_stream_num*64;
	if(sum_stream_size > max_sum_stream_size) max_sum_stream_size = sum_stream_size;
	cout << timestamp.tv_sec - offset_time.tv_sec << "	" << std::setw(15) << num_stream_disorderd << "	" << std::setw(15) << num_stream_created << "	" << std::setw(15) << num_stream_created - num_stream_deleted  << endl;

}

void Observer::PacketCreated(){
	num_packet_created++;
}

void Observer::PacketDeleted(){
	num_packet_deleted++;
}

void Observer::StreamCreated(){
	num_stream_created++;
	mpf_mem_inuse += mpf_mem_size;
}

void Observer::StreamDeleted(){
	num_stream_deleted++;
	mpf_mem_inuse -= mpf_mem_size;
}

void Observer::StreamDisorderd(){
	num_stream_disorderd++;
}

void Observer::StreamDisorderRecoverd(){
	num_stream_disorderd--;
}

void Observer::TcpConnCreated(){
	num_tcpconn_created++;
}

void Observer::TcpConnDeleted(){
	num_tcpconn_deleted++;
}
	
void Observer::PacketMallocd(size_t size){
	num_packet_mallocd += size;
	num_packet_mem_inuse += size;
}

void Observer::PacketFreed(size_t size){
	num_packet_freed += size;
	num_packet_mem_inuse -= size;
}

void Observer::StreamMallocd(size_t size){
	stream_mem_mallocd += size;
	stream_mem_inuse += size;
}

void Observer::SimPacketMallocd(size_t size){
	sim_stream_mode_mem_inuse += size;
}

void Observer::StreamFreed(size_t size, size_t streampktsize){
	stream_mem_freed += size;
	stream_mem_inuse -= size;
	
	sim_stream_mode_mem_inuse -= streampktsize;

}

void Observer::MPFActiveRuleMallocd(size_t size){
	mpf_activerule_mallocd += size;
	mpf_activerule_mem_inuse += size;
}

void Observer::MPFActiveRuleDeleted(size_t size){
	mpf_activerule_freed += size;
	mpf_activerule_mem_inuse -= size;
}

void Observer::MPFTempBufMallocd(size_t size){
	mpf_tempbuf_mallocd += size;
	mpf_tempbuf_mem_inuse += size;
}
	
void Observer::MPFTempBufDeleted(size_t size){
	mpf_tempbuf_freed += size;
	mpf_tempbuf_mem_inuse -= size;
}
void Observer::MPFStateMallocd(size_t size){mpf_state_mallocd += size; mpf_state_mem_inuse += size;};
void Observer::MPFStateDeleted(size_t size){mpf_state_freed += size; mpf_state_mem_inuse -= size;};
