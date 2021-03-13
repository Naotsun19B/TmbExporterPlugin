// Copyright 2020 Naotsun. All Rights Reserved.

#include "ThumbnailExporter.h"
#include "AssetThumbnailExporterGlobals.h"
#include "AssetThumbnailExporterSettings.h"
#include "Misc/FeedbackContext.h"
#include "ObjectTools.h"
#include "Misc/ObjectThumbnail.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"

namespace ThumbnailExporterInternal
{
	EImageFormat ConvertStringToImageFormat(const FString& FormatString)
	{
		if (FormatString == TEXT("PNG"))
		{
			return EImageFormat::PNG;
		}
		else if (FormatString == TEXT("JPEG"))
		{
			return EImageFormat::JPEG;
		}
		else if (FormatString == TEXT("EXR"))
		{
			return EImageFormat::EXR;
		}
		else
		{
			return EImageFormat::Invalid;
		}
	}
}

UThumbnailExporter::UThumbnailExporter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UObject::StaticClass();
	PreferredFormatIndex = TNumericLimits<int32>().Max();

	FormatExtension.Add(TEXT("EXR"));
	FormatDescription.Add(TEXT("Export Thumbnail As EXR"));
	FormatExtension.Add(TEXT("JPEG"));
	FormatDescription.Add(TEXT("Export Thumbnail As JPEG"));
	FormatExtension.Add(TEXT("PNG"));
	FormatDescription.Add(TEXT("Export Thumbnail As PNG"));
}

bool UThumbnailExporter::SupportsObject(UObject* Object) const
{
	auto* Settings = GetDefault<UAssetThumbnailExporterSettings>();
	if (Super::SupportsObject(Object) && IsValid(Settings) && IsValid(Object))
	{
		return (Object->IsAsset() && Settings->SupportsAssetClasses.Contains(Object->GetClass()));
	}

	return false;
}

bool UThumbnailExporter::ExportBinary(UObject* Object, const TCHAR* Type, FArchive& Ar, FFeedbackContext* Warn, int32 FileIndex, uint32 PortFlags)
{
	FVector2D ImageSize(512, 512);
	if (auto* Settings = GetDefault<UAssetThumbnailExporterSettings>())
	{
		ImageSize = Settings->ImageSize;
	}

	FObjectThumbnail ObjectThumbnail;
	ThumbnailTools::RenderThumbnail(
		Object,
		ImageSize.X, ImageSize.Y,
		ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush, NULL,
		&ObjectThumbnail
	);

	if (ObjectThumbnail.IsEmpty())
	{
		Warn->Log(TEXT("The thumbnail object was empty."));
		return false;
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ThumbnailExporterInternal::ConvertStringToImageFormat(Type));
	if (!ImageWrapper.IsValid())
	{
		Warn->Log(TEXT("Failed to get ImageWrapper."));
		return false;
	}
	
	const TArray<uint8>& RawImageData = ObjectThumbnail.AccessImageData();

	if (!ImageWrapper->SetRaw(RawImageData.GetData(), RawImageData.GetAllocatedSize(), ImageSize.X, ImageSize.Y, ERGBFormat::BGRA, 8))
	{
		Warn->Log(TEXT("Could not set data to ImageWrapper."));
		return false;
	}

	const TArray64<uint8>& CompressedData = ImageWrapper->GetCompressed(100);
	if (CompressedData.Num() == 0)
	{
		Warn->Log(TEXT("The compressed data was empty."));
		return false;
	}

	Ar.Serialize((void*)CompressedData.GetData(), CompressedData.GetAllocatedSize());

	return true;
}
