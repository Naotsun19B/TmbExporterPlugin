// Copyright 2020 Naotsun. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Exporters/Exporter.h"
#include "AssetThumbnailExporterSettings.generated.h"

/*
 * Editor settings for this plugin.
 */
UCLASS(Config = Editor, DefaultConfig)
class ASSETTHUMBNAILEXPORTER_API UAssetThumbnailExporterSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	// Image size when exporting.
	UPROPERTY(EditAnywhere, Config)
	FVector2D ImageSize;

	// List of supports assets for export function.
	// Cannot guarantee behavior other than the assets supported by default.
	UPROPERTY(EditAnywhere, Config)
	TArray<TSubclassOf<UObject>> SupportsAssetClasses;

public:
	// Register - unregister in the editor setting item.
	static void Register();
	static void Unregister();

protected:
	// UObject interface.
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	// End of UObject interface.
};