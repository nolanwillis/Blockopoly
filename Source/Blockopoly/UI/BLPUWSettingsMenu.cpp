// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPUWSettingsMenu.h"
#include "BLPUWMainMenu.h"
#include "BLPUWPauseMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UBLPUWSettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetDefaultWindowMode();
	SetDefaultScreenRes();
	
	RefreshAllGraphicsBtns();
	
	BindAllGraphicsBtns();
	
	if (!WindowModeComboBox) return;
	WindowModeComboBox->OnSelectionChanged.AddDynamic(this, &UBLPUWSettingsMenu::WindowModeComboBoxChanged);
	if (!ResComboBox) return;
	ResComboBox->OnSelectionChanged.AddDynamic(this, &UBLPUWSettingsMenu::ResComboBoxChanged);

	if (!ResScaleSlider) return;
	ResScaleSlider->OnValueChanged.AddDynamic(this, &UBLPUWSettingsMenu::ResScaleSliderAdjusted);

	if (!BackBtn) return;
	BackBtn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::BackBtnClicked);
	if (!ApplyBtn) return;
	ApplyBtn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::ApplyBtnClicked);
}

void UBLPUWSettingsMenu::SetDefaultWindowMode() const
{
	const UGameUserSettings* GameUserSettingsPtr = GEngine->GameUserSettings;
	if (!GameUserSettingsPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWSettingsMenu: GameUserSettingsPtr is null")); return; }

	const EWindowMode::Type CurrentWindowMode = GameUserSettingsPtr->GetFullscreenMode();

	if (CurrentWindowMode == EWindowMode::Fullscreen)
	{
		WindowModeComboBox->SetSelectedIndex(0);
	}
	else if (CurrentWindowMode == EWindowMode::Windowed)
	{
		WindowModeComboBox->SetSelectedIndex(1);
	}
	else if (CurrentWindowMode == EWindowMode::WindowedFullscreen)
	{
		WindowModeComboBox->SetSelectedIndex(2);
	}

}
void UBLPUWSettingsMenu::SetDefaultScreenRes() const
{
	const UGameUserSettings* GameUserSettingsPtr = GEngine->GameUserSettings;
	if (!GameUserSettingsPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWSettingsMenu: GameUserSettingsPtr is null")); return; }
	const FIntPoint CurrentScreenRes = GameUserSettingsPtr->GetScreenResolution();
    if (CurrentScreenRes == FIntPoint(1920, 1080))
    {
	    ResComboBox->SetSelectedIndex(0);
    }
	else if (CurrentScreenRes == FIntPoint(2560, 1440))
	{
		ResComboBox->SetSelectedIndex(1);
	}
	else if (CurrentScreenRes == FIntPoint(3840, 2160))
	{
		ResComboBox->SetSelectedIndex(2);
	}
}

void UBLPUWSettingsMenu::ApplyAllGraphicsSettings() const
{
	UGameUserSettings* GameUserSettingsPtr = GEngine->GameUserSettings;
	if (!GameUserSettingsPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWSettingsMenu: GameUserSettingsPtr is null")); return; }
	
	if (WindowMode == "Fullscreen") GameUserSettingsPtr->SetFullscreenMode(EWindowMode::Fullscreen);
	else if (WindowMode == "Windowed") GameUserSettingsPtr->SetFullscreenMode(EWindowMode::Windowed);
	else if (WindowMode == "Borderless") GameUserSettingsPtr->SetFullscreenMode(EWindowMode::WindowedFullscreen);

	if (Resolution == "1920 X 1080") GameUserSettingsPtr->SetScreenResolution(FIntPoint(1920, 1080));
	else if (Resolution == "2560 X 1440") GameUserSettingsPtr->SetScreenResolution(FIntPoint(2560, 1440));
	else if (Resolution == "3840 X 2160") GameUserSettingsPtr->SetScreenResolution(FIntPoint(3840, 2160));
	
	GameUserSettingsPtr->SetVSyncEnabled(VSyncOn);
	GameUserSettingsPtr->SetResolutionScaleValueEx(ResolutionScale);
	GameUserSettingsPtr->SetViewDistanceQuality(DrawDistanceLevelIndex);
	GameUserSettingsPtr->SetAntiAliasingQuality(AntiAliasingLevelIndex);
	GameUserSettingsPtr->SetPostProcessingQuality(AntiAliasingLevelIndex);
	GameUserSettingsPtr->SetShadowQuality(ShadowsLevelIndex);
	GameUserSettingsPtr->SetShadingQuality(ShadingLevelIndex);
	GameUserSettingsPtr->SetReflectionQuality(ReflectionsLevelIndex);
	GameUserSettingsPtr->SetGlobalIlluminationQuality(IlluminationLevelIndex);
	GameUserSettingsPtr->SetTextureQuality(TexturesLevelIndex);
	GameUserSettingsPtr->SetVisualEffectQuality(EffectsLevelIndex);
	GameUserSettingsPtr->SetFoliageQuality(FoliageLevelIndex);

	GameUserSettingsPtr->ApplySettings(true);
}

void UBLPUWSettingsMenu::RefreshAllGraphicsBtns() const
{
	RefreshVSyncBtns();
	RefreshDrawDistanceBtns();
	RefreshAntiAliasingBtns();
	RefreshPostProcessingBtns();
	RefreshShadowsBtns();
	RefreshShadingBtns();
	RefreshReflectionsBtns();
	RefreshIlluminationBtns();
	RefreshTexturesBtns();
	RefreshEffectsBtns();
	RefreshFoliageBtns();
}
void UBLPUWSettingsMenu::RefreshVSyncBtns() const
{
	if (VSyncOn)
	{
		VSync_Off_Btn->SetRenderOpacity(0.5f);
		VSync_On_Btn->SetRenderOpacity(1.0f);
	}
	else
	{
		VSync_Off_Btn->SetRenderOpacity(1.0f);
		VSync_On_Btn->SetRenderOpacity(0.5f);
	}
}
void UBLPUWSettingsMenu::RefreshDrawDistanceBtns() const
{
	switch (DrawDistanceLevelIndex)
	{
	case 0:
		DD_Low_Btn->SetRenderOpacity(1.0f);
		DD_Medium_Btn->SetRenderOpacity(0.5f);
		DD_High_Btn->SetRenderOpacity(0.5f);
		DD_Ultra_Btn->SetRenderOpacity(0.5f);
        DD_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		DD_Low_Btn->SetRenderOpacity(0.5f);
		DD_Medium_Btn->SetRenderOpacity(1.0f);
		DD_High_Btn->SetRenderOpacity(0.5f);
		DD_Ultra_Btn->SetRenderOpacity(0.5f);
		DD_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		DD_Low_Btn->SetRenderOpacity(0.5f);
		DD_Medium_Btn->SetRenderOpacity(0.5f);
		DD_High_Btn->SetRenderOpacity(1.0f);
		DD_Ultra_Btn->SetRenderOpacity(0.5f);
		DD_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		DD_Low_Btn->SetRenderOpacity(0.5f);
		DD_Medium_Btn->SetRenderOpacity(0.5f);
		DD_High_Btn->SetRenderOpacity(0.5f);
		DD_Ultra_Btn->SetRenderOpacity(1.0f);
		DD_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		DD_Low_Btn->SetRenderOpacity(0.5f);
		DD_Medium_Btn->SetRenderOpacity(0.5f);
		DD_High_Btn->SetRenderOpacity(0.5f);
		DD_Ultra_Btn->SetRenderOpacity(0.5f);
		DD_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}
}
void UBLPUWSettingsMenu::RefreshAntiAliasingBtns() const
{
	switch (AntiAliasingLevelIndex)
	{
	case 0:
		AA_Low_Btn->SetRenderOpacity(1.0f);
		AA_Medium_Btn->SetRenderOpacity(0.5f);
		AA_High_Btn->SetRenderOpacity(0.5f);
		AA_Ultra_Btn->SetRenderOpacity(0.5f);
		AA_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		AA_Low_Btn->SetRenderOpacity(0.5f);
		AA_Medium_Btn->SetRenderOpacity(1.0f);
		AA_High_Btn->SetRenderOpacity(0.5f);
		AA_Ultra_Btn->SetRenderOpacity(0.5f);
		AA_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		AA_Low_Btn->SetRenderOpacity(0.5f);
		AA_Medium_Btn->SetRenderOpacity(0.5f);
		AA_High_Btn->SetRenderOpacity(1.0f);
		AA_Ultra_Btn->SetRenderOpacity(0.5f);
		AA_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		AA_Low_Btn->SetRenderOpacity(0.5f);
		AA_Medium_Btn->SetRenderOpacity(0.5f);
		AA_High_Btn->SetRenderOpacity(0.5f);
		AA_Ultra_Btn->SetRenderOpacity(1.0f);
		AA_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		AA_Low_Btn->SetRenderOpacity(0.5f);
		AA_Medium_Btn->SetRenderOpacity(0.5f);
		AA_High_Btn->SetRenderOpacity(0.5f);
		AA_Ultra_Btn->SetRenderOpacity(0.5f);
		AA_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}
}
void UBLPUWSettingsMenu::RefreshPostProcessingBtns() const
{
	switch (PostProcessingLevelIndex)
	{
	case 0:
		PP_Low_Btn->SetRenderOpacity(1.0f);
		PP_Medium_Btn->SetRenderOpacity(0.5f);
		PP_High_Btn->SetRenderOpacity(0.5f);
		PP_Ultra_Btn->SetRenderOpacity(0.5f);
		PP_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		PP_Low_Btn->SetRenderOpacity(0.5f);
		PP_Medium_Btn->SetRenderOpacity(1.0f);
		PP_High_Btn->SetRenderOpacity(0.5f);
		PP_Ultra_Btn->SetRenderOpacity(0.5f);
		PP_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		PP_Low_Btn->SetRenderOpacity(0.5f);
		PP_Medium_Btn->SetRenderOpacity(0.5f);
		PP_High_Btn->SetRenderOpacity(1.0f);
		PP_Ultra_Btn->SetRenderOpacity(0.5f);
		PP_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		PP_Low_Btn->SetRenderOpacity(0.5f);
		PP_Medium_Btn->SetRenderOpacity(0.5f);
		PP_High_Btn->SetRenderOpacity(0.5f);
		PP_Ultra_Btn->SetRenderOpacity(1.0f);
		PP_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		PP_Low_Btn->SetRenderOpacity(0.5f);
		PP_Medium_Btn->SetRenderOpacity(0.5f);
		PP_High_Btn->SetRenderOpacity(0.5f);
		PP_Ultra_Btn->SetRenderOpacity(0.5f);
		PP_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}
}
void UBLPUWSettingsMenu::RefreshShadowsBtns() const
{
	switch (ShadowsLevelIndex)
	{
	case 0:
		SH_Low_Btn->SetRenderOpacity(1.0f);
		SH_Medium_Btn->SetRenderOpacity(0.5f);
		SH_High_Btn->SetRenderOpacity(0.5f);
		SH_Ultra_Btn->SetRenderOpacity(0.5f);
		SH_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		SH_Low_Btn->SetRenderOpacity(0.5f);
		SH_Medium_Btn->SetRenderOpacity(1.0f);
		SH_High_Btn->SetRenderOpacity(0.5f);
		SH_Ultra_Btn->SetRenderOpacity(0.5f);
		SH_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		SH_Low_Btn->SetRenderOpacity(0.5f);
		SH_Medium_Btn->SetRenderOpacity(0.5f);
		SH_High_Btn->SetRenderOpacity(1.0f);
		SH_Ultra_Btn->SetRenderOpacity(0.5f);
		SH_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		SH_Low_Btn->SetRenderOpacity(0.5f);
		SH_Medium_Btn->SetRenderOpacity(0.5f);
		SH_High_Btn->SetRenderOpacity(0.5f);
		SH_Ultra_Btn->SetRenderOpacity(1.0f);
		SH_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		SH_Low_Btn->SetRenderOpacity(0.5f);
		SH_Medium_Btn->SetRenderOpacity(0.5f);
		SH_High_Btn->SetRenderOpacity(0.5f);
		SH_Ultra_Btn->SetRenderOpacity(0.5f);
		SH_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}
}
void UBLPUWSettingsMenu::RefreshShadingBtns() const
{
	switch (ShadingLevelIndex)
	{
	case 0:
		SD_Low_Btn->SetRenderOpacity(1.0f);
		SD_Medium_Btn->SetRenderOpacity(0.5f);
		SD_High_Btn->SetRenderOpacity(0.5f);
		SD_Ultra_Btn->SetRenderOpacity(0.5f);
		SD_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		SD_Low_Btn->SetRenderOpacity(0.5f);
		SD_Medium_Btn->SetRenderOpacity(1.0f);
		SD_High_Btn->SetRenderOpacity(0.5f);
		SD_Ultra_Btn->SetRenderOpacity(0.5f);
		SD_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		SD_Low_Btn->SetRenderOpacity(0.5f);
		SD_Medium_Btn->SetRenderOpacity(0.5f);
		SD_High_Btn->SetRenderOpacity(1.0f);
		SD_Ultra_Btn->SetRenderOpacity(0.5f);
		SD_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		SD_Low_Btn->SetRenderOpacity(0.5f);
		SD_Medium_Btn->SetRenderOpacity(0.5f);
		SD_High_Btn->SetRenderOpacity(0.5f);
		SD_Ultra_Btn->SetRenderOpacity(1.0f);
		SD_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		SD_Low_Btn->SetRenderOpacity(0.5f);
		SD_Medium_Btn->SetRenderOpacity(0.5f);
		SD_High_Btn->SetRenderOpacity(0.5f);
		SD_Ultra_Btn->SetRenderOpacity(0.5f);
		SD_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}	
}
void UBLPUWSettingsMenu::RefreshReflectionsBtns() const
{
	switch (ReflectionsLevelIndex)
	{
	case 0:
		RF_Low_Btn->SetRenderOpacity(1.0f);
		RF_Medium_Btn->SetRenderOpacity(0.5f);
		RF_High_Btn->SetRenderOpacity(0.5f);
		RF_Ultra_Btn->SetRenderOpacity(0.5f);
		RF_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		RF_Low_Btn->SetRenderOpacity(0.5f);
		RF_Medium_Btn->SetRenderOpacity(1.0f);
		RF_High_Btn->SetRenderOpacity(0.5f);
		RF_Ultra_Btn->SetRenderOpacity(0.5f);
		RF_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		RF_Low_Btn->SetRenderOpacity(0.5f);
		RF_Medium_Btn->SetRenderOpacity(0.5f);
		RF_High_Btn->SetRenderOpacity(1.0f);
		RF_Ultra_Btn->SetRenderOpacity(0.5f);
		RF_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		RF_Low_Btn->SetRenderOpacity(0.5f);
		RF_Medium_Btn->SetRenderOpacity(0.5f);
		RF_High_Btn->SetRenderOpacity(0.5f);
		RF_Ultra_Btn->SetRenderOpacity(1.0f);
		RF_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		RF_Low_Btn->SetRenderOpacity(0.5f);
		RF_Medium_Btn->SetRenderOpacity(0.5f);
		RF_High_Btn->SetRenderOpacity(0.5f);
		RF_Ultra_Btn->SetRenderOpacity(0.5f);
		RF_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}
}
void UBLPUWSettingsMenu::RefreshIlluminationBtns() const
{
	switch (IlluminationLevelIndex)
	{
	case 0:
		IL_Low_Btn->SetRenderOpacity(1.0f);
		IL_Medium_Btn->SetRenderOpacity(0.5f);
		IL_High_Btn->SetRenderOpacity(0.5f);
		IL_Ultra_Btn->SetRenderOpacity(0.5f);
		IL_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		IL_Low_Btn->SetRenderOpacity(0.5f);
		IL_Medium_Btn->SetRenderOpacity(1.0f);
		IL_High_Btn->SetRenderOpacity(0.5f);
		IL_Ultra_Btn->SetRenderOpacity(0.5f);
		IL_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		IL_Low_Btn->SetRenderOpacity(0.5f);
		IL_Medium_Btn->SetRenderOpacity(0.5f);
		IL_High_Btn->SetRenderOpacity(1.0f);
		IL_Ultra_Btn->SetRenderOpacity(0.5f);
		IL_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		IL_Low_Btn->SetRenderOpacity(0.5f);
		IL_Medium_Btn->SetRenderOpacity(0.5f);
		IL_High_Btn->SetRenderOpacity(0.5f);
		IL_Ultra_Btn->SetRenderOpacity(1.0f);
		IL_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		IL_Low_Btn->SetRenderOpacity(0.5f);
		IL_Medium_Btn->SetRenderOpacity(0.5f);
		IL_High_Btn->SetRenderOpacity(0.5f);
		IL_Ultra_Btn->SetRenderOpacity(0.5f);
		IL_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}
}
void UBLPUWSettingsMenu::RefreshTexturesBtns() const
{
	switch (TexturesLevelIndex)
	{
	case 0:
		TX_Low_Btn->SetRenderOpacity(1.0f);
		TX_Medium_Btn->SetRenderOpacity(0.5f);
		TX_High_Btn->SetRenderOpacity(0.5f);
		TX_Ultra_Btn->SetRenderOpacity(0.5f);
		TX_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		TX_Low_Btn->SetRenderOpacity(0.5f);
		TX_Medium_Btn->SetRenderOpacity(1.0f);
		TX_High_Btn->SetRenderOpacity(0.5f);
		TX_Ultra_Btn->SetRenderOpacity(0.5f);
		TX_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		TX_Low_Btn->SetRenderOpacity(0.5f);
		TX_Medium_Btn->SetRenderOpacity(0.5f);
		TX_High_Btn->SetRenderOpacity(1.0f);
		TX_Ultra_Btn->SetRenderOpacity(0.5f);
		TX_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		TX_Low_Btn->SetRenderOpacity(0.5f);
		TX_Medium_Btn->SetRenderOpacity(0.5f);
		TX_High_Btn->SetRenderOpacity(0.5f);
		TX_Ultra_Btn->SetRenderOpacity(1.0f);
		TX_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		TX_Low_Btn->SetRenderOpacity(0.5f);
		TX_Medium_Btn->SetRenderOpacity(0.5f);
		TX_High_Btn->SetRenderOpacity(0.5f);
		TX_Ultra_Btn->SetRenderOpacity(0.5f);
		TX_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}
}
void UBLPUWSettingsMenu::RefreshEffectsBtns() const
{
	switch (EffectsLevelIndex)
	{
	case 0:
		EF_Low_Btn->SetRenderOpacity(1.0f);
		EF_Medium_Btn->SetRenderOpacity(0.5f);
		EF_High_Btn->SetRenderOpacity(0.5f);
		EF_Ultra_Btn->SetRenderOpacity(0.5f);
		EF_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		EF_Low_Btn->SetRenderOpacity(0.5f);
		EF_Medium_Btn->SetRenderOpacity(1.0f);
		EF_High_Btn->SetRenderOpacity(0.5f);
		EF_Ultra_Btn->SetRenderOpacity(0.5f);
		EF_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		EF_Low_Btn->SetRenderOpacity(0.5f);
		EF_Medium_Btn->SetRenderOpacity(0.5f);
		EF_High_Btn->SetRenderOpacity(1.0f);
		EF_Ultra_Btn->SetRenderOpacity(0.5f);
		EF_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		EF_Low_Btn->SetRenderOpacity(0.5f);
		EF_Medium_Btn->SetRenderOpacity(0.5f);
		EF_High_Btn->SetRenderOpacity(0.5f);
		EF_Ultra_Btn->SetRenderOpacity(1.0f);
		EF_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		EF_Low_Btn->SetRenderOpacity(0.5f);
		EF_Medium_Btn->SetRenderOpacity(0.5f);
		EF_High_Btn->SetRenderOpacity(0.5f);
		EF_Ultra_Btn->SetRenderOpacity(0.5f);
		EF_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}
}
void UBLPUWSettingsMenu::RefreshFoliageBtns() const
{
	switch (FoliageLevelIndex)
	{
	case 0:
		FL_Low_Btn->SetRenderOpacity(1.0f);
		FL_Medium_Btn->SetRenderOpacity(0.5f);
		FL_High_Btn->SetRenderOpacity(0.5f);
		FL_Ultra_Btn->SetRenderOpacity(0.5f);
		FL_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 1:
		FL_Low_Btn->SetRenderOpacity(0.5f);
		FL_Medium_Btn->SetRenderOpacity(1.0f);
		FL_High_Btn->SetRenderOpacity(0.5f);
		FL_Ultra_Btn->SetRenderOpacity(0.5f);
		FL_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 2:
		FL_Low_Btn->SetRenderOpacity(0.5f);
		FL_Medium_Btn->SetRenderOpacity(0.5f);
		FL_High_Btn->SetRenderOpacity(1.0f);
		FL_Ultra_Btn->SetRenderOpacity(0.5f);
		FL_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 3:
		FL_Low_Btn->SetRenderOpacity(0.5f);
		FL_Medium_Btn->SetRenderOpacity(0.5f);
		FL_High_Btn->SetRenderOpacity(0.5f);
		FL_Ultra_Btn->SetRenderOpacity(1.0f);
		FL_Max_Btn->SetRenderOpacity(0.5f);
		break;
	case 4:
		FL_Low_Btn->SetRenderOpacity(0.5f);
		FL_Medium_Btn->SetRenderOpacity(0.5f);
		FL_High_Btn->SetRenderOpacity(0.5f);
		FL_Ultra_Btn->SetRenderOpacity(0.5f);
		FL_Max_Btn->SetRenderOpacity(1.0f);
		break;
	default:
		break;
	}
}

void UBLPUWSettingsMenu::BindAllGraphicsBtns() const
{
	BindVSyncBtns();
	BindDrawDistanceBtns();
	BindAntiAliasingBtns();
	BindPostProcessingBtns();
	BindShadowsBtns();
	BindShadingBtns();
	BindReflectionsBtns();
	BindIlluminationBtns();
	BindTexturesBtns();
	BindEffectsBtns();
	BindFoliageBtns();
}
void UBLPUWSettingsMenu::BindVSyncBtns() const
{
	if (!VSync_Off_Btn) return;
  	VSync_Off_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::VSync_Off_BtnClicked);
  	if (!VSync_On_Btn) return;
  	VSync_On_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::VSync_On_BtnClicked);
}
void UBLPUWSettingsMenu::BindDrawDistanceBtns() const
{
	if (!DD_Low_Btn) return;
 	DD_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::DD_Low_BtnClicked);
 	if (!DD_Medium_Btn) return;
 	DD_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::DD_Medium_BtnClicked);
 	if (!DD_High_Btn) return;
 	DD_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::DD_High_BtnClicked);
 	if (!DD_Ultra_Btn) return;
 	DD_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::DD_Ultra_BtnClicked);
 	if (!DD_Max_Btn) return;
 	DD_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::DD_Max_BtnClicked);
}
void UBLPUWSettingsMenu::BindAntiAliasingBtns() const
{
	if (!AA_Low_Btn) return;
    AA_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::AA_Low_BtnClicked);
    if (!AA_Medium_Btn) return;
    AA_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::AA_Medium_BtnClicked);
    if (!AA_High_Btn) return;
    AA_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::AA_High_BtnClicked);
    if (!AA_Ultra_Btn) return;
    AA_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::AA_Ultra_BtnClicked);
    if (!DD_Max_Btn) return;
    AA_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::AA_Max_BtnClicked);
}
void UBLPUWSettingsMenu::BindPostProcessingBtns() const
{
	if (!PP_Low_Btn) return;
    PP_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::PP_Low_BtnClicked);
    if (!PP_Medium_Btn) return;
    PP_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::PP_Medium_BtnClicked);
    if (!PP_High_Btn) return;
    PP_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::PP_High_BtnClicked);
    if (!PP_Ultra_Btn) return;
    PP_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::PP_Ultra_BtnClicked);
    if (!DD_Max_Btn) return;
    PP_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::PP_Max_BtnClicked);
}
void UBLPUWSettingsMenu::BindShadowsBtns() const
{
	if (!SH_Low_Btn) return;
  	SH_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SH_Low_BtnClicked);
  	if (!SH_Medium_Btn) return;
  	SH_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SH_Medium_BtnClicked);
  	if (!SH_High_Btn) return;
  	SH_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SH_High_BtnClicked);
  	if (!SH_Ultra_Btn) return;
  	SH_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SH_Ultra_BtnClicked);
  	if (!SH_Max_Btn) return;
  	SH_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SH_Max_BtnClicked);
}
void UBLPUWSettingsMenu::BindShadingBtns() const
{
	if (!SD_Low_Btn) return;
   	SD_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SD_Low_BtnClicked);
   	if (!SD_Medium_Btn) return;
   	SD_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SD_Medium_BtnClicked);
   	if (!SD_High_Btn) return;
   	SD_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SD_High_BtnClicked);
   	if (!SD_Ultra_Btn) return;
   	SD_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SD_Ultra_BtnClicked);
   	if (!SD_Max_Btn) return;
   	SD_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::SD_Max_BtnClicked);
}
void UBLPUWSettingsMenu::BindReflectionsBtns() const
{
	if (!RF_Low_Btn) return;
   	RF_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::RF_Low_BtnClicked);
   	if (!RF_Medium_Btn) return;
   	RF_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::RF_Medium_BtnClicked);
   	if (!RF_High_Btn) return;
   	RF_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::RF_High_BtnClicked);
   	if (!RF_Ultra_Btn) return;
   	RF_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::RF_Ultra_BtnClicked);
   	if (!RF_Max_Btn) return;
   	RF_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::RF_Max_BtnClicked);
}
void UBLPUWSettingsMenu::BindIlluminationBtns() const
{
	if (!IL_Low_Btn) return;
  	IL_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::IL_Low_BtnClicked);
  	if (!IL_Medium_Btn) return;
  	IL_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::IL_Medium_BtnClicked);
  	if (!IL_High_Btn) return;
  	IL_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::IL_High_BtnClicked);
  	if (!IL_Ultra_Btn) return;
  	IL_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::IL_Ultra_BtnClicked);
  	if (!IL_Max_Btn) return;
  	IL_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::IL_Max_BtnClicked);
}
void UBLPUWSettingsMenu::BindTexturesBtns() const
{
	if (!TX_Low_Btn) return;
  	TX_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::TX_Low_BtnClicked);
  	if (!TX_Medium_Btn) return;
  	TX_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::TX_Medium_BtnClicked);
  	if (!TX_High_Btn) return;
  	TX_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::TX_High_BtnClicked);
  	if (!TX_Ultra_Btn) return;
  	TX_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::TX_Ultra_BtnClicked);
  	if (!TX_Max_Btn) return;
  	TX_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::TX_Max_BtnClicked);
}
void UBLPUWSettingsMenu::BindEffectsBtns() const
{
	if (!EF_Low_Btn) return;
  	EF_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::EF_Low_BtnClicked);
  	if (!EF_Medium_Btn) return;
  	EF_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::EF_Medium_BtnClicked);
  	if (!EF_High_Btn) return;
  	EF_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::EF_High_BtnClicked);
  	if (!EF_Ultra_Btn) return;
  	EF_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::EF_Ultra_BtnClicked);
  	if (!EF_Max_Btn) return;
  	EF_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::EF_Max_BtnClicked);
}
void UBLPUWSettingsMenu::BindFoliageBtns() const
{
	if (!FL_Low_Btn) return;
  	FL_Low_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::FL_Low_BtnClicked);
  	if (!FL_Medium_Btn) return;
  	FL_Medium_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::FL_Medium_BtnClicked);
  	if (!FL_High_Btn) return;
  	FL_High_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::FL_High_BtnClicked);
  	if (!FL_Ultra_Btn) return;
  	FL_Ultra_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::FL_Ultra_BtnClicked);
  	if (!FL_Max_Btn) return;
  	FL_Max_Btn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::UBLPUWSettingsMenu::FL_Max_BtnClicked);
}

void UBLPUWSettingsMenu::WindowModeComboBoxChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	WindowMode = SelectedItem;
}
void UBLPUWSettingsMenu::ResComboBoxChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	Resolution = SelectedItem;
}

void UBLPUWSettingsMenu::ResScaleSliderAdjusted(const float Value)
{
	ResolutionScale = Value;
	if (ResScaleTextBlock)
	{
		const FString NewValue = FString::FromInt(Value) + "%";
		ResScaleTextBlock->SetText(FText::FromString(NewValue));
	}
}

void UBLPUWSettingsMenu::BackBtnClicked()
{
	if (MainMenuParent)
	{
		MainMenuParent->CloseSettingsMenu();
	}
	else if (PauseMenuParent)
	{
		PauseMenuParent->CloseSettingsMenu();
	}
}


