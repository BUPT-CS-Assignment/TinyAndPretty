#include <service/ClockSys.h>
using namespace UTILSTD;

def_HttpEntry(API_Clock , req) {

	Json j; // used for response
	std::string_view action { req.queryParam("action") };

	CONSOLE_LOG(true,"api/clock called [action:%s]\n",action);

	/* Start listening server broadcast Notification */
	if (action == "c") {
		j.push_back({"code" , TimeStatus::DUP}); // for
		j.push_back({"msg"  , "Connection Duplication!"});

		return Timer::createComet(req , 
					new JsonResponse{j});

	/* Query Server Clock INFO */
	} else if (action == "q") {
		auto t_table = Timer::getVirtualTime();

		j.push_back({"code" , TimeStatus::SUCC});
		j.push_back({"msg"  , "Query Successfully!"});
		j.push_back({"data" , {
				{"ratio" , (int)Timer::getTimeLineRate()} ,
				{"year"  , t_table[0]} ,
				{"mon"   , t_table[1]} ,
				{"day"   , t_table[2]} ,
				{"week"  , t_table[3]} ,
				{"hour"  , t_table[4]} ,
				{"min"   , t_table[5]} ,
				{"sec"   , t_table[6]} 
			}});

		return new JsonResponse{j};

	/* Modify time speed , and inform all online user*/
	} else if (action == "m") {
		Timer::changeTimeLineRate(
			std::stoi(req.queryParam("rate").data())
		);

		{	// broadcast to all alive client;
			Json elc;
			elc.push_back({"code" , TimeStatus::BROAD});
			elc.push_back({"msg"  , "Time Rate Changed!"});
			Timer::launchBroadCast(
				new JsonResponse{elc}
			);
		}

		j.push_back({"code" , TimeStatus::SUCC});
		j.push_back({"msg"  , "Modify Successfully!"});
		
		return new JsonResponse{j};

	/* No 'action' param */
	} else  {
		j.push_back({"code" , TimeStatus::ERR});
		j.push_back({"msg"  , "Params Error:No 'action'"});
		return new JsonResponse{j , HTTP_STATUS_400};

	}
}