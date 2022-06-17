#include <service/NaviSys.h>
#include <service/TAPSystem.h>
#include <queue>
#include <algorithm>
using namespace UTILSTD;
#include <libs/Heap.hpp>

thread_local Point2D _s, _e;
thread_local EvalueMode emode;

using Path2D = std::vector<Point2D>;
using XTC_MAP = Point2D(*)[XTC_MAX_WIDTH];
using SHAHE_MAP = Point2D(*)[SHAHE_MAX_WIDTH];

template<typename T>
struct __SIZE_SWITCHER__
{
    static constexpr int height =
        std::is_same<T, SHAHE_MAP>::value ? SHAHE_MAX_HEIGHT :
        std::is_same<T, XTC_MAP  >::value ? XTC_MAX_HEIGHT :
        0;
    static constexpr int width =
        std::is_same<T, SHAHE_MAP>::value ? SHAHE_MAX_WIDTH :
        std::is_same<T, XTC_MAP  >::value ? XTC_MAX_WIDTH :
        0;
};

template<typename T>
struct __WALKING_SWITCHER__
{
    static constexpr void* value =
        std::is_same<T, SHAHE_MAP>::value ? (void*)shahe_walking :
        std::is_same<T, XTC_MAP  >::value ? (void*)xtc_walking :
        nullptr;
};
template<typename T>
struct __WALKING_BUSY_SWITCHER__
{
    static constexpr void* value =
        std::is_same<T, SHAHE_MAP>::value ? (void*)shahe_walking_busy :
        std::is_same<T, XTC_MAP  >::value ? (void*)xtc_walking_busy :
        nullptr;
};
template<typename T>
struct __BIKING_SWITCHER__
{
    static constexpr void* value =
        std::is_same<T, SHAHE_MAP>::value ? (void*)shahe_biking :
        std::is_same<T, XTC_MAP  >::value ? (void*)xtc_biking :
        nullptr;
};


/* -------------------------------------------------------------------*/
template<typename T>
static int CostFunc(const int ex, const int ey)
{
    switch(emode)
    {

        case SHORT_PATH:
            return 1;
        case SHORT_TIME:
            return (static_cast<int(*)[__SIZE_SWITCHER__<T>::width]>
                (__WALKING_BUSY_SWITCHER__<T>::value)[ex][ey]);

        case SHORT_BIKE:
            return (! (static_cast<int(*)[__SIZE_SWITCHER__<T>::width]>
                (__BIKING_SWITCHER__<T>::value)[ex][ey]) ? 1 :
                ! (static_cast<int(*)[__SIZE_SWITCHER__<T>::width]>
                    (__WALKING_SWITCHER__<T>::value)[ex][ey]) ? 5 : INF);

        default: break;
    }

    return INF;
}

template<typename T>
static bool ActiveFunc(const int ex, const int ey)
{
    auto constexpr MAXN_WIDTH = __SIZE_SWITCHER__<T>::width;
    auto constexpr MAXN_HEIGHT = __SIZE_SWITCHER__<T>::height;

    if(ex < 0 || ex >= MAXN_HEIGHT) return true;
    if(ey < 0 || ey >= MAXN_WIDTH) return true;

    switch(emode)
    {

        case SHORT_PATH:
        case SHORT_TIME:
            return (static_cast<int(*)[MAXN_WIDTH]>(__WALKING_SWITCHER__<T>::value)[ex][ey]);

        case SHORT_BIKE:
            return ((static_cast<int(*)[MAXN_WIDTH]>(__WALKING_SWITCHER__<T>::value)[ex][ey]) &&
                (static_cast<int(*)[MAXN_WIDTH]>(__BIKING_SWITCHER__<T>::value)[ex][ey]));

        default: break;
    }

    return true;
}

#define diss(i , j) (abs(_e.x - i) + abs(_e.y - j))
bool operator < (const Point2D& a, const Point2D& b)
{
    return a.dis + diss(a.x, a.y) > b.dis + diss(b.x, b.y);
}
bool operator == (const Point2D& a, const Point2D& b)
{
    return ((a.x == b.x) && (a.y == b.y));
}


