/*
 * Copyright (c) 2009 The Trustees of Indiana University and Indiana
 *                    University Research and Technology
 *                    Corporation.  All rights reserved.
 *
 * Author(s): Torsten Hoefler <htor@cs.indiana.edu>
 *            Timo Schneider <timoschn@cs.indiana.edu>
 *
 */


#define STRICT_ORDER // this is needed to keep order between Send/Recv and LocalOps in NBC case :-/
#define LIST_MATCH // enables debugging the queues (check if empty)
#define HOSTSYNC // this is experimental to count synchronization times induced by message transmissions

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <algorithm>

#ifndef LIST_MATCH
#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#endif

#include <queue>
#include "cmdline.h"

#include <sys/time.h>

#include "LogGOPSim.hpp"
#include "Network.hpp"
#include "TimelineVisualization.hpp"
#include "Noise.hpp"
#include "Parser.hpp"
#include "cmdline.h"

#ifndef LIST_MATCH
namespace std
{
   using namespace __gnu_cxx;
}
#endif

static bool print=true;

gengetopt_args_info args_info;

// TODO: should go to .hpp file

typedef struct {
  // TODO: src and tag can go in case of hashmap matching
  btime_t starttime; // only for visualization
  uint32_t size, src, tag, offset;
} ruqelem_t;

typedef unsigned int uint;
typedef unsigned long int ulint;
typedef unsigned long long int ullint;

#ifdef LIST_MATCH
// TODO this is really slow - reconsider design of rq and uq!
// matches and removes element from list if found, otherwise returns
// false
typedef std::list<ruqelem_t> ruq_t;
static inline bool match(const graph_node_properties &elem, ruq_t *q, ruqelem_t *retelem=NULL) {
  
  //std::cout << "UQ size " << q->size() << "\n";

  if(print) printf("++ [%i] searching matching queue for src %i tag %i\n", elem.host, elem.target, elem.tag);
  for(ruq_t::iterator iter=q->begin(); iter!=q->end(); ++iter) {
    if(elem.target == ANY_SOURCE || iter->src == ANY_SOURCE || iter->src == elem.target) {
      if(elem.tag == ANY_TAG || iter->tag == ANY_TAG || iter->tag == elem.tag) {
        if(retelem) *retelem=*iter;
        q->erase(iter);
        return true;        
      }
    }
  }
  return false;
}
#else
class myhash { // I WANT LAMBDAS! :)
  public:
  size_t operator()(const std::pair<int,int>& x) const {
    return (x.first>>16)+x.second;
  }
};
typedef std::hash_map< std::pair</*tag*/int,int/*src*/>, std::queue<ruqelem_t>, myhash > ruq_t;
static inline bool match(const graph_node_properties &elem, ruq_t *q, ruqelem_t *retelem=NULL) {
  
  if(print) printf("++ [%i] searching matching queue for src %i tag %i\n", elem.host, elem.target, elem.tag);

  ruq_t::iterator iter = q->find(std::make_pair(elem.tag, elem.target));
  if(iter == q->end()) {
    return false;
  }
  std::queue<ruqelem_t> *tq=&iter->second;
  if(tq->empty()) {
    return false;
  }
  if(retelem) *retelem=tq->front();
  tq->pop();
  return true;
}
#endif

