// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerTagWidget.h"

#include "Components/Image.h"

void UPlayerTagWidget::UpdateHoldingFlagStatus(bool bIsHolding)
{
    if (UImage* ImageFlag = Cast<UImage>(GetWidgetFromName(TEXT("Img_Flag"))))
    {
        ImageFlag->SetVisibility(bIsHolding ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}