template<typename T>
static void drawPath(
    T kmap,
    Path2D& __pans)
{
    __pans.push_back(_e);

    for(Point2D* now = &kmap[_e.x][_e.y]; now != nullptr; now = now->parent)
    {
        if(now->parent != nullptr &&
            now->parent->parent != nullptr && (
                now->parent->parent->x - now->parent->x != now->parent->x - now->x ||
                now->parent->parent->y - now->parent->y != now->parent->y - now->y)
            )
        {
            __pans.push_back(*(now->parent));
            //::printf("   -> %d : %d\n", now->parent->x,now->parent->y);
        }

    }
    __pans.push_back(_s);
    std::reverse(__pans.begin(), __pans.end());
}
template<typename T>
static void Astar(
    T kmap,
    Path2D& _pans1,
    Path2D& _pans2)
{
    size_t ans_cnt = 0;
    std::priority_queue<Point2D> que;

    int dis
    [__SIZE_SWITCHER__<T>::height]
    [__SIZE_SWITCHER__<T>::width]
    {INF};

    dis[_s.x][_s.y] = kmap[_s.x][_s.y].dis = 0;
    que.push(kmap[_s.x][_s.y]);

    while(!que.empty() && ans_cnt < 2)
    {
        Point2D now = que.top(); que.pop();

        if(now.dis != dis[now.x][now.y]) continue;
        //::printf("%d , %d : %d\n" , now.x , now.y , now.dis );

        for(size_t i = 0; i < 4; i ++)
        {
            int ex = now.x + dx[i], ey = now.y + dy[i];

            //::printf("       %d , %d = %d\n" , ex , ey , shahe_walking[ex][ey]);
            if(ActiveFunc<T>(ex, ey)) continue;
            int cost = now.dis + CostFunc<T>(ex, ey);

            if(ex == _e.x && ey == _e.y &&
                kmap[ex][ey].dis >= cost)
            {
                kmap[ex][ey].parent = &kmap[now.x][now.y];
                dis[ex][ey] = kmap[ex][ey].dis = cost;
                drawPath<T>(kmap, (ans_cnt == 0) ? _pans1 : _pans2);
                ans_cnt++; continue;
            }

            if(kmap[ex][ey].dis > cost)
            {
                kmap[ex][ey].parent = &kmap[now.x][now.y];
                dis[ex][ey] = kmap[ex][ey].dis = cost;
                que.push(kmap[ex][ey]);
            }
        }
    }
}

template<typename T>
std::pair<Path2D, Path2D >
AstarAnalyse(const Path2D& pset)
{
    constexpr auto MAX_HEIGHT = __SIZE_SWITCHER__<T>::height;
    constexpr auto MAX_WIDTH = __SIZE_SWITCHER__<T>::width;

    auto kmap = new Point2D[MAX_HEIGHT][MAX_WIDTH];

    for(int i = 0; i < MAX_HEIGHT; i ++) for(int j = 0; j < MAX_WIDTH; j++)
        kmap[i][j].x = i, kmap[i][j].y = j;

    _s = *pset.begin(), _e = *(pset.end() - 1);


    Path2D pans1, pans2;
    Astar<T>(kmap, pans1, pans2);

    delete[] kmap;

    return {pans1 , pans2};
}

static Json initPathList()
{
    Json list;
    list.push_back({"code" , 0});
    list.push_back({"msg" , "Query PathList Successfully!"});

    std::vector<Object> dpos;


    for(size_t i = 0; i < Building::arrlen; i++)
    {
        dpos.push_back({
            {"id" 	, Building::BuildingList[i].id	} ,
            {"name"	, Building::BuildingList[i].name} ,
            {"type"	, Building::BuildingList[i].bt	} ,
            {"x" 	, Building::BuildingList[i].posx} ,
            {"y" 	, Building::BuildingList[i].posy}
            });
    }
    list.push_back({"data" , dpos});
    return list;
}

#define INSERT_INFO(name) do { 								\
		insert_flag = true;									\
		list.push_back ({"nears" , {						\
			{"hour" , name[i].start_hour} ,					\
			{"mins" , name[i].start_mins}					\
		}});												\
	} while (0)

#define Query_List(name, len)   do {                        \
		for (size_t i = 0; i < len; i++) {                  \
			if (name[i].num[now[3]] == 0) continue;			\
			if (now[4] < name[i].start_hour)  {				\
				INSERT_INFO(name);   						\
				break;                       				\
			}                                               \
			else if (now[4] == name[i].start_hour &&        \
					 now[5] <  name[i].start_mins) {        \
				INSERT_INFO(name);                          \
				break;										\
			}                                               \
		}                                                   \
	} while (0)

