# Blockopoly
## Contents
* About the project 
  * Motivation
  * Goals of the project
  * Why the name?
* Components
  * Spaces
  * Turns and Rolling
  * Buying
  * Building
  * Property Management
  * Selling
  * Jail
  * Chance and Chest
* Conclusion
  * Things I would change / could be wrong / stuck out to me
  * Things that would be cool to add
  * Things I learned
  * Whats next for me?

## About the project
### Motivation
#### In 2022 I built my first independent game project (by independent I mean without following a Youtube video or online course) called Dungeon Defender. It was a simple side scroller build with Unity. An ok first project, but it was small and very basic. In the fall after completing the project, I took a C++ course for a senior elective at my university. This course was the most challenging programming course I had taken up to that point and it gave me a lot of confidence to try Unreal Engine again. I started learning game development in Unreal first, but I was terrible at C++ and really didn't like the blueprint system, so I switched to Unity. In the winter after taking the C++ course, I completed all the programming tutorials on the UE5 docs and got a better understanding UE5's game framework. After becoming a little more confident I switched my focus to UE5's gameplay ability system or GAS. I found a Youtube video series and attempted to follow along. I made it about halfway through the first video and was completely lost. It was talking about replication, multiplayer, networking, delegates, and a bunch of other concepts I had not encountered before. So, I hesitantly bought an intermediate UE5 multiplayer course on Udemy (my first attempt at Unreal was centered around two Udemy courses and it didn't go very well). This course was easier than the courses before, the concepts just made sense. I think this was a combination of knowing more C++ and reviewing what I had learned in the courses before. After completing the course, I started a project this project. I pushed my first commit on March 20th, my goal was to finish the project in 2 months. I didn't "finish" the project until August 1st.
### Goals of the project
* Create a session browser.
* Create a lobby with a ready system.
* Create a player list in both the lobby and game.
* Create client notifications that sync across all clients (ex: roll notification) 
* Use an event driven UI (no ticking).
* Fully understand session/replication.
* Create a fully functional monopoly game, with jail, chance cards, chest cards, building, mortgaging, forfeiting, trading, etc.
* Improve my ability to create UI elements that didn't look terrible.

### Why the name?
#### This project was my twin brother’s idea. The original pitch was to build a multiplayer monopoly game the utilized the Chronos blockchain for player payment. I would do the game development part and he was going to do the blockchain stuff (he's into Web3 and decentralized apps). So, we called the project Blockopoly. But the game development part was taking forever, and we decided that the blockchain incorporation was going to limited and kind of gimmicky, so we gave up on the joint effort. I decided to keep the name.
## Components
>### This section contains some highlighted parts of the project with a brief overview of how the code works. All the code shown is an abbreviated version.
### Spaces
#### The space system is what enables the game to move the player around the board and more importantly helps keep track of where each player is at a given time. Below is a list of space classes and subclasses utilized throughout the game (these are both cpp classes and blueprint classes):
*  BLPSpace
   *  BLPChanceSpace
      *  BP_Chance_BLPSpace
   * BLPChestSpace
     * BP_Chest_BLPSpace
   * BLPGoToJailSpace
     * BP_GoToJail_BLPSpace
   * BLPPropertySpace
     * BLPEstatePropertySpace
       * BP_BLPEstatePropertySpace
     * BP_Electric_BLPPropertySpace
     * BP_Water_BLPPropertySpace
     * BP_RailRoad_BLPPropertySpace
   * BLPJailSpace
     * BP_Jail_BLPSpace
   * BLPTaxSpace
     * BP_Tax_BLPSpace
   * BP_Parking_BLPSpace
   * BP_GO_BLPSpace

#### Each space has a SpaceID, Name, and a list of spawn points. 
```  
UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpaceData, meta = (AllowPrivateAccess = true))
FString Name;

UPROPERTY(EditAnywhere, Category = SpaceData, meta = (AllowPrivateAccess = true))
int SpaceID;

TArray<USceneComponent*> SpawnPoints{ SpawnPoint0, SpawnPoint1, SpawnPoint2, SpawnPoint3 };

```
#### The SpaceID is used for identification, the name is what the blueprints reference to setup the Text3D component (which puts 3D text in the world), and the SpawnPoints TArray is used when moving a player.

#### To allow game wide access to all spawned BLPSpaces, each space appends itself to a TArray in the GameState called SpaceList.
```
void ABLPSpace::BeginPlay()
{
	Super::BeginPlay();

	// Add self to SpaceList in GameState
	ABLPGameState* BLPGameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	if (BLPGameStatePtr)
	{
		BLPGameStatePtr->AddToSpaceList(this);
	}
}
```
#### It's important to not that BLPPropertySpaces are part of the SpaceList but they also add themselves to an AvailablePropertySpace list in their BeginPlay functions. This is used to make finding properties that can be purchases more convient.

