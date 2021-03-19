
//#include "client/client_proxy.h"
//#include "client/client.h"
#include "client/client_desktop.h"
#include "AsyncClient.h"

namespace client {

AsyncClient::AsyncClient() {

}

AsyncClient::~AsyncClient() {

}

bool AsyncClient::connectToHost(std::shared_ptr<base::TaskRunner> task_runner,client::Config config) {
	//client_ = std::make_unique<Client>(task_runner);
	client_ = std::make_unique<ClientDesktop>(task_runner);
	/*client_proxy_ = std::make_unique<ClientProxy>(
		qt_base::Application::ioTaskRunner(), std::move(client), config);

	client_proxy_->start();*/
	client_->start(config);

	return true;
}

}