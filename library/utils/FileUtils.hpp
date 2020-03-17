#pragma once

#include <library/utils/StringUtils.hpp>

#include <string>

#include <iostream>
using namespace std;

class FileUtils
{
//Â·¾¶²Ù×÷
public:
	static string getFileOrDirName(string path)
	{
		int index;
		for (int i = path.length(); i > 0; i--)
		{
			if (i == 0)
			{
				return path;
			}

			if (path[i] == '\\')
			{
				index = i;
				break;
			}
		}

		string result = StringUtils::getStringUsePos(path, index + 1, path.length());
		return result;
	}

	static string getPreviousLayerPath(string path)
	{
		int index;
		for (int i = path.length(); i > 0; i--)
		{
			if (i == 0)
			{
				return path;
			}

			if (path[i] == '\\')
			{
				index = i;
				break;
			}
		}

		return StringUtils::getStringUsePos(path, 0, index - 1);
	}

	static void deleteFile(string filePath)
	{
		SetFileAttributes(filePath.c_str(), 0);
		DeleteFile(filePath.c_str());
	}
};
