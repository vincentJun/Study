// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Components/RichTextBlockHyperLinkDecorator.h"
#include "UObject/SoftObjectPtr.h"
#include "Rendering/DrawElements.h"
#include "Framework/Text/SlateTextRun.h"
#include "Framework/Text/SlateTextLayout.h"
#include "Slate/SlateGameResources.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"
#include "Math/UnrealMathUtility.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SBox.h"
#include "Kismet/KismetStringLibrary.h"


DEFINE_LOG_CATEGORY(HyperLinkLog);



#define LOCTEXT_NAMESPACE "UMG"



class SRichInlineButton : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRichInlineButton)
	{
		
	}
	SLATE_END_ARGS()


public:
	void Construct(const FArguments& InArgs, const FRichHyperLinkRow* ButtonStyle, const FTextBlockStyle& TextStyle , const FString& text , const TArray<FString>& parm)
	{
		GetHyperLink();
		data.Empty();
		data = parm;
		if (ButtonStyle)
		{
			//measure font size set button size
			const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
			//const float IconSize = FMath::Min((float)FontMeasure->GetMaxCharacterHeight(TextStyle.Font,1.0f) , ButtonStyle->style.)
			ChildSlot
				[
					SNew(SButton)
					.ButtonStyle(&ButtonStyle->style)
				.TextStyle(&TextStyle)
				//.Text(FText::FromString(*text))
				.OnClicked(this, &SRichInlineButton::OnButtonClicked)
				.OnHovered(this, &SRichInlineButton::OnButtonHover)
				[
					SNew(STextBlock)
					.TextStyle(&TextStyle)
					.Text(FText::FromString(*text))
				]
				];
		}
	}
	void Construct(const FArguments& InArgs, const FRichHyperLinkRow* ButtonStyle, const FTextBlockStyle& TextStyle, const FString& text)
	{
		GetHyperLink();
		if (ButtonStyle)
		{
			//measure font size set button size
			const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
			//const float IconSize = FMath::Min((float)FontMeasure->GetMaxCharacterHeight(TextStyle.Font,1.0f) , ButtonStyle->style.)

			ChildSlot
				[
					SNew(SButton)
					.ButtonStyle(&ButtonStyle->style)
					.TextStyle(&TextStyle)
					//.Text(FText::FromString(*text))
					.OnClicked(this, &SRichInlineButton::OnButtonClicked)
					.OnHovered(this,&SRichInlineButton::OnButtonHover)
				[
					SNew(STextBlock)
					.TextStyle(&TextStyle)
					.Text(FText::FromString(*text))
				]
				];
		}
	}

public:

	UFUNCTION(BlueprintCallable, Category = "ButtonEvent")
		FReply OnButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "ButtonEvent")
		void OnButtonHover();
	UFUNCTION(BlueprintCallable, Category = "ButtonEvent")
		void OnButtonPressed();
	UFUNCTION(BlueprintCallable, Category = "ButtonEvent")
		void OnButtonUnHover();
	UFUNCTION(BlueprintCallable, Category = "ButtonEvent")
		void OnButtonReleased();

private:

	TArray<FString> data;
	class URichTextBlockHyperLinkDecorator* HyperLink;

	class URichTextBlockHyperLinkDecorator* GetHyperLink();

};

FReply SRichInlineButton::OnButtonClicked()
{
	UE_LOG(HyperLinkLog, Display, TEXT("on click"));
	if (HyperLink)
	{
		HyperLink->OnClickedHyperLinkButton.Broadcast(data);
	}
	return FReply::Handled();
}


void SRichInlineButton::OnButtonHover()
{
	UE_LOG(HyperLinkLog, Display, TEXT("hover"));
	if (HyperLink)
	{
		HyperLink->OnHoverHyperLinkButton.Broadcast(data);
	}
}

void SRichInlineButton::OnButtonPressed()
{
	UE_LOG(HyperLinkLog, Display, TEXT("pressed"));
	if (HyperLink)
	{
		HyperLink->OnPressedHyperLinkButton.Broadcast(data);
	}
}

void SRichInlineButton::OnButtonUnHover()
{
	UE_LOG(HyperLinkLog, Display, TEXT("release"));
	if (HyperLink)
	{
		HyperLink->OnReleasedHyperLinkButton.Broadcast(data);
	}
}

void SRichInlineButton::OnButtonReleased()
{
	UE_LOG(HyperLinkLog, Display, TEXT("unHover"));
	if (HyperLink)
	{
		HyperLink->OnUnHoverHyperLinkButton.Broadcast(data);
	}
}

class URichTextBlockHyperLinkDecorator* SRichInlineButton::GetHyperLink()
{
	if (HyperLink == NULL)
	{
		return HyperLink = URichTextBlockHyperLinkDecorator::GetInstance();
	}
	return HyperLink;
}

/////////////////////////////////////////////////////

URichTextBlockHyperLinkDecorator* URichTextBlockHyperLinkDecorator::HyperLinkInstance = NULL;
URichTextBlockHyperLinkDecorator::CGarbo URichTextBlockHyperLinkDecorator::garbo;

URichTextBlockHyperLinkDecorator::URichTextBlockHyperLinkDecorator(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}


