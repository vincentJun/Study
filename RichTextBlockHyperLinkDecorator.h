// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateTypes.h"
#include "Framework/Text/TextLayout.h"
#include "Framework/Text/ISlateRun.h"
#include "Framework/Text/ITextDecorator.h"
#include "Components/RichTextBlockDecorator.h"
#include "Engine/DataTable.h"
#include "RichTextBlockHyperLinkDecorator.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(HyperLinkLog, Display, All);

class ISlateStyle;
//delegate for button
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckedHyperLinkButton, const FString&, parms);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedHyperLinkButton, const TArray<FString>& , HyperLinkparameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPressedHyperLinkButton, const TArray<FString>&, HyperLinkparameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoverHyperLinkButton, const TArray<FString>&, HyperLinkparameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnHoverHyperLinkButton, const TArray<FString>&, HyperLinkparameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReleasedHyperLinkButton, const TArray<FString>&, HyperLinkparameter);

/** Simple struct for rich button styles */

USTRUCT()
struct FRichHyperLinkRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Appearance")
		FButtonStyle style;

};

/** Simple struct for rich text style*/
USTRUCT()
struct FRichHyperLinkTextRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Appearance")
		FTextBlockStyle HypertextStyle;
};


/**
 * Allows you to setup an button decorator that can be configured
 * to map certain keys to certain button.  We recommend you subclass this
 * as a blueprint to configure the instance.
 */
UCLASS()
class UMG_API URichTextBlockHyperLinkDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	URichTextBlockHyperLinkDecorator(const FObjectInitializer& ObjectInitializer);

	//find rich hyper link button style row
	FRichHyperLinkRow* FindHyperLinkRow(FName TagOrid, bool bWarnIfMissing);
	//find rich hyper link text block style row
	FRichHyperLinkTextRow* FindHyperLinkTextRow(FName TagOrid, bool bWarnIfMissing);


	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "data")
		TArray<FString> PramArray;

	UPROPERTY(BlueprintAssignable , BlueprintCallable , Category = "HyperLinkButtonDelegate")
		FOnCheckedHyperLinkButton HyperLinkButtonOnClicked;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "HyperLinkButtonDelegate")
		FOnClickedHyperLinkButton OnClickedHyperLinkButton;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "HyperLinkButtonDelegate")
		FOnHoverHyperLinkButton OnHoverHyperLinkButton;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "HyperLinkButtonDelegate")
		FOnPressedHyperLinkButton OnPressedHyperLinkButton;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "HyperLinkButtonDelegate")
		FOnReleasedHyperLinkButton OnReleasedHyperLinkButton;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "HyperLinkButtonDelegate")
		FOnUnHoverHyperLinkButton OnUnHoverHyperLinkButton;

	//slip char
	UFUNCTION(BlueprintCallable, Category = "ParseChar")
		TArray<FString> ParseChar(const FString& InString);

	TMap<FString, TArray<FString>> PramMap;

	//function

	UFUNCTION(BlueprintCallable, Category = "ButtonCallFunction")
		virtual void ButtonClickedCallFun(const TArray<FString>& param);

	UFUNCTION(BlueprintCallable, Category = "HyperInstance")
		URichTextBlockHyperLinkDecorator* BlueprintGetInstance();

	//instance handle
	static URichTextBlockHyperLinkDecorator* GetInstance()
	{
		FCriticalSection sysobj;
		if (HyperLinkInstance == NULL)
		{
			FScopeLock Lock(&sysobj);
			if (HyperLinkInstance == NULL)
			{
				HyperLinkInstance = NewObject<URichTextBlockHyperLinkDecorator>(URichTextBlockHyperLinkDecorator::StaticClass());
				HyperLinkInstance->AddToRoot();
			}
		}
		return HyperLinkInstance;
	}

private:
	static URichTextBlockHyperLinkDecorator* HyperLinkInstance;
	
	class CGarbo
	{
	public:
		~CGarbo()
		{
			if (URichTextBlockHyperLinkDecorator::HyperLinkInstance)
			{
				delete URichTextBlockHyperLinkDecorator::HyperLinkInstance;
			}
		}

	};

	static CGarbo garbo;

protected:
	UPROPERTY(EditAnywhere, Category = Appearance, meta=(RowType = "RichHyperLinkRow"))
		class UDataTable* ButtonStyleSet;

	UPROPERTY(EditAnywhere, Category = Appearance, meta = (RowType = "RichHyperLinkTextRow"))
		class UDataTable* TextStyleSet;

	
};
