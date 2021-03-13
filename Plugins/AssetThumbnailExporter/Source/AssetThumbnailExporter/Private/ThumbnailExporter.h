// Copyright 2020 Naotsun. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Exporters/Exporter.h"
#include "ThumbnailExporter.generated.h"

/*
 * Class for exporting asset thumbnails.
 */
UCLASS()
class ASSETTHUMBNAILEXPORTER_API UThumbnailExporter : public UExporter
{
	GENERATED_BODY()

public:
	UThumbnailExporter(const FObjectInitializer& ObjectInitializer);

	// UExporter interface.
	virtual bool SupportsObject(UObject* Object) const override;
	virtual bool ExportBinary(UObject* Object, const TCHAR* Type, FArchive& Ar, FFeedbackContext* Warn, int32 FileIndex = 0, uint32 PortFlags = 0) override;
	// End of UExporter interface.
};