/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Public Interface Wrapper Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

PublicInterfaceWrapper g_PublicInterfaceWrapper;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_PublicInterfaceWrapper);

HandleType_t g_GlobalFwdType = 0;
HandleType_t g_PrivateFwdType = 0;

cell_t LoadPluginEx(IPluginContext *pContext, const cell_t *params)
{
    char *path;
    bool debug = false;
    PluginType type = PluginType_MapUpdated;
    char *error;
    size_t maxlength;
    cell_t *wasloaded;

    pContext->LocalToString(params[1], &path);
    pContext->LocalToString(params[2], &error);
    maxlength = static_cast<size_t>(params[3]);
    pContext->LocalToPhysAddr(params[4], &wasloaded);

    IPlugin *pPlugin = plsys->LoadPlugin(path, debug, type, error, maxlength, reinterpret_cast<bool *>(wasloaded));

    if (pPlugin == NULL)
    {
        return static_cast<cell_t>(BAD_HANDLE);
    }
    
    return static_cast<cell_t>(pPlugin->GetMyHandle());
}

cell_t UnloadPluginEx(IPluginContext *pContext, const cell_t *params)
{
    Handle_t handle;
    HandleError *err;

    handle = params[1];
    pContext->LocalToPhysAddr(params[2], reinterpret_cast<cell_t **>(&err));

    IPlugin *pPlugin = plsys->PluginFromHandle(handle, err);

    if (pPlugin == NULL)
    {
        return static_cast<cell_t>(false);
    }

    *err = HandleError_None;

    return static_cast<cell_t>(plsys->UnloadPlugin(pPlugin));
}

static cell_t AddToGlobalForward(IPluginContext *pContext, const cell_t *params)
{
    char *fwdName;
	Handle_t plHandle = static_cast<Handle_t>(params[2]);
	HandleError err;
	IChangeableForward *pForward;
	IPlugin *pPlugin;

    pContext->LocalToString(params[1], &fwdName);

    if ((pForward = static_cast<IChangeableForward *>(forwards->FindForward(fwdName, NULL)))
        == NULL)
    {
        return pContext->ThrowNativeError("Invalid global forward name %s", fwdName);
    }

	if (plHandle == 0)
	{
		pPlugin = plsys->FindPluginByContext(pContext->GetContext());
	} else {
		pPlugin = plsys->PluginFromHandle(plHandle, &err);

		if (!pPlugin)
		{
			return pContext->ThrowNativeError("Plugin handle %x is invalid (error %d)", plHandle, err);
		}
	}

	IPluginFunction *pFunction = pPlugin->GetBaseContext()->GetFunctionById(params[3]);

	if (!pFunction)
	{
		return pContext->ThrowNativeError("Invalid function id (%X)", params[3]);
	}

	return pForward->AddFunction(pFunction);
}

static cell_t RemoveFromGlobalForward(IPluginContext *pContext, const cell_t *params)
{
    char *fwdName;
	Handle_t plHandle = static_cast<Handle_t>(params[2]);
	HandleError err;
	IChangeableForward *pForward;
	IPlugin *pPlugin;

    pContext->LocalToString(params[1], &fwdName);

    if ((pForward = static_cast<IChangeableForward *>(forwards->FindForward(fwdName, NULL)))
        == NULL)
    {
        return pContext->ThrowNativeError("Invalid global forward name %s", fwdName);
    }

	if (plHandle == 0)
	{
		pPlugin = plsys->FindPluginByContext(pContext->GetContext());
	} else {
		pPlugin = plsys->PluginFromHandle(plHandle, &err);

		if (!pPlugin)
		{
			return pContext->ThrowNativeError("Plugin handle %x is invalid (error %d)", plHandle, err);
		}
	}

	IPluginFunction *pFunction = pPlugin->GetBaseContext()->GetFunctionById(params[3]);

	if (!pFunction)
	{
		return pContext->ThrowNativeError("Invalid function id (%X)", params[3]);
	}

	return pForward->RemoveFunction(pFunction);
}

static cell_t RemoveAllFromGlobalForward(IPluginContext *pContext, const cell_t *params)
{
    char *fwdName;
	Handle_t plHandle = static_cast<Handle_t>(params[2]);
	HandleError err;
	IChangeableForward *pForward;
	IPlugin *pPlugin;

    pContext->LocalToString(params[1], &fwdName);

    if ((pForward = static_cast<IChangeableForward *>(forwards->FindForward(fwdName, NULL)))
        == NULL)
    {
        return pContext->ThrowNativeError("Invalid global forward name %s", fwdName);
    }

	if (plHandle == 0)
	{
		pPlugin = plsys->FindPluginByContext(pContext->GetContext());
	} else {
		pPlugin = plsys->PluginFromHandle(plHandle, &err);

		if (!pPlugin)
		{
			return pContext->ThrowNativeError("Plugin handle %x is invalid (error %d)", plHandle, err);
		}
	}

	return pForward->RemoveFunctionsOfPlugin(pPlugin);
}

const sp_nativeinfo_t natives[] = 
{
	{"LoadPluginEx",               LoadPluginEx},
    {"UnloadPluginEx",             UnloadPluginEx},
    {"AddToGlobalForward",         AddToGlobalForward},
    {"RemoveFromGlobalForward",    RemoveFromGlobalForward},
    {"RemoveAllFromGlobalForward", RemoveAllFromGlobalForward},
	{NULL,			               NULL},
};

bool PublicInterfaceWrapper::SDK_OnLoad(char *error, size_t maxlen, bool late)
{
    handlesys->FindHandleType("GlobalFwd", &g_GlobalFwdType);
    handlesys->FindHandleType("PrivateFwd", &g_PrivateFwdType);

    return true;
}

void PublicInterfaceWrapper::SDK_OnAllLoaded()
{
    sharesys->AddNatives(myself, natives);
}