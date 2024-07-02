// Fill out your copyright notice in the Description page of Project Settings.


#include "EOS_Setup/Public/Core/EOS_GameInstance.h"

#include "InterchangeResult.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

UEOS_GameInstance::UEOS_GameInstance()
{
}

void UEOS_GameInstance::Init()
{
	Super::Init();
	MyOnlineSubsystem = IOnlineSubsystem::Get();
	Login();
}

void UEOS_GameInstance::Login()
{
	if (IOnlineIdentityPtr Identity = MyOnlineSubsystem->GetIdentityInterface())
	{
		FOnlineAccountCredentials Credentials;
		Credentials.Id = FString("127.0.0.1:8081");
		Credentials.Token = FString("Test12");
		Credentials.Type =FString("developer");

		Identity->OnLoginCompleteDelegates->AddLambda([this](int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
		{
			UE_LOG(LogTemp, Warning, TEXT("Local Use Number = %i, Success = %d, Error %s"), LocalUserNum, bWasSuccessful, *Error);
			bIsLoggedIn = true;
			if (IOnlineIdentityPtr Identity = MyOnlineSubsystem->GetIdentityInterface())
			{
				Identity->ClearOnLoginCompleteDelegates(0, this);
			}
		});
		Identity->Login(0, Credentials);
	}
}

void UEOS_GameInstance::CreateSession(const FName SessionName)
{
	CurrentSessionName = SessionName;
	if (bIsLoggedIn)
	{
		if (MyOnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = MyOnlineSubsystem->GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = false;
				SessionSettings.bIsLANMatch = false;
				SessionSettings.NumPublicConnections = 5;
				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.Set(SEARCH_KEYWORDS, FString("Test12"), EOnlineDataAdvertisementType::ViaOnlineService);

				SessionPtr->OnCreateSessionCompleteDelegates.AddLambda([this](FName SessionName, bool bWasSuccessful)
				{
					UE_LOG(LogTemp, Warning, TEXT("Success %d"), bWasSuccessful);
					if (MyOnlineSubsystem)
					{
						if (IOnlineSessionPtr SessionPtr = MyOnlineSubsystem->GetSessionInterface())
						{
							SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
						}
					}
				});
				SessionPtr->CreateSession(0, CurrentSessionName, SessionSettings);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Create Sessions Because you are not logged in"));
	}
}

void UEOS_GameInstance::DestroySession()
{
	if(bIsLoggedIn)
	{
		if (MyOnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = MyOnlineSubsystem->GetSessionInterface())
			{
				SessionPtr->OnDestroySessionCompleteDelegates.AddLambda([this](FName SessionName, bool bWasSuccessful)
				{
					UE_LOG(LogTemp, Warning, TEXT("Session Destroyed %d"), bWasSuccessful);
					if (MyOnlineSubsystem)
					{
						if (IOnlineSessionPtr SessionPtr = MyOnlineSubsystem->GetSessionInterface())
						{
							SessionPtr->ClearOnDestroySessionCompleteDelegates(this);
						}
					}
				});
				SessionPtr->DestroySession(CurrentSessionName);
			}
		}
	}
}

void UEOS_GameInstance::GetAllFriends()
{
	if(bIsLoggedIn)
	{
		if (MyOnlineSubsystem)
		{
			if (IOnlineFriendsPtr FriendsPtr = MyOnlineSubsystem->GetFriendsInterface())
			{
				FriendsPtr->ReadFriendsList(0, FString(""), FOnReadFriendsListComplete::CreateUObject(this, &UEOS_GameInstance::OnGetAllFriendsListComplete));
			}
		}
	}
}

void UEOS_GameInstance::OnGetAllFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	if (bWasSuccessful)
	{
		if (MyOnlineSubsystem)
		{
			if (IOnlineFriendsPtr FriendsPtr = MyOnlineSubsystem->GetFriendsInterface())
			{
				TArray<TSharedRef<FOnlineFriend>> FriendsList;
				if (FriendsPtr->GetFriendsList(0, ListName, FriendsList))
				{
					for (auto OnlineFriend : FriendsList)
					{
						UE_LOG(LogTemp, Warning, TEXT("Friend Name = %s"), *OnlineFriend->GetRealName());
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to get friends list"));
				}
			}

		}	
	}
}
