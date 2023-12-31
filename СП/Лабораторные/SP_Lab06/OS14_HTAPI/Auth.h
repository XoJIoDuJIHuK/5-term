#pragma once

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

namespace ht
{
	bool doesUsergroupExist(const wchar_t* usersGroup);

	bool isCurrentUserInUsergroup(const wchar_t* usersGroup);

	bool isUserInUsergroup(const wchar_t* user, const wchar_t* usersGroup);
	bool verifyUser(const wchar_t* user, const wchar_t* password);
}