#### All the blueprint classes listed above are for cosmetic purposes only, the only blueprint scripting they contain is for setting up the Text3D component as mentioned before. The other C++ classes add various functionalites on top of the base BLPSpace class. This wraps up the overview of spaces, more details about the Property, EstateProperty, Chance, and Chest classes are explored later.
---
### Turns and Rolling
#### Turns and rolling took a few implementations to get right, most of the setbacks were cause when attempting to sync roll and turn notifications across users.

#### The turn system revolves around a BLPSpaceID which is assigned to each player in the GameMode when they connect to the session. 

```
void ABLPGameMode::PostLogin(APlayerController* NewPlayer)
{
  ...
	
  TArray<TObjectPtr<APlayerState>> PlayerArray = BLPGameStatePtr->PlayerArray;
  BLPPlayerStatePtr->SetBLPPlayerId(PlayerArray.Num()-1); 

  ...
}
```

#### Originally, this system used the position of the player in the GameState PlayerArray, but that didn't work at all. The PlayerArray dosen't like to keep its order, which caused problems when players left. I explored turn based strategy games on forums, most people recommended creating a separate PlayerArray that could be managed. That seemed like overkill, so I used ids instead.

#### To start the turn system a Server RPC is called in each GameMenu class on construction. It's kind of a weird place to start the turn system, but it's the only place that allowed the turn notifications to sync across all the players. Originally this was just a function called in the GameState on BeginPlay, which set a PlayerUpId in the GameState to 0. The problem with this was that the Host would be setup fine, but the clients wouldn't have their GameMenus constructed in time, so the turn notification would be blank.

```
void ABLPPlayerController::Server_SetInitialTurnStatus_Implementation(ABLPPlayerState* BLPPlayerStateInPtr)
{
	UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameState* BLPGameStatePtr = Cast<ABLPGameState>(World->GetGameState());
	
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: BLPGameStatePtr is null")); return; }
	if (!BLPPlayerStateInPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: BLPPlayerStatePtr is null")); return; }

	BLPPlayerStateInPtr->SetPlayerUpId(0);
}

```
#### This Server RPC is in the PlayerController (along with many other Server RPCs). It simply sets a RepNotify variable called PlayerUpId in the corresponding PlayerState to 0. The RepNotify function in the PlayerState then executes the following delegate:

`DECLARE_MULTICAST_DELEGATE(FPlayerUpIdSignature);`

#### The following function in the GameMenu is bound to that delegate and it displays the name of the player whose turn is up and adjusts the GameMenu accordingly. 

```
void UBLPUWGameMenu::PlayerUpId()
{
	...

    // If its my turn
	if (BLPPlayerStatePtr->GetBLPPlayerId() == BLPPlayerStatePtr->GetPlayerUpId())
	{
		FinishTurnBtn->SetVisibility(ESlateVisibility::Hidden);
		YourTurnText->SetText(FText::FromString("It's your turn"));

		if (BLPPlayerStatePtr->GetJailCounter() == 0)
		{
			// Only let RollBtn be visible if not in jail.
			RollBtn->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			// If in jail, pass true in to HasRolled() so FinishTurnBtn becomes visible.
			HasRolled(true);
			RollBtn->SetVisibility(ESlateVisibility::Hidden);
			if (BLPPlayerStatePtr->GetJailSkipCounter() > 1)                                                   
            SkipJailBtn->SetVisibility(ESlateVisibility::Visible);
		}		
	}
	// If its not my turn
	else
	{
      const ABLPPlayerState* PlayerUpBLPPlayerState = BLPGameStatePtr->
      GetBLPPlayerStateFromId(BLPPlayerStatePtr->GetPlayerUpId());
		  
      if (!PlayerUpBLPPlayerState) 
      { 
        UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: PlayerUpBLPPlayerState is null"));      
        return; 
      }

	  const FString PlayerUpName = PlayerUpBLPPlayerState->GetPlayerName();
    	
      YourTurnText->SetText(FText::FromString(PlayerUpName + " is up"));
      RollBtn->SetVisibility(ESlateVisibility::Hidden);
      FinishTurnBtn->SetVisibility(ESlateVisibility::Hidden);
      BuyBtn->SetVisibility(ESlateVisibility::Hidden);
      SkipJailBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}
```
#### When a player clicks the Finished Turn button, another Server RPC is called that simply advances the PlayerUpId in all the PlayerStates. Which in turn calls the RepNotify function shown above.
### Rolling
#### When it's a given players turn the RollBtn becomes visible, clicking it starts a dice roll by calling Server_Roll in the PlayerController.
```
void ABLPPlayerController::Server_Roll_Implementation(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	...

	const int DieOneValue = FMath::RandRange(1,6);
	const int DieTwoValue = FMath::RandRange(1,6);
	const int TotalDiceValue = DieOneValue + DieTwoValue;
	
	UE_LOG(LogTemp, Warning, TEXT("You rolled a: %d"), TotalDiceValue);

	int NewSpaceID = PlayerStatePtr->GetCurrentSpaceId() + TotalDiceValue;
	const int MaxSpaceID = GameStatePtr->GetSpaceList().Num()-1;
	
	// If we pass go
	if (NewSpaceID > MaxSpaceID)
	{
		NewSpaceID = NewSpaceID-MaxSpaceID-1;
		PlayerStatePtr->AddToBalance(200);
	}
	
	PlayerStatePtr->SetCurrentSpaceId(NewSpaceID);

	UE_LOG(LogTemp, Warning, TEXT("CurrentSpaceId is: %d"), PlayerStatePtr->GetCurrentSpaceId());

	GameStatePtr->AddRollNotificationToUI(TotalDiceValue, PlayerStatePtr);
}

```
#### This RPC generates a random dice roll and calculates which space the player should be moved to. It then adds 200 to the Balance of the player if they pass go, sets the CurrentSpaceId in the PlayerState of the player, then tells the GameState to add a roll notification to the GameMenu of each player. I could've added a roll notification to all the players in this RPC, but I decided to keep all the logic regarding game wide notifications in the GameState.

