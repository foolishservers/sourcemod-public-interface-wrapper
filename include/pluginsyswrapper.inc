#if defined _pluginsyswrapper_included_
  #endinput
#endif
#define _pluginsyswrapper_included_

public Extension __ext_pluginsyswrapper = 
{
	name = "PluginSys Wrapper",
	file = "pluginsyswrapper.ext",
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
bool UnloadPlugin(Handle handle)
{
    HandleError err;
    return UnloadPluginEx(handle, err);
}