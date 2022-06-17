#ifndef __P_LIST__
#define __P_LIST__

namespace Building {

enum BuildType {
	BUILD ,
	AREA  ,
};

struct Build {
	int id ;
	char name[32];
	BuildType bt;
	int posx , posy;
};


static const Build BuildingList[] = 
{
{10001,	"西大门"		,BUILD	,0,25},
{10002,	"西门外卖点"	,AREA	,0,24},
{10003,	"沙河快递站"	,BUILD	,4,21},
{10004,	"基建处"		,BUILD	,0,20},
{10005,	"田径场"		,BUILD	,21,6},
{10006,	"篮球场"		,BUILD	,21,18},
{10007,	"体育场AED"		,AREA	,19,13},
{10008,	"学生公寓E区"	,BUILD	,26,6},
{10009,	"学生公寓D2区"	,BUILD	,34,5},
{10010,	"E区休憩区"		,AREA	,32,9},
{10011,	"学生公寓C区"	,BUILD	,30,15},
{10012,	"学生公寓B区"	,BUILD	,27,18},
{10013,	"学生公寓A区"	,BUILD	,33,21},
{10014,	"学生公寓D1区"	,BUILD	,36,18},
{10015,	"生活服务区"	,BUILD	,33,21},
{10016,	"沙河展览台"	,AREA	,27,28},
{10017,	"信息楼S1"		,BUILD	,24,32},
{10018,	"学生公寓S2"	,BUILD	,38,34},
{10019,	"学生公寓S3"	,BUILD	,37,40},
{10020,	"学生公寓S4"	,BUILD	,37,47},
{10021,	"学生公寓S5"	,BUILD	,37,53},
{10022,	"学生公寓S6"	,BUILD	,37,61},
{10023,	"教工食堂"		,BUILD,	44	,6},
{10024,	"学生食堂"		,BUILD,	44,	17},
{10025,	"校园西餐厅"	,BUILD,	50,	20},
{10026,	"二维码广场"	,BUILD,	51,	13},
{10027,	"公共教学楼"	,BUILD,	50,	36},
{10028,	"沙河小公园"	,BUILD,	50,	49},
{10029,	"南区食堂"		,BUILD,	48,	55},
{10030,	"沙河南大门"	,BUILD,	63,	63},
{10031,	"沙河南门外卖点",AREA,	62,	63},
{10032,	"沙河医务室"	,BUILD,	56,	1},
{10033,	"沙河办公楼"	,BUILD,	56,	6},
{10034,	"沙河学生活动中心",BUILD,56	,17},
{10035,	"沙河小卖部"	,BUILD	,53,17},
{10036,	"沙河邮局"		,BUILD	,56	,24},
{10037,	"ATM"			,AREA,	56,	24},
{10038,	"东配楼"		,BUILD	,82,12},
{10039,	"沙河咖啡厅"	,BUILD	,77,19},
{10040,	"沙河图书馆"	,BUILD	,71,16},
{10041,	"教学楼N楼"		,BUILD	,71,31},
{10042,	"教学楼S楼"		,BUILD	,71,41},
{10043,	"沙河报告厅"	,BUILD	,69,36},
{10044,	"实验楼S1"		,BUILD	,101,31},
{10045,	"实验楼S2"		,BUILD	,101,41},
{10046,	"实验楼S3"		,BUILD	,101,50},
{10047,	"理学院楼"		,BUILD	,91,56},
{10048,	"售货机E"		,AREA	,28,6},
{10049,	"售货机D2"		,AREA	,35,18},
{10050,	"咖啡机S"		,AREA	,72,41},

{20001,	"西土城西门"	,BUILD	,1,58},
{20002,	"西土城主楼"	,BUILD	,42,59},
{20003,	"西土城教一楼"	,BUILD	,39,46},
{20004,	"西土城教二楼"	,BUILD	,39,76},
{20005,	"西土城教三楼"	,BUILD	,16,76},
{20006,	"西土城教四楼"	,BUILD	,16,44},
{20007,	"科学会堂"		,BUILD	,55,59},
{20008,	"创新楼"		,BUILD	,54,81},
{20009,	"西土城图书馆"	,BUILD	,41,32},
{20010,	"西土城篮球场"	,BUILD	,51,34},
};
constexpr unsigned long arrlen = sizeof(BuildingList) / sizeof(Build);

struct BusInfo {
	int start_hour , start_mins;
	int num[7];
} ;

static const BusInfo XTCBusList[] = {
/* hour:mins | Sun | Mon | Tue | Wes | Thu | Fri | Sat | */
{   6 , 50	  ,  0  , 2  ,  2  ,  2  ,  2  ,  2  ,  0  } ,
{	8 , 30	  ,  0  , 2  ,  2  ,  2  ,  2  ,  2  ,  0  } ,
{  12 , 00    ,  0  , 2  ,  2  ,  1  ,  2  ,  1  ,  0  } ,
{  13 , 30    ,  0  , 1  ,  1  ,  1  ,  1  ,  1  ,  0  } ,
{  14 , 20    ,  0  , 1  ,  1  ,  0  ,  1  ,  1  ,  0  } ,
{  16 , 60    ,  0  , 1  ,  1  ,  1  ,  1  ,  0  ,  0  } 

};

constexpr unsigned long XTCarrlen = sizeof(XTCBusList) / sizeof(BusInfo);

static const BusInfo SHBusList[] = {
/* hour:mins | Sun | Mon | Tue | Wes | Thu | Fri | Sat | */
{   9 , 50	  ,  0 ,  2  ,  2  ,  1  ,  2  ,  2  ,  0  } ,
{  11 , 40	  ,  0 ,  2  ,  2  ,  2  ,  2  ,  1  ,  0  } ,
{  13 , 00    ,  0 ,  1  ,  1  ,  2  ,  1  ,  1  ,  0  } ,
{  15 , 45    ,  0 ,  1  ,  1  ,  0  ,  1  ,  1  ,  0  } ,
{  17 , 10    ,  0 ,  1  ,  1  ,  1  ,  1  ,  1  ,  0  } ,
{  18 , 25    ,  0 ,  1  ,  1  ,  1  ,  1  ,  1  ,  0  } ,
{  21 , 10    ,  0 ,  1  ,  1  ,  0  ,  1  ,  0  ,  0  } 

};

constexpr unsigned long SHarrlen = sizeof(SHBusList) / sizeof(BusInfo);

};

#endif 