// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "BLPUWMainMenu.h"
#include "BLPUWSettingsMenu.generated.h"

class UBLPUWPauseMenu;
class UBLPUWMainMenu;
class UButton;
class USlider;
class UComboBoxString;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWSettingsMenu : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	void SetParent(UBLPUWMainMenu* InParent) { MainMenuParent = InParent; }
	void SetParent(UBLPUWPauseMenu* InParent) { PauseMenuParent = InParent; }

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	FString WindowMode = "Fullscreen";
	UPROPERTY()
	FString Resolution = "1920 X 1080";
	UPROPERTY()
	bool VSyncOn = true;
	UPROPERTY()
	float ResolutionScale = 100.0f;
	UPROPERTY()
	int DrawDistanceLevelIndex = 3;
	UPROPERTY()
	int AntiAliasingLevelIndex = 3;
	UPROPERTY()
	int PostProcessingLevelIndex = 3;
	UPROPERTY()
	int ShadowsLevelIndex = 3;
	UPROPERTY()
	int ShadingLevelIndex = 3;
	UPROPERTY()
	int ReflectionsLevelIndex = 3;
	UPROPERTY()
	int IlluminationLevelIndex = 3;
	UPROPERTY()
	int TexturesLevelIndex = 3;
	UPROPERTY()
	int EffectsLevelIndex = 3;
	UPROPERTY()
	int FoliageLevelIndex = 3;

	UPROPERTY()
	UBLPUWMainMenu* MainMenuParent;
	UPROPERTY()
	UBLPUWPauseMenu* PauseMenuParent;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* WindowModeComboBox;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResComboBox;

	UPROPERTY(meta = (BindWidget))
	USlider* ResScaleSlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResScaleTextBlock;

	UPROPERTY(meta = (BindWidget))
	UButton* VSync_Off_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* VSync_On_Btn;
	
	// Draw Distance Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* DD_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* DD_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* DD_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* DD_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* DD_Max_Btn;
	
	// Anti Aliasing Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* AA_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* AA_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* AA_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* AA_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* AA_Max_Btn;

	// Post Processing Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* PP_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* PP_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* PP_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* PP_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* PP_Max_Btn;

	// Shadows Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* SH_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* SH_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* SH_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* SH_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* SH_Max_Btn;

	// Shading Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* SD_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* SD_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* SD_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* SD_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* SD_Max_Btn;

	// Reflections Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* RF_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* RF_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* RF_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* RF_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* RF_Max_Btn;

	// Illumination Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* IL_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* IL_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* IL_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* IL_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* IL_Max_Btn;

	// Textures Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* TX_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* TX_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* TX_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* TX_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* TX_Max_Btn;

	// Effects Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* EF_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* EF_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* EF_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* EF_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* EF_Max_Btn;
	
	// Foliage Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* FL_Low_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* FL_Medium_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* FL_High_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* FL_Ultra_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* FL_Max_Btn;
	
	// Menu buttons
	UPROPERTY(meta = (BindWidget))
	UButton* ApplyBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;

	UFUNCTION()
	void SetDefaultWindowMode() const;
	UFUNCTION()
	void SetDefaultScreenRes() const;
	
	UFUNCTION()
	void ApplyAllGraphicsSettings() const;

	UFUNCTION()
	void RefreshAllGraphicsBtns() const;
	UFUNCTION()
	void RefreshVSyncBtns() const;
	UFUNCTION()
	void RefreshDrawDistanceBtns() const;
	UFUNCTION()
	void RefreshAntiAliasingBtns() const;
	UFUNCTION()
	void RefreshPostProcessingBtns() const;
	UFUNCTION()
	void RefreshShadowsBtns() const;
	UFUNCTION()
	void RefreshShadingBtns() const;
	UFUNCTION()
	void RefreshReflectionsBtns() const;
	UFUNCTION()
	void RefreshIlluminationBtns() const;
	UFUNCTION()
	void RefreshTexturesBtns() const;
	UFUNCTION()
	void RefreshEffectsBtns() const;
	UFUNCTION()
	void RefreshFoliageBtns() const;

	UFUNCTION()
	void BindAllGraphicsBtns() const;
	UFUNCTION()
	void BindVSyncBtns() const;
	UFUNCTION()
    void BindDrawDistanceBtns() const;
    UFUNCTION()
    void BindAntiAliasingBtns() const;
    UFUNCTION()
    void BindPostProcessingBtns() const;
    UFUNCTION()
    void BindShadowsBtns() const;
    UFUNCTION()
    void BindShadingBtns() const;
    UFUNCTION()
    void BindReflectionsBtns() const;
    UFUNCTION()
    void BindIlluminationBtns() const;
    UFUNCTION()
    void BindTexturesBtns() const;
    UFUNCTION()
    void BindEffectsBtns() const;
    UFUNCTION()
    void BindFoliageBtns() const;

	// ComboBox Handlers
	UFUNCTION()
	void WindowModeComboBoxChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void ResComboBoxChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	// Slider Handlers
	UFUNCTION()
	void ResScaleSliderAdjusted(const float Value);
	
	// VSync Button Handlers
	UFUNCTION()
	void VSync_Off_BtnClicked(){ VSyncOn = false; RefreshVSyncBtns(); }
	UFUNCTION()
	void VSync_On_BtnClicked(){ VSyncOn = true;; RefreshVSyncBtns(); }
	
	// Draw Distance Button Handlers
	UFUNCTION()
	void DD_Low_BtnClicked(){ DrawDistanceLevelIndex = 0; RefreshDrawDistanceBtns(); }
	UFUNCTION()
	void DD_Medium_BtnClicked(){ DrawDistanceLevelIndex = 1; RefreshDrawDistanceBtns(); }
	UFUNCTION()
	void DD_High_BtnClicked(){ DrawDistanceLevelIndex = 2; RefreshDrawDistanceBtns(); }
	UFUNCTION()
	void DD_Ultra_BtnClicked(){ DrawDistanceLevelIndex = 3; RefreshDrawDistanceBtns(); }
	UFUNCTION()
	void DD_Max_BtnClicked(){ DrawDistanceLevelIndex = 4; RefreshDrawDistanceBtns(); }
	// Anti Aliasing Button Handlers
	UFUNCTION()
	void AA_Low_BtnClicked(){ AntiAliasingLevelIndex = 0; RefreshAntiAliasingBtns(); }
	UFUNCTION()
	void AA_Medium_BtnClicked(){ AntiAliasingLevelIndex = 1; RefreshAntiAliasingBtns(); }
	UFUNCTION()
	void AA_High_BtnClicked(){ AntiAliasingLevelIndex = 2; RefreshAntiAliasingBtns(); }
	UFUNCTION()
	void AA_Ultra_BtnClicked(){ AntiAliasingLevelIndex = 3; RefreshAntiAliasingBtns(); }
	UFUNCTION()
	void AA_Max_BtnClicked(){ AntiAliasingLevelIndex = 4; RefreshAntiAliasingBtns(); }
	// Post Processing Button Handlers
	UFUNCTION()
	void PP_Low_BtnClicked(){ PostProcessingLevelIndex = 0; RefreshPostProcessingBtns(); }
	UFUNCTION()
	void PP_Medium_BtnClicked(){ PostProcessingLevelIndex = 1; RefreshPostProcessingBtns(); }
	UFUNCTION()
	void PP_High_BtnClicked(){ PostProcessingLevelIndex = 2; RefreshPostProcessingBtns(); }
	UFUNCTION()
	void PP_Ultra_BtnClicked(){ PostProcessingLevelIndex = 3; RefreshPostProcessingBtns(); }
	UFUNCTION()
	void PP_Max_BtnClicked(){ PostProcessingLevelIndex = 4; RefreshPostProcessingBtns(); }
	// Shadows Button Handlers
	UFUNCTION()
	void SH_Low_BtnClicked(){ ShadowsLevelIndex = 0; RefreshShadowsBtns(); }
	UFUNCTION()
	void SH_Medium_BtnClicked(){ ShadowsLevelIndex = 1; RefreshShadowsBtns(); }
	UFUNCTION()
	void SH_High_BtnClicked(){ ShadowsLevelIndex = 2; RefreshShadowsBtns(); }
	UFUNCTION()
	void SH_Ultra_BtnClicked(){ ShadowsLevelIndex = 3; RefreshShadowsBtns(); }
	UFUNCTION()
	void SH_Max_BtnClicked(){ ShadowsLevelIndex = 4; RefreshShadowsBtns(); }
	// Shading Button Handlers
	UFUNCTION()
	void SD_Low_BtnClicked(){ ShadingLevelIndex = 0; RefreshShadingBtns(); }
	UFUNCTION()
	void SD_Medium_BtnClicked(){ ShadingLevelIndex = 1; RefreshShadingBtns(); }
	UFUNCTION()
	void SD_High_BtnClicked(){ ShadingLevelIndex = 2; RefreshShadingBtns(); }
	UFUNCTION()
	void SD_Ultra_BtnClicked(){ ShadingLevelIndex = 3; RefreshShadingBtns(); }
	UFUNCTION()
	void SD_Max_BtnClicked(){ ShadingLevelIndex = 4; RefreshShadingBtns(); }
	// Reflections Button Handlers
	UFUNCTION()
	void RF_Low_BtnClicked(){ ReflectionsLevelIndex = 0; RefreshReflectionsBtns(); }
	UFUNCTION()
	void RF_Medium_BtnClicked(){ ReflectionsLevelIndex = 1; RefreshReflectionsBtns(); }
	UFUNCTION()
	void RF_High_BtnClicked(){ ReflectionsLevelIndex = 2; RefreshReflectionsBtns(); }
	UFUNCTION()
	void RF_Ultra_BtnClicked(){ ReflectionsLevelIndex = 3; RefreshReflectionsBtns(); }
	UFUNCTION()
	void RF_Max_BtnClicked(){ ReflectionsLevelIndex = 4; RefreshReflectionsBtns(); }
	// Illumination Button Handlers
	UFUNCTION()
	void IL_Low_BtnClicked(){ IlluminationLevelIndex = 0; RefreshIlluminationBtns(); }
	UFUNCTION()
	void IL_Medium_BtnClicked(){ IlluminationLevelIndex = 1; RefreshIlluminationBtns(); }
	UFUNCTION()
	void IL_High_BtnClicked(){ IlluminationLevelIndex = 2; RefreshIlluminationBtns(); }
	UFUNCTION()
	void IL_Ultra_BtnClicked(){ IlluminationLevelIndex = 3; RefreshIlluminationBtns(); }
	UFUNCTION()
	void IL_Max_BtnClicked(){ IlluminationLevelIndex = 4; RefreshIlluminationBtns(); }
	// Textures Button Handlers
	UFUNCTION()
	void TX_Low_BtnClicked(){ TexturesLevelIndex = 0; RefreshTexturesBtns(); }
	UFUNCTION()
	void TX_Medium_BtnClicked(){ TexturesLevelIndex = 1; RefreshTexturesBtns(); }
	UFUNCTION()
	void TX_High_BtnClicked(){ TexturesLevelIndex = 2; RefreshTexturesBtns(); }
	UFUNCTION()
	void TX_Ultra_BtnClicked(){ TexturesLevelIndex = 3; RefreshTexturesBtns(); }
	UFUNCTION()
	void TX_Max_BtnClicked(){ TexturesLevelIndex = 4; RefreshTexturesBtns(); }
	// Effects Button Handlers
	UFUNCTION()
	void EF_Low_BtnClicked(){ EffectsLevelIndex = 0; RefreshEffectsBtns(); }
	UFUNCTION()
	void EF_Medium_BtnClicked(){ EffectsLevelIndex = 1; RefreshEffectsBtns(); }
	UFUNCTION()
	void EF_High_BtnClicked(){ EffectsLevelIndex = 2; RefreshEffectsBtns(); }
	UFUNCTION()
	void EF_Ultra_BtnClicked(){ EffectsLevelIndex = 3; RefreshEffectsBtns(); }
	UFUNCTION()
	void EF_Max_BtnClicked(){ EffectsLevelIndex = 4; RefreshEffectsBtns(); }
	// Foliage Button Handlers
	UFUNCTION()
	void FL_Low_BtnClicked(){ FoliageLevelIndex = 0; RefreshFoliageBtns(); }
	UFUNCTION()
	void FL_Medium_BtnClicked(){ FoliageLevelIndex = 1; RefreshFoliageBtns(); }
	UFUNCTION()
	void FL_High_BtnClicked(){ FoliageLevelIndex = 2; RefreshFoliageBtns(); }
	UFUNCTION()
	void FL_Ultra_BtnClicked(){ FoliageLevelIndex = 3; RefreshFoliageBtns(); }
	UFUNCTION()
	void FL_Max_BtnClicked(){ FoliageLevelIndex = 4; RefreshFoliageBtns(); }

	// Menu Button Handlers
	UFUNCTION()
	void BackBtnClicked();

	UFUNCTION()
	void ApplyBtnClicked() { ApplyAllGraphicsSettings(); };
	
};