#### When the roll notification is finished, if the notification is on the screen of the player who's up, Server_ReflectRollInGame is called, this is where the player is moved.

```
void ABLPPlayerController::Server_ReflectRollInGame_Implementation(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
    ...

	const TArray<ABLPSpace*> SpaceList = GameStatePtr->GetSpaceList();
	MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	
	if (GetRemoteRole() == ROLE_AutonomousProxy)
	{
		PlayerStatePtr->Client_SimulateMoveLocally(PlayerStatePtr->GetCurrentSpaceId());
	}
	
	ApplySpaceEffect(PlayerStatePtr, GameStatePtr);
}
```

#### The SpaceList is passed into the MovePlayer function, this function finds the Space with a SpaceId corresponding to the previously updated CurrentSpaceId variable in the PlayerState. It then finds a SpawnPoint on the Space and moves the players Avatar to that SpawnPoint. If the player calling this RPC is a client, the Avatar movement is simulated locally. Then the ApplySpaceEffect function is called.

#### ApplySpaceEffect is where the side effect of the moved to Space is applied. Depending on the type of Space, a different side effect is applied.

```
void ABLPPlayerController::ApplySpaceEffect(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	...

	ABLPSpace* EnteredSpace = GameStatePtr->GetSpaceFromId(EnteredSpaceID);
	if (!EnteredSpace)
	{
		UE_LOG(LogTemp, Warning, TEXT("Entered space could not be found"));
		return;
	}
	if (Cast<ABLPPropertySpace>(EnteredSpace))
	{
		PlayerStatePtr->SetHasRolled(true);
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Property Space Entered"));
		const ABLPPropertySpace* EnteredPropertySpace = Cast<ABLPPropertySpace>(EnteredSpace);
		CheckIfPropertyIsForSale(PlayerStatePtr, GameStatePtr);
		ChargeRent(PlayerStatePtr, GameStatePtr, EnteredPropertySpace);
	}
	else if (Cast<ABLPChanceSpace>(EnteredSpace))
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Chance Space Entered"));
		DrawChanceCard(PlayerStatePtr, GameStatePtr);
	}
	else if (Cast<ABLPChestSpace>(EnteredSpace))
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Chest Space Entered"));
		DrawChestCard(PlayerStatePtr, GameStatePtr);
	}
	else if (Cast<ABLPGoToJailSpace>(EnteredSpace))
	{
		PlayerStatePtr->SetHasRolled(true);
		UE_LOG(LogTemp, Warning, TEXT("GO TO JAIL!!"));
		SendToJail(PlayerStatePtr, GameStatePtr->GetSpaceList());
	}
	else if (Cast<ABLPTaxSpace>(EnteredSpace))
	{
		PlayerStatePtr->SetHasRolled(true);
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Taxes Collected"));
		PlayerStatePtr->AddToBalance(-100);
	}
	else if (Cast<ABLPJailSpace>(EnteredSpace))
	{
		PlayerStatePtr->SetHasRolled(true);
	}
	else
	{
		PlayerStatePtr->SetHasRolled(true);
	}
}
```
#### These side effects are gone into more detail in the sections to come. Along with a side effect, the RepNotify variable HasRolled, in the PlayerState, is also set to true. Its RepNotify function fires a delegate making the FinishTurnBtn visible and allows the Server_FinishTurn to be called. In some cases, HasRolled is called within the side effect function (ex: a player that lands on a chance space, can only finish their turn after the chance card is applied).
---

