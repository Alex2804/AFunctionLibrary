#ifndef APLUGINSDK_PLUGININFOS_H
#define APLUGINSDK_PLUGININFOS_H

#include "implementation/macros.h"

namespace apl {
    extern "C" {
        struct APLUGINSDK_EXPORT PluginInfo;

        struct APLUGINSDK_EXPORT PluginFeatureInfo
        {
            const PluginInfo* pluginInfo;
            const char* featureGroup;
            const char* featureName;
            const char* returnType;
            const char* parameterList;
            char* parameterTypes;
            char* parameterNames;
            void* functionPointer;
        };

        struct APLUGINSDK_EXPORT PluginClassInfo
        {
            const PluginInfo* pluginInfo;
            const char* interfaceName;
            const char* className;
            void* createInstance;
            void* deleteInstance;
        };

        struct APLUGINSDK_EXPORT PluginInfo
        {
            const char* pluginName;
            size_t apiVersionMajor, apiVersionMinor, apiVersionPatch;
            size_t pluginVersionMajor, pluginVersionMinor, pluginVersionPatch;

            void*(*allocateMemory)(size_t size);
            void(*freeMemory)(void*);

            size_t(*getFeatureCount)();
            const PluginFeatureInfo*(*getFeatureInfo)(size_t index);
            const PluginFeatureInfo * const*(*getFeatureInfos)();

            size_t(*getClassCount)();
            const PluginClassInfo*(*getClassInfo)(size_t index);
            const PluginClassInfo* const*(*getClassInfos)();
        };
    }
}

#endif //APLUGINSDK_PLUGININFOS_H