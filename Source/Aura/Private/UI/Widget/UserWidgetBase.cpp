// Copyright rynnli


#include "UI/Widget/UserWidgetBase.h"

void UUserWidgetBase::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