### Buying Properties
#### When a player lands on a PropertySpace its side effect is called. A PropertySpaces side effect has two parts, the first part checks if the moved to property is for sale with the CheckIfPropertyIsForSale function. The second simply attempts to charge rent (it fails if the player owns said property) and isn't relevant to buying properties.
```
void ABLPPlayerController::CheckIfPropertyIsForSale(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr) const
{
	ABLPSpace* Space = GameStatePtr->GetSpaceFromId(PlayerStatePtr->GetCurrentSpaceId());
	if (const ABLPPropertySpace* PropertySpace = Cast<ABLPPropertySpace>(Space))
	{
		if (PropertySpace->GetOwnerId() == -1)
		{
			PlayerStatePtr->SetCanBuyCurrentProperty(true);
		}
		else
		{
			PlayerStatePtr->SetCanBuyCurrentProperty(false);
		}
	}
}
```

#### This function grabs the PropertySpace the player is currrently located on and checks if the OwnerId of the PropertySpace is -1 (there's no owner). The OwnerId is the BLPPlayerId of the player who owns the PropertySpace. The RepNotify variable CanBuyCurrentProperty is set accordingly. Its RepNotify function fires a delegate, signaling the GameMenu to make the BuyBtn visible.

#### When the BuyBtn is clicked, Server_BuyPropertySpace is called.
```
void ABLPPlayerController::Server_BuyPropertySpace_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	...
	
	if (!PlayerStatePtr->GetBLPPlayerId() == PlayerStatePtr->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}
	
    ABLPPropertySpace* PropertySpaceToPurchase = 
    Cast<ABLPPropertySpace>(GameStatePtr->GetSpaceFromId(PlayerStatePtr->GetCurrentSpaceId()));

	if (!PlayerStatePtr->GetCanBuyCurrentProperty())
	{
		UE_LOG(LogTemp, Warning, TEXT("This property is not available for purchase"));
		return;
	}

	const int RemainingBalance = PlayerStatePtr->GetBalance() - PropertySpaceToPurchase->GetPurchaseCost();
	if (RemainingBalance < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("You need %d more credits"), abs(RemainingBalance));
		return;
	}
	
	PropertySpaceToPurchase->SetOwnerId(PlayerStatePtr->GetBLPPlayerId());
	PlayerStatePtr->AddToOwnedPropertyList(PropertySpaceToPurchase);
	GameStatePtr->RemoveFromAvailablePropertySpaceList(PropertySpaceToPurchase);
	PlayerStatePtr->AddToBalance(-PropertySpaceToPurchase->GetPurchaseCost());
	PlayerStatePtr->SetCanBuyCurrentProperty(false);

	// If this is an estate property, check if player can now build on the family of estate properties.
	if (const ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(PropertySpaceToPurchase))
	{
		UpdateCanBuild(EstatePropertySpace, PlayerStatePtr);
	}
}
```
#### This function checks a couple of things, then sets the OwnerId of the PropertySpace to the calling players BLPPlayerId, adds it to the players OwnedPropertyList in their PlayerState, removes from the AvailablePropertySpaceList in the GameState, and charges the purchase cost.
---
### Building
#### EstatePropertySpaces, are PropertySpaces that can be built on. EstatePropertySpaces have a family color, a color bar, 4 house StaticMeshes, and 1 hotel StaticMesh. The visibility of the houses and hotel are changed based on the number of buildings purchased on an EstatePropertySpace. 

#### Building can only occur if a player owns the entire family of spaces. Each property keeps track of its buildability with a CanBuild flag. When a EstatePropertySpace is purchased the CanBuild flag of itself and its family members are updated with the UpdateCanBuild function.

