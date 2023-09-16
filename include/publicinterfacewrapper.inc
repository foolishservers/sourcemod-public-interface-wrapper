#if defined _publicinterfacewrapper_included_
  #endinput
#endif
#define _publicinterfacewrapper_included_

public Extension __ext_publicinterfacewrapper = 
{
	name = "PluginSys Wrapper",
	file = "publicinterfacewrapper.ext",
	autoload = 1,
	required = 1,
};

enum HandleError
{
    HandleError_None = 0,		/**< No error */
    HandleError_Changed,		/**< The handle has been freed and reassigned */
    HandleError_Type,			/**< The handle has a different type registered */
    HandleError_Freed,			/**< The handle has been freed */
    HandleError_Index,			/**< generic internal indexing error */
    HandleError_Access,			/**< No access permitted to free this handle */
    HandleError_Limit,			/**< The limited number of handles has been reached */
    HandleError_Identity,		/**< The identity token was not usable */
    HandleError_Owner,			/**< Owners do not match for this operation */
    HandleError_Version,		/**< Unrecognized security structure version */
    HandleError_Parameter,		/**< An invalid parameter was passed */
    HandleError_NoInherit,		/**< This type cannot be inherited */
};

/**
 * @brief Attempts to load a plugin.
 *
 * @param path		Path and filename of plugin, relative to plugins folder.
 * @param error		Buffer to hold any error message.
 * @param maxlength	Maximum length of error message buffer.
 * @param wasloaded	Stores if the plugin is already loaded.
 * @return			A new plugin handle on success, null otherwise.
 */
native Handle LoadPluginEx(const char[] path, char[] error, int maxlength, bool &wasloaded);

/**
 * @brief Attempts to load a plugin.
 *
 * @param path		Path and filename of plugin, relative to plugins folder.
 * @return			A new plugin handle on success, null otherwise.
 */
stock Handle LoadPlugin(const char[] path)
{
    char error[4];
    bool wasloaded;
    return LoadPluginEx(path, error, sizeof(error), wasloaded);
}

/**
 * @brief Attempts to unload a plugin.
 *
 * @param plugin	Handle.
 * @param err		Error, set on handle failure.
 * @return			True on success, false otherwise.
 */
native bool UnloadPluginEx(Handle handle, HandleError &err);

/**
 * @brief Attempts to unload a plugin.
 *
 * @param plugin	Handle.
 * @return			True on success, false otherwise.
 */
stock bool UnloadPlugin(Handle handle)
{
    HandleError err;
    return UnloadPluginEx(handle, err);
}

/**
 * Adds a function to a private forward's call list.
 *
 * @note Cannot be used during an incomplete call.
 *
 * @param name          Name of global forward.
 * @param plugin        Handle of the plugin that contains the function.
 *                      Pass INVALID_HANDLE to specify the calling plugin.
 * @param func          Function to add to forward.
 * @return              True on success, false otherwise.
 * @error               Invalid global forward name, invalid or corrupt plugin handle, or invalid function.
 */
native bool AddToGlobalForward(const char[] name, Handle plugin, Function func);

/**
 * Removes a function from a private forward's call list.
 *
 * @note Only removes one instance.
 * @note Functions will be removed automatically if their parent plugin is unloaded.
 *
 * @param name          Name of global forward.
 * @param plugin        Handle of the plugin that contains the function.
 *                      Pass INVALID_HANDLE to specify the calling plugin.
 * @param func          Function to remove from forward.
 * @return              True on success, false otherwise.
 * @error               Invalid global forward name, invalid or corrupt plugin handle, or invalid function.
 */
native bool RemoveFromGlobalForward(const char[] name, Handle plugin, Function func);

/**
 * Removes all instances of a plugin from a private forward's call list.
 *
 * @note Functions will be removed automatically if their parent plugin is unloaded.
 *
 * @param name          Name of global forward.
 * @param plugin        Handle of the plugin to remove instances of.
 *                      Pass INVALID_HANDLE to specify the calling plugin.
 * @return              Number of functions removed from forward.
 * @error               Invalid global forward name, invalid or corrupt plugin handle.
 */
native int RemoveAllFromGlobalForward(const char[] name, Handle plugin);