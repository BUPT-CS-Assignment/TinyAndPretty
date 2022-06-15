#include <service/NaviSys.h>
#include <queue>
#include <algorithm>

thread_local Point2D _s , _e;
thread_local EvalueMode emode;

static int CostFunc(const int ex , const int ey)
{
	switch (emode)
	{
	
	case SHORT_PATH: 
		return 1;
	case SHORT_TIME:
		return (shahe_walking_busy[ex][ey]);

	case SHORT_BIKE:
		return (!shahe_biking[ex][ey] ? 1 :
			   !shahe_walking[ex][ey] ? 3 : INF);

	default: break;
	}

	return INF;
}

/* TRUE stands for inactive , because of 'continue' */
static bool ActiveFunc(const int ex , const int ey)
{
	if (ex < 0 || ex >= MAX_HEIGHT) return true;
	if (ey < 0 || ey >= MAX_WIDTH)  return true;

	switch (emode)
	{
	
	case SHORT_PATH: 
	case SHORT_TIME:
		return (shahe_walking[ex][ey]);
	case SHORT_BIKE:
		return (shahe_walking[ex][ey] && shahe_biking[ex][ey]);

	default: break;
	}
	return true;

}

#define diss(i , j) (abs(_e.x - i) + abs(_e.y - j))
bool operator < (const Point2D& a , const Point2D& b) {
	return a.dis + diss(a.x , a.y) >  b.dis + diss(b.x , b.y);
}
bool operator == (const Point2D& a , const Point2D& b) {
	return ((a.x == b.x) && (a.y == b.y));
}

using Path2D = std::vector<Point2D>;

static void drawPath(
	Point2D (*kmap)[MAX_WIDTH], 
	Path2D& __pans) 
{
	__pans.push_back(_e);

	for (Point2D* now = &kmap[_e.x][_e.y] ; now != nullptr; now = now->parent) {
		if (now->parent != nullptr &&
			now->parent->parent != nullptr &&(
			now->parent->parent->x - now->parent->x != now->parent->x - now->x ||
			now->parent->parent->y - now->parent->y != now->parent->y - now->y )
		)	{__pans.push_back( *(now->parent) );
			//::printf("   -> %d : %d\n", now->parent->x,now->parent->y);
		}

	}
	__pans.push_back(_s);
	std::reverse(__pans.begin() , __pans.end());
}


static void Astar(
	Point2D (*kmap)[MAX_WIDTH] , 
	Path2D& _pans1 , 
	Path2D& _pans2) 
{
	size_t ans_cnt = 0;
	std::priority_queue<Point2D> que;

	int dis[MAX_HEIGHT][MAX_WIDTH] {INF};

	dis[_s.x][_s.y] = kmap[_s.x][_s.y].dis = 0;
	que.push(kmap[_s.x][_s.y]);

	while(!que.empty() && ans_cnt < 2) {
		Point2D now = que.top(); que.pop();

		if (now.dis != dis[now.x][now.y]) continue;
		//::printf("%d , %d : %d\n" , now.x , now.y , now.dis );

		for (size_t i = 0 ; i < 4 ; i ++) {
			int ex = now.x + dx[i] , ey = now.y + dy[i];

			if(ActiveFunc(ex , ey)) continue;
			int cost = now.dis + CostFunc(ex , ey);

			if (ex == _e.x && ey == _e.y && 
				kmap[ex][ey].dis >= cost ) {
				kmap[ex][ey].parent = &kmap[now.x][now.y];
				dis[ex][ey] = kmap[ex][ey].dis = cost;
				drawPath(kmap , (ans_cnt == 0) ? _pans1 : _pans2);
				ans_cnt++; continue;
			}

			if( kmap[ex][ey].dis >  cost ) {
				kmap[ex][ey].parent = &kmap[now.x][now.y];
				dis[ex][ey] = kmap[ex][ey].dis = cost;
				que.push(kmap[ex][ey]);
			}
		}
	}
}


std::pair<Path2D , Path2D >
AstarAnalyse(const Path2D& pset)
{
	auto kmap = new Point2D[MAX_HEIGHT][MAX_WIDTH];

	for(int i = 0 ; i < MAX_HEIGHT ; i ++) for(int j = 0 ; j < MAX_WIDTH ; j++)
		kmap[i][j].x = i , kmap[i][j].y = j;

	_s = *pset.begin() , _e = *(pset.end()-1);


	Path2D pans1 , pans2;
	Astar(kmap , pans1 , pans2);

	delete[] kmap;

	return {pans1 , pans2};
}

static Json init_func() {
	Json list;
	list.push_back({"code" , 0});
	list.push_back({"msg" , "Query Successfully!"});

	std::vector<Object> dpos;


	for (size_t i = 0 ; i < Building::arrlen; i++) {
		dpos.push_back({
			{"id" 	, Building::BuildingList[i].id	} ,
			{"name"	, Building::BuildingList[i].name} ,
			{"type"	, Building::BuildingList[i].bt	} ,
			{"x" 	, Building::BuildingList[i].posx} ,
			{"y" 	, Building::BuildingList[i].posy}
		});
	}
	list.push_back({"data" , dpos});
	Building::init_flag = true;
	return list;
}


def_HttpEntry(MapTest , req) {


	std::string_view action { req.queryParam("action") };

	if (action[0] == 'q') {
		Json j = init_func();
		return new JsonResponse {j};

	} else if (action[0] == 'r') {
		Json j;
		int pos1x = std::stoi(req.queryParam("x1").data());
		int pos1y = std::stoi(req.queryParam("y1").data());
		int pos2x = std::stoi(req.queryParam("x2").data());
		int pos2y = std::stoi(req.queryParam("y2").data());

		if 		(action[1] == 's') emode = SHORT_PATH;
		else if	(action[1] == 't') emode = SHORT_TIME;
		else if	(action[1] == 'b') emode = SHORT_BIKE;
		else {
			j.push_back({"code" , 1});
			j.push_back({"msg" ,"Bad Params\n"});
			return new JsonResponse {j};
		}

		j.push_back({"code" ,0});
		j.push_back({"msg" ,"Calc Successfully!"});

		Path2D s;
		s.push_back({pos1y , pos1x}); // start
		s.push_back({pos2y , pos2x}); // end
		// s.push_back({24 , 24}); // start
		// s.push_back({18 , 33}); // end
		auto [p1 , p2] = AstarAnalyse(s);
		std::vector <Object> ans1; 
		for (const auto& it : p1) {
			ans1.push_back({
				{"x" , it.y} ,
				{"y" , it.x}
			});
			printf("1 >> %d , %d\n" ,it.y , it.x);
		}		
		j.push_back({"path1" ,ans1});

		if ( p2.size() > 0 ){
			std::vector <Object> ans2; 
			for (const auto& it : p2) {
				ans2.push_back({
					{"x" , it.y} ,
					{"y" , it.x}
				});
				printf("2 >> %d , %d\n" ,it.y , it.x);
			}		
			j.push_back({"path2" ,ans2});
		}

		return new JsonResponse {j};
	}

	Json j;
	j.push_back({"code" , 1});
	j.push_back({"msg" ,"Bad Params\n"});
	return new JsonResponse {j};
}