```
// Updates can build bool for a family of estate properties
void ABLPPlayerController::UpdateCanBuild(const ABLPEstatePropertySpace* EstatePropertySpacePtr, const ABLPPlayerState* BLPPlayerStatePtr) const
{
	TArray<ABLPPropertySpace*> OwnedPropertyList = BLPPlayerStatePtr->GetOwnedPropertyList();
	const FString TargetFamilyColor = EstatePropertySpacePtr->GetFamilyColor();
	TArray<ABLPEstatePropertySpace*> MatchingEstateProperties;

	for (ABLPPropertySpace* PropertySpace : OwnedPropertyList)
	{
		if (ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(PropertySpace))
		{
			if (EstatePropertySpace->GetFamilyColor() == TargetFamilyColor)
			{
				MatchingEstateProperties.Add(EstatePropertySpace);
			}
		}
	}
	if (TargetFamilyColor == "Brown" || TargetFamilyColor == "DarkBlue")
	{
		if (MatchingEstateProperties.Num() == 2)
		{
			for (ABLPEstatePropertySpace* EstatePropertySpace : MatchingEstateProperties )
			{
				EstatePropertySpace->SetCanBuild(true);
			}
		}
	}
	else
	{
		if (MatchingEstateProperties.Num() == 3)
		{
			for (ABLPEstatePropertySpace* EstatePropertySpace : MatchingEstateProperties )
			{
				EstatePropertySpace->SetCanBuild(true);
			}
		}
	}
}
```
#### This function retrieves all owned family members (if any) from the players OwnedPropertyList. If all are owned the CanBuild flag of the EstatePropertySpace being purchased and its family members is set to true. If a player can build on an EstatePropertySpace, the BuildBtn becomes visible in the PropertyMenu.

#### When the build button is clicked, Server_BuyBuilding is called.
```
// Server RPC that enables a user to build a building on a property
void ABLPPlayerController::Server_BuyBuilding_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID)
{
	...

	if (SpaceID < 0) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: SpaceID invalid")); return; }
	
	if (!PlayerStatePtr->GetBLPPlayerId() == PlayerStatePtr->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: It's not your turn!"))
		return;
	}
	
	ABLPEstatePropertySpace* EstatePropertySpace = 
    Cast<ABLPEstatePropertySpace>(GameStatePtr->GetSpaceFromId(SpaceID));

    if (!EstatePropertySpace) 
    { 
      UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: This is not an estate property!")); 
      return; 
    }
	
	if (PlayerStatePtr->GetBLPPlayerId() != EstatePropertySpace->GetOwnerId())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: You don't own this estate property!"))
		return;
	}

	if (!EstatePropertySpace->GetCanBuild())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: You don't own all the %s estate properties "),
       *EstatePropertySpace->GetFamilyColor());
		return;
	}

	PlayerStatePtr->AddToBalance(-EstatePropertySpace->GetBuildingCost());
	const int UpdatedBuildingCount = EstatePropertySpace->GetBuildingCount() + 1;
	EstatePropertySpace->SetBuildingCount(UpdatedBuildingCount);
	UpdateBuildings(EstatePropertySpace, UpdatedBuildingCount);
	UpdateRent(EstatePropertySpace, UpdatedBuildingCount);
}
```
#### When successfully called the building cost is charged, then the EstatePropertySpace's building count, the visibility of its buildings, and its rent are all updated.
---
### Property Management
#### A seperate PropertyMenu, which resides in a WidgetSwitcher of the GameMenu, allows a player to view property information, sell a property to another player, build on properties, and mortgage properties. The main UI is split into the 3 parts: a list of all the player's owned titles, a details panel, and a selling menu.   

#### The OwnedPropertyList is a RepNotify TArray, when the list changes, a delegate is executed in the PlayerState which tells the PropertyMenu to update its PropertyWrapBox (the container that hold all the property title).
```
void UBLPUWPropertyMenu::RefreshPropertyWrapBox()
{
	PropertyWrapBox->ClearChildren();

	UWorld* WorldPtr = GetWorld();
	const ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();

	if (!WorldPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPropertyMenu: World is null")); return; }
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null")); return; }
	
	TArray<ABLPPropertySpace*> OwnedPropertyList = BLPPlayerStatePtr->GetOwnedPropertyList();
	for (ABLPPropertySpace* Property : OwnedPropertyList)
	{
		UBLPUWPropertyTitle* PropertyTitle = CreateWidget<UBLPUWPropertyTitle>(WorldPtr, PropertyTitleClass);
		if (!PropertyTitle) return;

		PropertyTitle->Setup(this, Property);
		PropertyTitle->PropertyNameTextBlock->SetText(FText::FromString(Property->GetName()));

		// Only set the color of the inner border if it's an EstateProperty
		if (const ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(Property))
		{
			FLinearColor CurrColor;
			EstatePropertySpace->GetColor()->GetMaterial()->GetVectorParameterValue(TEXT("Color"), CurrColor);

			PropertyTitle->PropertyTitleBorder->SetBrushColor(CurrColor);
		}
		
		PropertyWrapBox->AddChild(PropertyTitle);
	}
}
```
#### This function clears the PropertyWrapBox and constructs a title for each of the properties. After constructing a PropertyTitle, its Setup function is called. The PropertyMenu is passed as a parameter to the Setup function. This parameter is stored locally in each PropertyTitle. When a PropertyTitle's OnClick delegate is fired, the pointer to the PropertyMenu is used to refresh the PropertyDetails panel and set the SelectedPropertySpace variable in the PropertyMenu class.

