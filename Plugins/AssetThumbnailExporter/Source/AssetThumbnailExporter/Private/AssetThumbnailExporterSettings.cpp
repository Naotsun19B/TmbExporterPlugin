// Copyright 2020 Naotsun. All Rights Reserved.

#include "AssetThumbnailExporterSettings.h"
#include "ISettingsModule.h"
#include "AssetToolsModule.h"
#include "Particles/ParticleSystem.h"

#define LOCTEXT_NAMESPACE "AssetThumbnailExporter"

namespace AssetThumbnailExporterSettingsInternal
{
	static const FName ContainerName = TEXT("Editor");
	static const FName CategoryName = TEXT("Plugins");
	static const FName SectionName = TEXT("GraphPrinterSettings");

	ISettingsModule* GetSettingsModule()
	{
		return FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	}
}

UAssetThumbnailExporterSettings::UAssetThumbnailExporterSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ImageSize(512, 512)
{
	SupportsAssetClasses.Add(UBlueprint::StaticClass());
	SupportsAssetClasses.Add(UMaterial::StaticClass());
	SupportsAssetClasses.Add(UStaticMesh::StaticClass());
	SupportsAssetClasses.Add(USkeletalMesh::StaticClass());
	SupportsAssetClasses.Add(UParticleSystem::StaticClass());
}

void UAssetThumbnailExporterSettings::Register()
{
	if (ISettingsModule* SettingsModule = AssetThumbnailExporterSettingsInternal::GetSettingsModule())
	{
		SettingsModule->RegisterSettings(
			AssetThumbnailExporterSettingsInternal::ContainerName,
			AssetThumbnailExporterSettingsInternal::CategoryName,
			AssetThumbnailExporterSettingsInternal::SectionName,
			LOCTEXT("SettingName", "Asset Thumbnail Exporter"),
			LOCTEXT("SettingDescription", "Editor settings for Asset Thumbnail Exporter"),
			GetMutableDefault<UAssetThumbnailExporterSettings>()
		);
	}
}

void UAssetThumbnailExporterSettings::Unregister()
{
	if (ISettingsModule* SettingsModule = AssetThumbnailExporterSettingsInternal::GetSettingsModule())
	{
		SettingsModule->UnregisterSettings(
			AssetThumbnailExporterSettingsInternal::ContainerName,
			AssetThumbnailExporterSettingsInternal::CategoryName,
			AssetThumbnailExporterSettingsInternal::SectionName
		);
	}
}

void UAssetThumbnailExporterSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty == nullptr)
	{
		return;
	}

	if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UAssetThumbnailExporterSettings, SupportsAssetClasses))
	{
		TArray<TSubclassOf<UObject>> TempArray = SupportsAssetClasses;
		SupportsAssetClasses.Empty();
		for (const auto& Class : TempArray)
		{
			if (IsValid(Class.Get()))
			{
				IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
				if (AssetTools.IsAssetClassSupported(Class) && !SupportsAssetClasses.Contains(Class))
				{
					SupportsAssetClasses.Add(Class);
					continue;
				}
			}

			SupportsAssetClasses.Add(nullptr);
		}
	}
}

#undef LOCTEXT_NAMESPACE
