#include <service/NaviSys.h>


def_HttpEntry(MapTest , req) {
	Json j; std::vector <Object> ans ;
	std::cerr << "Hit" << "\n";
	int pos1x = std::stoi(req.queryParam("x1").data());
	int pos1y = std::stoi(req.queryParam("y1").data());
	int pos2x = std::stoi(req.queryParam("x2").data());
	int pos2y = std::stoi(req.queryParam("y2").data());

	std::cerr  << "Here "<<pos1x <<"," << pos1y << "\n";
	ans.push_back ({
		{"x" , pos1x} ,
		{"y" , pos1y}
	});
	ans.push_back ({
		{"x" , pos1x} ,
		{"y" , pos2y}
	});
	ans.push_back ({
		{"x" , pos2x} ,
		{"y" , pos2y}
	});
	j.push_back({"code" ,0});
	j.push_back({"msg" ,"Calc Successfully!"});
	j.push_back({"data" ,ans});

	return new JsonResponse {j};
}