```
void UBLPUWPropertyTitle::Setup(UBLPUWPropertyMenu* InParent, ABLPPropertySpace* PropertySpacePtr)
{
	// Bind OnClicked event
	if (!PropertyTitleContainer) return;
	PropertyTitleContainer->OnClicked.AddDynamic(this, &UBLPUWPropertyTitle::OnClick);
	Parent = InParent;
	AssociatedProperty = PropertySpacePtr;
}

void UBLPUWPropertyTitle::OnClick()
{
	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPPlayerStatePtr) 
    { 
      UE_LOG(LogTemp, Warning, TEXT("BLPUWPropertyTitle: BLPPlayerStatePtr is null")); 
      return; 
    }

	Parent->SetSelectedPropertySpace(AssociatedProperty);
	Parent->SetSelectedPropertyTitle(this);
	Parent->RefreshPropertyDetails();
	Parent->RefreshPropertyManagementButtons();
}
```
#### The sell, mortgage, and build buttons are also refreshed. Depending on the PlayerState and GameState certain buttons may be visible or invisible.
---
### Selling
#### If a player can sell a selected PropertySpace, the SellBtn is visible in the PropertyDetails panel. When this button is clicked the PropertyWrapBox is switched for a selling menu. The selling menu has a list of player names, a field for a price input, and a ConfirmBtn. The list of players is updated whenever a player joins, forfeits, or leaves. Only other players, that haven't forfeited are in this list. To sell the SelectedPropertySpace, you would select the player you want to sell to, enter a sale amount and click the ConfirmBtn. The ConfirmBtn calls Server_SendSaleRequest. A FPropertySaleData struct is passed to this function which contains a pointer to the OwningPlayer, TargetPlayer, PropertyToSell and the SalePrice.
```
void ABLPPlayerController::Server_SendSaleRequest_Implementation(const FPropertySaleData& SaleData)
{
	...

	if (!SaleData.OwningPlayer->GetBLPPlayerId() == SaleData.OwningPlayer->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("Its not your turn!"));
		return;
	}

	
  if (SaleData.PropertyToSell->GetIsMortgaged())
	{
		UE_LOG(LogTemp, Warning, TEXT("This property is mortgaged and cannot be sole"));
		return;
	}
	
	if (SaleData.PropertyToSell->GetHasPendingSale())
	{
		UE_LOG(LogTemp, Warning, TEXT("This property already has a sale active"));
		return;
	}

	SaleData.TargetPlayer->Client_AddSaleRequest(SaleData);
	SaleData.PropertyToSell->SetHasPendingSale(true);
}
``` 
#### This RPC checks the SaleData, calls a Client RPC which adds a sale request notification to the targeted player, and sets the HasPendingSale flag in the PropertySpace to true (this prevents building, selling, or mortgaging a property that is currently being sold).

#### The SaleRequest notification has all the relevant information and accept/decline buttons. Both buttons call Server_SendSaleResponse, with the SaleData and a boolean signifying if the sale was accepted. 
```
void ABLPPlayerController::Server_SendSaleResponse_Implementation(const FPropertySaleData& SaleData, const bool Status)
{
	...
	
	if (Status)
	{
		TransferOwnership(SaleData);
		// Only add sale response to target player if they accepted
		SaleData.TargetPlayer->Client_AddSaleResponse(SaleData, Status);
	}

	SaleData.OwningPlayer->Client_AddSaleResponse(SaleData, Status);

	SaleData.PropertyToSell->SetHasPendingSale(false);
}
```
#### If accepted TransferOwnerShip is called and SaleResponse is added to both the seller and buyers screen, which reflects the status of the sale. If declined, a SaleResponse is only sent to the seller.
---
### Jail
#### When a player lands on the GoToJailSpace or draws a go to jail Chance or Chest Card the SendToJail function in the PlayerController is called.
```
void ABLPPlayerController::SendToJail(ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& SpaceList)
{
	...
	
	PlayerStatePtr->SetJailCounter(3);
	PlayerStatePtr->SetCurrentSpaceId(10);
	MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerStatePtr->Client_SimulateMoveLocally(10);
}
```
#### This function sets the RepNotify JailCounter variable to 3 and moves the player to one of the JailCell spawn points on the JailSpace. The RepNotify function executes a delegate, which tells the GameMenu to display the in-jail icon, the amount of turns left in jail, and the SkipJailBtn if the player has get out of jail free cards. When the player finishes their turn, for the next 3 turns, the JailCounter variable is updated. When it reaches 0, the roll btn appears again and they can roll. When the turn system switches to a player who is in Jail, it automatically sets HasRolled to true in their PlayerState (to ensure the FinishTurnBtn is visible).

