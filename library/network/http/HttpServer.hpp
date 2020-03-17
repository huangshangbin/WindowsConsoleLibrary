#pragma once

#include <library/network/http/HttpService.hpp>


class HttpServer
{
private:
	Server m_sever;

	int m_mostThreadCount;

public:
	HttpServer() {}


public:
	void setMostThreadCount(int threadCount) { m_mostThreadCount = threadCount; }
	
	void addHttpService(HttpService* httpService)
	{
		map<string, HttpServiceDealFun>* getRouteDealFunMap = httpService->getGetRouteDealFunMap();
		for (map<string, HttpServiceDealFun>::iterator it = getRouteDealFunMap->begin(); it != getRouteDealFunMap->end(); it++)
		{
			m_sever.Get(it->first.c_str(), it->second);
		}

		map<string, HttpServiceDealFun>* postRouteDealFunMap = httpService->getPostRouteDealFunMap();
		for (map<string, HttpServiceDealFun>::iterator it = postRouteDealFunMap->begin(); it != postRouteDealFunMap->end(); it++)
		{
			m_sever.Post(it->first.c_str(), it->second);
		}

		delete httpService;
	}

	bool listen(int port)
	{
		return m_sever.listen("localhost", port);
	}




private:
	HttpServer(const HttpServer& httpSevrer) {}
	void operator = (const HttpServer& httpSevrer) {}
};