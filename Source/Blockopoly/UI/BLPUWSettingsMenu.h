// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "BLPUWSettingsMenu.generated.h"

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
	void SetParent(UBLPUWMainMenu* InParent) { Parent = InParent; }

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	int DrawDistanceLevelIndex = 0;
	
	UPROPERTY()
	int AntiAliasingLevelIndex;
	UPROPERTY()
	int PostProcessingLevelIndex;
	UPROPERTY()
	int ShadowsLevelIndex;
	UPROPERTY()
	int ShadingLevelIndex;
	UPROPERTY()
	int ReflectionsLevelIndex;
	UPROPERTY()
	int IlluminationLevelIndex;
	UPROPERTY()
	int TexturesLevelIndex;
	

	
	UPROPERTY()
	UBLPUWMainMenu* Parent;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* WindowModeComboBox;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResComboBox;

	UPROPERTY(meta = (BindWidget))
	USlider* ResScaleSlider;
	UPROPERTY(meta = (BindWidget))
	USlider* MasterVolSlider;
	UPROPERTY(meta = (BindWidget))
	USlider* MusicVolSlider;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResScaleTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MasterVolTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MusicVolTextBlock;

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

	// Draw Distance Button Handlers
	UFUNCTION()
	void DD_Low_BtnClicked(){};
	UFUNCTION()
	void DD_Medium_BtnClicked(){};
	UFUNCTION()
	void DD_High_BtnClicked(){};
	UFUNCTION()
	void DD_Ultra_BtnClicked(){};
	UFUNCTION()
	void DD_Max_BtnClicked(){};

	// Anti Aliasing Button Handlers
	UFUNCTION()
	void AA_Low_BtnClicked(){};
	UFUNCTION()
	void AA_Medium_BtnClicked(){};
	UFUNCTION()
	void AA_High_BtnClicked(){};
	UFUNCTION()
	void AA_Ultra_BtnClicked(){};
	UFUNCTION()
	void AA_Max_BtnClicked(){};

	// Post Processing Button Handlers
	UFUNCTION()
	void PP_Low_BtnClicked(){};
	UFUNCTION()
	void PP_Medium_BtnClicked(){};
	UFUNCTION()
	void PP_High_BtnClicked(){};
	UFUNCTION()
	void PP_Ultra_BtnClicked(){};
	UFUNCTION()
	void PP_Max_BtnClicked(){};

	// Shadows Button Handlers
	UFUNCTION()
	void SH_Low_BtnClicked(){};
	UFUNCTION()
	void SH_Medium_BtnClicked(){};
	UFUNCTION()
	void SH_High_BtnClicked(){};
	UFUNCTION()
	void SH_Ultra_BtnClicked(){};
	UFUNCTION()
	void SH_Max_BtnClicked(){};

	// Shading Button Handlers
	UFUNCTION()
	void SD_Low_BtnClicked(){};
	UFUNCTION()
	void SD_Medium_BtnClicked(){};
	UFUNCTION()
	void SD_High_BtnClicked(){};
	UFUNCTION()
	void SD_Ultra_BtnClicked(){};
	UFUNCTION()
	void SD_Max_BtnClicked(){};

	// Reflections Button Handlers
	UFUNCTION()
	void RF_Low_BtnClicked(){};
	UFUNCTION()
	void RF_Medium_BtnClicked(){};
	UFUNCTION()
	void RF_High_BtnClicked(){};
	UFUNCTION()
	void RF_Ultra_BtnClicked(){};
	UFUNCTION()
	void RF_Max_BtnClicked(){};

	// Illumination Button Handlers
	UFUNCTION()
	void IL_Low_BtnClicked(){};
	UFUNCTION()
	void IL_Medium_BtnClicked(){};
	UFUNCTION()
	void IL_High_BtnClicked(){};
	UFUNCTION()
	void IL_Ultra_BtnClicked(){};
	UFUNCTION()
	void IL_Max_BtnClicked(){};

	// Textures Button Handlers
	UFUNCTION()
	void TX_Low_BtnClicked(){};
	UFUNCTION()
	void TX_Medium_BtnClicked(){};
	UFUNCTION()
	void TX_High_BtnClicked(){};
	UFUNCTION()
	void TX_Ultra_BtnClicked(){};
	UFUNCTION()
	void TX_Max_BtnClicked(){};

	// Effects Button Handlers
	UFUNCTION()
	void EF_Low_BtnClicked(){};
	UFUNCTION()
	void EF_Medium_BtnClicked(){};
	UFUNCTION()
	void EF_High_BtnClicked(){};
	UFUNCTION()
	void EF_Ultra_BtnClicked(){};
	UFUNCTION()
	void EF_Max_BtnClicked(){};

	// Foliage Button Handlers
	UFUNCTION()
	void FL_Low_BtnClicked(){};
	UFUNCTION()
	void FL_Medium_BtnClicked(){};
	UFUNCTION()
	void FL_High_BtnClicked(){};
	UFUNCTION()
	void FL_Ultra_BtnClicked(){};
	UFUNCTION()
	void FL_Max_BtnClicked(){};
	
	// Menu Button Handlers
	UFUNCTION()
	void BackBtnClicked();
	UFUNCTION()
	void ApplyBtnClicked();
	
};