#### If the player elects to skip Jail by clicking the button, Server_SkipJail is called. 
```
void ABLPPlayerController::Server_SkipJail_Implementation(ABLPPlayerState* PlayerStatePtr)
{
	...
    
	if (!PlayerStatePtr->GetBLPPlayerId() == PlayerStatePtr->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}
	
	if (PlayerStatePtr->GetJailSkipCounter() >= 1)
	{
		PlayerStatePtr->SetJailSkipCounter(PlayerStatePtr->GetJailSkipCounter()-1);
		PlayerStatePtr->SetHasRolled(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You have no get out of jail free cards!"));
	}
}
```
#### The RepNotify variable JailSkipCounter keeps track of the players get out of jail free cards (it's limited to 2). This RPC decrements the JailSkipCounter and ensures the player can't roll again by setting HasRolled to true. The RepNotify function of JailSkipCounter, fires a delegate that updates the JailSkip icon on the GameMenu (it disappears when JailSkipCounter reaches 0).
---
### Chance and Chest
#### The Chance and Chest cards are implemented identically, so I'll only show the Chance cards to keep this long document a little shorter. This component was the most tedious and frustrating part of the project. This component starts in the GameState which has a TArray of 15 function pointers.
```
TArray<BLPGameStateFuncPtr> ChanceCards {&ABLPGameState::ChanceCard0, &ABLPGameState::ChanceCard1, &ABLPGameState::ChanceCard2, &ABLPGameState::ChanceCard3,&ABLPGameState::ChanceCard4, &ABLPGameState::ChanceCard4, &ABLPGameState::ChanceCard5, &ABLPGameState::ChanceCard6, &ABLPGameState::ChanceCard7, &ABLPGameState::ChanceCard8, &ABLPGameState::ChanceCard9, &ABLPGameState::ChanceCard10, &ABLPGameState::ChanceCard11, &ABLPGameState::ChanceCard12, &ABLPGameState::ChanceCard13, &ABLPGameState::ChanceCard14};
```
#### Each function takes the players state as a parameter and applies the effect of the card to the player. When a player lands on a ChanceSpace, DrawChanceCard is called in the PlayerController.

```
void ABLPPlayerController::DrawChanceCard(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr) const 
{	
  int RandomCardIndex = FMath::RandRange(0,GameStatePtr->GetMaxChanceCards()-1);
  while (RandomCardIndex == GameStatePtr->GetCurrentChanceCardIndex())
  {
	RandomCardIndex = FMath::RandRange(0, GameStatePtr->GetMaxChanceCards()-1);
  }
  GameStatePtr->SetCurrentChanceCardIndex(RandomCardIndex);

  FString Description;

  switch (RandomCardIndex)
  {
	case 0:
		Description = "Advance to Boardwalk";
		break;
	case 1:
		Description = "Advance to Go, collect $200";
		break;
	case 2:
		Description = "Advance to Illinois Avenue. If you pass Go, collect $200";
		break;
	case 3:
		Description = "Advance to St. Charles Place. If you pass Go, collect $200";
		break;
	case 4:
		Description = "Advance to the nearest Railroad. If unowned, you may buy it from the"
					  "Bank. If owned, pay the owner twice the rent";
		break;
	case 5:
		Description = "Advance to the nearest Railroad. If unowned, you may buy it from the"
					  "Bank. If owned, pay the owner twice the rent";
		break;
	case 6:
		Description = "Advance to the nearest Utility. If unowned, you may buy it from the"
					  "Bank. If owned, throw the dice and pay the owner ten times the amount"
					  "thrown";
		break;
	case 7:
		Description = "Bank pays you a dividend of $50";
		break;
	case 8:
		Description = "Get Out of Jail Free";
		break;
	case 9:
		Description = "Go back 3 Spaces";
		break;
	case 10:
		Description = "Go to Jail. Go directly to Jail, do not pass Go, do not collect $200.";
		break;
	case 11:
		Description = "Make general repairs on all your property. For each house pay $25."
					  "For each hotel pay $100.";
		break;
	case 12:
		Description = "Speeding fine $15.";
		break;
	case 13:
		Description = "Take a trip to Reading Railroad. If you pass Go, collect $200.";
		break;
	case 14:
		Description = "You have been elected Chairmen of the Board. Pay each player $50.";
		break;
	case 15:
		Description = "Your building loan matures. Collect $150.";
		break;
	default:
		break;
	}
	
	GameStatePtr->AddCardDrawNotificationToUI("Chance", Description, PlayerStatePtr);

```
#### A RandomCardIndex is generated and stored in the SetCurrentChanceCardIndex in the GameState for later. Based on this index the description for the notification is selected and a Chance notification is sent to all players from the GameState. When this notification finishes Server_ExecuteChanceCard is called.

