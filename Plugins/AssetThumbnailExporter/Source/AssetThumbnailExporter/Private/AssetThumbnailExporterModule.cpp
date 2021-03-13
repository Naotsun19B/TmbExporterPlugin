// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetThumbnailExporterGlobals.h"
#include "AssetThumbnailExporterSettings.h"

DEFINE_LOG_CATEGORY(LogAssetThumbnailExporter);

class FAssetThumbnailExporterModule : public IModuleInterface
{
public:
	// IModuleInterface interface.
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End of IModuleInterface interface.
};

#define LOCTEXT_NAMESPACE "FAssetThumbnailExporterModule"

void FAssetThumbnailExporterModule::StartupModule()
{
	UAssetThumbnailExporterSettings::Register();
}

void FAssetThumbnailExporterModule::ShutdownModule()
{
	UAssetThumbnailExporterSettings::Unregister();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetThumbnailExporterModule, AssetThumbnailExporter)