static Json initBusList(MapValue from)
{
    Json list;
    list.push_back({"code" , 0});
    list.push_back({"msg" , "Query BusList Successfully!"});

    bool insert_flag = false;
    auto now = Timer::getVirtualTime();

    if(from == SH)
    {
        Query_List(Building::SHBusList, Building::SHarrlen);
    }
    else if(from == XTC)
    {
        Query_List(Building::XTCBusList, Building::XTCarrlen);
    }

    if(!insert_flag)
        list.push_back({"nears" , {
            {"hour" , -1} ,
            {"mins" , -1}
        }});

    return list;
}

#define DONE_MAP(type, i1, i2)                              \
	do {                                                    \
		auto [p1, p2] = AstarAnalyse<type>(s);         \
                                                            \
		std::vector<Object> ans1;                           \
		for (const auto &it : p1) {                         \
			ans1.push_back({{"x", it.y}, {"y", it.x}});     \
		}                                                   \
		j.push_back({"path" #i1, ans1});                    \
                                                            \
		if (p2.size() > 0) {                                \
			std::vector<Object> ans2;                       \
			for (const auto &it : p2) {                     \
				ans2.push_back({{"x", it.y}, {"y", it.x}}); \
			}                                               \
			j.push_back({"path" #i2, ans2});                \
		}                                                   \
                                                            \
	} while (0)

def_HttpEntry(MapTest, req)
{
    std::string_view action{req.queryParam("action")};

    // std::string userid(req.queryHeader("userid"));
    // std::string token(req.queryHeader("token"));
    // if(TokenCheck(userid, token) != TOKEN_ACCESS)
    // {
    //     return new HttpResponse("ACCESS_DENIED", HTTP_STATUS_401);
    // }

    // CONSOLE_LOG(true, "* api/map called [user:%s  action:%s]\n", userid.c_str(), action.data());

    if(action[0] == 'q')
    {
        Json j = initPathList();
        return new JsonResponse{j};


    }
    else if(action[0] == 'b')
    {
        int mmap = std::stoi(req.queryParam("m").data());
        Json j = initBusList((MapValue)mmap);
        return new JsonResponse{j};

    }
    else if(action[0] == 'r')
    {
        Json j;
        int mmap1 = std::stoi(req.queryParam("m1").data());
        int mmap2 = std::stoi(req.queryParam("m2").data());
        int pos1x = std::stoi(req.queryParam("x1").data());
        int pos1y = std::stoi(req.queryParam("y1").data());
        int pos2x = std::stoi(req.queryParam("x2").data());
        int pos2y = std::stoi(req.queryParam("y2").data());

        {
            if(action[1] == 's') emode = SHORT_PATH;
            else if(action[1] == 't') emode = SHORT_TIME;
            else if(action[1] == 'b') emode = SHORT_BIKE;
            else
            {
                j.push_back({"code" , 1});
                j.push_back({"msg" ,"Bad Params\n"});
                return new JsonResponse{j};
            }
        }

        j.push_back({"code" ,0});
        j.push_back({"msg" ,"Calc Successfully!"});

        if(mmap1 == mmap2)
        {
            Path2D s;
            s.push_back({pos1y , pos1x}); // start
            s.push_back({pos2y , pos2x}); // end

            // start and end both at SH_MAP
            if(mmap1 == SH)
            {
                DONE_MAP(SHAHE_MAP, 1, 2);

                // start and end both at XTC_MAP
            }
            else if(mmap1 == XTC)
            {
                DONE_MAP(XTC_MAP, 1, 2);
            }

        }
        else
        {

            if(mmap1 == SH)
            {
                Path2D s;
                s.push_back({pos1y , pos1x});
                s.push_back({25 , 0});
                DONE_MAP(SHAHE_MAP, 1, 2);
            }
            else if(mmap1 == XTC)
            {
                Path2D s;
                s.push_back({pos1y , pos1x});
                s.push_back({58 , 0});
                DONE_MAP(XTC_MAP, 1, 2);
            }

            if(mmap2 == SH)
            {
                Path2D s;
                s.push_back({25 , 0});
                s.push_back({pos2y , pos2x});
                DONE_MAP(SHAHE_MAP, 3, 4);
            }
            else if(mmap2 == XTC)
            {
                Path2D s;
                s.push_back({58 , 0});
                s.push_back({pos2y , pos2x});
                DONE_MAP(XTC_MAP, 3, 4);
            }


        }

        return new JsonResponse{j,"NO_ERROR",HTTP_STATUS_200};

    }

    Json j;
    j.push_back({"code" , 1});
    j.push_back({"msg" ,"Bad Params\n"});
    return new JsonResponse{j,"NO_ERROR",HTTP_STATUS_200};
}