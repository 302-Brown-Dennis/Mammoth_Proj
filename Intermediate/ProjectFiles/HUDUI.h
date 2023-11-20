// HUDUI class currently a WIP
#pragma once


class HUDUI
{
public:
	void DrawHUD();
	void AddOverlay();
	void SetHUDAmmo(int);
	void SetHUDHealth(float, float);
	void SetHUDStamina(float, float);

private:
	void DrawCrosshair();
protected:
	void ShowMainMenu(float);
};