```
void ABLPPlayerController::Server_ExecuteChanceCard_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	GameStatePtr->ExecuteChanceCard(PlayerStatePtr);
	PlayerStatePtr->SetHasRolled(true);
}
```
#### This RPC calls ExecuteChanceCard in the GameState, which executes the function at the index in the ChanceCards array that matches the CurrentChanceCardIndex set earlier. HasRolled is then set to true in the PlayerState, making the player wait to finish their turn until the Chance notification is finished.

#### This whole system has 30 functions each applying different effects. This took forever to test and some of these functions had me stuck troubleshooting for multiple hours. The Community Chest cards were easier than the Chance cards, because most of them just pay or charge the player a certain amount.

#### This is the last component explored in this document. This is by no means all the components of the project, but they cover most of the mechanics in the game. I would also like to note that these components are by no means perfect, they just work, and that's all the really mattered in the long run.
---
## Conclusion  
### Things the I would change / could be wrong / stuck out to me
* To many RPCs: I used a lot of RPCs in this project. Is this normal? I used them mainly to guarantee I was on the server and to make sure game logic wasn't performed on the client (I'm not really concerned with cheating, but I wanted to simulate what it would be like on a real multiplayer game).
* Dependency Injection: almost every function uses a form of DI, mostly to pass either PlayerState or GameState pointers. There were some functions where this was unnecessary and probably could've been implemented differently (this could be a bad or good thing).
* No Dependency Inversion: I read Clean Architecture by Robert Martin while working on this project. The lack of interfaces and abundance of dependencies went against everything the book discussed. I explored some forum posts that mentioned this book and concerns about designing a project properly. Most recommendations where to basically ignore this book and adhere to the framework prescribed by Epic Games. So, I gave up on applying the concepts in Clean Architecture. Still, I feel like this project is cobbled together and could've employed some design principles.
* Use Animations not Timelines: I created my own notification class, that is used by almost every notification in this project. It uses timelines and curves to animate the notifications and call functions when those "animations" finished. I did those solely to learn timelines. The sale response notification uses the blueprint animator, which took a fraction of the time to implement. This wasted a lot of time, but I learned how to use timelines, so it wasn't a total lost. I'm honestly wondering when C++ timelines should even be used.
* Use More Blueprints: The game logic is all done in C++, certain aspects of the game probably would've been faster to implement in blueprint. I just used C++ to see if I could do it. 

### Things that would be cool to add
* Character customization and selection screen. Right now, the pawns are automatically selected, and all have a reflective silver color. The pawns are also boring shapes, not real monopoly pawns.
* A massive, realistic, city surrounding the game board. Maybe even use MegaScans.
* Being able to pause and save games.
* A friend list using Steam OSS.
* Dedicated servers on Azure Playfab or something similar

### Things I learned
* RepNotify functions are not automatically called on the host. This had me stuck for a while.
* Timing is everything. There were several bugs caused by things updating too slow or too fast, this leads into the next bullet.
* Frameworks can be annoying. I now understand why certain people and studios elect to build their own engine (even if its just a small base layer). Having no control over when certain functions are called and getting stuck in caveats of the engine can be time consuming and headache inducing. 
* Delegates can be tricky to debug, especially when they cause crashes.
* Lag can cause problems.
* Multiplayer is hard. State synchronization can be tricky and is something you don't really think about in single player games. 

### What’s next
#### I probably won't attempt another multiplayer project alone. I would if I was part of a larger group. It's just time consuming, and if I'm spending that much time on a project, I want it to be complete, with art, sound, landscapes, a story, etc. I've now built a project in Unreal and Unity, I was thinking about going lower level, maybe learning DirectX or something similar. As of writing this I'll be starting a master’s in Game Programming in about 5 weeks. I can take electives focusing on graphics, ai, animation programming, networking, etc. I'm thinking of focusing on either graphics or networking, but this might change because I don't have a lot of experience with either of these topics. I might build a couple more personal projects, but I'm at the point where I really need to get work experience. This project was a great experience, and I really learned a lot about multiplayer and Unreal Engine 5. If you want to try this game I'll post a build to my portfolio, or maybe to Itch. If you've read this far thank you so much for reading this doc, project synopsis thing. 

  -- Nolan, August 2nd, 2023
