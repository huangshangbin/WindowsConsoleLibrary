#pragma once

#include <library/network/http/private/httplib.h>
#include <library/utils/StringUtils.hpp>
#include <library/utils/TypeConvertUtils.hpp>
#include <library/fileDeal/ByteStreamFile.hpp>
#include <library/utils/FileUtils.hpp>

#include <iostream>
#include <string>

using namespace std;

using namespace httplib;


class UrlParse
{
public:
	UrlParse(string url)
	{
		url = StringUtils::splitStringGetOneStr(url, "//", 1);
		if (StringUtils::isExistStringInString(url, ":"))//°üº¬¶Ë¿ÚºÅµÄurl http:// localhost:3000/config/newVersion;
		{
			m_host = StringUtils::splitStringGetOneStr(url, ":", 0);

			string rightUrl = StringUtils::splitStringGetOneStr(url, ":", 1);

			m_route = "/" + StringUtils::getStringUseCharStart(rightUrl, '/');

			m_port = TypeConvertUtils::getInt(StringUtils::getStringUseCharEnd(rightUrl, '/'));
		}
		else // http:// localhost/config/newVersion;
		{
			m_host = StringUtils::getStringUseCharEnd(url, '/');
			m_route = "/" + StringUtils::getStringUseCharStart(url, '/');

			m_port = 80;
		}
	}

public:
	string m_host;
	string m_route;
	int m_port;
};



class HttpClient
{
public:
	HttpClient() {}
	~HttpClient() {}

public:
	static string get(string url)
	{
		UrlParse urlParse(url);
		httplib::Client httpClient(urlParse.m_host, urlParse.m_port);
		auto res = httpClient.Get(urlParse.m_route.c_str());

		return res->body;
	}

	static string post(string url, string requestData)
	{
		UrlParse urlParse(url);
		httplib::Client httpClient(urlParse.m_host, urlParse.m_port);

		auto res = httpClient.Post(urlParse.m_route.c_str(), requestData, "text/plain");

		return res->body;
	}

	static string postFile(string url, string filePath)
	{
		ByteStreamFile byteFile(filePath);
		string fileName = FileUtils::getFileOrDirName(filePath);

		MultipartFormDataItems itemList = {
			{ fileName, string(byteFile.getBuffer(), byteFile.getLength()), fileName, "application/octet-stream" }
		};

		UrlParse urlParse(url);
		httplib::Client httpClient(urlParse.m_host, urlParse.m_port);

		auto res = httpClient.Post(urlParse.m_route.c_str(), itemList);

		return res->body;
	}

	static string postFileList(string url, deque<string>& filePathList)
	{
		MultipartFormDataItems itemList;
		for (int i = 0; i < filePathList.size(); i++)
		{
			ByteStreamFile byteFile(filePathList[i]);
			string fileName = FileUtils::getFileOrDirName(filePathList[i]);

			MultipartFormData multipartData{ fileName, string(byteFile.getBuffer(), byteFile.getLength()), fileName, "application/octet-stream" };

			itemList.emplace_back(multipartData);
		}
		
		UrlParse urlParse(url);
		httplib::Client httpClient(urlParse.m_host, urlParse.m_port);

		auto res = httpClient.Post(urlParse.m_route.c_str(), itemList);

		return res->body;
	}
};

