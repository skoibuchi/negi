/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Master.C,v 5.41 2012-08-22 08:42:52 sin Exp $
*/
#include "Master.H"
#include "Global.H"
#include "SSS.H"

Master::Master(){

	observer_packet_counter = 0;
	all_packet_counter = 0;

	max_stream_size = 0;
	start_time = time(NULL);
	
	return;
}

Master::~Master(){
	return;
}
int pkt_cnt = 0;
void Master::Proc(Packet *pkt){
//	pkt_cnt++;
//	cout << pkt_cnt << endl;
//	pkt->Show();
	unsigned long int stream_id = 0;
	Stream * stream;
	MASTER_DEBUG(MSG("Master Proc!"));
	stream_rebuild->Proc(pkt);
	pkt->Show();
	cout << "before get stream_id"  << endl;

	if(pkt->GetStream() == NULL) return;
	stream_id = pkt->GetStream()->GetStreamId();
	cout << "stream_id" << stream_id << endl;
	stream_id = stream_id % 2;
	cout << "stream_id" << stream_id << endl;
	switch(stream_id){
		case 0:
			pthread_mutex_lock(&stream0_queue_mutex);
			stream0_queue.push(pkt);
			pthread_mutex_unlock(&stream0_queue_mutex);
		case 1:
			pthread_mutex_lock(&stream1_queue_mutex);
			stream1_queue.push(pkt);
			pthread_mutex_unlock(&stream1_queue_mutex);
			/*
		case 2:
			pthread_mutex_lock(&stream2_queue_mutex);
			stream2_queue.push(pkt);
			pthread_mutex_unlock(&stream2_queue_mutex);
		case 3:
			pthread_mutex_lock(&stream3_queue_mutex);
			stream3_queue.push(pkt);
			pthread_mutex_unlock(&stream3_queue_mutex);
			*/
	}

/*
	if(pkt->GetError() == 0){
		//ip_filter.Proc(pkt);
	MASTER_DEBUG(MSG("http_decode"));
		http_decoder->Proc(pkt);

	MASTER_DEBUG(MSG("match_pre"));
		match_pre_filter->Proc(pkt);

	MASTER_DEBUG(MSG("extract"));
		extractor->Proc(pkt);
	}

	OBSERVER_DEBUG(observer->ShowMem(pkt->GetTimestamp());); 

	static time_t prev_time;
	static time_t now_time;

	if(observer_packet_counter > 1000){
		now_time = time(NULL);
		if(now_time > prev_time){
			cout << now_time - start_time << "	" ;
			observer->ShowMem(pkt->GetTimestamp()); 
	//		MSG("Packet Count: " << all_packet_counter);
			observer_packet_counter = 0;
			prev_time = now_time;
		}
	}else{
		observer_packet_counter++;
//		all_packet_counter++;
	}
//	river_gate->ProcPacket(pkt);
	
	*/

/*
	if(pkt->GetProtocol() == IPPROTO_TCP && !(pkt->GetAck()) && (pkt->GetSyn() || pkt->GetFin())){
		pthread_mutex_lock(&sss_cond_mut);
		sss_send_value = new Packet(pkt->GetPacketContainer());
		pthread_cond_signal(&sss_cond);
		pthread_mutex_unlock(&sss_cond_mut);
	//	pkt->Show();
	}
*/

/*

	if(!end_stream_list.empty()){
		for(list<Stream *>::iterator it=end_stream_list.begin(); it != end_stream_list.end(); it++){
			if((*it)->GetSaveFlag()){
#ifdef USE_POSTGRES
				pgsql_saver->Proc(*it);
#endif
			}else if(atoi(config->get("save_all").c_str())){
#ifdef USE_POSTGRES
				pgsql_saver->Proc(*it);
#endif
			}

		}
	}
//	pgsql_saver.ProcPacket(pkt);
	packet_clean->Proc(pkt);
//	MASTER_DEBUG(observer.Show());

*/

} //master->proc


void* MasterProc0(void* arg){
	Packet* pkt;
	while(1){
		if(!stream0_queue.empty()){
			pkt = stream0_queue.front();
			pthread_mutex_lock(&stream0_queue_mutex);
			stream0_queue.pop();
			pthread_mutex_unlock(&stream0_queue_mutex);

			if(pkt->GetError() == 0){
				//ip_filter.Proc(pkt);
			MASTER_DEBUG(MSG("http_decode"));
				http_decoder->Proc(pkt);

			MASTER_DEBUG(MSG("match_pre"));
				match_pre_filter->Proc(pkt);

			MASTER_DEBUG(MSG("extract"));
				extractor->Proc(pkt);
			}


			if(!end_stream_list.empty()){
				for(list<Stream *>::iterator it=end_stream_list.begin(); it != end_stream_list.end(); it++){
					if((*it)->GetSaveFlag()){
		#ifdef USE_POSTGRES
						pgsql_saver->Proc(*it);
		#endif
					}else if(atoi(config->get("save_all").c_str())){
		#ifdef USE_POSTGRES
						pgsql_saver->Proc(*it);
		#endif
					}
				}
			}
		//	pgsql_saver.ProcPacket(pkt);
			packet_clean->Proc(pkt);
		//	MASTER_DEBUG(observer.Show());
		}
	}
}


void* MasterProc1(void* arg){
	Packet* pkt;
	while(1){
		if(!stream1_queue.empty()){
			pkt = stream1_queue.front();
			pthread_mutex_lock(&stream1_queue_mutex);
			stream1_queue.pop();
			pthread_mutex_unlock(&stream1_queue_mutex);

			if(pkt->GetError() == 0){
				//ip_filter.Proc(pkt);
			MASTER_DEBUG(MSG("http_decode"));
				http_decoder->Proc(pkt);

			MASTER_DEBUG(MSG("match_pre"));
				match_pre_filter->Proc(pkt);

			MASTER_DEBUG(MSG("extract"));
				extractor->Proc(pkt);
			}

			OBSERVER_DEBUG(observer->ShowMem(pkt->GetTimestamp());); 

			static time_t prev_time;
			static time_t now_time;

			if(!end_stream_list.empty()){
				for(list<Stream *>::iterator it=end_stream_list.begin(); it != end_stream_list.end(); it++){
					if((*it)->GetSaveFlag()){
		#ifdef USE_POSTGRES
						pgsql_saver->Proc(*it);
		#endif
					}else if(atoi(config->get("save_all").c_str())){
		#ifdef USE_POSTGRES
						pgsql_saver->Proc(*it);
		#endif
					}

				}
			}
		//	pgsql_saver.ProcPacket(pkt);
			packet_clean->Proc(pkt);
		//	MASTER_DEBUG(observer.Show());
		}
	}
}


