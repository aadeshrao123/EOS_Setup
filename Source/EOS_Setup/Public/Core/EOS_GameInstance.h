// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EOS_GameInstance.generated.h"

class IOnlineSubsystem;
/**
 * 
 */
UCLASS()
class EOS_SETUP_API UEOS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UEOS_GameInstance();

	virtual void Init() override;
	void Login();

	UFUNCTION(BlueprintCallable)
	void CreateSession(const FName SessionName);

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	UFUNCTION(BlueprintCallable)
	void GetAllFriends();

	void OnGetAllFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	UPROPERTY(BlueprintReadWrite)
	FName CurrentSessionName = FName();

private:
	IOnlineSubsystem* MyOnlineSubsystem = nullptr;
	bool bIsLoggedIn = false;
};
