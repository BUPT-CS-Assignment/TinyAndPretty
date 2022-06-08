#ifndef __TAP_CENTER__
#define __TAP_CENTER__

#include <memory>

class TAPCenter;
class ManagerBase;

class TAPManager{
public:
	TAPManager();
	void start();
	void loadSubManager( std::unique_ptr<ManagerBase> sub );
private:
	std::shared_ptr<TAPCenter> ptr;
};

#endif