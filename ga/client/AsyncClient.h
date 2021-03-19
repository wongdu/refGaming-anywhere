#ifndef __Async_Client_H__
#define	__Async_Client_H__

#include <memory>
#include "base/task_runner.h"
#include "client/client_config.h"

namespace client {

	class Client;

class AsyncClient
{
public:
	AsyncClient();
	~AsyncClient();

public:
	bool connectToHost(std::shared_ptr<base::TaskRunner> task_runner,client::Config config);
private:
	std::unique_ptr<Client> client_;
};

}
#endif	