void _simulation(
  const int o,
  const int O,
  const int g,
  const int L,
  const int G,
  const int S,
  char *filename,
  uint64_t *init_bias,
  uint64_t *comm_time,
  int num_of_rank)
{
  print = false;
  int argc = 1;
  char tmp_str[] = "LogGOPSim";
  char *argv[1];
  argv[0] = tmp_str;

	if (cmdline_parser(argc, argv, &args_info) != 0) {
    fprintf(stderr, "Couldn't parse command line arguments!\n");
    throw(10);
	}

  #ifdef STRICT_ORDER
    btime_t aqtime=0; 
  #endif

  // read input parameters
  Parser parser(filename, 0);
  Network net(&args_info);

  const uint p = parser.schedules.size();
  const int ncpus = parser.GetNumCPU();
  const int nnics = parser.GetNumNIC();

  if (p != num_of_rank) {
    printf("error: The number of process is different between SimBOT and LogGOPSim\n");
    exit(1);
  }

  if(print) printf("size: %i (%i CPUs, %i NICs); L=%i, o=%i g=%i, G=%i, O=%i, P=%i, S=%u\n", p, ncpus, nnics, L, o, g, G, O, p, S);

  TimelineVisualization tlviz(&args_info, p);
  Noise osnoise(&args_info, p);

  // the active queue 
  std::priority_queue<graph_node_properties,std::vector<graph_node_properties>,aqcompare_func> aq;
  // the queues for each host 
  std::vector<ruq_t> rq(p), uq(p); // receive queue, unexpected queue
  // next available time for o, g(receive) and g(send)
  std::vector<std::vector<btime_t> > nexto(p), nextgr(p), nextgs(p); 
#ifdef HOSTSYNC
  std::vector<btime_t> hostsync(p);
#endif

  // initialize o and g for all PEs and hosts
  for(uint i=0; i<p; ++i) {
    nexto[i].resize(ncpus);
    std::fill(nexto[i].begin(), nexto[i].end(), 0);
    nextgr[i].resize(nnics);
    std::fill(nextgr[i].begin(), nextgr[i].end(), 0);
    nextgs[i].resize(nnics);
    std::fill(nextgs[i].begin(), nextgs[i].end(), 0);
  }

	struct timeval tstart, tend;
	gettimeofday(&tstart, NULL);

  int host=0; 
  uint64_t num_events=0;
  /*for(Parser::schedules_t::iterator sched=parser.schedules.begin(); sched!=parser.schedules.end(); ++sched, ++host) {
    // initialize free operations (only once per run!)
    //sched->init_free_operations();

    // retrieve all free operations
    //typedef std::vector<std::string> freeops_t;
    //freeops_t free_ops = sched->get_free_operations();
    SerializedGraph::nodelist_t free_ops;
    sched->GetExecutableNodes(&free_ops);
    // ensure that the free ops are ordered by type
    std::sort(free_ops.begin(), free_ops.end(), gnp_op_comp_func());

    num_events += sched->GetNumNodes();

    // walk all new free operations and throw them in the queue 
    for(SerializedGraph::nodelist_t::iterator freeop=free_ops.begin(); freeop != free_ops.end(); ++freeop) {
      //if(print) std::cout << *freeop << " " ;

      freeop->host = host;
      freeop->time = 0;
#ifdef STRICT_ORDER
      freeop->ts=aqtime++;
#endif

      switch(freeop->type) {
        case OP_LOCOP:
          if(print) printf("init %i (%i,%i) loclop: %lu\n", host, freeop->proc, freeop->nic, (long unsigned int) freeop->size);
          break;
        case OP_SEND:
          if(print) printf("init %i (%i,%i) send to: %i, tag: %i, size: %lu\n", host, freeop->proc, freeop->nic, freeop->target, freeop->tag, (long unsigned int) freeop->size);
          break;
        case OP_RECV:
          if(print) printf("init %i (%i,%i) recvs from: %i, tag: %i, size: %lu\n", host, freeop->proc, freeop->nic, freeop->target, freeop->tag, (long unsigned int) freeop->size);
          break;
        default:
          printf("not implemented!\n");
      }

      aq.push(*freeop);
      //std::cout << "AQ size after push: " << aq.size() << "\n";
    }
  }*/

  // add init bias by calc op
  for (int i = 0; i < p; ++i) {
    graph_node_properties calc_op;
    calc_op.time = 0;
    calc_op.host = i;   // rank
    calc_op.proc = 0;   // cpu
    calc_op.nic = 0;
    calc_op.type = OP_LOCOP;
    calc_op.offset = -1;
    calc_op.size = init_bias[i];
    aq.push(calc_op);
  }

  bool new_events=true;
  uint lastperc=0;
  while(!aq.empty() || new_events) {
    graph_node_properties elem = aq.top();
    aq.pop();

    // the lists of hosts that actually finished someting -- a host is
    // added whenever an irequires or requires is satisfied
    std::vector<int> check_hosts;

    // the BIG switch on element type that we just found 
    switch(elem.type) {
      case OP_LOCOP: {  // a calc op, calc time = size
        if(print) printf("[%i] found loclop of length %lu - t: %lu (CPU: %i)\n", elem.host, (ulint)elem.size, (ulint)elem.time, elem.proc);
        if(nexto[elem.host][elem.proc] <= elem.time) {
          // check if OS Noise occurred
          btime_t noise = osnoise.get_noise(elem.host, elem.time, elem.time + elem.size);
          nexto[elem.host][elem.proc] = elem.time + elem.size + noise;

          if (elem.offset >= 0) {
            parser.schedules[elem.host].MarkNodeAsStarted(elem.offset);
            parser.schedules[elem.host].MarkNodeAsDone(elem.offset);
          }
          check_hosts.push_back(elem.host);
          // add to timeline
          tlviz.add_loclop(elem.host, elem.time, elem.time + elem.size, elem.proc);
          if(print) printf("[%i] offset:%d\n", elem.host, elem.offset);
          if(print) printf("[%i] start loclop of length %lu - t: %lu (CPU: %i)\n", elem.host, (ulint)elem.size, (ulint)elem.time, elem.proc);
        } else {
          if(print) printf("-- locop local o not available -- reinserting\n");
          elem.time = nexto[elem.host][elem.proc];
          aq.push(elem);
        } } break;

      case OP_SEND: { // a send op                       
        if(print) printf("[%i] found send to %i - t: %lu (CPU: %i)\n", elem.host, elem.target, (ulint)elem.time, elem.proc);
        if(std::max(nexto[elem.host][elem.proc], nextgs[elem.host][elem.nic]) <= elem.time) { // local o,g available!
          if(print) printf("-- satisfy local irequires\n");
          parser.schedules[elem.host].MarkNodeAsStarted(elem.offset);
          check_hosts.push_back(elem.host);

          // check if OS Noise occurred
          btime_t noise = osnoise.get_noise(elem.host, elem.time, elem.time + o);
          nexto[elem.host][elem.proc] = elem.time + o + (elem.size - 1) * O+ noise;
          nextgs[elem.host][elem.nic] = elem.time + g + (elem.size - 1) * G; // TODO: G should be charged in network layer only

          // add to timeline
          tlviz.add_osend(elem.host, elem.time, elem.time+o+ (elem.size-1)*O, elem.proc);
          tlviz.add_noise(elem.host, elem.time+o+ (elem.size-1)*O, elem.time + o + (elem.size-1)*O+ noise, elem.proc);

          // insert packet into network layer
          net.insert(elem.time, elem.host, elem.target, elem.size, &elem.handle);

          elem.type = OP_MSG;
          int h = elem.host;
          elem.host = elem.target;
          elem.target = h;
          elem.starttime = elem.time;
          elem.time = elem.time + o + L; // arrival time of first byte only (G will be charged at receiver ... +(elem.size-1)*G; // arrival time
#ifdef STRICT_ORDER
          num_events++; // MSG is a new event
          elem.ts = aqtime++; // new element in queue 
#endif
          if(print) printf("-- [%i] send inserting msg to %i, t: %lu\n", h, elem.host, (ulint)elem.time);
          aq.push(elem);

          if(elem.size <= S) { // eager message
            if(print) printf("-- [%i] eager -- satisfy local requires at t: %lu\n", h, (long unsigned int) elem.starttime+o);
            // satisfy local requires (host and target swapped!) 
            parser.schedules[h].MarkNodeAsDone(elem.offset);
            //tlviz.add_transmission(elem.target, elem.host, elem.starttime+o, elem.time-o, elem.size, G);
          } else {
            if(print) printf("-- [%i] start rendezvous message to: %i (offset: %i)\n", h, elem.host, elem.offset);
          }

        } else { // local o,g unavailable - retry later
          if(print) printf("-- send local o,g not available -- reinserting\n");
          elem.time = std::max(nexto[elem.host][elem.proc],nextgs[elem.host][elem.nic]);
          aq.push(elem);
        }

        } break;
      case OP_RECV: {
        if(print) printf("[%i] found recv from %i - t: %lu (CPU: %i)\n", elem.host, elem.target, (ulint)elem.time, elem.proc);

        parser.schedules[elem.host].MarkNodeAsStarted(elem.offset);
        check_hosts.push_back(elem.host);
        if(print) printf("-- satisfy local irequires\n");
        
        ruqelem_t matched_elem; 
        if(match(elem, &uq[elem.host], &matched_elem))  { // found it in local UQ 
          if(print) printf("-- found in local UQ\n");
          if(elem.size > S) { // rendezvous - free remote request
            // satisfy remote requires 
            parser.schedules[matched_elem.src].MarkNodeAsDone(matched_elem.offset); // this must be the offset of the remote packet!
            check_hosts.push_back(matched_elem.src);

            // TODO: uh, this is ugly ... we actually need to set the
            // time of the remote freed elements to elem.time now :-/
            // however, there is no simple way to do this :-(
            // but since we just got the elem with elem.time from the
            // queue, we can safely set the remote clocks to elem.time
            // (there is no event < elem.time in the queue) this
            // manipulation is dangerous, think before you change
            // anything!
            // TODO: do we need to add some latency for the ACK to
            // travel??? -- this can *not* be added here safely!
            if(nexto[matched_elem.src][elem.proc] < elem.time) nexto[matched_elem.src][elem.proc] = elem.time;
            if(nextgs[matched_elem.src][elem.nic] < elem.time) nextgs[matched_elem.src][elem.nic] = elem.time;
            
            if(print) printf("-- satisfy remote requires at host %i (offset: %i)\n", elem.target, elem.offset);
            tlviz.add_transmission(elem.host, matched_elem.src, matched_elem.starttime+o, elem.time, elem.size, G);
          }
          tlviz.add_orecv(elem.host, elem.time, elem.time+o, elem.proc);

          // satisfy local requires
          parser.schedules[elem.host].MarkNodeAsDone(elem.offset);
          check_hosts.push_back(elem.host);
          if(print) printf("-- satisfy local requires\n");
        } else { // not found in local UQ - add to RQ
          if(print) printf("-- not found in local UQ -- add to RQ\n");
          ruqelem_t nelem; 
          nelem.size = elem.size;
          nelem.src = elem.target;
          nelem.tag = elem.tag;
          nelem.offset = elem.offset;
#ifdef LIST_MATCH
          rq[elem.host].push_back(nelem);
#else
          rq[elem.host][std::make_pair(nelem.tag,nelem.src)].push(nelem);
#endif
        }
        } break;

      case OP_MSG: {
        if(print) printf("[%i] found msg from %i, t: %lu (CPU: %i)\n", elem.host, elem.target, (ulint)elem.time, elem.proc);
        uint64_t earliestfinish;
		    if((earliestfinish = net.query(elem.starttime, elem.time, elem.target, elem.host, elem.size, &elem.handle)) <= elem.time /* msg made it through network */ &&
           std::max(nexto[elem.host][elem.proc],nextgr[elem.host][elem.nic]) <= elem.time /* local o,g available! */) { 
          if(print) printf("-- msg o,g available (nexto: %lu, nextgr: %lu)\n", (long unsigned int) nexto[elem.host][elem.proc], (long unsigned int) nextgr[elem.host][elem.nic]);
          // check if OS Noise occurred
          btime_t noise = osnoise.get_noise(elem.host, elem.time, elem.time+o);
          nexto[elem.host][elem.proc] = elem.time+o+noise+std::max((elem.size-1)*O,(elem.size-1)*G) /* message is only received after G is charged !! TODO: consuming o seems a bit odd in the LogGP model but well in practice */;
          nextgr[elem.host][elem.nic] = elem.time+g+(elem.size-1)*G;
          
          ruqelem_t matched_elem; 
          if(match(elem, &rq[elem.host], &matched_elem)) { // found it in RQ
            if(print) printf("-- found in RQ\n");
            if(elem.size > S) { // rendezvous - free remote request
              // satisfy remote requires
              parser.schedules[elem.target].MarkNodeAsDone(elem.offset);
              check_hosts.push_back(elem.target);
              // TODO: uh, this is ugly ... we actually need to set the
              // time of the remote freed elements to elem.time now :-/
              // however, there is no simple way to do this :-(
              // but since we just got the elem with elem.time from the
              // queue, we can safely set the remote clocks to elem.time
              // (there is no event < elem.time in the queue) this
              // manipulation is dangerous, think before you change
              // anything!
              // TODO: do we need to add some latency for the ACK to
              // travel??? -- this can *not* be added here safely!
              if(nexto[elem.target][elem.proc] < elem.time) nexto[elem.target][elem.proc] = elem.time;
              if(nextgs[elem.target][elem.nic] < elem.time) nextgs[elem.target][elem.nic] = elem.time;

              if(print) printf("-- satisfy remote requires on host %i\n", elem.target);
            }
            tlviz.add_transmission(elem.target, elem.host, elem.starttime+o, elem.time, elem.size, G);
            tlviz.add_orecv(elem.host, elem.time+(elem.size-1)*G-(elem.size-1)*O, elem.time+o+std::max((elem.size-1)*O,(elem.size-1)*G), elem.proc);
            tlviz.add_noise(elem.host, elem.time+o+std::max((elem.size-1)*O,(elem.size-1)*G), elem.time+o+noise+std::max((elem.size-1)*O,(elem.size-1)*G), elem.proc);
            // satisfy local requires
            parser.schedules[elem.host].MarkNodeAsDone(matched_elem.offset);
            check_hosts.push_back(elem.host);


          } else { // not in RQ
            if(print) printf("-- not found in RQ - add to UQ\n");
            ruqelem_t nelem;
            nelem.size = elem.size;
            nelem.src = elem.target;
            nelem.tag = elem.tag;
            nelem.offset = elem.offset;
            nelem.starttime = elem.time; // when it was started
#ifdef LIST_MATCH
            uq[elem.host].push_back(nelem);
#else
            uq[elem.host][std::make_pair(nelem.tag,nelem.src)].push(nelem);
#endif
          }
        } else {
          elem.time=std::max(std::max(nexto[elem.host][elem.proc],nextgr[elem.host][elem.nic]), earliestfinish);
          if(print) printf("-- msg o,g not available -- reinserting\n");
          aq.push(elem);
        }
        
      } break;
        
      default:
        if(print) printf("not supported\n");
        break;
    }
    
    // do only ask hosts that actually completed something in this round!
    new_events=false;
    std::sort(check_hosts.begin(), check_hosts.end());
    check_hosts.erase(unique(check_hosts.begin(), check_hosts.end()), check_hosts.end());
    
    
    for(std::vector<int>::iterator iter=check_hosts.begin(); iter!=check_hosts.end(); ++iter) {
      host = *iter;
    //for(host = 0; host < p; host++) 
      SerializedGraph *sched=&parser.schedules[host];

      // retrieve all free operations
      SerializedGraph::nodelist_t free_ops;
      sched->GetExecutableNodes(&free_ops);
      // ensure that the free ops are ordered by type
      std::sort(free_ops.begin(), free_ops.end(), gnp_op_comp_func());
      
      // walk all new free operations and throw them in the queue 
      for(SerializedGraph::nodelist_t::iterator freeop=free_ops.begin(); freeop != free_ops.end(); ++freeop) {
        //if(print) std::cout << *freeop << " " ;

        new_events = true;

        // assign host that it starts on
        freeop->host = host;

#ifdef STRICT_ORDER
        freeop->ts=aqtime++;
#endif

        switch(freeop->type) {
          case OP_LOCOP:
            freeop->time = nexto[host][freeop->proc];
            if(print) printf("init %i (%i,%i) loclop: %lu, time: %lu, offset: %i\n", host, freeop->proc, freeop->nic, (long unsigned int) freeop->size, (long unsigned int)freeop->time, freeop->offset);
            break;
          case OP_SEND:
            freeop->time = std::max(nexto[host][freeop->proc], nextgs[host][freeop->nic]);
            if(print) printf("init %i (%i,%i) send to: %i, tag: %i, size: %lu, time: %lu, offset: %i\n", host, freeop->proc, freeop->nic, freeop->target, freeop->tag, (long unsigned int) freeop->size, (long unsigned int)freeop->time, freeop->offset);
            break;
          case OP_RECV:
            freeop->time = nexto[host][freeop->proc];
            if(print) printf("init %i (%i,%i) recvs from: %i, tag: %i, size: %lu, time: %lu, offset: %i\n", host, freeop->proc, freeop->nic, freeop->target, freeop->tag, (long unsigned int) freeop->size, (long unsigned int)freeop->time, freeop->offset);
            break;
          default:
            if(print) printf("not implemented!\n");
        }

        aq.push(*freeop);
      }
    }
    if(args_info.progress_given) {
      if(num_events/100*lastperc < aqtime) {
        if(print) printf("progress: %u %% (%llu) ", lastperc, (unsigned long long)aqtime);
        lastperc++;
        uint maxrq=0, maxuq=0;
        for(uint j=0; j<rq.size(); j++) maxrq=std::max(maxrq, (uint)rq[j].size());
        for(uint j=0; j<uq.size(); j++) maxuq=std::max(maxuq, (uint)uq[j].size());
        if(print) printf("[sizes: aq: %i max rq: %u max uq: %u]\n", (int)aq.size(), maxrq, maxuq);
      }
    }
  }
  
  gettimeofday(&tend, NULL);
	unsigned long int diff = tend.tv_sec - tstart.tv_sec;

#ifndef STRICT_ORDER
  ulint aqtime=0;
#endif
	if(print) printf("PERFORMANCE: Processes: %i \t Events: %lu \t Time: %lu s \t Speed: %.2f ev/s\n", p, (long unsigned int)aqtime, (long unsigned int)diff, (float)aqtime/(float)diff);

  // check if all queues are empty!!
  bool ok=true;
  for(uint i=0; i<p; ++i) {
#ifdef LIST_MATCH
    if(!uq[i].empty()) {
      if(print) printf("unexpected queue on host %i contains %lu elements!\n", i, (ulint)uq[i].size());
      for(ruq_t::iterator iter=uq[i].begin(); iter != uq[i].end(); ++iter) {
        if(print) printf(" src: %i, tag: %i\n", iter->src, iter->tag);
      }
      ok=false;
    }
    if(!rq[i].empty()) {
      if(print) printf("receive queue on host %i contains %lu elements!\n", i, (ulint)rq[i].size());
      for(ruq_t::iterator iter=rq[i].begin(); iter != rq[i].end(); ++iter) {
        if(print) printf(" src: %i, tag: %i\n", iter->src, iter->tag);
      }
      ok=false;
    }
#endif
  }
  
  if (ok) {
    host = 0;
    for(uint i=0; i<p; ++i) {
      btime_t maxo=*(std::max_element(nexto[i].begin(), nexto[i].end()));
      //std::cout << "Host " << i <<": "<< maxo << "\n";
      comm_time[i] = maxo;
    }
  }
}

/*int main() {
  const int num_of_rank = 32;
  int init_bias[num_of_rank];
  uint64_t comm_time[num_of_rank];
  char comm_file[50] = "schedule.bin";

  for (int i = 0; i < num_of_rank; ++i) {
    init_bias[i] = 0;
  }

  int o = 8000;
  int O = 0;
  int g = 300 + 6500;
  int L = 252;
  int G = 0;
  int S = 500;

  simulation(o, O, g, L, G, S, comm_file, init_bias, comm_time, num_of_rank);

  printf("Times: \n");
  for (int i = 0; i < num_of_rank; ++i) {
    std::cout << "Host " << i <<": "<< comm_time[i] << "\n";
  }
}*/