FRichHyperLinkRow* URichTextBlockHyperLinkDecorator::FindHyperLinkRow(FName TagOrid, bool bWarnIfMissing)
{
	if (ButtonStyleSet)
	{
		FString ContextString;
		return ButtonStyleSet->FindRow<FRichHyperLinkRow>(TagOrid, ContextString, bWarnIfMissing);
	}

	return nullptr;
}


FRichHyperLinkTextRow* URichTextBlockHyperLinkDecorator::FindHyperLinkTextRow(FName TagOrid, bool bWarnIfMissing)
{
	// && TextStyleSet->RowStruct->IsChildOf(FRichHyperLinkTextRow::StaticStruct())
	if (TextStyleSet)
	{
		for (const auto& Entry : TextStyleSet->RowMap)
		{
			FName SubStyleName = Entry.Key;
			if (SubStyleName == TagOrid)
			{
				return (FRichHyperLinkTextRow*)Entry.Value;
			}
		}
	}
	return nullptr;

	
}

class FRichInlineHyperLink : public FRichTextDecorator
{
public:
	FRichInlineHyperLink(URichTextBlock* InOwner, URichTextBlockHyperLinkDecorator* Docorator)
		:FRichTextDecorator(InOwner)
		, HyperLink(Docorator)
	{

	}

public:
	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
	
		if (RunParseResult.Name == TEXT("a") && RunParseResult.MetaData.Contains(TEXT("href")))
		{
			const FTextRange& IDRang = RunParseResult.MetaData[TEXT("href")];

			const FString ID = Text.Mid(IDRang.BeginIndex, IDRang.EndIndex - IDRang.BeginIndex);
			const bool bWarnIfMissing = false;
			const FTextRange* const TIDRang = RunParseResult.MetaData.Find(TEXT("textStyle"));
			if (TIDRang != NULL)
			{
				const FString TID = Text.Mid(TIDRang->BeginIndex, TIDRang->EndIndex - TIDRang->BeginIndex);
				
				return HyperLink->FindHyperLinkRow(*ID, bWarnIfMissing) != nullptr && HyperLink->FindHyperLinkTextRow(*TID, bWarnIfMissing) != nullptr;
			}
			return HyperLink->FindHyperLinkRow(*ID, bWarnIfMissing) != nullptr;
		}
		
		return false;
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& DefaultTextStyle) const override
	{
		const bool bWarnIfMissing = true;
		const FRichHyperLinkRow* HyperLinkRow = HyperLink->FindHyperLinkRow(*RunInfo.MetaData[TEXT("href")], bWarnIfMissing);
		const FString content = RunInfo.Content.ToString();
		FString RangID;
		if (RunInfo.MetaData.Find(TEXT("textStyle")) != NULL)
		{
			RangID = RunInfo.MetaData[TEXT("textStyle")];
		}
		if (RunInfo.MetaData.Find(TEXT("param"))!= NULL)
		{
			HyperLink->PramArray = HyperLink->ParseChar(RunInfo.MetaData[TEXT("param")]);
		}
		if (HyperLink->PramArray.Num() != 0)
		{
			if (RangID != "")
			{
				const FRichHyperLinkTextRow* HyperLinkTextRow = HyperLink->FindHyperLinkTextRow(*RunInfo.MetaData[TEXT("textStyle")], bWarnIfMissing);
				if (HyperLinkTextRow)
				{
					return SNew(SRichInlineButton, HyperLinkRow, HyperLinkTextRow->HypertextStyle, content, HyperLink->PramArray);
				}
			}

			return SNew(SRichInlineButton, HyperLinkRow, DefaultTextStyle, content, HyperLink->PramArray);
		}
		else
		{
			if (RangID != "")
			{
				const FRichHyperLinkTextRow* HyperLinkTextRow = HyperLink->FindHyperLinkTextRow(*RunInfo.MetaData[TEXT("textStyle")], bWarnIfMissing);
				if (HyperLinkTextRow)
				{
					return SNew(SRichInlineButton, HyperLinkRow, HyperLinkTextRow->HypertextStyle, content);
				}
			}

			return SNew(SRichInlineButton, HyperLinkRow, DefaultTextStyle, content);
		}
		
	}

private:
	URichTextBlockHyperLinkDecorator* HyperLink;

};


TSharedPtr<ITextDecorator> URichTextBlockHyperLinkDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	
	return MakeShareable(new FRichInlineHyperLink(InOwner, this));
}




TArray<FString> URichTextBlockHyperLinkDecorator::ParseChar(const FString& InString)
{
	//use ',' cut string
	return UKismetStringLibrary::ParseIntoArray(InString, ",", true);
}

void URichTextBlockHyperLinkDecorator::ButtonClickedCallFun(const TArray<FString>& param)
{
	UE_LOG(HyperLinkLog, Display, TEXT("successful"));
}

URichTextBlockHyperLinkDecorator* URichTextBlockHyperLinkDecorator::BlueprintGetInstance()
{
	if (HyperLinkInstance == NULL)
	{
		HyperLinkInstance = GetInstance();
	}
	return HyperLinkInstance;
	
}

#undef LOCTEXT_NAMESPACE

