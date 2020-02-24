#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <library/network/http/private/httplib.h>

using namespace httplib;
using namespace std;

using HttpServiceDealFun = std::function< void(const Request& request, Response& response) >;

class HttpService
{
private:
	map<string, HttpServiceDealFun> m_getRouteDealFunMap;
	map<string, HttpServiceDealFun> m_postRouteDealFunMap;

public:
	HttpService() {}


public:
	void bindGetRoute(string route,  HttpServiceDealFun dealFun)
	{
		m_getRouteDealFunMap[route] = dealFun;
	}

	void bindPostRoute(string route, HttpServiceDealFun dealFun)
	{
		m_postRouteDealFunMap[route] = dealFun;
	}

public:
	map<string, HttpServiceDealFun>* getGetRouteDealFunMap()
	{
		return &m_getRouteDealFunMap;
	}

	map<string, HttpServiceDealFun>* getPostRouteDealFunMap()
	{
		return &m_postRouteDealFunMap;
	}

private:
	HttpService(const HttpService& httpService) {}
	void operator = (const HttpService& httpService) {}
};

